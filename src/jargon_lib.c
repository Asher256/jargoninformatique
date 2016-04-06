/*******************************************************
 * Fonctions pour lire le dictionnaire jargon
 * informatique.
 *
 * Copyrigh (c) Asher256 2004
 * <asher256@gmail.com>
 * Site: http://jargon.tuxfamily.org/
 ******************************************************
 * Format de fichier du dictonnaire:
 * ---------------------------------
 * :MOT # TYPE_DE_MOT
 * ��THEME
 * Description ............................. 
 * ......................................
 * etc.
 * jusq'� prochain deux points 
 * 
 * Note: � = ascii 164
 ******************************************************/

//==================================================================
// LICENCE:
// --------
// Cette source est distribu�e sous Licence GNU General
// Public Licence version 2 ou ult�rieure. Vous avez donc
// le droit de l'utiliser, la distribuer, l'�tudier et de 
// l'am�liorer librement. Toute modification doit �tre envoy�e
// � l'auteur afin qu'elle soit publi�e dans le site offciel. 
// Lisez le fichier COPYING.txt (distribu� avec le logiciel) 
// pour plus d'informations...
//==================================================================


//===============================
// includes
//===============================
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <ctype.h> //tolower

#include "jargon_lib.h"

#include "gstr.h"

static char *traitement_description=0; //la variable utilis�e par traiter_ligne

//dict
static int traiter_ligne(FILE *,char *ligne);

static char *html_to_text(char *string);
static char *text_to_html(char *string);

//===============================
// Defines 
//===============================
#define TRUE  1
#define FALSE 0

//======================================
// convertir un th�me en chaine 
// compr�hensible
//======================================
#define Test(a,b) if(!strcmp(t,a)) return b;
char *dict_to_readable_theme(char *t)
{
	Test("AUTRES","Autres Th�mes (non class�s)");
	Test("ADMIN","Administration syst�me");
	Test("AFFICH","Affichage");
	Test("ALGO","Algorithmie");
	Test("APPLE","Apple");
	Test("APPLI","Applications majeures");
	Test("ARCHI","Architecture des ordinateurs");
	Test("ARGOT","Argot");
	Test("ASM","Assembleur (langage machine)");
	Test("BANDE","Bandes magn�tiques");
	Test("BANQUE","Informatique des banques");
	Test("BASDON","Base de donn�es");
	Test("BOOK","Ouvrage connu");
	Test("BOOT","D�marrage d'un ordinateur");
	Test("BOX","Boitiers d'ordinateurs");
	Test("BUS","Canaux de communication (Bus)");
	Test("CABLE","Cables, Fils, Tuyaux...");
	Test("CD","Types de CD");
	Test("CHAR","Caract�res et Symboles");
	Test("CHIFFRE","Chiffres de l'informatique");
	Test("CIEL","Classe d'application");
	Test("CINE","Cinema");
	Test("CMDE","Commandes courantes (copy, ls)");
	Test("COMM","Les t�l�coms");
	Test("COMPPC","Compatible PC");
	Test("COP","Police de l'informatique");
	Test("CORP","Soci�t�s les plus renomm�es");
	Test("CRYPTO","Cryptographie");
	Test("CS","Client-Serveur");
	Test("DATE","Date et Heure");
	Test("DEBUG","D�bogage de logiciels");
	Test("DECI","Informatique d�cisionnelle");
	Test("DEMO","D�mos et effets sp�ciaux");
	Test("DISQUE","Disques et Disquettes");
	Test("DRM","Droits d'auteur");
	Test("ELECTRON","�lectronique");
	Test("EQUIPCOM","Equipement r�seaux (hors c�ble)");
	Test("EXEC","Ex�cution de programmes");
	Test("EXT","Extention de fichiers");
	Test("FEELECT","�lectricit�");
	Test("FLUXDON","Flux de donn�es");
	Test("GAG","Humour");
	Test("GESTFICH","Gestion de fichiers");
	Test("GRAPH","Graphisme");
	Test("HELP","Aide et documentation");
	Test("HISTO","Termes pr�historiques");
	Test("IMPRIM","Impression");
	Test("INDUS","Informatique industrielle");
	Test("INTART","Intelligence artificielle");
	Test("INTERNET","Internet");
	Test("INTGRAF","Interface graphique");
	Test("IRC","IRC");
	Test("JAVA","Java");
	Test("JEU","Jeux vid�os");
	Test("KEY","Clavier");
	Test("LANG","Les langages");
	Test("LANGC","Langage C");
	Test("LINUX","Linux (syst�me d'exploitation)");
	Test("LOGIQUE","La logique");
	Test("MAIL","Courrier �lectronique");
	Test("MATH","Math�matiques");
	Test("MATOS","Mat�riel Informatique");
	Test("MEM","M�moires �lectroniques");
	Test("METIER","M�tiers de l'informatique");
	Test("MOTREC","Moteur de recherche");
	Test("MSDOS","MS-DOS");
	Test("MS","Microsoft");
	Test("NET","R�seaux en Informatique");
	Test("NETATM","R�seau (ATM)");
	Test("NETNP","R�seaux (Noms)");
	Test("NOMDECODE","Nom de code");
	Test("NORM","Normes");
	Test("ORDI","Machines les plus connues");
	Test("ORG","Organisations");
	Test("OROBJ","Programmation orient�e objet");
	Test("ORTH","Orthographe");
	Test("OS","Syst�me d'exploitation");
	Test("PACK","Compression");
	Test("PAPIER","Papier");
	Test("PERIPH","P�riph�riques");
	Test("PERL","Perl (langage)");
	Test("PERS","Grands noms de l'informatique");
	Test("PHONE","T�l�phonie");
	Test("POLITCRC","Politiquement Correct");
	Test("PROG","Programmation");
	Test("PROT","Protocoles de r�seau");
	Test("PROTINET","Protocoles (internet)");
	Test("PAO","Publication Assist�e par ordinateur");
	Test("PUCE","Puces, Chips...");
	Test("RACLAT","Rachines Linguistiques");
	Test("RAMROM","M�moires (RAM, ROM)");
	Test("SALON","Salons pro ou gd public");
	Test("SECU","Securit� Informatique");
	Test("SIGN","Signaux divers");
	Test("SOC","Faits de soci�t�");
	Test("SON","Traitement de son et musique");
	Test("SPAM","Spam (pourriel)");
	Test("SPECIF","Sp�cifications");
	Test("SYSEX","Syst�mes d'exploitation et noyaux");
	Test("SYSTM","Syst�me");
	Test("TIQUE","Terme (...TIQUE ...NIQUE)");
	Test("TLD","Nom de domaine");
	Test("TM","Marque d�pos�e");
	Test("TYPFICH","Type de fichiers");
	Test("TYPE","Type de donn�es");
	Test("UM","Unit� de mesure");
	Test("UNIX","Unix");
	Test("USENET","Forums de discussion USENET");
	Test("VIRUS","Les virus les plus connus");
	Test("WARE","Shareware, freeware...");
	Test("WEB","Word Wide Web");
	Test("WIDGET","�lements d'interface graphique");
	Test("WINDOWS","Windows (syst�me)");
	Test("X","X-Window");
	Test("XML","Xml (eXtensible Markup Language)");
	Test("XXXAO","Domaines Assist�s par Ordinateur");
	Test("VIDEO","Vid�o num�rique (Animation, 3D...)");
	Test("WM","Windows Manager");
	return t;
}
#undef Test

//======================================
// libere le dernier mot
// utile pour l'historique
//======================================
void dict_pop(DICT *dict)
{
    if(dict->size) {
        dict->size--;
        free(dict->entry[dict->size].word);
        free(dict->entry[dict->size].theme_list);
    }
}

//======================================
// stack
//======================================

//true_size va devenir = size
//(pour gagner un peu de place en m�m)
void dict_fix_true_size(DICT *dict)
{
	DICT_ENTRY *save;
	dict->entry = (DICT_ENTRY *)realloc(save=dict->entry, (dict->size)*sizeof(DICT_ENTRY));
	if(!dict->entry) {
		dict->entry=save;
		return;
	}
	dict->true_size = dict->size;
}

//incr�mente le true size de 'n' valeur
//nonz si erreur
int dict_add_true_size(DICT *dict, int n)
{
	DICT_ENTRY *save;
	if(n<0) return 0; //pas la peine puisque c'est une DEC
	dict->entry = (DICT_ENTRY *)realloc(save=dict->entry, (n+dict->true_size)*sizeof(DICT_ENTRY));
	if(!dict->entry) {
		dict->entry=save;
		return 1;
	}
	dict->true_size+=n; //applique cela � la fonction
	return 0;	
}

//initialise une variable DICT en y mettent
//des z�ros
void dict_init(DICT *list)
{
	memset(list,0,sizeof(DICT));
}

//pour ajouter un th�me � la liste des themes
//generaux
//retourne -1 si outmem
//retourne l'index du theme si c ok !
short dict_add_theme(DICT *d, const char *theme)
{
		char **save=d->theme;
		int i;
		//il cherche avant tout si ce theme existe deja
		for(i=0;i<d->theme_size;i++) {
				if(!strcmp(theme,d->theme[i])) {
						return i; //ce th�me existe d�j� :-)
				}
		}
		//le th�me n'existe pas! donc il va l'ajouter :-)
		d->theme=(char **)realloc(save=d->theme,(d->theme_size+1)*sizeof(char *));
		if(!d->theme) {
				d->theme=save;
				return -1;
		}
		//pas d'allocation possible?
		if(!(d->theme[d->theme_size]=strdup(theme))) 
				return -1; //erreur :-(

		d->theme_size++; //et enfin, prends en compte les modifications :-)
		return d->theme_size-1;
}

//ajouter un string + description dans le dictionnaire
//
// word doit �tre strdup()
//
// 'theme' la table ou il y a tous les th�mes de ce mot
// ATTENTION, cette table doit �tre alloqu�e par malloc
// car elle va �tre d�salloqu�e par la suite...
// elle doit finir par -1 
int dict_push(DICT *dict, char *word, FILE *handle, long offset, short *theme)
{
		int i = dict->size; //la nouvelle position

		if(!word) return 0;

		//si cela d�passe la 'cache' reserv�e
		if(i+1>dict->true_size) {
			//s'il y a erreur d'ajout... BYE
			if(dict_add_true_size(dict,200))
				return 1;
			//S'il n'y a aucune erreur...
			else
				dict->size++;
		}
		else
			dict->size++;

		dict->entry[i].word = word;
		dict->entry[i].offset=offset;
		dict->entry[i].handle=handle;
		dict->entry[i].theme_list=theme;

		return 0;
}

//d�sallocation de tout ce qu'il y a dans entry... c tout.
void dict_free_entry(DICT *dict)
{
		int i;
		if(dict->entry) {
				for(i=0;i<dict->size;i++) {
					free(dict->entry[i].word);
					free(dict->entry[i].theme_list);
				}
				free(dict->entry);
				dict->true_size=0;
				dict->entry=0;
		}
		dict->size=0;
}

//====================================================
// Charger le dictionnaire en m�moire...
//====================================================
static int premiere_fois=1;
DICT dict; /* il est memset 0 dans load_dict */
#define buffer_size 1024
static char buffer[buffer_size];

//cette fonction lit une ligne dans
//le fichier et met le tout dans la variable
//buffer.
//retourne nonz si fin de fichier...
static size_t caracteres_lus=0;

static int lire_ligne(FILE *handle)
{
		static size_t len;
		if(!fgets(buffer,buffer_size,handle)) return 1;
		len=strlen(buffer);
		caracteres_lus=len;
		if(len>0 && buffer[len-1]=='\n') buffer[len-1]=0;
		return 0;
}

//retourne la description... depuis le fichier bien s�re! 
//(pour que �a soit rapide et �a consomme moins de m�moire).
char *dict_get_description(int index) //la description de quel mot?
{
		long offset = dict.entry[index].offset;
		char *description=0;

		traitement_description=0;

		//se met automatiquement � la position voulue...
		fseek(dict.entry[index].handle,offset,SEEK_SET);

		//initialise la description compl�te (celle qu'on va retourner)
		//ajout du mot dans la description
		char *mot = gs_replace_m(dict.entry[index].word,"<","&lt;"); if(!mot) return 0;
		mot = gs_replace_r(mot,">","&gt;"); if(!mot) return 0;

		description=strdup("<i>");
		description=gs_strcat_r(description,mot); /* ajout et */ free(mot); /* d�sallocation */
		description=gs_strcat_r(description,"</i><p>");
		if(!description) return 0;

		//lit la description...
		while(!lire_ligne(dict.entry[index].handle) && !traitement_description) {
				if(traiter_ligne(dict.entry[index].handle,buffer)) break;
		}

		//free quelques variables dans traiter_ligne
		traiter_ligne(0,0);

		//ici il ajoute le tout!
		if(traitement_description) {
				description = gs_strcat_r(description,traitement_description);
				free(traitement_description);
				traitement_description=0;
				if(!description) { free(traitement_description); return description; }
		}
		else
				return strdup("Ne trouve pas la description...");

		free(traitement_description);
		traitement_description=0;

		return description;
}

//ajouter un handle dans la liste des handles ouverts
int dict_add_handle(FILE *_handle)
{
		FILE **save;
		dict.handles = (FILE **)realloc(save=dict.handles,sizeof(FILE *)*(dict.n_handles+1));
		if(!dict.handles) {
				dict.handles=save;
				return 1; //err
		}
		dict.handles[dict.n_handles]=_handle;
		dict.n_handles++;
		return 0;
}

//Charger un dictionnaire en m�moire dans la variable DICT dict (statique)
//cette fonction a besoin d'un filename et une fonction
//qui est appel�e � chaque incr�mentation du pourcentage.
int load_dict(const char *filename, void (*percent)(int percent))
{
	FILE *handle;
	long file_size;
	int one_percent=0; //quand on d�passe 1 pourcent...
	int count_percent=0; //une fois que cette ci == one_percent alors appeler percent
	int actual_percent=0;
	long offset=0; //num�ro de ligne
	long add_to_offset;
	long old_offset=0;

	if(premiere_fois) {
		premiere_fois=0;
		memset(&dict,0,sizeof(DICT));
	}

	if(percent) (*percent)(0);

	//ouvre le handle
	handle = fopen(filename,"r");
	if(!handle) return DICT_E_FOPEN;

	//ajoute le handle dans la liste des handles
	if(dict_add_handle(handle)) {
		fclose(handle);
		return DICT_E_OUTMEM;
	}

	//calcule le size du fichier...
	fseek(handle,0,SEEK_END);
	file_size=ftell(handle);
	rewind(handle);

	//mets les valeurs pour le calcul du pourcentage
	//rapide
	one_percent=(int)(file_size/100);

	//l� il fait un ajout de 10000 au d�but (optimiz realloc)
	if(dict.true_size<10100) dict_add_true_size(&dict,10100);

	//parcours le fichier...
	while(!feof(handle)) {
		//lecture d'une ligne depuis le fichier!
		if(lire_ligne(handle)) break; 

		add_to_offset=caracteres_lus;

		//s'il d�tecte un mot!
		if(buffer[0]==':') {
			static char *p;
			char *diaise;
			char *mot;
			short *n_theme=0; //la liste des num�ro de th�mes (l'appartenance :-))
			short n_theme_size=0;
			int type_de_mot_trouve=0; //utilis�e dans la boucle

			gs_rtrim(buffer);
			diaise = strrchr(buffer,'#');
			if(diaise) *diaise=0;
			//fait les transformations sur le mot
			mot=strdup(html_to_text(buffer)); if(!mot) return DICT_E_OUTMEM;
			mot=gs_replace_r(mot,"�","oe"); if(!mot) return DICT_E_OUTMEM;
			//maintenant l'ajoute! +1 pour d�passer le :
			p=strdup(mot+1);
			free(mot);
			if(!p) {continue;}
			gs_trim(p); //le mot doit �tre trim�, car j'ai eu le souci avec les mots comme coboliste qui pointe vers COBOL
			//l� il d�tecte le th�me et l'ajoute dans la liste

			type_de_mot_trouve=1; //il a trouve le type de mot! (toujours au d�but)

			//ici, il va parcourir ligne par ligne la description du mot pour r�colter tous les th�mes :-)
			while(1) {
				if(!type_de_mot_trouve && buffer[0]=='#') {
					char *s=buffer+1;
					//cherche le chiffre...
					do{ s++; }while(*s && *s<'0' && *s>'9');
					//il l'a trouv�??
					if(*s>='0' && *s<='9') {
						type_de_mot_trouve=1;
						continue;
					}
				} //SUPER un type :-)

				if(lire_ligne(handle)) break;  //lire une ligne en esp�rant qu'elle n'est pas finie

				add_to_offset+=caracteres_lus; //mise � jour de la variable
				//voit s'il a trouv� un type de mot selon cette ligne actuelle...
				gs_ltrim(buffer);
				if(!*buffer) break; //s'il ne trouve pas un type de mot....

				//MAINTENANT COMMENCE LA PROCEDURE!!
				if(type_de_mot_trouve) {
					type_de_mot_trouve=0; //annule �a bien s�re! car dans la prochaine ligne il doit y avoir le truc
					if(strncmp(buffer,"��",2)==0) {
						char *fin,*debut=buffer;
						while((debut=strstr(debut,"��"))) {
							debut+=2;
							fin=strstr(debut,"��"); //si pas de prochain carac alors pas de nouveau th�me

							if(fin) *fin=0;

							//enleve les espaces de gauche et de droite
							gs_trim(debut);

							//ici il ajoute le th�me � la liste :-)
							if(*debut) {
								short n=dict_add_theme(&dict,debut);

								if(n!=-1) {
									short *save;
									n_theme=(short *)realloc(save=n_theme,sizeof(short)*(n_theme_size+2));
									if(!n_theme) {
										n_theme=save; //comme si rien n'�tait
									}
									else {
										n_theme[n_theme_size]=n;
										n_theme[n_theme_size+1]=-1;
										n_theme_size+=1;
									}
								}
							}

							//puis, il sait que c'est bon, on a fini :-)
							if(!fin) break;
							else *fin='�';
						}

					}
					else if(*buffer=='#') {
						//ici, il n'a pas trouv� les deux euros...mais il se peut
						//que la boucle soit dans le premier type de mot par exemple:
						// :mot  [ya pas de type ici]
						// # 1. ici y a le type
						// description
						//comme par exemple pour le mot @+
						continue;
					}
				}
			} 

			//si aucun th�me pour le mot...
			if(n_theme==0) {
				short n=dict_add_theme(&dict,"");
				n_theme=(short *)malloc(sizeof(short)*2);
				if(n_theme) {
					n_theme[0]=n;
					n_theme[1]=-1;
				}
			}
			//et ajoute le mot :-)
			//p est alloqu� (donc bien ajout� comme il faut)
			if(dict_push(&dict,p,handle,offset,n_theme)) {
				free(p);
				return DICT_E_OUTMEM; 
			}
		}

		//ajoute tous les caract�res lus!
		offset+=add_to_offset;

		//fait le pourcentage
		if(percent) {
			count_percent+=offset-old_offset; old_offset=offset;
			if(count_percent>=one_percent) {
				count_percent=0;
				(*percent)((int)++actual_percent);
			}
		}
	}

	traiter_ligne(0,0); //free ce qui est rest� en m�moire,
	//on ne sait jamais si le dictionnaire contient des
	//erreurs de syntaxe et que des choses ne se d�salloquent pas...

	//le dernier traitement pourcentage!
	if(percent) (*percent)(100);

	dict_fix_true_size(&dict); //pour gagner de la place en m�moire

	return 0;
}

//fermeture avec gestion de handles
void close_dict()
{
	//enl�ve tous les noms des th�mes g�n�raux...
	if(dict.theme_size) {
			int i;
			for(i=0;i<dict.theme_size;i++) {
					free(dict.theme[i]);
			}
			free(dict.theme); dict.theme=0;
			dict.theme_size=0;
	}
	//ferme tous les fichiers ouverts
	if(dict.n_handles) { 
			int i;
			for(i=0;i<dict.n_handles;i++) 
					fclose(dict.handles[i]);
			free(dict.handles); dict.handles=0;
			dict.n_handles=0;
	}
	//d�salloque la liste de mots
	dict_free_entry(&dict); 
}

//============================================================
// Traiter une ligne lue dans le fichier, cette
// fonction lis le nom, puis la description, etc.
// quand elle sait qu'elle a finie de collecter
// les donn�es d'une seule entr�e, elle ajoute cette entr�e 
// dans le dictionnaire.
//
// si on passe NULL � la fonction on lui dit de 
// faire un free � quelques variables pas encore
// free. Et d'ajouter les variables pas encore
// ajout�es...
//============================================================
#define F(p) {if(p) {free(p);p=0;}}
#define free_all() {F(description);}
#define test_mem(p) {if(!p) { free_all(); return DICT_E_OUTMEM; }}

//�a c'est pour les liens entre les euros qui sont
//apr�s les images
//ce que fait cette fonctione c'est qu'elle utilise ligne
//pour lire son contenu et puis ajoute �a dans description
static int _traiter_euro(char *ligne, char **description)
{
	char *prochain, *precedent=ligne;
	int euro_trouve=0;
	*description = *description?*description:strdup("");
	while((prochain=strchr(precedent,164))) {
		*prochain=0; prochain++;
		//premier euro
		if(!euro_trouve) {
			euro_trouve=1;
			*description = gs_strcat_r(*description,precedent);
			*description = gs_strcat_r(*description,"<b><a href=\"word:");
		}
		else {
			*description = gs_strcat_r(*description,precedent);
			*description = gs_strcat_r(*description,"\">");
			*description = gs_strcat_r(*description,precedent);
			*description = gs_strcat_r(*description,"</a></b>");
			euro_trouve=0;
		}
		if(!*description) return DICT_E_OUTMEM;
		*(prochain-1)=164;
		precedent=prochain;
	}
	*description = gs_strcat_r(*description,precedent);
	return 0;
}

static int traiter_ligne(FILE *handle, char *ligne)
{
	static int mot=0;
	static char *description=0;
	static int euro_trouve=0; //s'il trouve un euro alors true 
	//s'il trouve encore alors false...
	size_t len;

debut:
	//pour la mise en forme des noms entre les euros
	if(!description) euro_trouve=0;

	//si on veux FREE
	//alors paser null ds ligne
	if(!ligne) {
		euro_trouve=0;
		if(mot && description) 
			goto ajouter; //si l'on doit ajouter
		else
			free_all();
		return DICT_OK;
	}

	gs_trim(ligne);
	len = strlen(ligne);

	//si ligne sans aucun autre caract�re, alors cela veut 
	//dire ajouter le tout!
	if(!*ligne) { //si ls carac != espace >0 alors ajout!
		if(mot && description) {
ajouter:
			description = description?text_to_html(description):strdup("");
			if(!description) {
				mot=0; description=0;
				free_all();
				return DICT_E_OUTMEM;
			}

			//avant d'ajouter description
			//on va remplacer tous les \n entre <tt></tt>
			//par des <br>
			{
				char *new_description=strdup("");
				char *p=description;
				char *old_p;
				char save_c;
				while(p) {
					old_p=p;//save avant qu'il change
					//cherche <tt>
					while(*p) { if(strncasecmp(p,"<tt>",4)==0) break; p++; }
					if(!*p) {
						new_description=gs_strcat_r(new_description,old_p);
						test_mem(new_description);
						break;
					}

					//ajoute ce qu'il y a entre old_p et p
					p+=4; //d�passe le <tt> pour l'ajouter avant ce qu'il y a avant!
					{
						save_c=*p; *p=0;
						new_description = gs_strcat_r(new_description,old_p);
						*p=save_c;
					}

					//si on est devant une tt?
					//l� on remplace!
					while(*p && strncasecmp(p,"</tt>",5)) {
						if(*p=='\n') 
							new_description=gs_strcat_r(new_description,"<br>"); 
						else {
							char s[2] = { *p, 0};
							new_description=gs_strcat_r(new_description,s);
						}
						test_mem(new_description); 
						p++;
					}
				}

				free(description);
				description=new_description;
			}

			//ICI LE PUSH 
			traitement_description=description; //met la description sans la strdup
			mot=0; description=0;
			return DICT_OK;
		}
		//ne fait rien et continue � ajouter le tout
		//dans la description...
		return DICT_OK;
	}

	// s'il a d�j� trouv� un mot
	// alors cela ne sera qu'une description!
	// Description!
	else if(mot){
		description = (!description)?strdup(""):description;

		//d�tecte la date et fait un peu le malin pour voir si apr�s ( il y a un chiffre
		if(len>=4 && ligne[0]=='(' && (ligne[1]>='0' && ligne[1]<='9') && (ligne[2]>='0' && ligne[2]<='9') && (ligne[3]=='-')) {
			description = gs_strcat_r(description?description:strdup(""), "<p><b><i>"); 
			description = gs_strcat_r(description, ligne);
			description = gs_strcat_r(description, "</i></b><br>\n"); 

			description = gs_replace_r(description,"�","oe");

			test_mem(description);

			return DICT_OK;
		}

		//une liste a puces, alors fait un petit <br> pour �viter deux puces dans la m�me ligne...
		if(*ligne=='*') { description = gs_strcat_r(description?description:strdup(""), "<br>"); }

		//si url
		if(strncmp(ligne,"<URL:",4)==0) {
			char *url;
			char *apres;

			//mets dans url le lien...
			url=ligne+5;

			//cherche la fin de l'url pour laisser la suite dans ligne
			apres=strchr(url,'>');
			if(apres) { 
				*apres=0;
				apres++; 
			}
			else
				apres="";

			//ici le traitement de la ligne avec conversion en HTML!
			description = gs_strcat_r(description?description:strdup(""), "<p><b><a href=\""); 
			description = gs_strcat_r(description, url);
			description = gs_strcat_r(description,"\">"); 
			description = gs_strcat_r(description, url);
			description = gs_strcat_r(description,"</a></b> "); 
			//annul� pour mettre � la place _traiter_euro: 
			if(_traiter_euro(apres,&description)) return DICT_E_OUTMEM;
			//description = gs_strcat_r(description,apres);
			description = gs_strcat_r(description,"<br>");
			description = gs_replace_r(description,"�","oe");
			test_mem(description);
			return DICT_OK;
		}

		//si warning
		if(strncmp(ligne,"��warn",6)==0) {
			ligne+=6; //d�passe le warn
		}

		//ANNULER int�gr� avec la parser -- si figure (ancien format)
		if(strncmp(ligne,"��fig",5)==0) {
			//mets un 0 dans |
			char *separateur;
			ligne+=5;
			separateur=strchr(ligne,'|');
			if(separateur) *separateur=0;

			//mets la figure!
			description = gs_strcat_r(description?description:strdup(""),"<p><img src=\"");
			description = gs_strcat_r(description,ligne);
			description = gs_strcat_r(description,"\"><br>");

			//remet le sep
			if(separateur) {
				*separateur='|';
				//met ce qu'il y a apr�s le fig...
				//annuler au profit de _traiter_euro pour int�grer les liens des les images<< description = gs_strcat_r(description,separateur+1);
				if(_traiter_euro(separateur+1,&description)) return DICT_E_OUTMEM;
			}

			test_mem(description);
			//traitement des images...

			//il ignore les figures, pour l'instant
			return DICT_OK;
		}

		//nouveau format de FIG
		//au format
		// <fig>fichier.png|description</fig>
		if(strncmp(ligne,"<fig>",5)==0) {
			char *copie_ligne = strdup(ligne);
			char *zero = strstr(copie_ligne,"</fig>"),save;
			if(!copie_ligne) return DICT_E_OUTMEM;

			//s'il ne le trouve pas dans cette ligne
			//alors cherche dans la prochaine version
			if(!zero) {
				if(lire_ligne(handle)) {
					free(copie_ligne);
					return DICT_OK; //si derni�re ligne alors bye.
				}
				zero = strstr(ligne,"</fig>");
				if(zero) { //IL A TROUVE UN /fig!
					copie_ligne = gs_strcat_r(copie_ligne," ");
					copie_ligne = gs_strcat_r(copie_ligne,ligne);
					zero = strstr(copie_ligne,"</fig>");
				}
			}

			if(zero) {
				char *zero2 = strstr(copie_ligne,"|"),save2;
				save=*zero;
				*zero=0;
				//ici il va s�parer le nom du fichier image
				if(zero2) {
					save2=*zero2; *zero2=0;

					//ici il va convertir le nouveau format � l'ancien :-)
					//sans oublier que ceux qui contiennent .. sont suppos�s �tres bons...
					if(strncmp(copie_ligne,"<fig>../",8)==0) {
						description = gs_strcat_r(description?description:strdup(""), "<p><img src=\""); 
					}
					//ceux qui ne contiennent pas .. sont imp�rativement des fig
					else
						description = gs_strcat_r(description?description:strdup(""), "<p><img src=\"../fig/"); 

					//et la suite
					description = gs_strcat_r(description,copie_ligne+5); // +5 = d�passer <fig>
					description = gs_strcat_r(description,"\"><br>");
					//au lieu de �a je vais traiter les euros... description = gs_strcat_r(description,zero2+1); //ce qu'il y a apr�s |
					if(_traiter_euro(zero2+1,&description)) return DICT_E_OUTMEM;
					description = gs_strcat_r(description,"<br>"); //de pr�f�rence pour le formattage

					*zero2=save2;
				}
				*zero=save; //bon c'est inutile mais je pr�f�re le mettre

				//bon... reprends l� ou il �tait :-) � la fin de la balise fig...
				zero = strstr(ligne,"</fig>");
				ligne=zero+6; //d�passe le </fig>
			}

			free(copie_ligne);
		}

		//enleve warn
		gs_replace(ligne,"<br><br>","<p>");
		gs_replace(ligne,"<br>","<p>");

		//mise � jour du len car il peux �tre
		//modifi� dans gs_replace... (comme dans <br><br> par <p>)
		len = strlen(ligne);

		// **THEME
		// *=carac sp�cial
		// apr�s le mot...
		if(strncmp(ligne,"��",2)==0)
			//{ 
			//	ligne+=2; MD
			//	gs_replace(ligne, "��", ", ");
		{
			char *tmp, *a;

			description = gs_strcat_r(description?description:strdup(""), "<b><i>"); 

			//ajoute les themes
			tmp=strdup(ligne+2);
			a=strtok(tmp,"�");
			do {    /* pour chaque theme trouv� */
				if(!a) break; //pour �viter un bug (Erreur de segmentation) quand il n'y a pas de th�me
				if(a!=tmp) // si pas le premier
					description=gs_strcat_r(description,", ");
				description = gs_strcat_r(description,dict_to_readable_theme(a));
			} while((a=strtok(0,"�")));
			free(tmp);

			description = gs_strcat_r(description, "</i></b><p>\n\n"); 
			test_mem(description);

			return DICT_OK;
		}

		//comme �a on est s�re qu'il n'y a pas de NULL
		if(!description) description=strdup("");

		//si #1, #2 etc
		//ligne contenant le type du mot
		//comme par exemple # .en .np .m
		if(*ligne=='#') {
			description=gs_strcat_r(description,"<p><b>");
			gs_replace(ligne,"��","");
			description=gs_strcat_r(description,ligne);
			description=gs_strcat_r(description,"</b><br>");
			test_mem(description);
			return DICT_OK;
		}

		//remise en forme... des caract�res entre les euros
		{
			char *prochain, *precedent=ligne;
			while((prochain=strchr(precedent,164))) {
				*prochain=0; prochain++;
				//premier euro
				if(!euro_trouve) {
					euro_trouve=1;
					description = gs_strcat_r(description,precedent);
					description = gs_strcat_r(description,"<b><a href=\"word:");
				}
				else {
					description = gs_strcat_r(description,precedent);
					description = gs_strcat_r(description,"\">");
					description = gs_strcat_r(description,precedent);
					description = gs_strcat_r(description,"</a></b>");
					euro_trouve=0;
				}
				test_mem(description);
				*(prochain-1)=164;
				precedent=prochain;
			}

			//fin et dernier euro non trouve?
			if(euro_trouve) {
				char *debut = strdup(precedent);
				euro_trouve=0; //dans tous les cas. on suppose que le prochain euro est trouv�!
				debut = gs_strcat_r(debut," ");
				if(!debut) return DICT_E_OUTMEM;
				if(lire_ligne(handle)) return DICT_OK; //si derni�re ligne alors bye.
				prochain = strchr(buffer,164); //cherche le prochain 164
				if(!prochain) {
					//word vide veut rien dire!
					description = gs_strcat_r(description,"\"></a></b>"); //lien vide!
					description = gs_strcat_r(description,debut);
					test_mem(description);
					free(debut);
					ligne=buffer;
					goto debut; //traiter la nouvelle ligne normalement
				}
				//s'il a trouv� le prochain !
				*prochain = 0;

				//ajoute le tout dans la description
				description = gs_strcat_r(description,debut);
				description = gs_strcat_r(description,buffer);
				description = gs_strcat_r(description,"\">");
				description = gs_strcat_r(description,debut);
				description = gs_strcat_r(description,buffer);
				description = gs_strcat_r(description,"&nbsp;"); //avec un br � la fin
				description = gs_strcat_r(description,"</a></b>"); 

				//fin!
				*prochain=164;
				free(debut);
				ligne=prochain+1; 
				goto debut; //parser ce qui reste !
			}

			//ajoute la fin normalement...
			else
				description = gs_strcat_r(description,precedent);
		}

		//ajoute un espace � la fin et retour � la ligne
		description = gs_strcat_r(description, "\n"); 
		test_mem(description);

		// Remplacement des autres caract�res...
		// comme le o par oe
		description = gs_replace_r(description,"�","oe");

		return DICT_OK;
	}

	// Nouveau mot!
	else if(ligne[0]==':') {
		free_all(); //supprimer toutes les vars locales
		mot=1;
		return DICT_OK;
	}

	return DICT_OK;
}
#undef F
#undef free_all
#undef test_mem

//====================================================
// Quelques fonctions pour string
// utiles car utilise resize
//====================================================

//enlever les trucs html
//ceci est utilis� dans : les noms
static char *html_to_text(char *string)
{
	gs_replace(string,"&amp;","&");
	gs_replace(string,"&lt;","<");
	gs_replace(string,"&gt;",">");
	gs_replace(string,"&nbsp;"," ");
	return string;
}

//enlever les trucs non html surtout les accents
//qui posent probl�me sous windows
//ceci est utilis� dans la description
//
//string doit �tre une chaine realloc
//elle retourne soit string realloqu� avec
//toutes les replaces soit 0 qui veut dire pas
//assez de m�moire
static char *text_to_html(char *string) 
{
		if(!string) return string;
		string = gs_replace_r(string, "�", "&eacute;");
		string = gs_replace_r(string, "�", "&egrave;");
		string = gs_replace_r(string, "�", "&ecirc;");
		string = gs_replace_r(string, "�", "&agrave;");
		string = gs_replace_r(string, "�", "&acirc;");
		string = gs_replace_r(string, "�", "&ucirc;");
		string = gs_replace_r(string, "�", "&ocirc;");
		string = gs_replace_r(string, "�", "&icirc;");
		string = gs_replace_r(string, "�", "&ccedil;");
		return string;
}


