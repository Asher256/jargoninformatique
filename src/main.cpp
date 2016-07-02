//==================================================================
//
// Jargon Informatique
//
// Fichier principal pour la gestion du logiciel
// Jargon Informatique.
//
// Copyright (c) Asher256 
// Email:  asher256@gmail.com
// Site:   http://jargon.asher256.com/
//
//==================================================================
// LICENCE:
// --------
// Cette source est distribuée sous Licence GNU General
// Public Licence version 2 ou ultérieure. Vous avez donc
// le droit de l'utiliser, la distribuer, l'étudier et de 
// l'améliorer librement. Toute modification doit être envoyée
// à l'auteur afin qu'elle soit publiée dans le site offciel. 
// Lisez le fichier COPYING.txt (distribué avec le logiciel) 
// pour plus d'informations...
//==================================================================

/* Headers Windows */
#ifdef WIN32
#include <windows.h>
#include <shellapi.h>
#endif

/* Configuration générée par le Makefile */
#include "config.h"

/* libc */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> //tolower et toupper
#include <sys/types.h>
#include <dirent.h>

/* Linux/Unix */
#ifdef LINUX
#include <unistd.h> //fork & stat aussi mais aussi exec*
#include <sys/types.h> //fork & stat aussi
#include <sys/stat.h>
#endif

#include <signal.h>
#ifdef LINUX
#include <sys/wait.h>
#endif

/* Fltk */
#include "resource.h"
#include "main_widget.h"
#include <FL/fl_ask.H>
#include <FL/fl_draw.H>
#include "flnice.h"
#include <FL/Fl_Preferences.H> //fichier de config
#include <FL/Enumerations.H>
#include <FL/x.H>
#include <FL/x.H>
#include <FL/Enumerations.H>

/* Icône */
#ifdef __linux__
#include <X11/xpm.h>
#include "icon.xpm"
Pixmap icon_pixmap,icon_mask;
#endif

/* Libs Jargon Informatique */
#include "jargon_lib.h"
#include "gstr.h"
#include "spath.h"

// Quelques variables pour le fichier de configuration, afin qu'il soit unique
#define PREF_ROOT     Fl_Preferences::USER
#define PREF_VENDOR   "Asher256"
#define PREF_APP      "JargonInformatique"

/* Chaines utiles */
char *jargon_path=0;
char *dictionnaire_path=0;

/* Pointeur vers l'interface graphique */
Interface *I=0; //l'interface (héritée de la version 0.x de Jargon Informatique)
Fl_Window *window=0;
Fl_Window *about=0;
Fl_Window *news_window=0;
Fl_Window *config_window=0;
SelectionCouleur *selection_couleur=0;
Credit *credit=0; //boite de dialogue pour le credit scroller
LoadingDlg *loading_dlg=0; //fenetre de chargement... (au début du programme avec un pourcentage)
ContribWindow *contrib_window=0;
SelectionThemeMot *selection_theme_mot=0;
NavigateurDlg *selectionner_navigateur=0;
AutreOptionDlg *autre_option_dlg=0;

/* L'historique */
DICT history; //c'est initialisé en bas dans main
int history_pos=0; //position dans la plage: 0 --> history.size

/* masque du thèmes (afin de pouvoir filtrer les thèmes lors d'une recherche) */
char *theme_accepte=0;

/* déclaration de certaines fonctions */
char cdel_accent(char c);
void finir_config_autre();

//centrer la fenêtre
void center(Fl_Window *window)
{
	window->position(
			/* x */			
			Fl::x()+
			(Fl::w()-window->w())/2

			/* y */ ,
			Fl::y()+
			(Fl::h()-window->h())/2
	);
}

//center une fenï¿½re fille selon sa mï¿½e
void center_in_window(Fl_Window *pere, Fl_Window *fils)
{
	fils->position(
			/* x */
			pere->x() + ((pere->w()-fils->w())/2),
			/* y */
			pere->y() + ((pere->h()-fils->h())/2)
	);
}

void set_transparent_icon(Fl_Window *window)
{
#ifdef __linux__
	/* La fenêtre doit impérativement être affichée */
	if(!window->shown()) return;

	/* On applique la transparence */
	XWMHints *hints = XGetWMHints(fl_display,fl_xid(window));
	hints->flags|=IconMaskHint;
	hints->icon_mask = icon_mask;
	XSetWMHints(fl_display,fl_xid(window),hints);
	XFree(hints);
#endif
}

/* Mettre les icônes dans les fenêtres */
void set_icons()
{
	static bool first = true;

#ifdef WIN32
	if(first) {
		I->MainWindow->icon((char *)LoadIcon((HINSTANCE)fl_display,MAKEINTRESOURCE(IDI_ICONE)));
		first = false;
	}
#endif

#ifdef __linux__
	Fl_Window *win[] = {window, 0 };
	Fl_Window **p;

	if(first) {
		p=win;
		XpmCreatePixmapFromData(fl_display, DefaultRootWindow(fl_display), icon_xpm, &icon_pixmap, &icon_mask, NULL);
		while(*p) {
			window->icon((char *)icon_pixmap);
			p++;
		}
		first = false;
	}

	p=win;
	while(*p) {
		set_transparent_icon(*p);
		p++;
	}
#endif
}

/*************************************************
 * --- LINUX SEULEMENT --- 
 *
 * (pour la gestion des navigateurs internet)
 *
 *************************************************/
#ifdef LINUX
//le nom de la constante explique tout :-)
#define NAVIGATEUR_PAR_DEFAUT "firefox,mozilla,epiphany,konqueror,opera,dillo" 

//Cette fonction configue le navigateur (avec la boite de dialogue)
void config_navigateur()
{
	char *navigateur=0;

	//ouvrir le fichier de configuration
	Fl_Preferences *pref = new Fl_Preferences(PREF_ROOT,PREF_VENDOR,PREF_APP);

	//prends les informations du fichier
	pref->get("navigateur",navigateur,NAVIGATEUR_PAR_DEFAUT);
	selectionner_navigateur->nom->value(navigateur);

	//pour le bouton par défaut
	selectionner_navigateur->navigateur_par_defaut=NAVIGATEUR_PAR_DEFAUT;

	//affiche le dlg...
	if(selectionner_navigateur->show(config_window->x()+40,config_window->y()+40)) //et modifie si ok
		pref->set("navigateur",selectionner_navigateur->nom->value());

	free(navigateur);
	delete pref; //sauvegarde!
}
void config_navigateur_callback(Fl_Widget *)
{ config_navigateur(); }
#endif

/*************************************************
 * Savoir si un fichier est exécutable...
 *************************************************/
#ifdef LINUX
int is_exec(const char *filename)
{
	static struct stat _stat;

	if(stat(filename,&_stat)) return 0;

	//teste
	if(
	// si exécutable ou GIE/UID
	(_stat.st_mode&(S_IXGRP|S_IXUSR|S_IXOTH|S_ISUID|S_ISGID)) 
	 	&&
	//mais il doit aussi etre un fichier régulier (car, presque tous les répertoires sont exécutables)
	S_ISREG(_stat.st_mode)
	) return -1;

	return 0;
}
#endif

/*************************************************
 * Cherche dans $PATH si une commande existe.
 *
 * Par exemple si "commande" pointe vers 
 * "mozilla" alors il va retourner un nouveau
 * pointeur vers une nouvelle chaine contenant
 * "/usr/bin/mozilla" (qui est le chemin
 * absolu de la commande).
 *
 * sinon, il retourne NULL ! Pas trouvé!
 *
 * ATTENTION: la nouvelle chaine retournée
 * est une chaine malloc
 *************************************************/
#ifdef LINUX
char *trouve_dans_path(char *commande)
{
	char *PATH;     //contient $PATH
	char *s;        //petit pointeur de service :-)

	PATH = getenv("PATH");
	if(!PATH) return 0; //eh oui! pas de path alors... zï¿½o :-)

	//maintenant fait une copie en forme de DUP puisque la variable path
	//n'est pas alloquée
	PATH = strdup(PATH);
	if(!PATH) return 0;

	//pacours PATH à la recherche de la commande
	s = strtok(PATH,":");
	while(s) {
		char *path = (char *)malloc(strlen(s)+1+strlen(commande)+1);
		if(!path) { free(PATH); return 0; }
		
		//crée le chemin :-)
		sprintf(path,"%s/%s",s,commande);

		//Ici pas la peine de faire exists() puisque is_exec
		//fait l'affaire, un fichier exécutable implique qu'il
		//soit un fichier et en plus exécutable :-)
		if(is_exec(path)) { 
			commande=(char *)malloc(strlen(path)+1);
			if(!commande) { free(PATH); return 0; } //pas de mémoire = pas trouvï¿½(peu de chance que cela se produise)
			strcpy(commande,path);
			free(path); free(PATH);
			return commande; //trouvé !! donc un nouveau malloc (il ne touche pas à commande évidement!)
		}
		
		// autre strtok
		free(path);

		//cherche dans le prochain chemin dans path...
		s = strtok(0,":");
	}

	free(PATH);//les bonnes habitudes
	return NULL;
}
#endif

/*************************************************
 * Cette fonction portable entre linux et 
 * et windows pour lancer le navigateur
 *************************************************/
void charger_navigateur(const char *url)
{
#ifdef LINUX
		char *save;
		static char *navigateur,*p,*p2;

		//charge les informations du fichier de configuration
		Fl_Preferences *pref = new Fl_Preferences(PREF_ROOT,PREF_VENDOR,PREF_APP);
		pref->get("navigateur",navigateur,NAVIGATEUR_PAR_DEFAUT);
		delete pref;
		
		p=navigateur;
		if(!*p) //pas de navigateur????
			p=0; 
		else { //sinon
			//il va parcourir tous les navigateurs sï¿½arï¿½ par une virgule
			//puis cherche si l'un de ces navigateurs existe dans PATH, si oui
			//alors il l'exï¿½ute avec l'URL
			while(*p) {
				p2=strchr(p,',');  //cherche la prochaine virgule et la met dans un nouveau pointeur

				//il a trouvï¿½une virgule: donc un nouveau navigateur nous attends dans le prochain cycle
				//il mets un NULL afin que dans p on ne trouve que le navigateur actuel
				if(p2) *p2=0;
				
				//Cherche dans PATH si p existe (qui est actuellement le navigateur recherché, s'il
				//le trouve alors il retourne une realloc de p avec un chemin absolu.
				save=p;
				gs_trim(p);
				
				{
				char *espace = strpbrk(p," \t");
				char save_espace=0;
				
				if(espace) { save_espace = *espace; *espace=0; }
				
				if((p=trouve_dans_path(p))) {
					if(espace) { 
						*espace=save_espace; 
						p = gs_strcat_r(p, espace); 
					}
					if(p) {
						p = gs_strcat_r(p, " \""); 
						p = gs_strcat_r(p, url); 
						p = gs_strcat_r(p, "\""); 
					}
					break; //si il trouve, il va devoir le désalloquer
				}

				if(espace) { *espace=save_espace; }
				
				}
				
				p=save; //bah sinon, il remet l'ancien et continue comme si rien n'était :-)
				
				//si le navigateur est caché alors il ne le cache plus ici afin de continuer l'algorithme
				if(p2) { *p2=','; p=p2+1;/*maintnant p pointe vers le prochain navigateur */ }
				//s'il n'nétait pas caché cela veut dire qu'il n'y a plus de prochain navigateur (chaque prochain
				//navigateur est ... par une virgule) donc, bye :-)
				else { p=0; break; }
			}
			if(p && !*p) {
					free(p);
					p=0;//ahhh je suis parano dans ce genre de choses :-)
			}
		}

		if(!p) {
			fl_alert("Le programme n'a trouvé aucun de ces navigateurs:\n\"%s\"\npour mettre le nom de votre navigateur il faut aller\ndans 'Options' puis 'Choisir le navigateur Internet'...",navigateur);
		}
		//exécute le navigateur... selon ce qu'il y a dans p
		else {
			//Il affiche le navigateur trouvï¿½ fier de lui :-)
			printf("Navigateur: \"%s\"\n",p);	
			//un petit fork
			pid_t pid=fork();
			if(pid==0) { //le fils
					execlp("sh","sh","-c",p,NULL);
					printf("Erreur lors de l'execution du navigateur...");
					exit(1);
			}
			else if(pid<0) { //erreur
					printf("Erreur lors de la creation du processus fils...");
					exit(1);
			}
			//else { //le père
							//rien du tout
			//}
		}

		free(navigateur);
		free(p); //car p a aussi un malloc donné par la fonction trouve_dans_path() //rien!
#endif
#ifdef WIN32
		//sous windows, ... on s'en tape puisqu'il s'occupe de tout! :-)
		ShellExecute(NULL,"OPEN",url,"","",SW_SHOWNORMAL);
#endif
}

//==============================================
// La fenêtre de chargement... 0% 1% 100%
//==============================================

//cette fonction s'occupe de mettre jour
//la barre de progression ou il y a le 
//pourcentage
//void input_fill();
void loading_proc(int percent)
{
	static char buffer[30]; //il ne dépassera pas ça!
	static int old=0;
	if(percent-old<2) return; old=percent;//n'afficher que quand ça change...
	sprintf(buffer,"%i%%",percent);
	loading_dlg->progress->value(percent);
	loading_dlg->progress->label(buffer);
	Fl::check();
}

// Fonction qui charge le dictionnaire
//cette fonction affiche une fenï¿½re qui nous montre la progression 
//du dictionnaire en pourcentage
int _hide_splash=0;
void loading()
{
	int err;
	char *p;

	// Chargement
	if(!_hide_splash) {
	loading_dlg->reset();            //remet le tout
	center(loading_dlg->MainWindow); //il centre la fenetre
	loading_dlg->MainWindow->show(); //affiche le fenetre de chargement
	}

	//création du chemin
	p = sp_pathcat(dictionnaire_path,"jargon.dic");
	if(!p) { fl_alert("Pas assez de mémoire...\n"); exit(1); }

	//sinon, il charge le nouveau dictionnaire...
	printf("Chargement dictionnaire: \"%s\"...\n",p);

	//chargement du dictionnaire...
	err=load_dict(p, _hide_splash?0:loading_proc);

	if(!_hide_splash && loading_dlg->MainWindow->shown()) loading_dlg->MainWindow->hide();

	//ERREUR?
	if(err) {
		if(err==DICT_E_OUTMEM) {
			//bon on est simpa on va meme laisser l'utilisateur utiliser le programme
			if(!fn_ask("Pas assez de mémoire pour charger tout le dictionnaire...\nVoulez vous lancer le programme même si le dictionnaire\nn'est pas complet?")){
				free(p);
				exit(1);
			}
		}
		else {
			//Et là c'est une erreur inconnue :-( cela veut dire que l'on va quitter :-(
			fl_alert("Erreur dans le chargement du dictionnaire.\nL'emplacement du dictionnaire est:\n\"%s\"...",p);
			free(p);
			exit(1);
		}
	}
	free(p);
}

//==============================================
// Supprime quelques accents d'une chaine
// mais ne les suppriment pas tous...
// 
// Je n'ai mis que les accents les plus
// utilisés
//==============================================

char cdel_accent(char c)
{
	static bool prepared=0;
	static int list[256];
	if(!prepared) {
		for(int i=0;i<=255;i++) list[i]=i;
		list[234]=101;  //ecirc
		list[233]=101;  //eacute
		list[232]=101;  //egrave
		list[202]=69;   //Ecirc
		list[201]=69;   //Eacute
		list[224]=97;   //agrave
		list[226]=97;   //acirc
		list[239]=105;  //i trema
		list[238]=105;  //i accent circ
		list[252]=75;   //u trema
		prepared=true;
		//TODO: j'ai l'essentiel ici. a completer au cas de besoin...
	}
	return (char)list[(int)(unsigned char)c];
}

// Del l'accent dans toute une chaine...
void del_accent(char *str)
{
	while(*str) {
		*str=cdel_accent(*str); 
		str++;
	}
}

//==============================================
//pour vider l'output
//==============================================
void vider_output()
{
	char *str;
	char *path;

	//création du path
	path = sp_pathcat(jargon_path,"misc/logo.png");
	if(!path) { return; }

	//création de la page HTML
	str = strdup("<html><body><p><p><center><img src=\"");

	str = gs_strcat_r(str,path); free(path);
	str = gs_strcat_r(str,"\"></center></body></html>");
	if(!str) { return; }

	//affichage du résultat
	I->output->value(str);

	//bye
	free(str);
}

//==============================================
//Quand on écrit un mot cette fonction
//s'occupe d'afficher la liste des mots corres-
//pondants
//==============================================
void input_completion(Fl_Widget *)
{
    int i;
    const char *mot = I->input->value();
    size_t len=0;
    char *_mot=0;

    // désalloque la liste précédente
#ifdef INDEX_ALLOC
    for(i=1;i<=I->list->size();i++) {
        int *p_index;
        p_index=(int *)I->list->data(i);
        delete p_index;
    }
#endif
    // initialise la liste 
    I->list->clear();

    if(*mot) {
        len = strlen(mot);
        _mot = strdup(mot);
        if(!_mot) { fl_alert("Pas assez de mémoire..."); return; }
        del_accent(_mot);
        gs_lcase(_mot);
    }

    for(i=0;i<dict.size;i++) {    // pour tout les mots du dictionnaire
        // teste si mot est dans un theme selectionné
        short *list=dict.entry[i].theme_list;
        bool pas_le_droit=true;
        if(list) {
            while(*list!=-1) {
                if(theme_accepte[*list]) { pas_le_droit=false; break; }
                list++;
            }
        }
        if(pas_le_droit) continue;

        if(*mot) {    // teste si le mot correspond
            char *_mot2 = strdup(dict.entry[i].word);
            if(!_mot2) continue;
            del_accent(_mot2); 
            gs_lcase(_mot2);

            if(strncmp(_mot,_mot2,len)) {
                free(_mot2);
                continue;
            }
            free(_mot2);
        }
        
        char *s = gs_replace_m(dict.entry[i].word,"@","@@");

        // tous les tests sont passé, ajouter dans la liste MD
	#ifdef INDEX_ALLOC
        int *new_i=new int;
        *new_i=i;
        I->list->add(s,(void *)new_i);
	#else //ajoute directement la copie de l'index
	static INDEX_TYPE j;
	j=(INDEX_TYPE)i;
	I->list->add(s,(void *)j);
	#endif

        free(s);
    }
    if(*mot)
        free(_mot);
}

//==============================================
// Exporter en html
//==============================================
void exporter_html(const char *fichier);

//je prérère placer cette fonction avant exporter_html, 
//pour la visibilité
void exporter_html_click(Fl_Widget *)
{
	char *HOME=getenv("HOME");

	//charge le fichier de configuration
	Fl_Preferences *pref = new Fl_Preferences(PREF_ROOT,PREF_VENDOR,PREF_APP);

	FN_PATTERN pattern[] = { {"Fichier Html","html"},{"Fichier Html","htm"},{0,0}};
	//l'utilisateur doit au moins avoir sélectionner un élément...
	if(I->list->value()<1) { 
		fl_alert("Vous devez au moins sélectionner un mot pour exporter sa description..."); 
		I->input->take_focus();
		return; 
	}
	//maintenant commence l'exportation...
	const char *file = fn_save_chooser("Exportation vers un fichier Html...",pattern,HOME);
	if(file) {
		//sous le file chooser de FLU, il ne mets pas l'extention par dï¿½aut
		//ce qui fait qu'il va demander de la mettre
		if(!*sp_extname_p(file)) {
			if(fn_ask("je vois que vous n'avez pas spécifié d'extention .html au fichier, souhaitez vous l'ajouter?")) {
				char *s = (char *)malloc(strlen(file)+5+1);
				sprintf(s,"%s.html",file);
				if(!s) {
					//presque impossible :-) mais bon les bonnes habitudes.
					fl_alert("Pas assez de mémoire pour ajouter l'extention");
				}
				else{
					exporter_html(s); free(s);		
				}
			}
			else
				exporter_html(file);
		}
		else
			exporter_html(file);
	}

	//il mets le focus dans l'Input
	I->input->take_focus();

	//sauvegarde le contenu dans le fichier de configuration
	delete pref;
}

//exporter vers html
void exporter_html(const char *fichier)
{
	FILE *handle;

	//le test de l'existence
	if(sp_exists(fichier) && !fn_ask("Le fichier:\n%s\nExiste déjà voulez vous l'écraser?",fichier)) return;
	
	//ouverture du fichier en ï¿½riture
	handle = fopen(fichier,"w");
	if(!handle) { fl_alert("Erreur dans l'ouverture du fichier:\n%s\nen écriture...",fichier); return; }

	//chargement de la description
	#ifdef INDEX_ALLOC
	int *pIndex = (int *)I->list->data(I->list->value()); //MD
	int index = pIndex?*pIndex:-1;
	#else
	INDEX_TYPE index = (INDEX_TYPE )I->list->data(I->list->value());
	#endif
	//int *pIndex = (int *)I->list->data(I->list->value()); 
	//int index = pIndex?(int)pIndex:-1;
	if(index==-1) { fl_alert("Erreur interne"); fclose(handle); return; }
	char *description = dict_get_description(index);
	if(!description) { fl_alert("Pas assez de mémoire..."); fclose(handle); return; }
	char *mot = dict.entry[index].word;//prends le mot
	
	//mettre le contenu
	fprintf(handle,"<!-- Exportation faite avec le logiciel Jargon Informatique version %s, par Asher256 -->\n",VERSION);
	fprintf(handle,"<!-- Pour plus d'informations, le site du logiciel: http://jargon.asher256.com/ -->\n\n");
	fprintf(handle,"<html>\n<head>\n\t<title>Explication du mot %s</title>\n",mot);
	
	//maintenant, il va mettre le style de la page
	fprintf(handle,"<style>\n");
	fprintf(handle,"p, body, td { font-family: verdana, arial, helvetica; font-size: 12px }\n");
	fprintf(handle,"</style>\n");
	
	//on continue :-)
	fprintf(handle,"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">");
	fprintf(handle,"</head>\n<body bgcolor=\"#7F91BF\">\n");
	fprintf(handle,"<table align=\"center\" width=\"500\" cellpadding=\"4\" cellspacing=\"0\" border=\"1\" bordercolor=\"#000000\" bgcolor=\"#FFFFFF\">\n");
	//fprintf(handle,"<tr><td align=\"center\"><b><font color=\"#0000AA\">%s</font></b></td></tr>",mot);

	//transformations dans la description
	char *new_description=(char *)malloc(strlen(description)+1); //la transformation
	if(!new_description) { free(description); fl_alert("Pas assez de mémoire"); fclose(handle); return; }
	char *s=description,*s2=new_description;
	*new_description=0; //la vider
	while(*s) {
			//si c'est une balise à passer...
			if(strncmp(s,"<a href=\"word:",14)==0) {
					char *_s=s;
					s=strstr(s,">"); //cherche la fin
					//s'il a pu dépasser
					if(s) {
						s++; //dépasse le >
						//cherche la fin de la balise... en maj et en min
						_s=strstr(s,"</a>"); if(!_s) _s=strstr(s,"</A>");
						//FIN NON TROUVEE
						if(!_s) break; //erreur dans le parser...
						//OK OK OK
						char save=*_s;//cache le </A>
						*_s=0;
						strcat(new_description,"<b>"); //largement suffisant pour contenir 
						strcat(new_description,s);
						strcat(new_description,"</b>");
						*_s=save; //remets le caractère
						//maintenant se positionne après le </A>
						s=_s+4;
						//mise à jour du pointeur qui pointe vers la fin de description
						s2=new_description+strlen(new_description);
						continue;
					}

					//SINON
					s=_s; //remets l'ancienne valeur afin qu'il ne contienne pas le null
					break; //bye, car c'est une erreur dans le parsing quand on ne trouve pas de >
			}
			//ajout le caractère new_description
			*s2=*s; *(s2+1)=0;
			//et aussi... incrémente les deux variables afin de parcourir toute la chaine
			s2++; s++;
	}

	//et mets la fin aussi
	fprintf(handle,"<tr><td>%s</td></tr>\n",new_description);
	fprintf(handle,"<tr>\n\t<td align=\"center\"><font size=\"-1\">Exporté avec <a href=\"http://jargon.asher256.com/\" href=\"_blank\">Jargon Informatique</a> version %s</font></td></tr>\n",VERSION);
	fprintf(handle,"</table>\n</body></html>");

	//et enfin, fermeture
	free(new_description); free(description);
	fclose(handle);
}

//==============================================
// Mettre l'explication dans le 
// help view (la fenetre qui est a droite)
//==============================================
void expliquer(int historiser) //ou n est le numéro du mot
{
    	//créer un fichier html en mémoire
	char *avant="<html><body>";
	char *apres="</body></html>";
	
	//Lis l'entry...
	#ifdef INDEX_ALLOC
	int *p_word_index = (int *)I->list->data(I->list->value()); //MD
	int word_index = p_word_index?*p_word_index:-1;
	#else
	INDEX_TYPE word_index = (INDEX_TYPE)I->list->data(I->list->value());
	#endif

	//int word_index = (int)I->list->data(I->list->value());
	if(word_index==-1) return; //word non trouvé
	
	//charger la description du fichier...
	char *description = dict_get_description(word_index);
	if(!description) return; //s'il ne trouve pas de description... ou pas assez de mem...

	//assemble le tout
	char *tout = (char *)malloc(strlen(avant)+strlen(description)+strlen(apres)+1);
	if(!tout) { free(description); fl_alert("Pas assez de mémoire..."); return; }

	//ici, le tout !
	strcpy(tout,avant);
	//sous windows, les trucs << >> ne s'affichent pas bien. je les remplacent par " "
	#ifdef WIN32 
	gs_creplace(description,171,'\"');
	gs_creplace(description,187,'\"');
	#endif
	strcat(tout,description);
	strcat(tout,apres);
	I->output->value(tout);

	//désallocation
	free(tout);
	free(description);

	//si on historise pas alors autant quitter maintenant!
	if(!historiser) return;

	//Libère les history après l'actuel...
	while(history_pos<history.size)
		dict_pop(&history);

	I->suivant->deactivate(); //dans tous les cas suivant est bye!
	
	//maintenant ajouter dans history et mettre précédent true
	//puis mettre actual_history à jour...
	{
		int index;
		index=I->list->value();

		/* On teste si le mot existe déjà */
		if(history.size>0 && history.entry[history.size-1].offset==index && !strcmp(history.entry[history.size-1].word, I->input->value())) 
			return;

		/* On ajoute le mot */
		if(!dict_push(&history,strdup(I->input->value()),0,index,0) && history.size>0) 
			I->precedent->activate();

		history_pos=history.size;
	}
}

//==============================================
// Si l'on clique sur un mot dans la liste
//==============================================
void list_click(Fl_Widget *)
{
	expliquer(1);
	I->input->take_focus();
}

//==============================================
// agrandir/diminuer
//==============================================
#define TAILLE_POLICE_MINIMALE   10
#define TAILLE_POLICE_PAR_DEFAUT 14
#define TAILLE_POLICE_MAXIMALE   26
#define TAILLE_POLICE_INC        2
void sauver_taille_police()
{
	if(!I) return;
	if(!window) return;
	Fl_Preferences *pref = new Fl_Preferences(PREF_ROOT,PREF_VENDOR,PREF_APP);
	pref->set("taille_police",I->output->textsize());
	delete pref;
}

void charger_taille_police()
{
	int taille;
	Fl_Preferences *pref = new Fl_Preferences(PREF_ROOT,PREF_VENDOR,PREF_APP);
	pref->get("taille_police",taille,TAILLE_POLICE_PAR_DEFAUT);
	I->output->textsize(taille);
	//désactiver au cas ou l'on atteint l'extrême...
	I->big->activate(); I->little->activate();
	if(taille>=TAILLE_POLICE_MAXIMALE)   I->big->deactivate();
	else if(taille<=TAILLE_POLICE_MINIMALE) I->little->deactivate();
	delete pref;
}

void big_click(Fl_Widget *)
{
	I->output->textsize(I->output->textsize()+TAILLE_POLICE_INC);
	if(I->output->textsize()>=TAILLE_POLICE_MAXIMALE) { 
		I->output->textsize(TAILLE_POLICE_MAXIMALE);
		I->big->deactivate();
	}
	I->little->activate();
	I->input->take_focus();
}

void little_click(Fl_Widget *)
{
	I->output->textsize(I->output->textsize()-TAILLE_POLICE_INC);
	if(I->output->textsize()<=TAILLE_POLICE_MINIMALE) { 
		I->output->textsize(TAILLE_POLICE_MINIMALE); //on ne sait jamais si un jour le nombre est impair...
		I->little->deactivate();
	}
	I->big->activate();
	I->input->take_focus();
}

//==============================================
// Quitter
//==============================================
void quitter_click(Fl_Widget *widget)
{
	exit(0);
}

//==============================================
// convertissement...
//==============================================
void html_to_text(char *str)
{
		gs_replace(str,"&iuml;","ï");
		gs_replace(str,"&eacute;","é");
		gs_replace(str,"&egrave;","è");
		gs_replace(str,"&ecirc;","ê");
		
		gs_replace(str,"&acirc;","â");
		gs_replace(str,"&agrave;","à");
		
		gs_replace(str,"&ocirc;","ô");
		
		gs_replace(str,"&ugrave;","ù");
		gs_replace(str,"&ucirc;","û");
		
		gs_replace(str,"&icirc;","î");
		
		gs_replace(str,"&ccedil;","ç");
		gs_replace(str,"&icirc;","î");

		gs_replace(str,"&amp;","&");
		gs_replace(str,"&lt;","<");
		gs_replace(str,"&gt;",">");
		gs_replace(str,"&nbsp;"," ");
}

//======================================================================
// cherche le meilleur mot qui existe dans 'list', d'après ce qu'il 
//y a dans 'input'
//======================================================================
int chercher_meilleur_mot()
{
	const char *input_value = I->input->value(); //pour avoir le pointeur une fois pour toute :-)
	int i;
	int meilleur=0; //s'il trouve un meilleur mot... il va être nonz. (car le i commence par 1)

	for(i=2;i<=I->list->size();i++) {
			char c1,c2;//pour la visibilité
			const char *val=""; //valeur de la liste actuelle

			#ifdef INDEX_ALLOC
			int *wordindex; //MD
			wordindex=(int *)I->list->data(i);
			val = dict.entry[*wordindex].word;
			#else
			static INDEX_TYPE wordindex;
			wordindex=(INDEX_TYPE)I->list->data(i);
			val = dict.entry[wordindex].word;
			#endif

			c1 = cdel_accent(*val);
			c2 = cdel_accent(*input_value);
			//si difféent et en plus c'est une autre lettre
			if(tolower(c1)!=tolower(c2)) break; //et pas de meilleur! donc pas de changement
			//fait un test de super précision :-)
			//si le premier est exactement comme le deuxième
			if(!strcmp(val,input_value)) meilleur=i;
		}
	return meilleur;
}


//======================================================================
// cherche le meilleur mot qui existe dans dict->entry
// s'il existe (la précisition ne compte pas beaucoup) il retourne
//======================================================================
int chercher_mot_dict(const char *input_value)
{
	int i;
	char c1,c2;
	const char *val;
	for(i=0;i<dict.size;i++) {
			val=dict.entry[i].word;
			c1 = *val;
			c2 = *input_value;
			if(c1==c2 && !strcmp(val,input_value)) return 1;
	}
	return 0;
}

//==============================================
// Lien cliqué dans la description
//==============================================
#define SURPLUS 30
const char *link_click(Fl_Widget *w, const char *url)
{
		char *dup;
		static char buffer[1025+SURPLUS]=""; //1025 pour URL (1024+1zero). SURPLUS pour le chemin en plus
		static char buffer2[1025+SURPLUS]="";
		int i;

		//=== les alias pour les adresses ===
		char *p;
		strncpy(buffer,url,1024);
		
		//si c'est une image...
		if((p=strstr(buffer,"../deco/"))) {
			if(p!=buffer) {
				snprintf(buffer2,1024+SURPLUS,"file:%s%c%s",dictionnaire_path,C_SLASH,p+2);
				strcpy(buffer,buffer2);
			}
			return buffer;
		}

		if((p=strstr(buffer,"../fig/"))) {
			if(p!=buffer) {
				snprintf(buffer2,1024+SURPLUS,"file:%s%c%s",dictionnaire_path,C_SLASH,p+2);
				strcpy(buffer,buffer2);
			}
			return buffer;
		}

		//ajout du général
		if((p=strstr(buffer,"../general/"))) {
			if(p!=buffer) {
				snprintf(buffer2,1024+SURPLUS,"%s%c%s",dictionnaire_path,C_SLASH,p+2);
				sp_fix_slash_repeated(buffer2);
				sp_make_system_slash(buffer2);
				strcpy(buffer,buffer2);
				charger_navigateur(buffer);
			}
			return 0;
		}



		//SI C UNE URL
		if(strncmp(url,"http://",7)==0) {
			charger_navigateur(url);
			return 0;
		}
		
		//===================================

		//pas de word alors bye
		if(strncmp(url,"word:",5)) return url;
		
		//il dup le mot pour y appliquer un petit
		//filtre html
		dup = strdup(url+5);
		if(!url) { fl_alert("Pas assez de mémoire"); return 0; }
		html_to_text(dup);

		//là il va voir s'il y a un thème qui est désactivé
		bool theme_desactive=false;
		for(i=0;i<dict.theme_size;i++){
			if(!theme_accepte[i]) {
				theme_desactive=true;
				break;
			}
		}

		//il va voir si ce mot existe au moins dans le dictionnaire...
		//(j'ai ajouté cette recherche car ça permet d'être sûre
		// que le mot existe au cas ou il aurait sélectionné
		// d'autres thèmes)
		int meilleur = chercher_mot_dict(dup);
		if(!meilleur) {
			fl_alert("Ce mot n'existe pas dans le dictionnaire...");
			free(dup);
			return 0;
		}
		//bon, s'il y a un thème qui n'est pas activé, cela veut dire 
		//qu'on doit en savoir plus sur l'existence du mot!
		else if(theme_desactive) {
			short *theme_list=dict.entry[meilleur].theme_list;
			bool trouve=false; //true s'il a trouvé que ce mot appartient à un thème
			while(*theme_list!=-1) {
				if(theme_accepte[*theme_list]) { trouve=true; break; }
				theme_list++; //le prochain
			}

			//s'il n'a pas trouvé de thème... il demande la fameuse question!
			//(il sait maintenant qu'il appartient à un autre thème...
			// qui n'est pas actuellement sélectionné!)
			if(!trouve) {
				if(!fn_ask("Ce mot a été trouvé, mais pas dans les thèmes sélectionnés.\nSouhaitez vous sélectionner tous les thèmes pour lire la description de ce mot?")) {
					free(dup);
					return 0;
				}
				else {
					for(i=0;i<dict.theme_size;i++) 
						theme_accepte[i]=1;
					//et là, il va remettre history à 0
					for(i=0;i<history.size;i++)
						dict_pop(&history);
					history_pos=0;
					I->suivant->deactivate(); 
					I->precedent->deactivate(); 
				}	
			}
		}
		
		//là, comme il est sûre que le mot existe dans le dictionnaire
		//il va le mettre dans value
		I->input->value(dup);
		free(dup);
		I->input->do_callback();

		//il va maintenant chercher le mot! (cliqué)
		//première chose, c'est qu'il va choisir le premier!
		I->list->select(1);
		
		meilleur = chercher_meilleur_mot();
			
		if(meilleur) I->list->select(meilleur);

		I->list->do_callback();
		return 0;
}
#undef SURPLUS

//==============================================
// Gestion de l'historique
//==============================================
void maj_apres_history()
{
	bool changement=false;

	//si on a changé quelque chose?
	if(strcmp(history.entry[history_pos-1].word,I->input->value())) changement=true;
	
	//et va vers le précédent
	I->input->value(history.entry[history_pos-1].word);
	if(changement) I->input->do_callback();

	//met la description
	I->list->select((int)history.entry[history_pos-1].offset);

	//génère la description
	expliquer(0);
}
void precedent_click(Fl_Widget *)
{
	I->input->take_focus();
	if(!history_pos) return;
	history_pos--;

	if(history_pos)
		maj_apres_history();
	else {
		bool changement=false; if(strcmp(I->input->value(),"")) changement=true;
		I->input->value("");
		if(changement) I->input->do_callback(); else { I->list->deselect(); I->list->bottomline(1); }
		vider_output();
		I->precedent->deactivate();
	}

	I->suivant->activate();
}
void suivant_click(Fl_Widget *)
{
	I->input->take_focus();
	if(history_pos>=history.size) return;

	history_pos++;
	maj_apres_history();

	//mets les activate/desactivate
	if(history.size==history_pos)
		I->suivant->deactivate();
	I->precedent->activate();
}

//==============================================
// trucs pour le news...
//==============================================
void show_news_click(Fl_Widget *)
{
		center_in_window(window,news_window);
		news_window->show();
}

void news_ok_click(Fl_Widget *)
{
		news_window->hide();
}

int credit_enabled=0;
void scroll_credit(void * = 0)
{
		if(!credit_enabled) return;
		credit->group->position(credit->group->x(),credit->group->y()-1);
		credit->MainWindow->redraw();
		Fl::add_timeout(0.1,scroll_credit);
}

void show_credit_click(Fl_Widget *)
{
		credit_enabled=1;
		credit->show();
		scroll_credit();
		while(credit->MainWindow->shown()) Fl::wait();
		credit_enabled=0;
}

#define ajouter(str)  credit->add("@c" str)
#define titre(str)    credit->add("@c@C15@i" str)
#define espace()      credit->add("@m  ");
void credit_list_init()
{
		titre("Programmation et Design de l'interface");
		ajouter("Asher256");
		espace();

		titre("Contenu du dictionnaire");
		ajouter("Roland Trique");
		espace();

		titre("Contributeurs");
		ajouter("Miguel2i");
		ajouter("Superna");
		ajouter("Gauvin Pocentek");
		espace();

		titre("Outils de développement");
		ajouter("Système d'exploitation utilisé: GNU/Linux");
		ajouter("Codé avec l'éditeur Vim (www.vim.org)");
		ajouter("ctags (pour la navigation dans le code source)");
		ajouter("GNU Make pour le Makefile");
		ajouter("Fltk pour le GUI (www.fltk.org)");
		ajouter("valgrind pour les tests mémoire");
		espace();

		titre("Compilateurs C++");
		ajouter("GCC pour GNU/Linux");
		ajouter("Mingw32 pour MS-Windows");
		espace();

		titre("Créateurs Installation");
		ajouter("NSIS sous Windows (Script d'installation Winamp)");
		espace();
}
#undef ajouter
#undef titre
#undef espace

#define add(str) I->news_list->add(str)
#define F "@b@c"
#define F2 "@i@c"

//une news list dans le programme. je trouve que c'est plus
//accessible qu'un fichier texte :-)
void news_list_init()
{
add("@c@m@bJargon Informatique");
add("@i@cTous les changement dans le programme...");
add("");
add("");
add(F "Version 1.3.6");
add(F2 "(Avril 2006)");
add("- Correction d'un petit bug dans l'historique (les liens ne sont pas");
add("  ajoutés dans l'historique quand ils sont cliqués).");
add("");
add(F "Version 1.3.5");
add(F2 "(Avril 2006)");
add("- Le même mot n'est jamais ajouté deux fois dans l'historique");
add("- Correction d'un petit bug qui se produisait quand un mot contenait");
add("  les caractères < ou >. Solution: Remplacer < par &lt; et > par &gt.");
add("- Ajout de la gestion des signaux SIGCHLD (pour éviter les zombies)");
add("  et SIGINT (pour tout désalloquer même après un CTRL-C)");
add("- Ajout d'une icône dans la barre des titres sous Linux et Windows");
add("- Contribution de Naji Mammeri <naji.mammeri@gmail.com>");
add("        * Quelques corrections dans le script install.sh");
add("- Conversion du nom de tous les fichiers en minuscule");
add("  (ex: au lieu de /usr/bin/JargonInformatique c'est devenu");
add("  maintenant /usr/bin/jargoninformatique)");
add("- Adaptation du Makefile aux fichiers en minuscule");
add("- Modification du script 'configure' pour rendre CFLAGS et");
add("  LDFLAGS plus propres dans le fichier config.mak");
add("- Adaptation du fichier install.sh aux fichier minuscules");
add("- Adaptation du fichier install.sh pour qu'il installe les fichiers");
add("  man.");
add("- Contributions de Gauvain Pocentek:");
add("        * Ajout de deux fichiers man (jargoninformatique.1.gz");
add("          et jargoninformatique.1.fr.gz)");
add("        * Modification du Makefile pour y inclure l'installation");
add("          des fichiers man (français et anglais)");
add("        * Ajout de distclean dans le Makefile");
add("- Contributions de Gauvain Pocentek:");
add("      * Man page Jargon Informatique");
add("      * Paquet Ubuntu");
add("      * Petites modification pour que le logiciel soit");
add("        compatible avec le debian policy");
add("");
add(F "Version 1.3.4");
add(F2 "(Août 2005)");
add("- Correction d'un petit bug dans le parser qui se produisait");
add("  sous windows seulement dans le mot RTFM");
add("  Signalisation par Radu-Cristian <rfotescu@cambric.ro>");
add("- Ajout du support pour plusieurs fichier dans ../general");
add("- Correction de deux petits bugs. Le premier qui est");
add("  le i tréma qui ne s'affichait pas dans la description et le");
add("  deuxième est le dico des smileys qui donne un lien mort");
add("  Signalisation par Radu-Cristian <rfotescu@cambric.ro>");
add("- Contribution miguel2i: Adaptation de configure à mingw");
add("- Changement dans la recherche du navigateur sous Linux.");
add("  Cela permet d'utiliser: xterm -e links (par exemple)");
add("  La fonction ne teste que ce qu'il y a avant le premier espace");
add("- Correction d'un bug dans la fonction qui affiche les thèmes");
add("  dans un format readable (dans traiter_ligne)"); 
add("  Quand il n'y a pas de thème cela cause une erreur de segmentation");
add("- Ajout d'un raccourci clavier 'Entrée' pour selectionner le premier");
add("  mot de la liste");
add("- Ajout de raccourcis clavier pour les boutons suivant/precedent/f/F");
add("  et aussi pour la navigation dans la liste (haut/bas)");
add("- Ajout d'une désallocation de traitement_description (jargon_lib.c)");
add("- Ajout d'une fonction qui permet de demander à l'utilisateur");
add("  s'il doit recocher automatiquement tous les thèmes pour trouver");
add("  ce qu'il cherche");
add("- Contribution de miguel2i: Ajout de dict_pop() et modification");
add("  de history_true_size par history_pos");
add("- Contribution de superna avec jargoninformatique.spec");
add("- Ajout de la configuration des sizeof(void *) et sizeof(int)");
add("  afin de décider du type d'index à mettre");
add("- Contribution miguel2i: réécriture d'input_completion");
add("  pour plus de lisibilité");
add("- Contribution superna: adaptation de configure.sh et ");
add("  Makefile pour Mac (darwin)");
add("- Contribution miguel2i: Affichage du nom complet du theme");
add("  dans la description");
add("- Ajout d'une optimisation dans dict_push() afin de ne");
add("  pas faire realloc à chaque fois...");
add("- Ajout du fichier configure.bat pour les personnes");
add("  n'ayant pas un environnement comme cygwin sous windows");
add("- Contribution de miguel2i: Script configure.sh");
add("  (pour la configuration automatique avant compilation");
add("- Contribution de superna: Ajout du fichier README.macosx");
add("- Contribution de superna: Création de paquets pour Debian");
add("- Contribution de superna: Création d'un support+paquet pour MacOSX");
add("- Contribution miguel2i: répertoire debian/ (pour créer paquets)");
add("- Contribution miguel2i: Fixation de certains bugs en accès mémoire");
add("  ainsi que plusieurs désallocations");
add("- Compilation à l'aide de l'outil apgcc qui permet de créer un");
add("  fichier binaire linux qui fonctionne dans plus de distributions");
add("  (il compile par exemple plusieurs symboles pour ne pas avoir");
add("  par exemple la petite dépendance du dernier libc GLIBC_2.x.x)");
add("");
add(F "Version 1.3.3");
add(F2 "(Juillet 2005)");
add("- Ajout de deux versions pour linux (dans le site officiel)");
add("  une version avec Antialiasing et une autre sans");
add("- Correction d'un bug mineur dans le parser. Les lien après");
add("  les images ne s'affichaient comme étant du texte normal. Comme");
add("  dans le mot 'OpenBSD'");
add("- Migration du site officiel de http://jargon.new.fr à");
add("  http://jargon.asher256.com");
add("- Modification de tous les liens (dans le logiciel et les");
add("  readme) vers http://jargon.asher256.com/ et tous les");
add("  emails d'asher256@gmail.com vers");
add("  asher256@gmail.com");
add("");

add(F "Version 1.3.2");
add(F2 "(Juin 2005)");
add("- Passage du logiciel en Libre sous Licence GPL");
add("- Ecriture de tous les noms de thèmes dans un format");
add("  plus convivial");
add("- Ajout du support pour les images (par exemple dans le mot \"Google\")");
add("- Correction d'un petit bug dans l'ouverture du mot passé en ligne");
add("  de commande. Par exemple Jargon Informatique \"Internet\"");
add("- Intégration de jargon.dic version 3.4.109");
add("- Ecriture de la compatibilité avec la balise <fig> du");
add("  jargon 3.4.109");
add("");

add(F "Version 1.3.1");
add(F2 "(Mai 2005)");
add("- Possibilité de classer les mots par thèmes et de filtrer des");
add("  mots selon leur appartenance à un ou plusieurs thèmes...");
add("- Ajout de l'Exportation de l'explication d'un mot au format HTML");
add("- Corection d'un bug de recherche de lien quand il clique sur un mot");
add("  petit Comme le lien 'X' sous l'explication de 'KDE'");
add("- Ajout de la configuration des couleurs de l'interface");
add("  dans cette version il y a 4 couleurs: vert, rouge, bleu, jaune");
add("- Option pour choisir si le splash screen (la fenêtre du pourcentage)");
add("  s'affiche ou non (pour la rapidité)");
add("- Option pour choisir si la fenêtre doit être centrée");
add("- Correction de quelques petits bugs mineurs (les mots");
add("  qui respectent la casse comme internet et Internet, correction");
add("  de certaines interpretations anormales du parser)");
add("- Option pour se souvenir du mot qu'on a sélectionné précédement");
add("- Ajout du bouton effacer avec (le petit bouton avec une croix)");
add("- Le programme se souvient de la taille des fonts sélectionné");
add("- Création d'une icone pour le système linux");
add("- Support l'affichage d'un mot demandé par ligne de commande");
add("  par exemple: JargonInformatique \"internet\"");
add("");

add(F "Version 1.2");
add(F2 "(Janvier 2005)"); 
add("- Optimisation du chargement (actuellement il est environ");
add("  7 fois plus rapide - testé sous un pentium 1, 150mhz -).");
add("- Optimisation de la taille occupée par le programme. Le");
add("  programme ne charge que ce qui est nécessaire.");
add("- Ajout du support pour les liens internet afin d'ouvrir les");
add("  liens http:// dans votre navigateur par défaut sous Windows...");
add("- Ajout du support pour les liens internet sous Linux avec");
add("  une fenêtre de configuration pour y mettre les navigateurs");
add("- Fixation des couleurs de l'inteface pour qu'elles fontionnent");
add("  de la même façon quel que soit les couleurs du bureau de");
add("  l'ordinateur.");

add("");
add(F  "Version 1.1 (Beta)");
add(F2 "(Novembre 2004)");
add("");
add("- Ajout de la fenêtre de chargement au début du programme");
add("- Ajout du support pour les liens. Maintenant la navigation");
add("  entre les mots est devenu beaucoup plus facile!");
add("- Intégration d'une fenêtre contenant un Credit Scroller");
add("- Ajout du bouton 'Suivant'");
add("- Correction de quelques caractères qui ne s'affichent pas");
add("  correctement sous windows... Notamment le &ccedil;");
add("- Intégration du fichier texte news.txt dans l'interface du");
add("  programme (ce que vous êtes entrain de lire)");
add("- Correction d'un petit bug dans les liens quand les");
add("  noms entre ¤mot¤ sont séparés par un \\n (cela cause");
add("  une sorte de lien mort dans le dictionnaire!");
add("- Correction de la fonction de parsing. Tous les \\n entre");
add("  <tt>...</tt> ne s'affichaient plus (comme si il n'y avait");
add("  plus de \\n).");

add("");
add(F "Version 1.0");
add(F2 "(Debut septembre 2004)");
add("");
add("- Optimisation du chargement du dictionnaire");
add("- Ecriture de la compatibilité avec Windows");
add("- Interface refaite avec un nouveau style");
add("- Amélioration du retour arrière qui fait un retour avec");
add("  la liste toutes les occurences.");
add("- Elimination de petits bugs mineurs");
add("- Ajout de la fenêtre About");

add("");
add(F  "Version 0.x (Beta)");
add(F2 "(Fin Aout 2004)");
add("");
add("Première version contenant:");
add("- le chargement du dictionnaire");
add("- Le retour en arrière");
add("- Chargement du dictionnaire optimisé");
add("- Version Linux seulement");
add("- Agrandir/réduire les caractères");

}
#undef add
#undef F

//==============================================
// trucs pour l'about
//==============================================
void about_click(Fl_Widget *)
{
	center_in_window(window,about);
	about->show();
	I->input->take_focus();
}
void fermer_about(Fl_Widget *)
{
	about->hide();
}

//==============================================
// trucs pour l'about
//==============================================
void ouvrir_options(Fl_Widget *)
{
	center_in_window(window,config_window);
	config_window->show();
	I->input->take_focus();
}
void fermer_options(Fl_Widget *)
{
	config_window->hide();
}


//========================================
//création du chemin du dictionnaire
//
// les chemins finissent par un /
//========================================
void creer_chemin_dictionnaire(char *argv)
{
	//cherche le chemin actuel de l'exécutable
	char *exec = sp_executable_path(argv);
	//s'il ne peux pas faire ça alors qu'est ce qu'il peut faire ;)
	if(!exec) {
		exec=strdup(""); //chemin actuel
		if(!exec) {
			fl_alert("Pas assez de mémoire...");
			exit(1);
		}
	}
	sp_dirname(exec);

	char *new_path = sp_pathcat(exec,"dict/jargon.dic"); 
	if(!new_path) {fl_alert("Pas assez de mémoire..."); exit(1);}
	sp_make_system_slash(new_path);
	free(exec);

	//cherche dans le répertoire actuel
	if(sp_exists(new_path)) {
		char *p;
		exec = sp_executable_path(argv);
		if(!exec) { fl_alert("Pas assez de mémoire..."); exit(1); }
		sp_dirname(exec);

		//création du path de base
		jargon_path=exec; 
		jargon_path = gs_strcat_r(jargon_path,SLASH);

		//création du path du dictionnaire
		p = sp_pathcat(exec,"dict");
		p = gs_strcat_r(p,SLASH);
		dictionnaire_path=p;
	}
	
	#ifdef LINUX
	//cherche dans le path du préfixe
	else if(sp_exists(PREFIX "/share/jargoninformatique/dict/jargon.dic")) {
		jargon_path=strdup(PREFIX "/share/jargoninformatique/");
		dictionnaire_path=strdup(PREFIX "/share/jargoninformatique/dict/");
	}

	//cherche dans l'ancien chemin...
	else if(sp_exists("/etc/jargon/jargon.dic")) {
		jargon_path=strdup("/etc/jargon/");
		dictionnaire_path=strdup("/etc/jargon/");
	}
	#endif
	
	//si on est ici...c'est que rien n'a été trouvé!
	else {
		#ifdef LINUX
			fl_alert("Le programme n'a pas trouvé le dictionnaire dans:\n    " PREFIX "/share/jargoninformatique/jargon.dic\nou\n    dict/jargon.dic");
		#endif
		#ifdef WIN32
			fl_alert("Le programme n'a pas trouvé le dictionnaire dans: dict/jargon.dic");
		#endif
		free(new_path);
		exit(1);
	}

	free(new_path);

	if(!jargon_path || !dictionnaire_path) {
			free(jargon_path); free(dictionnaire_path);
			fl_alert("Pas assez de mémoire...");
			exit(1);
	}

	sp_make_system_slash(jargon_path);
	sp_make_system_slash(dictionnaire_path);

	printf("Repertoire de base: %s\n",jargon_path);
	printf("Repertoire ou il y a le dictionnaire: %s\n\n",dictionnaire_path);
}

//BOUTON
//la petit croix qui efface le mot ï¿½trouver
void effacer_mot_click(Fl_Widget *)
{
	I->input->value("");
	I->input->take_focus();
	I->input->do_callback();
	vider_output();
}
	
/*********************************************************************
 * Procédure appelée à la fin du programme
 *
 * En d'autres termes: Les bonnes habitudes de tout désalloquer
 ********************************************************************/
void bye()
{ 
	static bool first = true;
	if(!first) return;
	first=false;

	selection_theme_mot->list->clear();
	//bye, le masque des thèmes
	free(theme_accepte); theme_accepte=0;
	//sauve la configuration
	finir_config_autre();
	//ne pas oublier ça
	sauver_taille_police();
	//ici, il enlève tous les éléments de l'historique
	dict_free_entry(&history);
	//il ferme le dictionnaire
	close_dict(); 
	//free les chemins alloqués
	free(jargon_path); free(dictionnaire_path); 
	//et enfin, supprime tous les index ajoutés
	//dans la liste du dictionnaire
#ifdef INDEX_ALLOC
	int *p_index; //MD
	for(int i=1;i<=I->list->size();i++) {
		p_index=(int *)I->list->data(i);
		delete p_index;
	}
#endif
	//un petit delete sur les window par les classes
	I->list->clear(); //que ce soit avec ou sans alloc

	//[les bonnes habitudes de désalloquer le tout :-)]
	//selection couleur
	//les dlg
	I->news_list->clear();
	delete contrib_window;
	delete window;
	delete about;
	delete news_window;
	delete config_window;
	delete I;
	delete credit;
	delete loading_dlg;
	delete selection_couleur;
	delete selectionner_navigateur;
	delete selection_theme_mot;
	delete autre_option_dlg;
}

//****************************************************************
//pour ouvrir la fenetre de configuration des couleurs
//****************************************************************

//changer la couleurs par une autre
void basculer_vers_couleur(int couleur)
{
	Fl_Window *fenetre[]={window,contrib_window->MainWindow,loading_dlg->MainWindow,selection_theme_mot->MainWindow, config_window, selection_couleur->MainWindow, selectionner_navigateur->MainWindow, autre_option_dlg->MainWindow, about, credit->MainWindow, news_window, NULL};
	Fl_Window **f=fenetre;
	
	//si un malin se met à modifier le fichier de configuration lui même
	//ou si un jour j'ajoute des couleurs alors que la personne installe
	//une ancienne version de ce logiciel :-)
	if(couleur<0 || couleur>3) couleur=0; //donc, couleur par dï¿½aut!

	//pour la couleur verte (par défaut)
	//couleur par défaut (bleu)
	int fgcolor,bgcolor,b_bgcolor,b_fgcolor;
	fgcolor=146;
	bgcolor=196;
	b_bgcolor=138;
	b_fgcolor=FL_WHITE;

	//pour le rouge
	if(couleur==1) {
		fgcolor=213;
		bgcolor=80;
		b_bgcolor=FL_DARK_RED;
		b_fgcolor=FL_WHITE;
	}

	//pour le vert
	if(couleur==2) {
		bgcolor=156;
		b_fgcolor=FL_WHITE;
		b_bgcolor=106;
		fgcolor=58; //fgcolor=couleurs des labels comme pour I->input
	}

	//Pour le Jaune
	if(couleur==3) {
		fgcolor=FL_DARK_RED;
		bgcolor=94;
		b_bgcolor=174;
		b_fgcolor=FL_DARK_RED;
	}

	//définition des couleurs par défaut
	while(*f) {
		Fl_Window *window = *f;
		
		//ici, modifie les couleurs de la fenêtre
		window->color(bgcolor);

		//ses enfants
		for(int i=0;i<window->children();i++) {
			Fl_Widget *child = window->child(i);
			if(!child) continue;
			//ne modifier que le label
			if(child==I->input || child==I->list || child==I->output) {
				child->labelcolor((Fl_Color)fgcolor);
				continue;
			}
			child->selection_color((Fl_Color)b_bgcolor);
			child->labelcolor((Fl_Color)b_fgcolor);
			child->color((Fl_Color)b_bgcolor);
		}
		
		//passe à la prochaine fenêtre
		f++;
	}

	//ici, les couleurs statiques:
	loading_dlg->progress->labelcolor(FL_BLACK); //chargement
	loading_dlg->progress->color(167);
	loading_dlg->progress->selection_color(175);
	
	selection_theme_mot->list->labelcolor(FL_FOREGROUND_COLOR); //selection
	selection_theme_mot->list->color(FL_WHITE);
	selection_theme_mot->list->textcolor(72);

	selection_couleur->couleur_verte->labelcolor(couleur==1?FL_WHITE:FL_BLACK);
	selection_couleur->couleur_rouge->labelcolor(couleur==1?FL_WHITE:FL_BLACK);
	selection_couleur->couleur_jaune->labelcolor(couleur==1?FL_WHITE:FL_BLACK);
	selection_couleur->couleur_bleu->labelcolor(couleur==1?FL_WHITE:FL_BLACK);
	selection_couleur->label->labelcolor(couleur==1?FL_WHITE:FL_BLACK);

	selectionner_navigateur->box->color(bgcolor);
	selectionner_navigateur->label->labelcolor(couleur==1?FL_WHITE:FL_BLACK);

	autre_option_dlg->check_fenetre_centree->labelcolor(couleur==1?FL_WHITE:FL_BLACK);
	autre_option_dlg->check_souvenir->labelcolor(couleur==1?FL_WHITE:FL_BLACK);
	autre_option_dlg->check_hide_splash->labelcolor(couleur==1?FL_WHITE:FL_BLACK);

	I->petit_readme->labelcolor(FL_BLACK);
	I->petit_readme->color(FL_WHITE);
	credit->view->color(bgcolor);
	I->news_list->color(FL_WHITE);

	if(couleur!=3) {
	I->label_auteur->labelcolor((Fl_Color)215);
	I->label_email->labelcolor((Fl_Color)206);
	I->label_url->labelcolor((Fl_Color)175);
	}
	
	//maintenant, il fait un petit refresh
	if(!window->shown()) return;
	{
		bool show_config_window=false;
		if(config_window->shown()) {
			config_window->hide();
			show_config_window=true;
		}
		window->hide(); 
      		window->show();
		if(show_config_window) config_window->show(); 
	}

	/* maj des icons */
	set_icons();
}

//lance la fenêtre de configuration
void chargement_couleur_debut()
{
	int couleur=0;
	Fl_Preferences *pref = new Fl_Preferences(PREF_ROOT,PREF_VENDOR,PREF_APP);
	pref->get("couleur",couleur,0);
	basculer_vers_couleur(couleur);
	delete pref;
}

void config_couleurs_click(Fl_Widget *)
{
	int couleur=0;
	Fl_Preferences *pref = new Fl_Preferences(PREF_ROOT,PREF_VENDOR,PREF_APP);
	pref->get("couleur",couleur,0);
	
	selection_couleur->MainWindow->position(
		config_window->x()+40,
		config_window->y()+40
	);

	int new_couleur=selection_couleur->show(couleur);

	//-1 veut dire ne rien faire
	if(new_couleur!=-1) {
		pref->set("couleur",new_couleur);
		basculer_vers_couleur(new_couleur);
	}

	delete pref;
}

//pour charger les autres options au défaut
void chargement_config_autre()
{
	int centrer_fenetre;
	int souvenir;
	Fl_Preferences *pref = new Fl_Preferences(PREF_ROOT,PREF_VENDOR,PREF_APP);

	//pour le centrage de la fenetre
	pref->get("centrer_fenetre",centrer_fenetre,1);
	if(centrer_fenetre) center(window);

	//se souvenir du mot?
	pref->get("souvenir",souvenir,1);
	if(souvenir) {
		char *input_value=0;
		char *mot=0;
		pref->get("souvenir/mot",mot,"");
		pref->get("souvenir/input_value",input_value,"");
		
		if(*mot) { //au moins un mot
			//remets le mot :-)
			I->input->value(input_value);
			I->input->do_callback();

			//ici, le recherche dans la liste déroulante...
			bool trouve=false;
			for(int i=1;i<=I->list->size();i++) {
				#ifdef INDEX_ALLOC
				static int *index; //MD
				index = (int *)I->list->data(i);
				if(!index) continue;
				#else
				static INDEX_TYPE *index;
				static INDEX_TYPE j;
				j = (INDEX_TYPE)I->list->data(i);
				index=&j;
				#endif
				//static int index;
				//index = (int)I->list->data(i);
				if(!strcmp(mot,dict.entry[*index].word)) {
					I->list->value(i);
					I->list->do_callback();
					trouve=true;
					break;
				}
			}

			//s'il ne trouve pas...
			if(!trouve) {
				printf("WARNING: Ne peut se souvenir du mot \"%s\" car celui-ci n'a pas ete trouve...\n",mot);
				I->input->value("");
				I->list->value(-1);
			}
		}

		//désalloque le tout
		free(input_value); free(mot);
	}

	delete pref;
}

void finir_config_autre()
{
	int b;
	if(!I) return;
	if(!window) return;
	//pas de souvenir!
	Fl_Preferences *pref = new Fl_Preferences(PREF_ROOT,PREF_VENDOR,PREF_APP);
	if(I->list->value()<1) {
		printf("NOTE: Ne pas se souvenir du mot..., car aucun mot n'est selectionne\n");
		pref->set("souvenir/input_value","");
		pref->set("souvenir/mot","");
		delete pref;
		return;
	}

	//maintenant il applique le souvenir
	pref->get("souvenir",b,1);
	if(b) {//si les souvenirs sont activés
#ifdef INDEX_ALLOC
		int *index; //MD
		index = (int *)(I->list->data(I->list->value())); //index du mot dans la variable dict
#else
		INDEX_TYPE *index, j;
		j=(INDEX_TYPE)(I->list->data(I->list->value()));
		index=&j;
#endif
		//index = (int)(I->list->data(I->list->value())); //index du mot dans la variable dict
		if(index) {//au moins l'index existe...
			pref->set("souvenir",1);
			printf("NOTE: Le programme se souvient du mot %s\n",dict.entry[*index].word);
			pref->set("souvenir/input_value",I->input->value());
			pref->set("souvenir/mot",dict.entry[*index].word);
		}
	}
	delete pref;
}

//pour lancer les autres configurations
void config_autre_callback(Fl_Widget *)
{
	int centrer_fenetre;
	int souvenir;
	int hide_splash;

	//chargement du fichier de configuration
	Fl_Preferences *pref = new Fl_Preferences(PREF_ROOT,PREF_VENDOR,PREF_APP);
	pref->get("centrer_fenetre",centrer_fenetre,1);
	pref->get("souvenir",souvenir,1);
	pref->get("hide_splash",hide_splash,0);

	//coche ou dï¿½oche les cases selon les options
	autre_option_dlg->check_fenetre_centree->value(centrer_fenetre?1:0);
	autre_option_dlg->check_souvenir->value(souvenir?1:0);
	autre_option_dlg->check_hide_splash->value(hide_splash?1:0);

	//affiche la fenêtre et attends le bouton ok
	if(autre_option_dlg->show(config_window->x()+40,config_window->y()+40)) {
		//si ok
		//FENETRE CENTREE
		if(autre_option_dlg->check_fenetre_centree->value()) centrer_fenetre=1; else centrer_fenetre=0;
		if(autre_option_dlg->check_souvenir->value()) souvenir=1; else souvenir=0;
		if(autre_option_dlg->check_hide_splash->value()) hide_splash=1; else hide_splash=0;
		pref->set("centrer_fenetre",&centrer_fenetre,1);
		pref->set("souvenir",&souvenir,1);
		pref->set("hide_splash",&hide_splash,1);
	}

	delete pref;
}

//retourne 1 si hide splash = true
//son changement se fait dans config_autre
void chargement_config_hide_splash()
{
	int hide_splash=0;
	Fl_Preferences *pref = new Fl_Preferences(PREF_ROOT,PREF_VENDOR,PREF_APP);
	pref->get("hide_splash",hide_splash,0);
	_hide_splash=hide_splash?1:0;
	delete pref;
}

//classement des mots par thème (la configuration)
void classement_par_theme_click(Fl_Widget *)
{
	center_in_window(window,selection_theme_mot->MainWindow);
	
	//efface toute la sélection de thèmes
	selection_theme_mot->list->clear();
	
	int *tri;
	
	//la table pour le tri
	tri=new int[dict.theme_size];	
	for(int j=0;j<dict.theme_size;j++) tri[j]=j; 

	//création d'une liste ou il y a tous les thèmes convertis...
	char **liste = new char * [dict.theme_size];
	for(int j=0;j<dict.theme_size;j++) liste[j]=dict_to_readable_theme(dict.theme[j]);

	//maintenant procède au tri...
	for(int j=0;j<dict.theme_size;j++) {
		for(int k=0;k<dict.theme_size;k++) {
			if(tolower(cdel_accent(*liste[tri[j]]))<tolower(cdel_accent(*liste[tri[k]]))) {
				int temp = tri[j]; //un petit swap
				tri[j]=tri[k];
				tri[k]=temp;
			}
		}
	}

	delete [] liste;
		
	//mise à jour de la liste des thèmes
	char autres[]="AUTRES";
	for(short i=0;i<dict.theme_size;i++) {
		char *s;
		s=dict_to_readable_theme((*dict.theme[tri[i]])? dict.theme[tri[i]] : autres);
		selection_theme_mot->list->add(s,theme_accepte[tri[i]]);
	}
	
	//et là il affiche la fenêtre
	if(selection_theme_mot->show()) {
		//remet à jour la variable des thèmes acceptés
		for(short i=1;i<=dict.theme_size;i++) {
			theme_accepte[tri[i-1]]=selection_theme_mot->list->checked(i)?1:0;
		}

		//remet à jour la liste de mots
		I->input->value("");
		I->input->do_callback();

		//un petit reset de l'historique
		I->precedent->deactivate();
		I->suivant->deactivate();
		history_pos=0;
		dict_free_entry(&history);
		dict_init(&history);
	}

	I->input->take_focus();
	delete [] tri;
}

/************************************************
 * Affiche la liste des contributeurs
 * à Jargon Informatique
 ***********************************************/
void show_contrib_click(Fl_Widget *)
{
	center_in_window(about,contrib_window->MainWindow);
	contrib_window->show();
}

/* Gérer les signaux */
static void handle_signal(int signal)
{
#ifdef LINUX
	if(signal==SIGCHLD) {
		wait(NULL);
	}
#endif
	if(signal==SIGINT) {
		bye(); 
		exit(1);
	}
}

int main(int argc,char **argv)
{
	/* On met history à 0 */
	dict_init(&history); //important, pour remettre l'historique

	/* atexit */
	if(atexit(bye)) 
		fprintf(stderr,"Impossible d'installer atexit()...");

	/* Gestion des signaux */
	signal(SIGINT, handle_signal);
#ifdef LINUX
	signal(SIGCHLD, handle_signal);
#endif

	//initialisation de la lib des images
	fl_register_images();

	// Window principal
	I = new Interface;
	window = I->make_main_window();
	about  = I->make_about();
	news_window = I->make_news();
	config_window = I->make_config_window();
	selection_theme_mot = new SelectionThemeMot;
	selectionner_navigateur = new NavigateurDlg;
	autre_option_dlg = new AutreOptionDlg;
	selection_couleur = new SelectionCouleur;
	loading_dlg = new LoadingDlg;
	credit = new Credit;
	contrib_window = new ContribWindow;

	//petit chargements avant la fenêtre de chargement
	chargement_config_hide_splash();

	//enlève la configuration du navigateur dans la version windows
#ifdef WIN32
	{
		int y;
		y=I->config_navigateur->y();
		I->config_navigateur->hide();
		for(int i=0;i<config_window->children();i++) {
			Fl_Widget *w = config_window->child(i);
			if(w->y()>y) w->position(w->x(),w->y()-(I->config_navigateur->h()+10));
		}
		config_window->resize(config_window->x(),config_window->y(),config_window->w(),config_window->h()-(I->config_navigateur->h()+10));
	}
#endif

	//show
	window->size_range(window->w(),window->h(),window->w(),window->h());

	//chargement du dictionnaire
	chargement_couleur_debut();
	
	//chargement des dictionnaires
	creer_chemin_dictionnaire(argv[0]);
	loading(); //charge le dictionnaire principal... jargon.dic

	//callback
		//la fenêtre des news
		I->news_ok->callback(news_ok_click);
		I->show_news->callback(show_news_click);
		I->show_credit->callback(show_credit_click);
		news_list_init(); //mets les news dans la liste!
		credit_list_init();

		//about
		I->fermer_about->callback(fermer_about);
		I->about->callback(about_click);
		I->show_contrib->callback(about_click);

		//bouton options
		I->ouvrir_options->callback(ouvrir_options);
		I->fermer_options->callback(fermer_options);

		//bouton quitter
		I->quitter->callback(quitter_click);
		window->callback(quitter_click);

		//bouton input (le mot)
		I->input->callback(input_completion);
		
		//la liste
		I->list->callback(list_click);

		//exporter en html
		I->exporter_html->callback(exporter_html_click);

		//effacer
		I->effacer_mot->callback(effacer_mot_click);
		
		// output
		vider_output();
		I->output->textfont(0);
		charger_taille_police(); //maintenant c'est enregistré dans le fichier de configuration
		I->output->link(link_click);

		I->big->callback(big_click);
		I->little->callback(little_click);

		I->config_couleurs->callback(config_couleurs_click);
			
		//autres configurations
		I->config_autre->callback(config_autre_callback);

		//le bouton du classement par thème
		I->classement_par_theme->callback(classement_par_theme_click);

		//configuration du navigateur...
#ifdef LINUX
		I->config_navigateur->callback(config_navigateur_callback);
#endif
#ifdef WIN32
		I->config_navigateur->hide();
#endif

		//boutons précédent
		I->precedent->callback(precedent_click);
		I->suivant->callback(suivant_click);

		//contribution
		I->show_contrib->callback(show_contrib_click);

	//maintenant il charge le masque
	{
		theme_accepte=(char *)malloc(dict.theme_size*sizeof(char));
		if(!theme_accepte) {
			fl_alert("Pas assez de mémoire...");
			exit(1);
		}
		for(short i=0;i<dict.theme_size;i++) {
			theme_accepte[i]=1;
		}
	}

	//remplir la liste de mots
	input_completion(0); 
	
	//il se souvient du mot
	chargement_config_autre(); 

	Fl::visual(FL_RGB|FL_DOUBLE);
	Fl::background(0xEE,0xEE,0xEE);

	//On choisit le mot donné dans les arguments
#ifdef __linux__
	if(argc>1) {
		I->input->value(argv[1]);
		I->input->do_callback();
		I->list->value(1);
		I->list->do_callback();
	}
#endif

	/* Mettre les icônes */
	set_icons();

	//end callback
	//Affichage de la fenêtre principale
	char *_argv[] = {argv[0],0};
	window->show(1,_argv);

	set_icons();

	while(Fl::wait()) {
		static bool down_pressed=false;
		static bool up_pressed=false;
		//si le window principal n'est pas affiché
		//alors aucune capture ne sera faite!
		if(!window->shown()) break;

		//capture des touches
		if(Fl::first_window()==window) {
			//DOWN pour sélectionner le prochain élément dans 'list'
			//(qui contient un handle vide pour ce qui est d'FL_KEYBOARD')
			if(Fl::event_key(FL_Down)) {
				int size=I->list->size();
				int value=I->list->value();
				if(!down_pressed && value>=0 && value<size) {
					I->list->select(value+1);
					I->list->do_callback();
				}
				down_pressed=true;
				I->input->take_focus();
			}
			else
				down_pressed=false;

			//UP pour sélectionner le précédent élément dans 'list'
			//(qui contient un handle vide pour ce qui est d'FL_KEYBOARD')
			if(Fl::event_key(FL_Up)) {
				int value=I->list->value();
				if(!up_pressed && value>1) {
					I->list->select(value-1);
					I->list->do_callback();
				}
				up_pressed=true;
				I->input->take_focus();
			}
			else
				up_pressed=false;
		}
	}
	return 0;
}


