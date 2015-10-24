//======================================
// Librairie pour lire
// le dictionnaire du jargon 
// informatique.
// 
// pour plus d'info reportez
// vous à jargon_lib.c
//
// Auteur: Asher256
// Email:  contact@asher256.com
//======================================

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

#ifndef JARGON_LIB_H
#define JARGON_LIB_H

#ifdef __cplusplus
	extern "C" {
#endif

//les erreurs
#define DICT_OK         0
#define DICT_E_FOPEN    1
#define DICT_E_OUTMEM   2

//structure du dictionnaire
typedef struct { 
	char *word;          //pointeur vers le mot alloqué
	long offset;         //l'offset du mot dans le fichier
	FILE *handle;        //ptr vers handle du fichier
	short *theme_list;    //le numéro de theme (la table themes...) -1 = fin des themes
} DICT_ENTRY;

typedef struct {
	DICT_ENTRY *entry; //les entrées dans le dictionnaire
	int size;          //nombre de mots
	int true_size;     //la vrai taille d'entry en mémoire
	
	//les thèmes
	char **theme;      //les thèmes...
	short theme_size;
	
	//les handles ouverts qui vont êtres fermés à la fin
	//car quand le programme charge un dictionnaire
	//il l'ajoute au dictionnaire actuel donc, un fichier DICT
	//peut contenir plusieurs dictionnaire déjà ouverts
	FILE **handles;       //les handles...
	int n_handles;       //nombre de handles
} DICT;

// par exemple X11 devient X-Window
char *dict_to_readable_theme(char *t);
	
//variables
extern DICT dict; //tout le dictionnaire se charge ici avec load_dict()

//Fonctions
char *dict_get_description(int index); //retourne un strdup de la description qui est dans index
int load_dict(const char *filename, void (*percent)(int percent));
void close_dict();

//quelques fonctions pour la gestion du dictionnaire
void dict_pop(DICT *dict);
int dict_push(DICT *dict, char *word, FILE *handle, long offset, short *theme);
void dict_fix_true_size(DICT *dict);
int dict_add_true_size(DICT *,int); //ajouter dans la mémoire cache
void dict_init(DICT *list);
void dict_free_entry(DICT *dict);
short dict_add_theme(DICT *d, const char *theme);
int dict_add_handle(FILE *_handle);

#ifdef __cplusplus
	}
#endif

#endif
