NAME			= diskerror_pcre2

PHPE			= /etc/php5
INI_DIR			= $(PHPE)/mods-available/
EXTENSION_DIR	= $(shell php-config --extension-dir)
EXTENSION 		= $(NAME).so
INI 			= $(NAME).ini

COMPILER		= clang
LINKER			= clang
COMPILER_FLAGS	= -Wall -c -Ofast -std=c++11 -fpic -I/usr/local/include
LINKER_FLAGS	= -shared

LDLIBS = \
	-lphpcpp \
	-L/usr/local/lib \
	-Wl,-rpath,/usr/local/lib \
	-lpcre2-8

CPP	= $(COMPILER) $(COMPILER_FLAGS) -include "precompile.hpp" $< -o $@

OBJECTS = \
	obj/Pcre2.o \
	obj/Replace.o \
	obj/HasMatch.o \
	obj/Match.o \
	obj/main.o


all: $(EXTENSION)

pre: cleanpre \
	obj/precompile.o

$(EXTENSION): obj/precompile.o $(OBJECTS)
	$(LINKER) $(OBJECTS) $(LINKER_FLAGS) $(LDLIBS) -o $@

obj/precompile.o: precompile.hpp
	mkdir -p obj
	$(COMPILER) $(COMPILER_FLAGS) $< -o $@

obj/Pcre2.o: Pcre2/Pcre2.cp Pcre2/Pcre2.h
	$(CPP)

obj/Replace.o: Pcre2/Replace.cp Pcre2/Replace.h
	$(CPP)

obj/HasMatch.o: Pcre2/HasMatch.cp Pcre2/HasMatch.h
	$(CPP)

obj/Match.o: Pcre2/Match.cp Pcre2/Match.h
	$(CPP)

obj/main.o: main.cp Pcre2/Replace.h Pcre2/HasMatch.h Pcre2/Match.h
	$(CPP)


install: $(EXTENSION)
	cp -f $(EXTENSION) $(EXTENSION_DIR)
	chmod 644 $(EXTENSION_DIR)/$(EXTENSION)
	echo "extension = "$(EXTENSION)"\n" > $(INI_DIR)$(INI)
	chmod 644 $(INI_DIR)$(INI)
	if [ -d $(PHPE)/apache2/conf.d/ ]; then \
		ln -sf $(INI_DIR)$(INI) $(PHPE)/apache2/conf.d/;\
	fi
	if [ -d $(PHPE)/cli/conf.d/ ]; then \
		ln -sf $(INI_DIR)$(INI) $(PHPE)/cli/conf.d/;\
	fi
	if [ -d $(PHPE)/cgi/conf.d/ ]; then \
		ln -sf $(INI_DIR)$(INI) $(PHPE)/cgi/conf.d/;\
	fi

uninstall:
	rm -f $(EXTENSION_DIR)/$(EXTENSION) \
		$(INI_DIR)$(INI) \
		$(PHPE)/apache2/conf.d/$(INI) \
		$(PHPE)/cli/conf.d/$(INI) \
		$(PHPE)/cgi/conf.d/$(INI)


clean:
	rm -f $(EXTENSION) $(OBJECTS)

cleanpre:
	rm -rf obj
