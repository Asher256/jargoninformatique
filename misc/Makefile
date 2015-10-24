# Avant d'exécuter Make il vous faut:
#  sous linux tapez:         ./configure.sh
#  sous windows+mingw tapez: configure.bat

# lis la configuration
OLD_PREFIX:=$(PREFIX)

include config.mak

ifneq ($(OLD_PREFIX),)
ifneq ($(OLD_PREFIX),$(PREFIX))
	PREFIX:=$(OLD_PREFIX)
endif
endif

ifeq ($(SYSTEM),mingw)
RES=$(OBJ_DIR)/xjargon.res
endif

RM = rm -f
CP = cp

SRC_DIR=src

# vpath
vpath %.cpp $(SRC_DIR)
vpath %.c $(SRC_DIR)
vpath %.o $(OBJ_DIR)

# les fichiers objet
OBJS = flnice.o spath.o main_widget.o gstr.o jargon_lib.o main.o

all: $(EXE) 

%.o : %.cpp src/config.h
	$(CPP) -c $< $(CFLAGS) -o $(OBJ_DIR)/$@

%.o : %.c
	$(CC) -c $< $(CFLAGS) -o $(OBJ_DIR)/$@

$(EXE) : $(OBJS) $(RES)
	$(CPP) $(addprefix $(OBJ_DIR)/, $(OBJS)) $(RES) -o $(EXE) $(LIBS)

clean :
	$(RM) $(addprefix $(OBJ_DIR)/, $(OBJS)) 
	$(RM) $(EXE)

ifeq ($(SYSTEM),darwin)
install:
	tar xfz misc/mac-bundle.tar.gz
	cp -r dict jargoninformatique.app/Contents/MacOS
	$(CP) $(EXE) jargoninformatique.app/Contents/MacOS
endif

ifeq ($(SYSTEM),linux)
distclean: clean
	$(RM) Makefile configure-last config.mak 

install :
	install -d $(PREFIX)/bin
	install -s -m 755 $(EXE) $(PREFIX)/bin
	install -d $(PREFIX)/share/jargoninformatique/dict/fig $(PREFIX)/share/jargoninformatique/dict/deco $(PREFIX)/share/jargoninformatique/dict/general
	install -m 644 dict/jargon.dic $(PREFIX)/share/jargoninformatique/dict 
	install -m 644 dict/fig/* $(PREFIX)/share/jargoninformatique/dict/fig
	install -m 644 dict/deco/* $(PREFIX)/share/jargoninformatique/dict/deco
	install -m 644 dict/general/* $(PREFIX)/share/jargoninformatique/dict/general
	install -d $(PREFIX)/share/jargoninformatique/misc
	install -m 644 misc/logo.png $(PREFIX)/share/jargoninformatique/misc
	install -d $(PREFIX)/share/applications $(PREFIX)/share/pixmaps
	install -m 644 misc/jargoninformatique.desktop $(PREFIX)/share/applications
	install -m 644 misc/icon.png $(PREFIX)/share/pixmaps/jargoninformatique.png
	install -d $(PREFIX)/share/man/man1 $(PREFIX)/share/man/fr/man1
	install -m 644 misc/jargoninformatique.1.gz $(PREFIX)/share/man/man1
	install -m 644 misc/jargoninformatique.fr.1.gz $(PREFIX)/share/man/fr/man1/jargoninformatique.1.gz

uninstall :
	$(RM) $(PREFIX)/bin/$(EXE)
	$(RM) $(PREFIX)/share/jargoninformatique -fr
	$(RM) $(PREFIX)/share/applications/jargoninformatique.desktop
	$(RM) $(PREFIX)/share/pixmaps/jargoninformatique.png
	$(RM) $(PREFIX)/share/man/man1/jargoninformatique.1.gz
	$(RM) $(PREFIX)/share/man/fr/man1/jargoninformatique.1.gz
endif


ifeq ($(SYSTEM),mingw)
$(OBJ_DIR)/xjargon.res : $(SRC_DIR)/xjargon.rc
	windres -i $(SRC_DIR)/xjargon.rc -I rc -o $(OBJ_DIR)/xjargon.res -O coff
endif

