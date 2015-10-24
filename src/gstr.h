/************************************
 * gestion de string
 *
 * Auteur: Asher256
 * Email:  contact@asher256.com
 ***********************************/

#ifndef __GSTR_H
#define __GSTR_H

#ifdef __cplusplus
   extern "C" {
#endif

#include <stdio.h>
#include <string.h>

#define MALLOC  malloc
#define REALLOC realloc
#define FREE    free

//gestion des listes de strings...
typedef struct {
	char **ptr;  //il y a toujours un NULL dans le dernier ptr
	size_t size; //nombre d'éléments ds ptr
} GS_LIST;

// Joker
int gs_wildcard(const char *p, const char *joker);
int gs_ncase_wildcard(const char *p, const char *joker);

// Quelques fonctions de comparaison
// utilisés avec gs_index par exemple...
int gs_is_affix(const char *string, const char *affix);
int gs_is_caseaffix(const char *string, const char *affix);

// Fonctions de recherche avec comparaison
char *gs_indexof(const char *string, const char *rech, int (*isaffix)(const char *, const char *));
char *gs_last_indexof(const char *string, const char *rech, int (*isaffix)(const char *, const char *));

// pr supprimer des caractères depuis un string
char *gs_sdel(char *string, size_t pos, size_t len);
char *gs_cdel(char *string, size_t pos);

// Quelques calc dans une chaine
size_t gs_nstr(const char *str, const char *rech); // nombre de rech dans str

// Split/Join
char *gs_join(GS_LIST *list, const char *separator);
GS_LIST *gs_split_ex(const char *string, const char *delim, char *(*_strstr)(const char *,const char *), size_t delim_len);
GS_LIST *gs_split(const char *string, const char *delim);
GS_LIST *gs_csplit(const char *string, const char *delim);

// Les listes
GS_LIST *gs_list_new();
void gs_list_free(GS_LIST **list);
char *gs_list_push(GS_LIST *list, char *str);
char *gs_list_push_m(GS_LIST *list, const char *str);

// mettre au dimentions du string
char *gs_fix_realloc(char *str);

// Similaires à libc
char *gs_strcat_r(char *dest, const char *src); 
char *gs_strcat_m(const char *dest, const char *src);

// CTRIM
char *gs_c_trim(char *str, const char *reject);
char *gs_c_rtrim(char *str, const char *reject);
char *gs_c_ltrim(char *str, const char *reject);

// TRIM des espaces
char *gs_trim(char *str);
char *gs_rtrim(char *str);
char *gs_ltrim(char *str);

// RECHERCHE
char *gs_accept(char *str, const char *accept); // ret first carac = un carac de accept
char *gs_r_accept(char *str, const char *accept);
char *gs_reject(char *str, const char *reject); //ret first carac != un carac de reject
char *gs_r_reject(char *str, const char *reject);

// REMPLACEMENT
char *gs_replace_m(const char *str, const char *src, const char *repl);
char *gs_replace_r(char *str, const char *src, const char *repl); // avec realloc
char *gs_replace(char *str, const char *src, const char *repl);
char *gs_creplace(char *str, char src, char repl);

// ucase/lcase
char *gs_ucase(char *str);
char *gs_lcase(char *str);
char gs_cucase(char c);
char gs_clcase(char c);

// SUBSTR
char *gs_substr_m(char *str, size_t pos, size_t len);
char *gs_substr(char *str, size_t pos, size_t len);
char *_gs_substr(char *str, size_t pos, size_t len);

char *gs_change(char *str, size_t pos, size_t len, const char *value);
char *gs_change_r(char *str, size_t pos, size_t len, const char *value);
char *gs_change_m(const char *str, size_t pos, size_t len, const char *value);

#ifdef __cplusplus
   }
#endif

#endif

