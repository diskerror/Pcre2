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

CPP	= $(COMPILER) $(COMPILER_FLAGS) -DEXT_NAME=\"$(NAME)\" -include src/precompile.hpp $< -o $@

OBJECTS = \
	obj/Pcre2Base.o \
	obj/Replacer.o \
	obj/Matcher.o \
	obj/main.o


all: $(EXTENSION)

$(EXTENSION): obj/precompile.o $(OBJECTS)
	$(LINKER) $(OBJECTS) $(LINKER_FLAGS) $(LDLIBS) -o $@

$(INI): $(EXTENSION)
	echo "extension = "$(EXTENSION) > $(INI); \
	echo "# $(NAME).default_compile_flags = 0x40080000"		>> $(INI); \
	echo "# $(NAME).default_match_flags = 0x40000004"		>> $(INI); \
	echo "# $(NAME).default_replace_flags = 0x40000104"		>> $(INI); \
	echo "# $(NAME).jit_stack_min = 32"						>> $(INI); \
	echo "# $(NAME).jit_stack_max = 100"					>> $(INI); \
	echo "# $(NAME).compile_cache = 0"						>> $(INI); \
	echo "# $(NAME).jit_stack_min = \"/dev/shm/$(NAME)\""	>> $(INI)

obj/precompile.o: src/precompile.hpp
	mkdir -p obj
	$(COMPILER) $(COMPILER_FLAGS) $< -o $@

obj/Pcre2Base.o: src/Pcre2Base.cp src/Pcre2Base.h src/flags/Compile.h src/flags/Base.h
	$(CPP)

obj/Replacer.o: src/Replacer.cp src/Replacer.h src/Pcre2Base.h
	$(CPP)

obj/Matcher.o: src/Matcher.cp src/Matcher.h src/Pcre2Base.h src/flags/Match.h \
 src/flags/Base.h src/flags/Compile.h
	$(CPP)

obj/main.o: src/main.cp src/flags/Compile.h src/flags/Base.h src/flags/Replace.h src/flags/Match.h \
 src/Pcre2Base.h src/Matcher.h src/Replacer.h
	$(CPP)


# Installation and cleanup. (tested on Debian 8 and CentOS 6)
install: $(INI)
	cp -f $(EXTENSION) $(EXTENSION_DIR)
	chmod 644 $(EXTENSION_DIR)/$(EXTENSION)
	if [ -d $(INI_PATH)/mods-available/ ]; then \
		cp -f $(INI) $(INI_PATH)/mods-available/; \
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
		cp -f $(INI) /etc/php.d/$(INI); \
		chmod 644 /etc/php.d/$(INI); \
	fi
	if [ -d /etc/php-zts.d/ ]; then \
		cp -f $(INI) /etc/php-zts.d/$(INI); \
		chmod 644 /etc/php-zts.d/$(INI); \
	fi

uninstall:
	rm -f $(EXTENSION_DIR)/$(EXTENSION)
	find $(INI_PATH) -name $(INI) | xargs rm -f
	rm -f /etc/php.d/$(INI) /etc/php-zts.d/$(INI)

clean:
	rm -rf $(EXTENSION) $(INI) obj
