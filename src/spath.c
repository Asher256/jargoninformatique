//===================================================================
// Librairie pour la gestion des chemins portable entre 
// windows (les anti slash) et les unix (avec des SLASH).
//
// Auteur: Asher256
// Email:  asher256@gmail.com
//===================================================================
//-------------
// Licence:
//-------------
// Cette source est distribuée sous licence
// GNU General Public Licence version 2.
//
// Vous avez le droit de la distribuer, 
// l'étudier ou l'améliorer tant que le 
// code source reste libre et conforme à
// sa licence.
//
// Il n'y a aucune garantie quand au 
// fonctionnenent de ses routines.
//
//===================================================================
//-------------
// NOTE:
//-------------
//   Pour utiliser ce code source il  faut #definir soit
//   LINUX ou WIN32.
//
//   LINUX peut être définit pour tous 
//   les types d'UNIX comme darwin de MacOSX.
//===================================================================
//-------------
// Historique:
//-------------
// 0.1 (Août 2004)
//     * Création des fonctions de base
// 0.2 (Août 2005)
//     * sp_which(): ajout d'un free(path) qui manquait dans un
//       return...
//     * ajout d'une petite fonction de sp_pathcat qui permet 
//       d'éliminer les slash de fin de path1
//     * Dans sp_executable_path() fixation de certaines free(PATH)
//       oubliés.
//     * sp_pwd(): fixation du risque de boucle infinie
//       et ajout d'économie mémoire avec un realloc à la fin.
//     * sp_truepath(): correction de certains petits détails
//       comme des free() et adjustement de certaines fonctions.
//     * Création de nouvelle fonction: sp_pwd_drive()
//     * sp_basename retourne le pointeur vers path au lieu
//       d'une adresse mémoire contenue dans path+x
//     * Correction d'un bug dans sp_basename qui retournait
//       une chaine vide si le chemin est déjà un basename
//     * Ajout d'une nouvelle fonction sp_basename_p qui
//       fait comme l'ancienne sp_basename.
//     * Création d'une nouvelle fonction: sp_extname_p
//     * sp_fix_slash_repeated() nouvelle fonction qui en 
//       profite pour mettre un system slash avec la fixation
//       du slash repeated.
//     * sp_wich() est maintenant compatible avec le %PATH% de
//       windows
//     * Remplacement de toutes les strdup par STRDUP une #define
//       facilement changeable.
//===================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "spath.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef WIN32
#include <io.h> //chdir
#endif

// TOUT: LINUX
#ifdef __linux__
#include <linux/unistd.h> //pour getcwd (car sous mac/darwin ça n'existe pas...)
#endif

//TODO ajouter les lecteurs
//


//===================================================================
// Fonctionne comme la ligne de commande *nix which.
//
// Si par exemple 'commande' est égale à "mozilla"
// il cherche dans $PATH pour retourner son vrai
// chemin "/usr/bin/mozilla".
//
// Retour:
// -------
// Le résultat retourné est un pointeur alloqué contenant
// le chemin complet du fichier exécutable.
//
// La fonction retourne strdup("") si la commande n'a pas
// été trouvé.
//
// La fonction retourne NULL s'il n'y a pas assez de mémoire.
//===================================================================
//OK
#ifdef WIN32
#define DEUX_POINTS ";"
#endif
#ifdef LINUX
#define DEUX_POINTS ":"
#endif
char *sp_which(char *commande)
{
	char *PATH;     //contient $PATH
	char *s;        //petit pointeur de service :-)
#ifdef LINUX
	SP_STAT info;
#endif

	PATH = getenv("PATH");
	if(!PATH) return strdup(""); //eh oui! pas de path alors... pas de recherche!

	//maintenant fait une copie en forme de DUP puisque la variable path
	//n'est pas alloquée, comme ça il va pouvoir modifier dessus certaines
	//choses avec strtok...
	PATH = strdup(PATH);
	if(!PATH) return NULL;

	//pacourt PATH à la recherche de la commande
	s = strtok(PATH,DEUX_POINTS);
	while(s) {
		char *path = (char *)malloc(strlen(s)+1+strlen(commande)+1);
		if(!path) { free(PATH); return NULL; }
		
		//crée le chemin :-)
		strcpy(path,s);
		strcat(path,SLASH);
		strcat(path,commande);

		//Ici pas la peine de faire exists() puisque is_exec
		//fait l'affaire, un fichier exécutable implique qu'il
		//soit un fichier quand même :-)
#ifdef LINUX
		if(sp_stat(path,&info) && sp_isexec(&info)) { 
#endif
#ifdef WIN32
		if(sp_file_exists(path)) {
#endif
			commande=(char *)malloc(strlen(path)+1);
			if(!commande) { free(path); free(PATH); return NULL; } //pas de mémoire = pas trouvï¿½(peu de chance que cela se produise)
			strcpy(commande,path);
			free(path); free(PATH);
			return commande; //trouvé !! donc un nouveau malloc (il ne touche pas à commande évidement!)
		}
		
		// autre strtok
		free(path);

		//cherche dans le prochain chemin dans path...
		s = strtok(0,DEUX_POINTS);
	}

	free(PATH);//les bonnes habitudes
	return strdup(""); //rien n'a été trouvé...
}
#undef DEUX_POINTS

//===================================================================
// Ressembler deux chemins puis retourner le résultat en forme
// de chaine malloc.
//
// Retourne NULL si pas assez de mémoire.
//
// NOTE: il met le slash ou l'antislash selon le système bien 
//       sûre.
//
// NOTE: si path1 finit par un / ou \ alors il ne le mets pas.
//
//===================================================================
//OK
char *sp_pathcat(const char *path1, const char *path2)
{
		const char *end;
		char *resultat;
		size_t len_path1;

		//le len
		len_path1 = strlen(path1);

		//il va prendre la fin du string dans end...
		end = path1+len_path1;

		//calcule combient il y a de slash à la fin...
		while(end>path1) {
			end--;
			if(!isslash2(*end)) break;
			len_path1--; //efface ces caractères... indirectement.
		}

		//allocation de resultat
		resultat = (char *)malloc(len_path1+1+strlen(path2)+1);
		if(!resultat) return resultat;

		//printf
		strncpy(resultat,path1,len_path1); resultat[len_path1]=0;
		if(*end && !isslash2(*end)) strcat(resultat,SLASH);
		strcat(resultat,path2);
		return resultat;
}

//===================================================================
// Retourne nonz si le fichier ou répertoire 'filename' existe.
//===================================================================
//OK
int sp_exists(const char *filename)
{
	SP_STAT stat;
	if(sp_stat(filename,&stat)) return -1; //il existe!
	return 0;
}

//===================================================================
// Retourne nonz si le fichier 'filename' existe.
//===================================================================
//OK
int sp_file_exists(const char *filename)
{
	SP_STAT stat;
	if(sp_stat(filename,&stat) && sp_isreg(&stat)) return -1;
	return 0;
}

//===================================================================
// Retourne le nom du fichier exécutable en utilisant
// ce qui est passé à argv0.
//
// "" (du vide alloqué) Si pas trouvé
// 0  Si pas assez de mémoire
//===================================================================
//OK
char *sp_executable_path(const char *argv0)
{
		char *PATH;
		char *s;
		#ifdef WIN32
		char *separateur=";";
		#else
		char *separateur=":";
		#endif
		
#ifdef WIN32
		//sous windows, c'est le répertoire actuel qui est utilisé...
		//s'il existe sans aucune retouche... alors :-)...
		//VOIR COMMENT SOUS WINDOWS TESTER S'IL EXISTE 
		if(sp_exists(argv0)) return sp_truepath(argv0);
#endif

		//est-ce un chemin relatif?
		if(
		//si chemin relatif du répertoire précédent	
		(strncmp(argv0,"..",2)==0 && isslash2(argv0[2])) ||
	 	
		//OU si chemin relatif du répertoire actuel
		(strncmp(argv0,".",1)==0 && isslash2(argv0[1])) ||

		//ou peu être une racine...
		//si chemin absolu /home/asher256 ou \program files\Jargoninformatique
		isslash2(argv0[0]) 

		//pour WINDOWS seulement, si chemin absolu avec lecteur
		#ifdef WIN32
		   //si lecteur C:\ par exemple
		   || (strlen(argv0)>=3 && (
			//soit c un lecteur
			(argv0[1]==':' && isalpha(argv0[0]) && isslash2(argv0[2])) ||
			//soit c'est une racine
			(isslash2(argv0[0]))
			)
		   )
		#endif
		  ) {
			//retourne ce chemin complet
			return sp_truepath(argv0);
		}
		
		//sinon, il faut chercher dans path!
		PATH = getenv("PATH");
		if(!PATH) return strdup(""); //pas de path. alors pas trouvé d'avant.
		PATH = strdup(PATH);
		if(!PATH) return 0; //ici pas assez de mémoire!

		//ici il cherche dans path
		s = strtok(PATH,separateur);
		while(s) {
			static char *path2;
			path2 = (char *)malloc(strlen(s)+1+strlen(argv0)+1);
			if(!path2) {
				free(PATH);
				return 0; //pas assez de mémoire...
			}

			//création du path complet
			strcpy(path2,s);
			strcpy(path2,SLASH);
			strcpy(path2,argv0);

			//il doit exister bien sûre!
			if(sp_exists(path2)) {
				static char *path;
				//conv path2 en truepath...
				path = sp_truepath(path2);
				free(path2); //bye!
				if(!path) {
					free(PATH);
					return 0;
				}
				//il teste si le chemin est bon...
				if(*path) {
					free(PATH);
					return path;
				}

				//sinon, c pas bon...
				//il cherche autre chose...
				free(path);
			}
			else 
				free(path2);
			s=strtok(0,separateur);
		}

		free(PATH);
		return strdup("");
}

//===================================================================
// Retourne le chemin actuel dans un drive...
//
// Fonction disponible sous windows seulement.
//===================================================================
//OK REV
#ifdef WIN32
char *sp_pwd_drive(char drive)
{
	static char true_drive[3]="C:"; //par exemple :-)
	char *pwd,*pwd2;
	true_drive[0]=drive;

	//ici il prend le chemin actuel (utile pour
	//revenir là ou on était)
	pwd=sp_pwd();
	if(!pwd || !*pwd) return pwd;

	//va dans le lecteur true_drive
	if(chdir(true_drive)) { free(pwd); return strdup(""); }

	//comme il est maintenant dans le lecteur
	//true_drive il va prendre une copie du chemin
	//dans pwd2
	pwd2=sp_pwd();

	//retourne là ou il était au début
	//et désalloque pwd, puisque inutile.
	chdir(pwd); free(pwd);

	//teste si le deuxième chemin a bien été alloqué...
	//if(!pwd2 || !*pwd2) return pwd2; <<- pas la peine puisque en bas il y a le même return

	//et enfin il retourne le résultat
	return pwd2;
}
#endif

//===================================================================
// La fonction retourne un pointeur vers le chemin actuel.
//
// NULL si pas assez de mémoire.
// "" (chaine vide) si le répertoire actuel n'existe pas.
//===================================================================
//OK
char *sp_pwd()
{
	unsigned long len = 50; //on commence par 50
	char *save;
	char *buffer =(char *)malloc(len+1);
	int repeat=0;
	if(!buffer) return 0;
	while(!getcwd(buffer,len)) {
			//si le répertoire actuel n'existe pas alors
			//retourne ""
			if(errno==ENOENT) {
				free(buffer);
				return strdup("");
			}
			//realloque le tout
			len+=25;
			buffer = (char *)realloc(save=buffer,len+1); 
			if(!buffer) {
				free(save);
				return 0; //pas assez de mémoire!
			}
			//pour éviter (on ne sait jamais) une boucle 
			//infinie due à un getcwd() qui retourne toujours
			//un NULL (??!!)
			repeat++;
			if(repeat>=50) {
				free(buffer);
				return strdup("");
			}
	}

	sp_make_system_slash(buffer);

	//et évite la perte de mémoire...
	buffer = (char *)realloc(save=buffer,strlen(buffer)+1);
	if(!buffer) return save;
	
	//là on a dans buffer le chemin actuel!
	return buffer;
}

//===================================================================
// Concertir path en un vrai path absolu.
// Qu'il soit relatif, absolu...
//
// Cette fonction supporte les chemins ayant des slash
// multiples. Dans le genre: c:\\\\windows///command
//
// Valeur de retour:
// -----------------
// Elle retourne le pointeur malloc.
//
// NULL si pas assez de mémoire
// "" (dup) si le chemin n'existe pas...
//===================================================================
//OK REV
char *sp_truepath(const char *path)
{
	char *true_path;
	size_t len_path;
	char *pwd;
	char *p;

	//TODO tester sp_executable() avec truepath
	
	//calcule le len
	len_path = strlen(path);

#ifdef WIN32
	// Si c'est un lecteur... comme C:
	// il y a deux cas:
	//
	// Lecteur Relatif: c:repertoire
	// Lecteur Absolu:  d:/chemin/repertoire
	if((len_path>=2 && isalpha(path[0]) && path[1]==':')) {
		//si lecteur absolu
		if(isslash2(path[2])) {
			p=strdup(path);
			if(!p) return p;
			sp_make_system_slash(p);
			return p;
		}
		//si lecteur relatif
		//il va utiliser pwd...
		else {
			//prend le répertoire du chemin path...
			pwd = sp_pwd_drive(*path); 
			if(!pwd || !*pwd) return pwd;

			//ici il va mettre le chemin actuel du
			//lecteur + le path... ce qu'il y a après
			//le lecteur)
			true_path = sp_pathcat(pwd,path+2);
			free(pwd);
			if(!true_path) return NULL;
			sp_make_system_slash(true_path);
			return true_path;
		}
	}
	
	// ça c'est si c'est une racine sous windows...
	// il doit ajouter le lecteur!
	if(isslash(*path)) {
		char *pwd = sp_pwd();
		if(!pwd) return pwd;
		p=(char *)malloc(strlen(path)+2+1); //2 = c:
		if(!p) {free(pwd);return p;}
		*p=0; //vide p
		//il voit s'il trouve un lecteur...
		if(strlen(pwd)>2 && pwd[1]==':' && isalpha(pwd[0])) {
			p[0]=pwd[0]; p[1]=':'; p[2]=0; //ça c'est le lecteur !
		}
		//et maintenant il ajoute le chemin complet!
		strcat(p,path);
		//et enfin, met les slash du système
		sp_make_system_slash(true_path);
		free(pwd);
		return p;
	}
#else
	//si c'est une racine sous linux... une simple copie
	//suffit puisqu'il n'y a qu'une racine!
	if(isslash2(*path)) {
		p=strdup(path);
		if(!p) return p;
		sp_make_system_slash(p);
		return p;
	}
#endif
	// -- là c'est le dernier recours --
	// -- si ce n'est pas lectuer/chemin relatif/absolu... --

	//Là il met le pwd actuel dans la variable une fois pour toute
	pwd = sp_pwd(); 
	if(!pwd || !*pwd)  return pwd;
	
	//Additionne le chemin actuel avec le chemin relatif "path"
	//dans true path
	true_path = (char *)malloc(strlen(path)+1+strlen(pwd)+1);
	if(!true_path) { free(pwd); return true_path; }

	strcpy(true_path,pwd);
	strcat(true_path,SLASH);
	strcat(true_path,path);

	//désallocation de ce qui reste...
	free(pwd);

	//fixation pour avoir un chemin sympa!
	sp_fix_p_cdpp(true_path);
	
	return true_path;
}

//===================================================================
// dirname
// basename
//conv path en directory
//par ex: c:\windows\command
//deviens c:\windows
//
//attention la fonction retourne
//path. mais elle le modifie
//en ajoutant un zero pour ne 
//laisser que le dirname.
//===================================================================
//OK
char *sp_dirname(char *path)
{
	size_t len = strlen(path);
	char *end = path + len;
	if(end!=path) end--; else return path;
	while(end!=path && isslash2(*end)) end--;
	while(end!=path && !isslash2(*end)) end--;
	while(end!=path && isslash2(*end)) end--; //dépasse les slash et n'en laisse qu'un seul...
	
#ifdef WIN32
	//teste si c'est un lecteur... si oui alors il faut laisser
	//le slash (si c'est mis bien sûre...!)
	if(len>=3 && end==path+1 && *end==':' && isalpha(*path)) {
		*(end+1)=C_SLASH;
		*(end+2)=0;
		return path;
	}
#endif
	
	//là il va tester si le caractère n'est pas un slash
	//de préférence...
	if(!isslash2(*end)) *(end+1)=0; else *end=0; //"else" au cas ou il y a des slash doublés...
	return path;
}

//===================================================================
// Retourne la position du nom de fichier (dans un répertoire)
//
// Attention. il ne modifie pas path.
//
// par ex: c:\windows\command
// devient "command"
//
// La fonction ne modifie pas le path
// mais elle modifie le pointeur
//===================================================================
//OK
char *sp_basename(char *path)
{
		char *end = path + strlen(path);
		while(end!=path && !isslash2(*end)) end--;
		if(isslash2(*end))
			memmove(path,end+1,strlen(end+1)+1);
		else
			if(end!=path) memmove(path,end,strlen(end)+1);
		return path;
}

//===================================================================
// Même chose que sp_basename mais cette fonction au lieu de 
// déplacer le basename au début de path retourne un pointeur
// vers le basename sans toucher à path
//===================================================================
//OK
const char *sp_basename_p(const char *path)
{
		const char *end = path + strlen(path);
		while(end!=path && !isslash2(*end)) end--;
		if(isslash2(*end))
			return end+1;
		return end;
}

//===================================================================
// Modifie path pour qu'elle contiennt que l'extention du
// fichier qui est dans 'path'
//===================================================================
//OK
char *sp_extname(char *path)
{
	const char *p=sp_extname_p(path);
	if(p!=path) memmove(path,p,strlen(p)+1);
	return path;
}

//===================================================================
// Retourne le pointeur vers l'extention contenue dans le
// chemin du fichier path.
//===================================================================
//OK
const char *sp_extname_p(const char *path)
{
	const char *end = path + strlen(path);
	const char *save_end=end;
	while(end!=path && (!isslash2(*end) && *end!='.')) end--;
	if(*end=='.') return end;
	return save_end; //retourne vide... (la fin de path)
}

//===================================================================
// Convertit tous les splash de 'path' en slash
// du système. par exemple sous windows il n'y aura que des 
// anti slash alors que sous linux il y aura des slash.
//===================================================================
//OK
char *sp_make_system_slash(char *path)
{
	char *save=path;
	while(*path) {
		if(*path==C_SLASH2)
			*path=C_SLASH;
		path++;
	}
	return save;
}

//===================================================================
// On supprime tous les ../ et ./ en n'oubliant pas
// d'appliquer ce ../ (par exemple).
// 
// -------------
// Par exemple:
// -------------
// quand on applique à ce chemin cette fonction
//                /home/asher256/../travail/.
// va devenir:
//                travail
//===================================================================
//OK
char *sp_fix_p_cdpp(char *path)
{
	char *p=path;

	sp_make_system_slash(path);
	sp_fix_slash_repeated(path);

	//tant qu'il trouve un slash
	while(1) {
		if(!*p) break;

		//passer le slash
		if(isslash(*p)) p++;
		if(!*p) break;

		//si le .. avec quelque chose après...
		if((strncmp(p,".." SLASH,3)==0 || strncmp(p,"..",3)==0)) {
			char *old_p=p-2;

			//si un .. dans le début...
			// si p ../salut
			// ou   /../salut
			if(p==path || (p==path+1 && *path==C_SLASH)) {
				//annuler car cela ne sert à rien!
				//strcpy(p,p+3);
				memmove(p,p+3,strlen(p+3)+1);
				continue;
			}

			//va dans le précédent slash
			while(old_p!=path && !isslash(*old_p)) old_p--;

			//met le contenu actuel dans ce p
			if(old_p!=p) {
				//strcpy(old_p,p+2);
				memmove(old_p,p+2,strlen(p+2)+1);
				p=old_p;
			}
			continue;
		}
		else
			//si le .
			if(strncmp(p,"." SLASH,2)==0 || strncmp(p,".",2)==0) {
				//strcpy(p,p+2);
				memmove(p,p+2,strlen(p+2)+1);
				continue;
			}
		p++;
	}
	return path;
}

//===================================================================
// Il met tous les slash qui se répètent en un seul
// par exemple /home//asher256///travail
// devient /home/asher256/travail
//
// NOTE:
// -----
// Fonctionne avec les deux slash
// par exemple
// c:\/\/windows//\/command
// devient c:\windows\command
//===================================================================
//OK
char *sp_fix_slash_repeated(char *path)
{
	char *p=path;
	while(*path) {
		if(isslash2(*path)) {
			char *save=path+1; //save=après premier slash

			*path=C_SLASH; //bon, pour en profiter et mettre un system slash :-)

			//parcourt à la recheche de != slash
			while(isslash2(*path)) path++;

			//s'il y a plus qu'un seul slash
			//il procède à la copie de tout
			//ce qui reste à la place
			//(parceque s'il y a un slash, il n'y
			//a rien a faire bien sûre).
			if(path!=save) {
				memmove(save,path,strlen(path)+1);
				path=save+1; /* +1 car on est sûre que le 1st est non slash */
			}
		}
		else
			path++;
	}
	return p;
}

//===================================================================
// Pour savoir la nature d'un fichier selon ses stat.
// 
// La fonction met les informations dans info.
//
// Elle retourne le pointeur info si c'est bon. Sinon,
// elle retourne zero.
//===================================================================
SP_STAT *sp_stat(const char *filename, SP_STAT *info)
{
	memset(&info->s,0,sizeof(info->s));
	if(stat(filename,&info->s)==-1) {
		memset(info,0,sizeof(SP_STAT));
		return NULL;
	}
	info->filename=filename;
	return info;
}

//===================================================================
// Retourne nonz si 'info' est un fichier exécutable
//
// ATTENTION: avant de lancer cette fonction il faut avant
// tout initialiser le contenu de info avec sp_stat()
//===================================================================
int sp_isexec(SP_STAT *info)
{
	//if(info->s.st_mode&(S_IXUSR|S_IXOTH|S_IXGRP)) return 1;
	//S_IXUSR  00100
	//S_IXOTH  00001
	//S_IXGRP  00010
	if(info->s.st_mode&(0x00100|0x00001|0x00010)) return 1;
	return 0;
}

//===================================================================
// Teste si la structure info contient un répertoire
//
// ATTENTION: Il faut avant tout utiliser sp_stat() sur la variable
// info avant le lancer ce test.
//===================================================================
int sp_isdir(SP_STAT *info)
{
	if(S_ISDIR(info->s.st_mode)) return 1;
	return 0;
}

//===================================================================
// Teste si info est un fichier régulier...
//
// ATTENTION: initialiser 'info' avec sp_stat() au préalable.
//===================================================================
int sp_isreg(SP_STAT *info)
{
		if(S_ISREG(info->s.st_mode)) return 1;
		return 0;
}

