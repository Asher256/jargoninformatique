// generated by Fast Light User Interface Designer (fluid) version 1.0107

#include "main_widget.h"
#include <FL/fl_ask.H>

myList::myList(int x, int y, int w, int h, const char *str) : Fl_Hold_Browser(x,y,w,h,str) {
}

int myList::handle(int e) {
  if(e==FL_KEYBOARD /* && Fl::event_key()==FL_Escape*/ ) 
	return 1;

return Fl_Hold_Browser::handle(e);
}

myWindow::myWindow(int w, int h, const char *str) : Fl_Double_Window(w,h,str) {
  event_enabled=1;
}

int myWindow::handle(int e) {
  if(!event_enabled) return 1;

if(e==FL_KEYBOARD && Fl::event_key()==FL_Escape) 
	return 1;

return Fl_Double_Window::handle(e);
}

void NavigateurDlg::cb_ok_i(Fl_Button*, void*) {
  MainWindow->hide();
ret=1;
}
void NavigateurDlg::cb_ok(Fl_Button* o, void* v) {
  ((NavigateurDlg*)(o->parent()->user_data()))->cb_ok_i(o,v);
}

void NavigateurDlg::cb_cancel_i(Fl_Button*, void*) {
  MainWindow->hide();
}
void NavigateurDlg::cb_cancel(Fl_Button* o, void* v) {
  ((NavigateurDlg*)(o->parent()->user_data()))->cb_cancel_i(o,v);
}

void NavigateurDlg::cb_Par_i(Fl_Button*, void*) {
  if(navigateur_par_defaut) nom->value(navigateur_par_defaut);
}
void NavigateurDlg::cb_Par(Fl_Button* o, void* v) {
  ((NavigateurDlg*)(o->parent()->user_data()))->cb_Par_i(o,v);
}

NavigateurDlg::NavigateurDlg() {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = MainWindow = new Fl_Double_Window(410, 150, "Choisir le navigateur...");
    w = o;
    o->color((Fl_Color)156);
    o->user_data((void*)(this));
    { Fl_Button* o = ok = new Fl_Button(123, 115, 80, 23, "&Ok");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelfont(1);
      o->labelsize(12);
      o->labelcolor(FL_WHITE);
      o->callback((Fl_Callback*)cb_ok);
    }
    { Fl_Button* o = cancel = new Fl_Button(214, 115, 80, 23, "&Annuler");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelsize(12);
      o->labelcolor(FL_WHITE);
      o->callback((Fl_Callback*)cb_cancel);
    }
    { Fl_Group* o = box = new Fl_Group(10, 15, 389, 90);
      o->box(FL_ENGRAVED_BOX);
      o->color((Fl_Color)156);
      { Fl_Box* o = label = new Fl_Box(18, 25, 357, 20, "Mettez le nom des navigateurs (s\351par\351s par des virgules):");
        o->color(FL_WHITE);
        o->labelsize(12);
        o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
      }
      { Fl_Input* o = nom = new Fl_Input(20, 58, 275, 24);
        o->labelsize(12);
        o->textsize(12);
      }
      o->end();
    }
    { Fl_Button* o = new Fl_Button(305, 59, 80, 23, "&Par d\351""faut");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelsize(10);
      o->labelcolor(FL_WHITE);
      o->callback((Fl_Callback*)cb_Par);
    }
    o->set_modal();
    o->end();
  }
  navigateur_par_defaut=0;
}

int NavigateurDlg::show(int x, int y) {
  MainWindow->position(
x,y);
ret=0;
MainWindow->show();
while(MainWindow->shown()) Fl::wait();
return ret;
}

NavigateurDlg::~NavigateurDlg() {
  delete MainWindow;
}

void Credit::cb_Ok_i(Fl_Button*, void*) {
  MainWindow->hide();
}
void Credit::cb_Ok(Fl_Button* o, void* v) {
  ((Credit*)(o->parent()->user_data()))->cb_Ok_i(o,v);
}

Credit::Credit() {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = MainWindow = new Fl_Double_Window(395, 130);
    w = o;
    o->color((Fl_Color)156);
    o->user_data((void*)(this));
    { Fl_Group* o = group = new Fl_Group(0, 0, 395, 400);
      { Fl_Browser* o = view = new Fl_Browser(0, 0, 395, 900);
        o->box(FL_FLAT_BOX);
        o->color((Fl_Color)156);
        o->labelfont(4);
        o->labelsize(12);
        o->textfont(4);
        o->textsize(12);
      }
      o->end();
    }
    { Fl_Button* o = new Fl_Button(150, 100, 95, 20, "&Ok");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelfont(1);
      o->labelsize(12);
      o->labelcolor(FL_WHITE);
      o->callback((Fl_Callback*)cb_Ok);
    }
    o->set_modal();
    o->end();
  }
}

void Credit::show() {
  MainWindow->position(
Fl::x() + (Fl::w()-MainWindow->w())/2,
Fl::y() + (Fl::h()-MainWindow->h())/2
);

group->position(0,MainWindow->h());

MainWindow->show();
}

void Credit::add(const char *str) {
  view->add(str);
}

Credit::~Credit() {
  delete MainWindow;
}

myWindow* Interface::make_main_window() {
  myWindow* w;
  { myWindow* o = MainWindow = new myWindow(720, 535, "Jargon Informatique");
    w = o;
    o->box(FL_FLAT_BOX);
    o->color((Fl_Color)156);
    o->selection_color(FL_BACKGROUND_COLOR);
    o->labeltype(FL_NO_LABEL);
    o->labelfont(0);
    o->labelsize(14);
    o->labelcolor(FL_FOREGROUND_COLOR);
    o->user_data((void*)(this));
    o->align(FL_ALIGN_TOP);
    o->when(FL_WHEN_RELEASE);
    { Fl_Box* o = new Fl_Box(210, 21, 300, 35, "Jargon Informatique");
      o->box(FL_ROUNDED_BOX);
      o->color((Fl_Color)106);
      o->labelsize(18);
      o->labelcolor(FL_WHITE);
    }
    { Fl_Input* o = input = new Fl_Input(22, 74, 177, 25, "Mot \340 trouver:");
      o->box(FL_BORDER_BOX);
      o->labelfont(1);
      o->labelsize(12);
      o->labelcolor((Fl_Color)58);
      o->textsize(12);
      o->align(FL_ALIGN_TOP_LEFT);
      o->when(FL_WHEN_CHANGED);
    }
    { Fl_Button* o = effacer_mot = new Fl_Button(201, 76, 22, 22, "x");
      o->tooltip("Reinitialiser la recherche dans le mot \340 trouver...\nRaccourci: CTRL-u");
      o->box(FL_ROUND_UP_BOX);
      o->shortcut(0x40075);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelfont(1);
      o->labelsize(10);
      o->labelcolor(FL_BACKGROUND2_COLOR);
    }
    { Fl_Button* o = precedent = new Fl_Button(235, 75, 57, 25, "@<-");
      o->tooltip("Le mot pr\351""c\351""dent...\nRaccourci: Alt+h");
      o->box(FL_ROUND_UP_BOX);
      o->shortcut(0x80068);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelsize(12);
      o->labelcolor(FL_WHITE);
      o->deactivate();
    }
    { Fl_Button* o = suivant = new Fl_Button(295, 75, 44, 25, "@->");
      o->tooltip("Le mot suivant...\nRaccourci ALT+J");
      o->box(FL_ROUND_UP_BOX);
      o->shortcut(0x8006a);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelsize(12);
      o->labelcolor(FL_WHITE);
      o->deactivate();
    }
    { Fl_Button* o = little = new Fl_Button(341, 75, 33, 25, "F");
      o->tooltip("Diminuer la taille de la police...\nRaccourci: ALT+k");
      o->box(FL_ROUND_UP_BOX);
      o->shortcut(0x8006b);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelfont(1);
      o->labelsize(11);
      o->labelcolor(FL_WHITE);
    }
    { Fl_Button* o = big = new Fl_Button(376, 75, 33, 25, "F");
      o->tooltip("Augmenter la taille de la police...\nRaccourci: ALT+L");
      o->box(FL_ROUND_UP_BOX);
      o->shortcut(0x8006c);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelfont(1);
      o->labelcolor(FL_WHITE);
    }
    { Fl_Button* o = ouvrir_options = new Fl_Button(414, 75, 80, 25, "&Options");
      o->tooltip("Modifier certaines options du logiciel...");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelsize(11);
      o->labelcolor(FL_WHITE);
    }
    { Fl_Button* o = exporter_html = new Fl_Button(496, 75, 35, 25, "&Exp");
      o->tooltip("Exporter l\'explication au format HTML...");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelsize(10);
      o->labelcolor(FL_WHITE);
    }
    { Fl_Button* o = about = new Fl_Button(534, 75, 84, 25, "&A propos");
      o->tooltip("A propos du logiciel, mises \340 jour, auteurs...");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelsize(11);
      o->labelcolor(FL_WHITE);
    }
    { Fl_Button* o = quitter = new Fl_Button(620, 75, 80, 25, "&Quitter@>");
      o->tooltip("Quitter le logiciel...");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelsize(11);
      o->labelcolor(FL_WHITE);
    }
    { Fl_Button* o = classement_par_theme = new Fl_Button(22, 488, 201, 25, "&Classement par Theme...");
      o->tooltip("Classer les mots selon leur appartenance \340 un th\350me...\nPar exemple le \
th\350me LINUX contiendra tous les mots relatifs \340 ce syst\350me...");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelsize(11);
      o->labelcolor(FL_BACKGROUND2_COLOR);
    }
    { myList* o = list = new myList(22, 112, 201, 369);
      o->type(2);
      o->box(FL_BORDER_BOX);
      o->color(FL_WHITE);
      o->selection_color((Fl_Color)135);
      o->labeltype(FL_NORMAL_LABEL);
      o->labelfont(1);
      o->labelsize(12);
      o->labelcolor((Fl_Color)58);
      o->textsize(12);
      o->textcolor(73);
      o->align(FL_ALIGN_TOP_LEFT);
      o->when(FL_WHEN_CHANGED);
    }
    { Fl_Help_View* o = output = new Fl_Help_View(235, 112, 465, 401);
      o->box(FL_BORDER_BOX);
      o->color(FL_WHITE);
      o->labelfont(1);
      o->labelsize(12);
      o->labelcolor((Fl_Color)58);
      o->align(FL_ALIGN_TOP_LEFT);
    }
    o->end();
  }
  return w;
}

Fl_Double_Window* Interface::make_about() {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = new Fl_Double_Window(350, 495, "A propos...");
    w = o;
    o->color((Fl_Color)156);
    o->user_data((void*)(this));
    { Fl_Button* o = fermer_about = new Fl_Button(126, 462, 99, 22, "&Fermer...");
      o->tooltip("Fermer la fen\352tre...");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelsize(12);
      o->labelcolor(FL_WHITE);
    }
    { Fl_Box* o = petit_readme2 = new Fl_Box(11, 341, 324, 110, "Le contenu du dictionnaire est \351""crit par Roland Trique et quelques contr\
ibuteurs. Il est distribu\351 sous licence GNU Free Documentation Licence vers\
ion 1.1. Vous avez le droit de le distribuer, le copier et de l\'am\351liorer.\
 Le dictionnaire distribu\351 avec cette version du logiciel est le 3.4.109. L\
e site officiel du dictionnaire est\nhttp://www.linux-france.org/prj/jargonf");
      o->box(FL_ROUNDED_BOX);
      o->color((Fl_Color)57);
      o->labelsize(10);
      o->labelcolor(FL_BACKGROUND2_COLOR);
      o->align(196|FL_ALIGN_INSIDE);
    }
    { Fl_Box* o = new Fl_Box(92, 7, 160, 33, "A propos...");
      o->box(FL_RFLAT_BOX);
      o->color((Fl_Color)106);
      o->labelfont(1);
      o->labelsize(16);
      o->labelcolor(FL_WHITE);
    }
    { Fl_Box* o = label_version = new Fl_Box(92, 43, 160, 15, "Version 1.3.6 (Avril 2006)");
      o->color(FL_SELECTION_COLOR);
      o->labelfont(1);
      o->labelsize(10);
      o->labelcolor(FL_BACKGROUND2_COLOR);
    }
    { Fl_Box* o = petit_readme = new Fl_Box(11, 155, 325, 177, "Jargon Informatique est un logiciel qui vous permettra naviguer d\'une mani\
\350re conviviale dans un dictionnaire informatique tr\350s fourni (plus de 10\
000 mots!). Le dictionnaire contient tous les termes importants du Jargon Info\
rmatique. La base de donn\351""e des mots+explication a \351t\351 faite par Ro\
land Trique (voir en bas pour la licence). Le logiciel Jargon Informatique est\
 un logiciel libre distribu\351 sous la licence GNU General Public Licence Ver\
sion 2. Vous avez donc le droit de le distribuer et de l\'utiliser gratuitemen\
t. Vous pouvez aussi de consulter et am\351liorer son code source. Lisez le fi\
chier LISEZ-MOI.htm et COPYING.txt pour plus d\'informations.  ");
      o->box(FL_BORDER_BOX);
      o->color(FL_WHITE);
      o->labelsize(10);
      o->align(196|FL_ALIGN_INSIDE);
    }
    { Fl_Box* o = label_auteur = new Fl_Box(11, 66, 235, 15, "Auteur: Asher256");
      o->labelfont(5);
      o->labelsize(12);
      o->labelcolor((Fl_Color)215);
      o->align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE);
    }
    { Fl_Box* o = label_email = new Fl_Box(10, 86, 235, 15, "Email:  contact@asher256.com");
      o->labelfont(5);
      o->labelsize(12);
      o->labelcolor((Fl_Color)206);
      o->align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE);
    }
    { Fl_Box* o = label_url = new Fl_Box(11, 106, 235, 15, "Url:    http://jargon.tuxfamily.org/");
      o->labelfont(5);
      o->labelsize(12);
      o->labelcolor((Fl_Color)175);
      o->align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE);
    }
    { Fl_Button* o = show_news = new Fl_Button(280, 69, 55, 20, "&News");
      o->tooltip("La liste des mises \340 jour...");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelsize(10);
      o->labelcolor(FL_WHITE);
    }
    { Fl_Button* o = show_credit = new Fl_Button(280, 95, 55, 21, "Cr&edits");
      o->tooltip("Les credits...");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelsize(10);
      o->labelcolor(FL_WHITE);
    }
    { Fl_Button* o = show_contrib = new Fl_Button(103, 128, 140, 20, "&Contributeurs");
      o->tooltip("Liste des contributeurs...");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelfont(1);
      o->labelsize(10);
      o->labelcolor(FL_WHITE);
    }
    o->set_modal();
    o->end();
  }
  return w;
}

Fl_Double_Window* Interface::make_news() {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = new Fl_Double_Window(545, 465);
    w = o;
    o->color((Fl_Color)156);
    o->user_data((void*)(this));
    { Fl_Browser* o = news_list = new Fl_Browser(15, 13, 515, 407);
      o->labelfont(4);
      o->labelsize(12);
      o->textfont(4);
      o->textsize(12);
      Fl_Group::current()->resizable(o);
    }
    { Fl_Button* o = news_ok = new Fl_Button(237, 432, 70, 22, "&Ok");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelsize(12);
      o->labelcolor(FL_WHITE);
    }
    o->set_modal();
    o->end();
  }
  return w;
}

Fl_Double_Window* Interface::make_config_window() {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = new Fl_Double_Window(330, 180, "Configuration");
    w = o;
    o->color((Fl_Color)156);
    o->user_data((void*)(this));
    { Fl_Button* o = fermer_options = new Fl_Button(116, 146, 99, 22, "&Fermer...");
      o->tooltip("Fermer la fen\352tre...");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelsize(12);
      o->labelcolor(FL_WHITE);
    }
    { Fl_Box* o = new Fl_Box(77, 7, 176, 33, "Configuration");
      o->box(FL_ROUNDED_BOX);
      o->color((Fl_Color)106);
      o->labelfont(1);
      o->labelcolor(FL_WHITE);
    }
    { Fl_Button* o = config_navigateur = new Fl_Button(25, 54, 280, 20, "&Choisir votre navigateur Internet");
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelsize(10);
      o->labelcolor(FL_WHITE);
    }
    { Fl_Button* o = config_couleurs = new Fl_Button(25, 84, 280, 20, "&Les couleurs de l\'interface");
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelsize(10);
      o->labelcolor(FL_WHITE);
    }
    { Fl_Button* o = config_autre = new Fl_Button(25, 113, 280, 20, "&Autres Options");
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelsize(10);
      o->labelcolor(FL_WHITE);
    }
    o->set_modal();
    o->end();
  }
  return w;
}

LoadingDlg::LoadingDlg() {
  myWindow* w;
  { myWindow* o = MainWindow = new myWindow(280, 110, "Chargement...");
    w = o;
    o->box(FL_FLAT_BOX);
    o->color((Fl_Color)156);
    o->selection_color(FL_BACKGROUND_COLOR);
    o->labeltype(FL_NO_LABEL);
    o->labelfont(0);
    o->labelsize(14);
    o->labelcolor(FL_FOREGROUND_COLOR);
    o->user_data((void*)(this));
    o->align(FL_ALIGN_TOP);
    o->when(FL_WHEN_RELEASE);
    { Fl_Box* o = new Fl_Box(20, 60, 240, 15, "Chargement du dictionnaire en cours...");
      o->labelfont(2);
      o->labelsize(12);
    }
    { Fl_Progress* o = progress = new Fl_Progress(20, 79, 240, 15, "0%");
      o->box(FL_ROUNDED_BOX);
      o->color((Fl_Color)167);
      o->selection_color((Fl_Color)175);
      o->labelsize(10);
    }
    { Fl_Box* o = new Fl_Box(38, 15, 203, 33, "Jargon Informatique");
      o->box(FL_RFLAT_BOX);
      o->color((Fl_Color)106);
      o->labelfont(1);
      o->labelcolor(FL_WHITE);
    }
    o->set_non_modal();
    o->clear_border();
    o->end();
  }
  MainWindow->event_enabled=0;
}

void LoadingDlg::reset() {
  progress->value(0);
progress->label("0%");
}

LoadingDlg::~LoadingDlg() {
  delete MainWindow;
}

void SelectionCouleur::cb_Ok1_i(Fl_Button*, void*) {
  MainWindow->hide();
couleur=get_couleur_selectionnee();
}
void SelectionCouleur::cb_Ok1(Fl_Button* o, void* v) {
  ((SelectionCouleur*)(o->parent()->user_data()))->cb_Ok1_i(o,v);
}

void SelectionCouleur::cb_Annuler_i(Fl_Button*, void*) {
  MainWindow->hide();
couleur=-1;
}
void SelectionCouleur::cb_Annuler(Fl_Button* o, void* v) {
  ((SelectionCouleur*)(o->parent()->user_data()))->cb_Annuler_i(o,v);
}

SelectionCouleur::SelectionCouleur() {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = MainWindow = new Fl_Double_Window(235, 235, "Les couleurs...");
    w = o;
    o->color((Fl_Color)156);
    o->selection_color((Fl_Color)156);
    o->user_data((void*)(this));
    { Fl_Box* o = label = new Fl_Box(10, 14, 215, 40, "Vous pouvez choisir ici les couleurs de l\'interface:");
      o->labelsize(12);
      o->align(133|FL_ALIGN_INSIDE);
    }
    { Fl_Button* o = new Fl_Button(29, 190, 81, 23, "&Ok");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelsize(12);
      o->labelcolor(FL_WHITE);
      o->callback((Fl_Callback*)cb_Ok1);
    }
    { Fl_Button* o = new Fl_Button(120, 190, 82, 23, "&Annuler");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelsize(12);
      o->labelcolor(FL_WHITE);
      o->callback((Fl_Callback*)cb_Annuler);
    }
    { Fl_Round_Button* o = couleur_verte = new Fl_Round_Button(22, 59, 190, 25, "&Bleu (par d\351""faut)");
      o->tooltip("Couleur Simpatique");
      o->type(102);
      o->down_box(FL_ROUND_DOWN_BOX);
      o->color((Fl_Color)156);
      o->labelsize(12);
    }
    { Fl_Round_Button* o = couleur_rouge = new Fl_Round_Button(22, 88, 190, 25, "&Rouge");
      o->tooltip("Couleur endiabl\351""e");
      o->type(102);
      o->down_box(FL_ROUND_DOWN_BOX);
      o->color((Fl_Color)156);
      o->labelsize(12);
    }
    { Fl_Round_Button* o = couleur_bleu = new Fl_Round_Button(22, 120, 190, 25, "&Vert");
      o->tooltip("Couleur Naturelle");
      o->type(102);
      o->down_box(FL_ROUND_DOWN_BOX);
      o->color((Fl_Color)156);
      o->labelsize(12);
    }
    { Fl_Round_Button* o = couleur_jaune = new Fl_Round_Button(22, 152, 190, 25, "&Jaune");
      o->tooltip("pour les excentriques :-)");
      o->type(102);
      o->down_box(FL_ROUND_DOWN_BOX);
      o->color((Fl_Color)156);
      o->labelsize(12);
    }
    o->set_modal();
    o->end();
  }
}

int SelectionCouleur::show(int sel) {
  couleur_bleu->value(0); couleur_rouge->value(0);
couleur_verte->value(0); couleur_jaune->value(0);

if(sel==3) couleur_jaune->value(1);
else if(sel==2) couleur_bleu->value(1);
else if(sel==1) couleur_rouge->value(1);
else couleur_verte->value(1);
couleur=-1;
MainWindow->show();
while(MainWindow->shown()) { Fl::wait(); }
return couleur;
}

int SelectionCouleur::get_couleur_selectionnee() {
  if(couleur_verte->value()) return 0;
if(couleur_rouge->value()) return 1;
if(couleur_bleu->value())  return 2;
if(couleur_jaune->value()) return 3;
return -1; //si c'est une erreur, alors retouner la couleur par d�faut, le vert :-)
}

SelectionCouleur::~SelectionCouleur() {
  delete MainWindow;
}

void AutreOptionDlg::cb_ok1_i(Fl_Button*, void*) {
  MainWindow->hide();
ret=1;
}
void AutreOptionDlg::cb_ok1(Fl_Button* o, void* v) {
  ((AutreOptionDlg*)(o->parent()->user_data()))->cb_ok1_i(o,v);
}

void AutreOptionDlg::cb_cancel1_i(Fl_Button*, void*) {
  MainWindow->hide();
}
void AutreOptionDlg::cb_cancel1(Fl_Button* o, void* v) {
  ((AutreOptionDlg*)(o->parent()->user_data()))->cb_cancel1_i(o,v);
}

AutreOptionDlg::AutreOptionDlg() {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = MainWindow = new Fl_Double_Window(310, 145, "Autres Options...");
    w = o;
    o->color((Fl_Color)156);
    o->user_data((void*)(this));
    { Fl_Button* o = ok = new Fl_Button(70, 108, 80, 23, "&Ok");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelfont(1);
      o->labelsize(12);
      o->labelcolor(FL_WHITE);
      o->callback((Fl_Callback*)cb_ok1);
    }
    { Fl_Button* o = cancel = new Fl_Button(161, 108, 80, 23, "&Annuler");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelsize(12);
      o->labelcolor(FL_WHITE);
      o->callback((Fl_Callback*)cb_cancel1);
    }
    { Fl_Check_Button* o = check_fenetre_centree = new Fl_Check_Button(20, 10, 260, 25, "&D\351marrer la fen\352tre centr\351""e");
      o->tooltip("Imposer au gestionnaire de fen\352tre que l\'interface de Jargon Informatique\
 soit au centre de l\'\351""cran (au d\351marrage)...");
      o->down_box(FL_DOWN_BOX);
      o->labelsize(12);
    }
    { Fl_Check_Button* o = check_souvenir = new Fl_Check_Button(20, 41, 260, 25, "&Se souvenir du pr\351""c\351""dent mot");
      o->tooltip("Quand le logiciel sera lanc\351, il va choisir le pr\351""c\351""dent mot s\
\351lectionn\351.");
      o->down_box(FL_DOWN_BOX);
      o->labelsize(12);
    }
    { Fl_Check_Button* o = check_hide_splash = new Fl_Check_Button(20, 72, 260, 25, "&Ne pas afficher le Splash Screen");
      o->tooltip("Si vous cochez cette case, vous choisirez de ne pas avoir la fen\352tre de ch\
argement (avec le pourcentage) quand vous lancez le logiciel...");
      o->down_box(FL_DOWN_BOX);
      o->labelsize(12);
    }
    o->set_modal();
    o->end();
  }
}

int AutreOptionDlg::show(int x, int y) {
  MainWindow->position(
x,y);
ret=0;
MainWindow->show();
while(MainWindow->shown()) Fl::wait();
return ret;
}

AutreOptionDlg::~AutreOptionDlg() {
  delete MainWindow;
}

void SelectionThemeMot::cb_Ok2_i(Fl_Button*, void*) {
  //teste si au moins un th�me est s�lectionn�...
if(list->nchecked()==0) {
	fl_alert("Il faut au moins s�lectionner un th�me...");
	return;
}

//et enfin, il fini �a :-)
MainWindow->hide();
ret=1;
}
void SelectionThemeMot::cb_Ok2(Fl_Button* o, void* v) {
  ((SelectionThemeMot*)(o->parent()->user_data()))->cb_Ok2_i(o,v);
}

void SelectionThemeMot::cb_Annuler1_i(Fl_Button*, void*) {
  MainWindow->hide();
}
void SelectionThemeMot::cb_Annuler1(Fl_Button* o, void* v) {
  ((SelectionThemeMot*)(o->parent()->user_data()))->cb_Annuler1_i(o,v);
}

void SelectionThemeMot::cb_Tout_i(Fl_Button*, void*) {
  for(int i=1;i<=list->nitems();i++) list->check_all();
}
void SelectionThemeMot::cb_Tout(Fl_Button* o, void* v) {
  ((SelectionThemeMot*)(o->parent()->user_data()))->cb_Tout_i(o,v);
}

void SelectionThemeMot::cb_Tout1_i(Fl_Button*, void*) {
  for(int i=1;i<=list->nitems();i++) list->check_none();
}
void SelectionThemeMot::cb_Tout1(Fl_Button* o, void* v) {
  ((SelectionThemeMot*)(o->parent()->user_data()))->cb_Tout1_i(o,v);
}

SelectionThemeMot::SelectionThemeMot() {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = MainWindow = new Fl_Double_Window(385, 385, "Themes de mots");
    w = o;
    o->color((Fl_Color)156);
    o->selection_color((Fl_Color)156);
    o->user_data((void*)(this));
    { Fl_Button* o = new Fl_Button(285, 15, 92, 23, "&Ok");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelfont(1);
      o->labelsize(12);
      o->labelcolor(FL_WHITE);
      o->callback((Fl_Callback*)cb_Ok2);
    }
    { Fl_Button* o = new Fl_Button(285, 47, 92, 23, "&Annuler");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelfont(2);
      o->labelsize(12);
      o->labelcolor(FL_WHITE);
      o->callback((Fl_Callback*)cb_Annuler1);
    }
    { Fl_Check_Browser* o = list = new Fl_Check_Browser(9, 15, 266, 332);
      o->box(FL_DOWN_BOX);
      o->labelsize(12);
      o->textsize(12);
      o->textcolor(72);
      Fl_Group::current()->resizable(o);
    }
    { Fl_Button* o = new Fl_Button(10, 353, 125, 23, "Tout &Sel.");
      o->tooltip("Selectionner tous les th\350mes...");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelsize(12);
      o->labelcolor(FL_WHITE);
      o->callback((Fl_Callback*)cb_Tout);
    }
    { Fl_Button* o = new Fl_Button(150, 353, 125, 23, "Tout &Desel.");
      o->tooltip("D\351selectionner tout les th\350mes...");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelsize(12);
      o->labelcolor(FL_WHITE);
      o->callback((Fl_Callback*)cb_Tout1);
    }
    o->set_modal();
    o->end();
  }
  MainWindow->size_range(MainWindow->w(),MainWindow->h());
}

int SelectionThemeMot::show() {
  ret=0;
MainWindow->show();
while(MainWindow->shown()) {
	Fl::wait();
}
return ret;
}

SelectionThemeMot::~SelectionThemeMot() {
  delete MainWindow;
}

void ContribWindow::cb_Fermer_i(Fl_Button*, void*) {
  MainWindow->hide();
}
void ContribWindow::cb_Fermer(Fl_Button* o, void* v) {
  ((ContribWindow*)(o->parent()->user_data()))->cb_Fermer_i(o,v);
}

ContribWindow::ContribWindow() {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = MainWindow = new Fl_Double_Window(405, 395, "Contributeurs");
    w = o;
    o->color((Fl_Color)156);
    o->selection_color((Fl_Color)156);
    o->user_data((void*)(this));
    { Fl_Button* o = new Fl_Button(151, 352, 109, 24, "&Fermer");
      o->box(FL_ROUND_UP_BOX);
      o->color((Fl_Color)106);
      o->selection_color((Fl_Color)106);
      o->labelsize(12);
      o->labelcolor(FL_WHITE);
      o->callback((Fl_Callback*)cb_Fermer);
    }
    { Fl_Box* o = new Fl_Box(70, 90, 65, 20, "Miguel2i");
      o->labelfont(1);
    }
    { Fl_Box* o = new Fl_Box(15, 110, 180, 20, "Miguel2i@@free.fr");
      o->labelfont(1);
      o->labelcolor((Fl_Color)178);
    }
    { Fl_Box* o = new Fl_Box(15, 132, 180, 82, "B\352ta test, correction de plusieurs bugs. Ajout de certaines fonctions,  re\
marques sur le code source.");
      o->labelsize(12);
      o->align(129|FL_ALIGN_INSIDE);
    }
    { Fl_Box* o = new Fl_Box(260, 90, 65, 21, "Superna");
      o->labelfont(1);
    }
    { Fl_Box* o = new Fl_Box(208, 110, 180, 20, "superna@@na-prod.com");
      o->labelfont(1);
      o->labelcolor((Fl_Color)178);
    }
    { Fl_Box* o = new Fl_Box(208, 132, 182, 82, "B\352ta test, Contribution avec le support debian i386 et amd64, support MacO\
SX. Compatibilit\351 avec ces syst\350mes.");
      o->labelsize(12);
      o->align(129|FL_ALIGN_INSIDE);
    }
    { Fl_Box* o = new Fl_Box(115, 13, 175, 35, "Contributeurs");
      o->box(FL_ROUNDED_BOX);
      o->color((Fl_Color)207);
      o->labelfont(1);
      o->labelsize(16);
    }
    { Fl_Box* o = new Fl_Box(67, 56, 270, 25, "Remerciement sp\351""ciaux \340:");
      o->labelfont(1);
    }
    { Fl_Box* o = new Fl_Box(120, 219, 165, 21, "Gauvain Pocentek");
      o->labelfont(1);
    }
    { Fl_Box* o = new Fl_Box(96, 241, 213, 20, "gauvainpocentek@@yahoo.fr");
      o->labelfont(1);
      o->labelcolor((Fl_Color)178);
    }
    { Fl_Box* o = new Fl_Box(86, 263, 233, 80, "Propositions pour rendre le logiciel compatible avec la Debian policy, manpag\
e (fran\347""ais et anglais), ajout de Jargon Informatique dans la distributio\
n Ubuntu.");
      o->labelsize(12);
      o->align(129|FL_ALIGN_INSIDE);
    }
    o->set_modal();
    o->end();
    o->resizable(o);
  }
}

ContribWindow::~ContribWindow() {
  delete MainWindow;
}

void ContribWindow::show() {
  MainWindow->show();
while(MainWindow->shown()) { Fl::wait(); }
}
