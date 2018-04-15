NAME			= diskerror_pcre2

EXTENSION_DIR	= $(shell php-config --extension-dir)
EXTENSION 		= $(NAME).so
INI 			= $(NAME).ini
INI_PATH        = $(shell php --ini | head -1 | sed -r 's/.+: (.+)\/cli/\1/')

COMPILER		= g++
LINKER			= g++
COMPILER_FLAGS	= -Wall -c -O2 -std=c++11 -fPIC -I/usr/local/include
LINKER_FLAGS	= -shared

LDLIBS = \
	-lphpcpp \
	-L/usr/local/lib \
	-Wl,-rpath,/usr/local/lib \
	-lpcre2-8

CPP	= $(COMPILER) $(COMPILER_FLAGS) -include precompile.hpp $< -o $@

OBJECTS = \
	obj/Pcre2Base.o \
	obj/Flags.o \
	obj/Replacer.o \
	obj/Matcher.o \
	obj/main.o


all: $(EXTENSION)

pre: cleanall \
	obj/precompile.o

$(EXTENSION): obj/precompile.o $(OBJECTS)
	$(LINKER) $(OBJECTS) $(LINKER_FLAGS) $(LDLIBS) -o $@

obj/precompile.o: precompile.hpp
	mkdir -p obj
	$(COMPILER) $(COMPILER_FLAGS) $< -o $@

obj/Pcre2Base.o: src/Pcre2Base.cp src/Pcre2Base.h \
		src/flags/Base.h src/flags/Compile.h src/flags/Match.h
	$(CPP)

obj/Flags.o: src/flags/Base.cp src/flags/Base.h
	$(CPP)

obj/Replacer.o: src/Replacer.cp src/Replacer.h \
		src/Pcre2Base.h src/flags/Base.h src/flags/Compile.h src/flags/Match.h src/flags/Replace.h
	$(CPP)

obj/Matcher.o: src/Matcher.cp src/Matcher.h \
		src/Pcre2Base.h src/flags/Base.h src/flags/Compile.h src/flags/Base.h src/flags/Match.h
	$(CPP)

obj/main.o: src/main.cp src/flags/Base.h src/Pcre2Base.h src/Matcher.h src/Replacer.h \
		src/flags/Compile.h src/flags/Match.h src/flags/Replace.h
	$(CPP)


# Installation and cleanup. (tested on Debian 8 and CentOS 6)
install: $(EXTENSION)
	cp -f $(EXTENSION) $(EXTENSION_DIR)
	chmod 644 $(EXTENSION_DIR)/$(EXTENSION)
	if [ -d $(INI_PATH)/mods-available/ ]; then \
		echo "extension = "$(EXTENSION) > $(INI_PATH)/mods-available/$(INI); \
		chmod 644 $(INI_PATH)/mods-available/$(INI); \
		if [ -d $(INI_PATH)/apache2/conf.d/ ]; then \
			ln -sf $(INI_PATH)/mods-available/$(INI) $(INI_PATH)/apache2/conf.d/; \
		fi; \
		if [ -d $(INI_PATH)/cli/conf.d/ ]; then \
			ln -sf $(INI_PATH)/mods-available/$(INI) $(INI_PATH)/cli/conf.d/; \
		fi; \
		if [ -d $(INI_PATH)/cgi/conf.d/ ]; then \
			ln -sf $(INI_PATH)/mods-available/$(INI) $(INI_PATH)/cgi/conf.d/; \
		fi; \
	fi
	if [ -d /etc/php.d/ ]; then \
		echo "extension = $(EXTENSION)" > /etc/php.d/$(INI);\
		chmod 644 /etc/php.d/$(INI);\
	fi
	if [ -d /etc/php-zts.d/ ]; then \
		echo "extension = $(EXTENSION)" > /etc/php-zts.d/$(INI);\
		chmod 644 /etc/php-zts.d/$(INI);\
	fi

uninstall:
	rm -f $(EXTENSION_DIR)/$(EXTENSION)
	find $(INI_PATH) -name $(INI) | xargs rm -f
	rm -f /etc/php.d/$(INI) /etc/php-zts.d/$(INI)

clean:
	rm -rf $(EXTENSION) obj
