# Makefile for phpdivelog
# $Id$

DESTDIR=
prefix=/usr/local
datarootdir=$(DESTDIR)$(prefix)/share
datadir=$(datarootdir)/phpdivelog
INSTALL=install
INSTALL_DATA=$(INSTALL) -m 644

WEBROOT=$(DESTDIR)/var/www
LINKTO=$(WEBROOT)/phpdivelog

install: installdirs
	$(INSTALL_DATA) *.php $(datadir)
	$(INSTALL_DATA) inc/*.inc $(datadir)/inc
	$(INSTALL_DATA) lang/* $(datadir)/lang
	cp -pr templates/* $(datadir)/templates
	cp -pr install/adl/* $(datadir)/install/adl
	$(INSTALL_DATA) install/LICENSE $(datadir)/install
	$(INSTALL_DATA) install/README $(datadir)/install
	$(INSTALL_DATA) install/history $(datadir)/install
	$(INSTALL_DATA) install/*.txt $(datadir)/install
	cp -pr doc/* $(datadir)/doc
	cp -pr diver/demo/* $(datadir)/diver/demo
	touch $(datadir)/cache/.placeholder
	if [ ! -e $(LINKTO) ]; then ln -s $(datadir) $(LINKTO); fi

installdirs:
	mkdir -p $(datadir)/templates/aqua/images
	mkdir -p $(datadir)/templates/default/images
	mkdir -p $(datadir)/install/adl/template
	mkdir -p $(datadir)/lang
	mkdir -p $(datadir)/inc
	mkdir -p $(datadir)/doc/inc
	mkdir -p $(datadir)/doc/templates
	mkdir -p $(datadir)/doc/text
	mkdir -p $(datadir)/diver/demo/data
	mkdir -p $(datadir)/diver/demo/fotos/dive
	mkdir -p $(datadir)/diver/demo/fotos/site
	mkdir -p $(datadir)/diver/demo/images
	mkdir -p $(datadir)/cache
	if [ ! -e $(WEBROOT) ]; then mkdir -p $(WEBROOT); fi

uninstall:
	if [ "`readlink $(LINKTO)`" = "$(datadir)" ]; then rm -f $(LINKTO); fi
	rm -rf $(datadir)

