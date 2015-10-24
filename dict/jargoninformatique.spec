Summary: French dictionary of computing jargon
Summary(fr): Dictionnaire du Jargon Informatique
Name: jargoninformatique
Version: 1.3.6
Release: 1
License: GPL
Group: Documentation
Packager: Asher256 <contact@asher256.com>
Source: http://jargon.tuxfamily.org/%{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-buildroot
URL: http://jargon.tuxfamily.org/
#BuildRequires: xorg-x11-devel fontconfig-devel freetype-devel fltk-devel
#Requires: fltk libjpeg libpng zlib 

%description
French computering dictionary.

%description -l fr
Dictionnaire contenant la description de tous les mots importants du Jargon
Informatique (plus de 10000!) avec une interface conviviale qui vous permet
de rechercher naviguer facilement pour trouver ce que vous cherchez.
Que veut dire vimiste? emaciste? nerd? Zombie? ou encore les mots que l'on
entends tous les jours dans les pubs, magazines, télé, etc. Comme par exemple
que signifie le mot ADSL? RPC? TCP/IP? un bit?

%prep
%setup -q

%build
./configure --prefix=/usr --optimiz --static-all
make

%install
rm -fr $RPM_BUILD_ROOT
make install PREFIX=$RPM_BUILD_ROOT/usr

%clean
rm -fr $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%{_bindir}/jargoninformatique
%{_datadir}/jargoninformatique/
%{_datadir}/applications/jargoninformatique.desktop
%{_datadir}/pixmaps/jargoninformatique.png
%{_datadir}/man

%changelog
* Wed Aug 24 2005 Remi Collet <remi.collet@univ-reims.fr> 1.3.4-2
- .spec update for more compatibility
* Wed Aug 24 2005 Asher256 <contact@asher256.com> 1.3.4-1
- First version of the .spec
- Version 1.3.4

