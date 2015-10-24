//==================================================================
// Quelques fonctions pour que sous Windows
// et sous Linux il ouvre les dialogues
// les plus adaptés au système.
//
// Sous Windows il ouvre le file select de
// windows, les messages box de windows...
//
// et sous Linux il ouvre par défaut les
// fonctions d'fltk mais si USE_FLU est défini
// il utilise les bonne fonctions de FLU.
//
// Le file selector d'Fltk c'est pas le top
// d'ou ce fichier...
//
// Auteur: Asher256 2004
// contact@asher256.com
// http://jargon.tuxfamily.org/
//
// Quelques defines:
// =================
// windows: WIN32
//
// Linux:   LINUX
//          USE_FLU
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

//=================> Includes
#include "flnice.h"

#include <FL/Fl.H>

#ifdef WIN32
#include <windows.h>
#include <FL/x.H>
#endif

#ifdef LINUX
	#ifdef USE_FLU
		#include <FLU/Flu_File_Chooser.h>
	#else
		#include <FL/Fl_File_Chooser.H>
	#endif

	#include <FL/fl_ask.H>
#endif

#include <stdio.h>  //vsprintf
#include <stdlib.h> //free
#include <string.h> //strdup
#include <stdarg.h> //va_start vsprintf

//=================> Déclaration de fonctions
static char *m_strcat(char *dest, const char *src);
#ifdef LINUX
static char *fn_pattern_to_fltk(FN_PATTERN *pattern);
#endif
#ifdef WIN32
static char *fn_pattern_to_win32(FN_PATTERN *pattern);
#endif

//=================> Les Messages Box
// buffer pour les messages
#ifdef VISUALC
#define BUFFER_SIZE 4028 //le maximum-1 (le zero) dans un message box...
#else
#define BUFFER_SIZE 1024 //le maximum-1 (le zero) dans un message box...
#endif

// hwnd (pour les message box)
#define GET_HWND ((Fl::first_window()) ? fl_xid(Fl::first_window()) : (HWND) 0)

//=================> Les Dialogues...

int fn_ask(const char *format, ...)
{
	va_list ap;
	char out[BUFFER_SIZE]; //maximum dans un message...

	//start
	va_start(ap,format);
#ifdef VISUALC
	vsprintf(out,format,ap);
#else
	vsnprintf(out,BUFFER_SIZE,format,ap);
#endif
	va_end(ap);

#ifdef WIN32
	return (MessageBox(GET_HWND,out,"",MB_YESNO|MB_ICONQUESTION)==IDYES)?1:0;
#endif
#ifdef LINUX
        return fl_choice(out,"Oui","Non",0)?0:1;
#endif
}

void fn_message(const char *format, ...)
{
	va_list ap;
	char out[BUFFER_SIZE]; //maximum dans un message...

	//start
	va_start(ap,format);
#ifdef VISUALC
	vsprintf(out,format,ap);
#else
	vsnprintf(out,BUFFER_SIZE,format,ap);
#endif
	va_end(ap);

#ifdef WIN32
	MessageBox(GET_HWND,out,"" /* mettre ici le titre de la fenêtre parente? */,MB_OK|MB_ICONINFORMATION);
#endif
#ifdef LINUX
	fl_alert(out);
#endif
}

void fn_alert(const char *format, ...)
{
	va_list ap;
	char out[BUFFER_SIZE]; //maximum dans un message...

	//start
	va_start(ap,format);
#ifdef VISUALC
	vsprintf(out,format,ap);
#else
	vsnprintf(out,BUFFER_SIZE,format,ap);
#endif
	va_end(ap);

#ifdef WIN32
	MessageBox(GET_HWND,out,"" /* mettre ici le titre de la fenêtre parente? */,MB_OK|MB_ICONEXCLAMATION);
#endif
#ifdef LINUX
	fl_alert(out);
#endif
}

//=================> La gestion du pattern

//
//convertissement de fn_pattern vers le file chooser
//d'fltk ou FLU
//
//si pas assez de mémoire alors il retourne 0
#ifdef LINUX
static char *fn_pattern_to_fltk(FN_PATTERN *pattern)
{
	char *pattern_str=strdup("");

	//si pattern vide ou pas asez de mémoire retourner le résultat!
	if(!pattern || !pattern_str) return pattern_str;

	while(pattern->name) { //tant que pas null
		// All Files (*.*)
		pattern_str = m_strcat(pattern_str, pattern->name);
		pattern_str = m_strcat(pattern_str, " ");
		pattern_str = m_strcat(pattern_str, "(*.");
		pattern_str = m_strcat(pattern_str, pattern->ext);
		pattern_str = m_strcat(pattern_str, ")");
		
		//pas assez de mémoire
		if(!pattern_str) return pattern_str; 

		pattern++;
		if(pattern->name) pattern_str = m_strcat(pattern_str, "\t");
	}

	//retourne le résultat
	return pattern_str;
}
#endif

//
//convertissement de fn_pattern vers le dialog file chooser
//de windows
//la fonction retourne une variable malloc
//
//si pas assez de mémoire alors il retourne 0
#ifdef WIN32
static char *fn_pattern_to_win32(FN_PATTERN *pattern)
{
	char *pattern_str=strdup("");
	char tmp_delim = (char)0xFF; //le délimiteur, pour que ça soit pas 0
	char str_delim[] = {tmp_delim,0};
	if(!pattern_str) return 0; //retourner pattern vide... 

	//si pattern vide ou pas asez de mémoire retourner le résultat!
	if(!pattern || !pattern_str) return pattern_str;

	while(pattern->name) { //tant que pas null
		// All Files (*.*)
		pattern_str = m_strcat(pattern_str, pattern->name);
		pattern_str = m_strcat(pattern_str, " ");
		pattern_str = m_strcat(pattern_str, "(*.");
		pattern_str = m_strcat(pattern_str, pattern->ext);
		pattern_str = m_strcat(pattern_str, ")");
		pattern_str = m_strcat(pattern_str, str_delim);
		
		// \0 
		pattern_str = m_strcat(pattern_str, "*.");
		pattern_str = m_strcat(pattern_str, pattern->ext);
		pattern_str = m_strcat(pattern_str, str_delim);

		//pas assez de mémoire
		if(!pattern_str) return pattern_str; 

		pattern++;
	}

	//mets les \0\0 de la fin
	pattern_str = m_strcat(pattern_str, str_delim);
	pattern_str = m_strcat(pattern_str, str_delim);

	//convertissement de la chaine patter_str
	char *s = pattern_str;
	while(*s) {
		if(*s==tmp_delim) *s=0;
		s++;
	}

	//retourne le résultat
	return pattern_str;
}
#endif

//=================> Le Dir Chooser
#ifdef WIN32
const char *win_dir_chooser(const char *message, const char *fname)
{
	OPENFILENAME ofn;
	static char szFileName[MAX_PATH]=""; //static car cette valeur est retournée

	//si filename, il le mets sans dépassé MAX_PATH...
	if(fname) strncpy(szFileName,fname,MAX_PATH);

	//mets tout les slash en antislash
	{
		char *s = szFileName;
		while(*s) { if(*s=='/') *s='\\'; s++; }

		//si le dernier caractre est un anti slash alors
		//l'enlever...
		s=szFileName+strlen(szFileName);
		while(s!=szFileName) { if(*s!='\\' && *s!=0) break; *s=0; s--; };
	}
	
	//si vide déclancher réaction en chaine
	//avec prochaine condition
	if(!*szFileName) 
		strcpy(szFileName,"C:");

	//si C: par exemple
	if((strlen(szFileName)==2 && szFileName[1]==':')) //then
		strcat(szFileName,"\\choose directory");

	//mets le tout à zero
	ZeroMemory(&ofn,sizeof(ofn));

	// mets les options
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GET_HWND;
	ofn.lpstrFile = szFileName;
	ofn.lpstrFilter = "All Files\0*.*\0\0";
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = message;

	ofn.Flags = OFN_EXPLORER; 

	// ouvre le dialog box...
	if(GetOpenFileName(&ofn)) {
		char *s = szFileName+strlen(szFileName);

		//enlve le nom de fichier...
		while(s!=szFileName) { if(*s=='\\') { *s=0; break; } s--; }

		//si c le lecteur alors... tolre ;)
		if(strlen(szFileName)==2 && szFileName[1]==':') 
				strcat(szFileName,"\\");

		return szFileName;
	}

	return 0;
}
#endif

// message et fname peuvent être des null
const char *fn_dir_chooser(const char *message, const char *fname)
{
#ifdef WIN32
	return win_dir_chooser(message,fname);
#endif
#ifdef LINUX
#ifdef USE_FLU
	return flu_dir_chooser(message,fname);
#else
	return fl_dir_chooser(message,fname);
#endif
#endif
}

//=================> Le File Chooser
#ifdef WIN32
const char *win_file_chooser(const char *message, FN_PATTERN *pattern, const char *fname, int save) 
{
	OPENFILENAME ofn;
	static char szFileName[MAX_PATH]=""; //static car cette valeur est retournée

	char *pattern_str = fn_pattern_to_win32(pattern);
	if(!pattern_str) { fn_alert("Out of memory..."); return 0; }

	//si filename, il le mets sans dépasser MAX_PATH...
	if(fname) strncpy(szFileName,fname,MAX_PATH);

	//mets tout les slash en antislash
	{
			char *s = szFileName;
			while(*s) { if(*s=='/') *s='\\'; s++; }
	}

	//mets le tout à zero
	ZeroMemory(&ofn,sizeof(ofn));

	// mets les options
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GET_HWND;
	ofn.lpstrFile = szFileName;
	ofn.lpstrFilter = pattern_str;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = message;

	ofn.Flags = OFN_EXPLORER|OFN_PATHMUSTEXIST; 
	if(!save) ofn.Flags|=OFN_FILEMUSTEXIST; //quand on fait load, alors le fichier doit au moins exister...

	// ouvre le dialog box...
	if((save && GetSaveFileName(&ofn)) || (!save && GetOpenFileName(&ofn))) {
		free(pattern_str);
		return szFileName;
	}

	free(pattern_str);

	return 0;
}
#endif

//====================>
//== File Chooser
//====================>

//kle file chooser
//messae, pattern et fname peuvent tous les trois contenir un caractère null
//message=null      pas de message
//pattern=null      alors tous les fichiers
//fname=0           alors précédent filename
static const char *fn_file_chooser_ex(const char *message, FN_PATTERN *pattern, const char *fname, int save) 
{
#ifdef WIN32
	FN_PATTERN all_files[] = {{"All Files","*"},{0,0}};
	return win_file_chooser(message,(!pattern)?all_files:pattern,fname,save);
#endif

#ifdef LINUX
	const char *ret; //valeur de retour
	char *pattern_str=fn_pattern_to_fltk(pattern); //pas encore...
	if(!pattern_str) { fn_alert("Out of memory..."); return 0; }

	#ifdef USE_FLU
		if(!save)
			ret=flu_file_chooser(message,pattern_str,fname);
		else
			ret=flu_save_chooser(message,pattern_str,fname);
	#else
		ret=fl_file_chooser(message,pattern_str,fname,1 /* po relative*/);
	#endif
	
	free(pattern_str);

	//retourne ce qu'il doit retourner
	return ret;
#endif
}

const char *fn_save_chooser(const char *message, FN_PATTERN *pattern, const char *fname) 
{return fn_file_chooser_ex(message,pattern,fname,1);}

const char *fn_file_chooser(const char *message, FN_PATTERN *pattern, const char *fname) 
{return fn_file_chooser_ex(message,pattern,fname,0);}

//=================> Quelques fonctions de string

//ajoute src à dest
//si dest null ret null
//si pas assez de mémoire
//alors free dest et retourne
//null
//
//la fonc ret null si pas assez de m
//mem et desalloque automa dest laissé en
//mem
static char *m_strcat(char *dest, const char *src)
{
	char *save;
	char *ptr;
	if(!dest) return dest;
	ptr = (char *)realloc(save=dest,strlen(dest)+strlen(src)+1);
	if(!ptr) { free(save); return ptr;}
	strcat(ptr,src);
	return ptr;
}

