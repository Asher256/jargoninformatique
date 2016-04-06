//==================================================================
// Voir le fichier Fl_Nice.cpp pour plus
// d'informations
//
// Copyright (c) Asher256
// Email:  asher256@gmail.com
// Site:   http://jargon.tuxfamily.org/
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

#ifndef __FL_NICE_H
#define __FL_NICE_H

#include "config.h"

#if !defined(LINUX) && !defined(WIN32)
#error "vous devez définir soit LINUX soit WIN32..."
#endif

#if defined(LINUX) && defined(WIN32)
#error "Vous n'avez pas le droit de definir au meme temps LINUX et WIN32"
#endif

#if defined(WIN32) && defined(USE_FLU)
#error "Vous n'avez pas le droit de definir au meme temps WIN32 et USE_FLU"
#endif

//====> les includes
#include <FL/Fl_Window.H>

// Pattern
typedef struct { //le dernier pattern doit être zero
	char *name;   // Par exemple: "Fichier exécutable"
	char *ext;    // par exemple: "exe"
} FN_PATTERN;

// Dir Search
const char *fn_dir_chooser(const char *message, const char *fname);

// File Load/Save 
const char *fn_file_chooser(const char *message, FN_PATTERN *pattern, const char *fname);
const char *fn_save_chooser(const char *message, FN_PATTERN *pattern, const char *fname);

// Messages box
void fn_alert(const char *format, ...);
void fn_message(const char *format, ...);
int fn_ask(const char *format, ...);

#endif

