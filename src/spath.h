//============================================
// Voir spath.c pour plus de détails
// sur la licence et le fonctionnement 
// de la librairie.
//
// Auteur: Asher256
// Email:  contact@asher256.com
//============================================
// Fichier source distribué en GPL v2
//============================================

#ifndef SPATH_H
#define SPATH_H

#ifdef __cplusplus
	extern "C" {
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct {
	struct stat s;
	const char *filename;
} SP_STAT;

#ifdef WIN32
#define SLASH     "\\"
#define C_SLASH   '\\'
#define SLASH2    "/"
#define C_SLASH2  '/'
#else
#define SLASH     "/"
#define C_SLASH   '/'
#define SLASH2    "\\"
#define C_SLASH2  '\\'
#endif

#define isslash(s) (s==C_SLASH)
#define isslash2(s) (s==C_SLASH || s==C_SLASH2)

//paths
char *sp_executable_path(const char *argv0);
char *sp_pwd();
#ifdef WIN32
char *sp_pwd_drive(char drive);
#endif
char *sp_truepath(const char *path);
char *sp_dirname(char *path);
char *sp_extname(char *path);
const char *sp_extname_p(const char *path);
char *sp_basename(char *path);
const char *sp_basename_p(const char *path);
char *sp_pathcat(const char *path1, const char *path2);
char *sp_which(char *commande);

//fixation dans le path
char *sp_fix_slash_repeated(char *path);
char *sp_make_system_slash(char *path);
char *sp_fix_p_cdpp(char *path);

// pour lire des informations sur
// les fichiers
SP_STAT *sp_stat(const char *filename, SP_STAT *info);
int sp_isdir(SP_STAT *info);
int sp_isreg(SP_STAT *info);
int sp_isexec(SP_STAT *info);
int sp_file_exists(const char *filename);
int sp_exists(const char *filename);

#ifdef __cplusplus
	}
#endif

#endif

