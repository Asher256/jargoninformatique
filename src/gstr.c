/************************************************************
 * Librairie pour la gestion de chaine de caractères
 * en C.
 *
 * Auteur: Asher256
 * Email:  contact@asher256.com
 ************************************************************
 * NOTE: les commentaires de ce code source sont
 *       écrit au format utf8.
 ************************************************************
 *==================
 * Licence:
 *==================
 * Cette source est distribuée sous licence
 * GNU General Public Licence version 2.
 *
 * Elle n'est distribuée sans aucune garantie.
 * Vous avez le droit de la distribuer, 
 * l'étudier ou l'améliorer tant que le 
 * code source reste libre et conforme à
 * la licence GPL.
 ************************************************************
 *==================
 * Historique:
 *==================
 * 0.1 (Août 2004):
 *     * Création de toutes les fonctions de base 
 *     * Version pas complètement testée
 * 0.2 (Juillet 2005):
 *     * Elimination de tous les warning critiques
 *     * Correction de quelque bugs trouvés grâce à
 *       certaines programmes qui utilisent gstr.c
 *       (comme Jargon Informatique)
 * 0.3 (Août 2005)
 *     * Relecture de toutes les fonctions une à une
 *       et réécriture de certaines (pour optimisation)
 *     * Réécriture de presque tous les commentaires
 *       incorrectement écrits pour avoir mieux de
 *       détails sur l'utilisation des fonctions.
 *     * Changement du nom gs_joker() par gs_wildcard()
 *     * Correction de certains bugs dans gs_wildcard
 *     * Réécriture de gs_is_affix() et gs_is_caseaffix()
 *       gs_indexof() gs_last_indexof().
 *     * Remplacement dans gs_sdel() strcpy() par
 *       memmove() plus adaptée à cela et valgrind
 *       provoque quelques erreurs.
 *     * Changement de gs_join() pour qu'il alloque
 *       le tout d'un coup au lieu de faire des realloc
 *       à chaque fois...
 *     * Elimination de l'allocation mémoire dans
 *       gs_split_ex()
 *     * Correction de tous les malloc/realloc/free
 *       par des MALLOC/REALLOC/FREE (les defines
 *       pour faciliter l'intégration avec un
 *       autre type d'allocations)
 *     * correction de gs_strcat_r ou dest n'était
 *       pas désalloqué quand il n'y avait pas assez
 *       de mémoire.
 *     * Correction d'un bug dans gs_replace_m() et 
 *       gs_replace_r() qui n'alloque pas la bonne
 *       taille.
 *     * Correction d'un bug dans gs_replace_m()
 *       qui ne teste pas le NULL dans la bonne
 *       variable.
 *     * Petite optimisation dans gs_nstr() au lieu
 *       de recalculer strlen rech, il la stocke dans
 *       une variable au début.
 *     * Optimisation de gs_replace() pour qu'il 
 *       test le type de remplacement au début...
 *     * gs_*case, remplacement do{}while par while{}
 *       car plus adaptée.
 *     * Correction d'un petit bug dans _gs_substr()
 *       qui ne retournait pas str + pas de memmove
 *     * Création de fonction gs_change pour être le
 *       pilier de gs_change_m et gs_change_r
 *     * Correction d'un bug dans gs_r_accept() qui 
 *       ignore le premier caractère lors de la recherche.
 *     * Correction de gs_r_reject() - même bug de 
 *       gs_r_accept()
 *
 ************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <ctype.h> // toupper/tolower

#include "gstr.h"

/*********************************************
 * Compare 'str' avec 'joker'.
 * Permet de mettre un wilcard dans la
 * chaine joker.
 * Comme par exemple: *.exe
 *********************************************/
//OK

int gs_wildcard(const char *p, const char *joker)
{
	while(*p && *joker) {
		//test joker *
		if(*joker=='*') {
			//chercher prochain caractère
			//dans joker qui est diff '?' et '*'
			joker++;
			while(*joker=='*' || *joker=='?') joker++;

			//cherche le caractère qui égale
			//celui demandé par le joker.
			while(*p && *p!=*joker) p++;

			//pas trouvé?? alors erreur...
			//comme par exemple acha = ach*raf
			if(*p!=*joker) return 0; 

			//si fin mais égalité
			if(!*p && !*joker) return -1;
		}

		// S'il n'y a pas d'étoile... Il va soit
		// tester avec un joker '?' soit faire un
		// vrai test pour voir si le *joker actuel
		// correspond à *p
		else if(*joker!='?' && *joker!=*p) return 0;

		//incrémente les deux... Comme toujours...
		joker++; p++;
	}

	//si l'on finit avant l'autre...
	if(*p!=*joker) {
		//si le joker a déjà fini... donc c pas bon.
		if(!*joker) return 0;
		//cherche s'il n'y a que des wildcard
		//à la fin de joker car si c'est le cas
		//ça veut dire que c'est parfait.
		//par exemple: gs_wildcard("asher","ash*")
		//p va finir avant le dernier 'a' de ash
		//et puis  la fonction va croire que c faux.
		//mais maintenant après cette boucle il saura
		//qu'il n'y a "que" des wildcard
		while(*joker && (*joker=='*' || *joker=='?')) joker++;
		//si joker est arrivé à la fin!! ça veut dire que c ok
		if(!*joker) return -1;
		return 0;
	}
	
	//et sinon... cela veut dire que c'est pas bon :-)
	return -1;
}

/************************************************************
 * Teste si 'affix' est l'affixe de 'string'.
 * Si c'est oui, il retourne une valeur différente
 * de zéro.
 * 
 * Attention:
 * ----------
 *  string et affix doivent être de vrai pointeurs non NULL.
 ************************************************************/
//OK
int gs_is_affix(const char *string, const char *affix)
{
	while(*affix) {
		if(*string!=*affix) return 0;
		string++; affix++;
	}
	return 1;
}

/************************************************************
 * Teste si 'affix' est l'affixe de 'string'.
 * Si c'est oui, il retourne une valeur différente
 * de zéro.
 *
 * Ce test est différent de gs_is_affix car il ignore
 * les majuscules ou minuscules.
 * 
 * Attention:
 * ----------
 * string et affix doivent être de vrai pointeurs non NULL.
 ************************************************************/
//OK
int gs_is_caseaffix(const char *string, const char *affix)
{
	while(*affix) {
		if(tolower(*string)!=tolower(*affix)) return 0;
		string++; affix++;
	}
	return 1;
}

/************************************************************
 * Recheche dans 'string' si 'rech' existe.
 * Il retourne l'index de ce qui concorde à 'rech'
 * dans 'string'.
 *
 * isaffix: marche de la même façon que gs_is_affix()
 ************************************************************/
//OK
char *gs_indexof(const char *string, const char *rech, int (*isaffix)(const char *, const char *))
{
	if(!isaffix) isaffix=gs_is_affix;
	while(*string) {
		if((*isaffix)(string,rech)) return (char *)string;
		string++;
	}
	return NULL; //pas trouvé!
}

//OK
char *gs_last_indexof(const char *string, const char *rech, int (*isaffix)(const char *, const char *))
{
	const char *s = string+strlen(string)-strlen(rech);
	if(!isaffix) isaffix=gs_is_affix;

	while(s>=string) {
		if((*isaffix)(s,rech)) return (char *)s;
		s--;
	}
	return NULL;
}

/************************************************************
 * Supprime 'len' caractères depuis la position 'pos'
 * dans la chaine 'string'.
 ************************************************************/
//OK
char *gs_sdel(char *string, size_t pos, size_t len)
{
	size_t slen;

	//fix les dépassements
	slen=strlen(string);
	if(pos>=slen) return string;
	if(pos+len>=slen) len=slen-pos;

	//delete...
	memmove(string+pos,string+pos+len, slen-pos-len+1);

	return string;
}

// Même chose que gs_sdel mais pour un seul caractère
//OK
char *gs_cdel(char *string, size_t pos)
{
	return gs_sdel(string,pos,1);
}

/************************************************************
 * Cette fonction prend toutes les chaines qui sont
 * dans 'list' pour les joindre en utilisant 'separator'.
 *
 * Par exemple la liste contient:
 *    * bonjour
 *    * le
 *    * monde
 *
 * Si on utilise cette fonction avec separator=":"
 * Alors le résultat (alloqué) sera:
 *
 *  "bonjour:le:monde"
 ************************************************************/
//OK
char *gs_join(GS_LIST *list, const char *separator)
{
	static size_t i;
	static char *s;
	static size_t len_general;

	//teste si list est vide. très important
	//pour ce qui est en bas...
	if(!list->size) return strdup("");

	//calcul du len général (toutes les chaines de 'list' + separateurs)
	len_general=strlen(separator)*(list->size-1); //y a toujours un séparateur en moins
	i=0; 
	while(i<list->size) { len_general+=strlen(list->ptr[i]); i++; }

	//allocation du tout...
	s = (char *)MALLOC(len_general+1);
	if(!s) return s;
	*s=0; //vide la chaine
	
	//maintenant il va copier le contenu de toutes
	//les chaines de 'list' + separateur dans s
	i=1;
	strcpy(s,list->ptr[0]);
	while(i<list->size) { strcat(s,separator); strcat(s,list->ptr[i]); i++; }
	
	return s;
	//----

	//ancienne façon:
	//j'ai préféré la remplacer pour éviter
	//la réallcation à chaque fois...
#if 0
	for(i=0;i<list->size;i++) {
		if(i!=0) s=gs_strcat_r(s,separator);
		s=gs_strcat_r(s,list->ptr[i]);
		if(!s) return s;
	}
#endif
	return s;
}

/************************************************************
 * Cette fonction découpe 'string' en morceaux en 
 * utilisant le délimiteur 'delim'
 *
 * _strstr(chaine,delim)
 * =====================
 * La fonction _strstr() se charge de trouver
 * 'delim' dans 'chaine'
 ************************************************************/
//OK
GS_LIST *gs_split_ex(const char *string, const char *delim, char *(*_strstr)(const char *,const char *), size_t delim_len)
{
	static const char *suivant, *precedent, *cp /*const p*/;
       	char *p;
	static size_t len;

	//Création d'une liste
	GS_LIST *list = gs_list_new();
	if(!list) return list;

	//opération de split
	precedent=string;
	while((suivant=(*_strstr)(precedent,delim))) {
		//calcule le len de 'precedent' à 'len'
		cp=precedent; len=0;
		while(cp<suivant) { len++; cp++; }

		//allocation d'une nouvelle chaine
		p = (char *)MALLOC(len+1); //le 1 du zero
		if(!p) {
			gs_list_free(&list);
			return NULL;
		}
		strncpy(p,precedent,len);
		p[len]=0; //le zero de la fin...

		//ajoute...
		if(!gs_list_push(list,p)) {
			gs_list_free(&list);
			return NULL;
		}

		precedent=suivant+delim_len;
	}

	//si oui alors on l'ajoute!
	p=strdup(precedent);
	if(!gs_list_push(list,p)) {
		gs_list_free(&list);
		return NULL;
	}

	//et enfin, il retourne le résultat!
	return list;
}

GS_LIST *gs_split(const char *string, const char *delim)
{
	return gs_split_ex(string,delim,strstr,strlen(delim));
}

GS_LIST *gs_csplit(const char *string, const char *delim)
{
	return gs_split_ex(string,delim,strpbrk,1);
}

/************************************************************
 * Création d'une nouvelle liste GS_LIST.
 *
 * Cette alloque le contenu, mais aussi la variable GS_LIST
 * (qui est d'ailleurs retournée).
 *
 * NOTE:
 * -----
 * La liste 'list->ptr' est une char**, elle est compatible
 * avec les chaines qui finissent avec un zero.
 * C'est pour cette raison qu'un zero est toujours ajouté
 * à la fin...
 ************************************************************/
//OK
GS_LIST *gs_list_new()
{
	//création d'une liste vide
	GS_LIST *list = (GS_LIST *)MALLOC(sizeof(GS_LIST));
	memset(list,0,sizeof(GS_LIST));

	// créer un élément contenant zero
	list->ptr=(char **)MALLOC(sizeof(char *));
	if(!list->ptr) { FREE(list); return NULL; }
	list->ptr[0]=NULL;

	//retourne le résultat
	return list;
}

/************************************************************
 * Ajoute un élément dans la liste 'list'.
 * 
 * NOTE: str doit être un pointeur malloc! car gs_list_push
 *       n'ajoute que le pointeur 'str'
 *       Utilisez gs_list_push_m() pour avoir une fonction
 *       qui copie str avec strdup() 
 *
 * NOTE: 'list' doit être alloquée avec gs_list_new()
 *
 * Retour:
 * -------
 * S'il n'y a pas assez de mémoire pour ajouter un
 * élément dans 'list', cette fonction retourne NULL.
 * Sinon, elle retourne str.
 ************************************************************/
//OK
char *gs_list_push(GS_LIST *list, char *str)
{
	static char **save;
	if(!str) return str;

	list->ptr = (char **)REALLOC(save=list->ptr, (sizeof(GS_LIST)*(list->size+1))+1);
	if(!list->ptr) {
		list->ptr=save;
		return NULL;
	}

	//mets les valeurs
	list->ptr[list->size]  = str;
	list->ptr[list->size+1] = NULL;  // le dernier contiens toujours NULL

	//exit normalement
	list->size++;
	return str;
}

/************************************************************
 * Ajoute une copie malloc de str dans list.
 * Voir l'explication dans gs_list_push() pour avoir plus
 * de détails.
 ************************************************************/
//OK
char *gs_list_push_m(GS_LIST *list, const char *str)
{
	return gs_list_push(list,strdup(str));
}

/************************************************************
 * Désalloque la liste de strings contenue dans le pointeur
 * du pointeur 'list'.
 * 
 * NOTE: Cette fonction met le pointeur 'list' à 0.
 ************************************************************/
// Ok
void gs_list_free(GS_LIST **list)
{
	size_t list_size=(*list)->size,i;
	//free tous les pointeurs dans la liste
	i=0;
	while(i<list_size) {
		FREE((*list)->ptr[i]);
		i++;
	}
	//free la liste
	FREE((*list)->ptr);
	//mets null
	*list=NULL;
}

/************************************************************
 * Retourne une nouvelle allocation mémoire contenant 
 * dest+src.
 *
 * Note: Si dest/src=0 alors ret NULL
 * 
 ************************************************************/
//OK
char *gs_strcat_m(const char *dest, const char *src)
{
	char *ptr;
	if(!dest || !src) return NULL;
	ptr=(char *)MALLOC(strlen(dest)+strlen(src)+1);
	if(!ptr) return 0;
	strcpy(ptr,dest);
	strcat(ptr,src);
	return ptr;
}

/************************************************************
 * Même chose que gs_strcat_m() mais cette fois-ci c'est
 * 'dest' qui est realloqué pour contenir dest+src
 *
 * ATTENTION:
 * ----------
 * Quand il n'y a pas assez de mémoire, il retourne NULL
 * et free 'dest' automatiquement.
 * Il ne pas réutiliser dest après un NULL
 * retourné par cette fonction.
 *
 * ATTENTION:
 * ----------
 * Si dest ou src sont NULL elle retourne systématiquement
 * NULL et désallque dest si celui-ci est non NULL.
 *
 * Notes:
 * ------
 * SI: !dest     ret strdup(src)
 * SI: !src      ret NULL
 *
 * Si pas assez de mémoire retourne
 * 0 mais le pointeur on doit faire 
 * free(dest) après (comme realloc)
 ************************************************************/
//OK
char *gs_strcat_r(char *dest, const char *src)
{
	static char *save;
	if(!dest || !src) { free(dest); return NULL; }
	dest = (char *)REALLOC(save=dest, strlen(dest)+strlen(src)+1);
	if(!dest) {free(save); return dest;}
	return strcat(dest,src);
}

/************************************************************
 * Cette fonction remplace le caractère 'src' par 'repl'
 * dans la chaine 'string'.
 *
 * Elle retourne un simple pointeur vers 'string'.
 ************************************************************/
//OK
char *gs_creplace(char *string, char src, char repl)
{
	char *save=string;
	while(*string) {
		if(*string==src) *string=repl;
		string++;
	}
	return save;
}

/************************************************************
 * Remplacement de 'src' par 'repl' dans
 * 'str'.
 *
 * Valeur de retour:
 * -----------------
 * Retourne un nouveau pointeur "malloc"
 * contenant le résultat (remplacement).
 *
 * ATTENTION: !str alors ret NULL.
 ************************************************************/
//OK
char *gs_replace_m(const char *str, const char *src, const char *repl)
{
	char *str_m; //string alloqué
	size_t len;

	if(!str) return NULL;

	// calcule le nombre de repl dans src
	len=gs_nstr(str,src);
	if(!len) return strdup(str);
	len=(strlen(str)-(len*strlen(src)))+(len*strlen(repl))+1;

	// Allocation
	str_m = (char *)MALLOC(len); // src fois * len2
	if(!str_m) return NULL;

	strcpy(str_m,str);

	return gs_replace(str_m,src,repl);
}

/************************************************************
 * Remplacement de 'src' par 'repl'
 * 'src' est realloqué et contient
 * le résultat du remplacement.
 *
 * Valeur de retour:
 * -----------------
 * La fonc retourne un ptr realloqué de
 * str contenant le remplacement...
 *
 * Si pas assez de mémoire retourne NULL.
 *
 * ATTENTION: !str alors ret NULL.
 *
 ************************************************************/
//OK
char *gs_replace_r(char *str, const char *src, const char *repl)
{
	char *save;
	size_t len;

	if(!str) return NULL;

	len=gs_nstr(str,src);
	if(!len) return str;
	len=strlen(str) - (len*strlen(src)) + (len*strlen(repl)) + 1;

	str = (char *)REALLOC(save=str,len); // src fois * len2 
	if(!str) {
		FREE(save);
		return 0;
	}

	return gs_replace(str,src,repl);
}

/************************************************************
 * Calcule le nombre de fois que 'rech'
 * est trouvée dans 'str'.
 ************************************************************/
//OK
size_t gs_nstr(const char *str, const char *rech)
{
	const char *s=str;
	size_t n=0;
	size_t len_rech = strlen(rech);

	while(s) {
		s=strstr(s,rech);
		if(s) { 
			n++; s+=len_rech;
		}
	}

	return n;
}

/************************************************************
 * remplace 'src' par 'repl' sans aucune 
 * reallocation...
 *
 * Retour:
 * -------
 * La fonction un pointeur vers 'str'.
 *
 * ATTENTION:
 * ----------
 * Il faut être sûre que 'str' peut contenir 
 * le remplacement de tous les 'src' par les 
 * 'repl'.
 ************************************************************/
//OK OPTI
char *gs_replace(char *str, const char *src, const char *repl)
{
	static size_t len,len2;
	static int difference;
	char *save=str;

	// Mets les infos
	len=strlen(src);
	len2=strlen(repl);
	difference=len-len2;

	//ici, le type de remplacement quand il ont la même taille
	if(!difference) {
		while((str=strstr(str,src))) {
			strncpy(str,repl,len2);
			str+=len;
		}
	}
	//si str>repl
	//Donc il met ce qu'il y a dans 'repl' à la place de 'src'
	//puis il déplace tout ce qui reste remplit le vide laissé
	//par 'repl' (puisqu'il est petit) par ce qui reste de str.
	else if(difference>0) {
		while((str=strstr(str,src))) {
			strncpy(str,repl,len2);
			str+=len2;
			memmove(str,str+difference,strlen(str+difference)+1);//+1=zero
		}
	}
	//si str<repl
	//Alors il va laisser de la place à repl pour qu'elle
	//puisse se permettre de s'installer là ou 'str' est logée.
	else {
		while((str=strstr(str,src))) {
			memmove(str+len2,str+len,strlen(str+len)+1); //+1 = null final
			strncpy(str,repl,len2);
			str+=len2;
		}
	}

	return save;
}

/************************************************************
 * Cette fonction concertit tous les caractères de 'str'
 * en minuscule.
 * Elle retourne le pointeur 'str'.
 ************************************************************/
//OK
char *gs_lcase(char *str)
{
	char *save=str;
	while(*str) {
		*str=gs_clcase(*str);
		str++;
	}
	return save;
}

/************************************************************
 * Cette fonction concertit tous les caractères de 'str'
 * en majuscule.
 * Elle retourne le pointeur 'str'.
 ************************************************************/
//OK
char *gs_ucase(char *str)
{
	char *save=str;
	while(*str) {
		*str=gs_cucase(*str);
		str++;
	}
	return save;
}

//OK
char gs_clcase(char c)
{ return (char)tolower(c); }
char gs_cucase(char c)
{ return (char)toupper(c); }

/************************************************************
 * _gs_substr fait un substring. Cela veut dire que
 * le string contient ce qu'il y a à la position 'pos'
 * à la longueur 'len'.
 *
 * ATTENTION:
 * ----------
 * Elle ne teste pas si pos->len sont dans le string
 * ce qui fait que si vous appelez cette fonction il 
 * faut être sûre de cette donnée.
 ************************************************************/
//OK
char *_gs_substr(char *str, size_t pos, size_t len)
{
	char *s=str+pos;
	s[len]=0;
	if(pos) {
		static size_t n_len;
		char *p_str=str;
		//memmove (str,s)
		for(n_len=len;n_len;n_len--) {
			*p_str=*s;
			p_str++; s++;
		}
		str[len]=0; //le zero de la fin
	}
	return str;
}

/************************************************************
 * La même chose que _gs_substr() mais cette fonction est
 * plus haut-niveau puisqu'elle teste au moins si 'pos'
 * et 'len' existent dans 'str' et essaye de les ajuster
 * si c'est possible.
 *
 * NOTE:
 * -----
 * En plus elle permet de prendre une position de
 * la fin du string.
 * Si pos est négatif cela veut dire
 * pos=len+pos
 * position depuis la fin!
 *
 * p.ex: -1 c le dernier caractère
 ************************************************************/
//OK
char *gs_substr(char *str, size_t pos, size_t len)
{
	size_t str_size = strlen(str);

	// positionne "pos"
	if(pos<0) pos=str_size+pos;
	if(!len) {*str=0; return str; } //un string vide si pas de len

	//il teste si len dépasse
	if(pos+len>str_size) len=str_size-pos;

	//fait maintenant un substr
	return _gs_substr(str,pos,len);
}

/************************************************************
 * Retourne le substr du pointeur 'str' en version malloc.
 ************************************************************/
// OK
char *gs_substr_m(char *str, size_t pos, size_t len)
{
	char *ptr;
	if(!str) return NULL;

	ptr = strdup(str);
	if(!ptr) return NULL;

	return gs_substr(ptr,pos,len);
}

/************************************************************
 * Change une partie d'un string
 *
 * NOTE:
 * il faut être sûre qu'il y a assez d'espace mém
 * pour remplacer le 'len' par strlen'value'.
 ************************************************************/
//OK
char *gs_change(char *str, size_t pos, size_t len, const char *value)
{
	size_t value_len;
	if(!str) return NULL;

	value_len = strlen(value);

	//Le plus simple...
	if(value_len==len) 
		memmove(str+pos,value,value_len);
	else { //sinon!
		static char *dest;
		static char *src;
		
		src  = str+pos+len;
		dest = str+pos+value_len;

		memmove(dest,src,strlen(src)+1);
		memmove(str+pos,value,value_len);
	}

	return str;
}
 
/************************************************************
 * Change une partie d'un string par une valeur
 *
 * Cette version _m retourne le résultat en malloc (ce
 * qui veut dire que cette fonction ne touche pas à str)
 ************************************************************/
//OK
char *gs_change_m(const char *str, size_t pos, size_t len, const char *value)
{
	char *p;
	if(!str) return NULL;
	p=(char *)malloc(strlen(str)+strlen(value)-len+1);
	if(!p) return NULL;
	return gs_change(p,pos,len,value);
}

/************************************************************
 * Changer une partie d'un string en y mettant une valeur.
 *
 * si realloc fail. alors free old str
 * et ret null.
 ************************************************************/
//OK
char *gs_change_r(char *str, size_t pos, size_t len, const char *value)
{
	if(!str) return NULL;
	str=(char *)realloc(str,strlen(str)+strlen(value)-len+1);
	if(!str) return str;
	return gs_change(str,pos,len,value);
}

/************************************************************
 * Retourne le pointeur vers le premier caractère dans
 * str qui appartient à 'accept'.
 ************************************************************/
//OK
char *gs_accept(char *str, const char *accept)
{	
	while(*str) {
		if(strchr(accept,*str))
			return str;
		str++;
	}
	return NULL;
}

/************************************************************
 * La même chose que gs_accept() mais à l'envert.
 ************************************************************/
//OK
char *gs_r_accept(char *str, const char *accept)
{
	char *end;

	end=str+strlen(str);

	while(--end>=str) {
		if(strchr(accept,*end)) return end;
	}

	return NULL;
}

/************************************************************
 * Retourne le pointeur vers le premier caractère qui
 * n'appartient pas à la chaine reject.
 ************************************************************/
//OK
char *gs_reject(char *str, const char *reject)
{
	while(*str) {
		if(!strchr(reject,*str)) return str;
		str++;
	}
	return NULL;
}

//OK
char *gs_r_reject(char *str, const char *reject)
{
	char *end=str+strlen(str);

	while(--end>=str) {
		if(!strchr(reject,*end)) return end;
	}

	return NULL;
}

/************************************************************
 * Supprime l'emsemble des "reject"
 * de droite et de gauche de la chaine 'str.
 ************************************************************/
// OK
char *gs_c_trim(char *str, const char *reject)
{ return gs_c_ltrim(gs_c_rtrim(str,reject),reject); }

/************************************************************
 * Supprime l'ensemble des "reject" de gauche.
 ************************************************************/
//OK
char *gs_c_ltrim(char *str, const char *reject)
{
	char *s = gs_reject(str,reject);
	if(!s) {
		*str=0;
		return str;
	}
	if(s==str) return str;
	//strcpy(str,s);
	memmove(str,s,strlen(s)+1);
	return str;
}

/************************************************************
 * Supprime l'ensemble des "reject" de droite.
 ************************************************************/
//OK
char *gs_c_rtrim(char *str, const char *reject)
{
	char *s=gs_r_reject(str,reject);
	if(!s) {
		*str=0;
		return str;
	}
	s[1]=0;
	return str;
}

#define TRIM_REJECT " \t\v\r\n\f"
// \t tabulation horizontale
// \v tabulation verticale
// \r carriage return
// \n line feed (saut de ligne)
// \f form feed (saut de page)

/************************************************************
 * Supprime les espaces de gauche.
 ************************************************************/
//OK
char *gs_ltrim(char *str)
{
	return gs_c_ltrim(str,TRIM_REJECT);
}

/************************************************************
 * Supprime les espaces de gauche.
 ************************************************************/
//OK
char *gs_rtrim(char *str) 
{
	return gs_c_rtrim(str,TRIM_REJECT);
}

/************************************************************
 * Supprime les espaces de gauche et de droite
 ************************************************************/
//OK
char *gs_trim(char *str) 
{
	return gs_ltrim(gs_rtrim(str));
}

/************************************************************
 * realloque la chaine 'str' selon son len.
 ************************************************************/
//OK
char *gs_fix_realloc(char *str)
{
	if(!str) return 0;
	return (char *)REALLOC(str,strlen(str)+1);
}

