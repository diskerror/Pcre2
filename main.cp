
#include "Pcre2/Replace.h"
#include "Pcre2/HasMatch.h"
#include "Pcre2/Match.h"

using namespace std;


extern "C" {

/**
 *  Function that is called by PHP right after the PHP process
 *  has started, and that returns an address of an internal PHP
 *  strucure with all the details and features of your extension
 *
 *  @return void*   a pointer to an address that is understood by PHP
 */
PHPCPP_EXPORT void *get_module()
{
	// static(!) Php::Extension object that should stay in memory
	// for the entire duration of the process (that's why it's static)
	static Php::Extension extension("diskerror_pcre2", "0.2");
	
	////////////////////////////////////////////////////////////////////////////
	//	the PHP "exec" method also calls the C++ __invoke
	Php::Class <Pcre2::Replace> replace("Diskerror\\Pcre2\\Replace");
	replace.method<&Pcre2::Replace::__construct>("__construct");
	replace.method<&Pcre2::Replace::exec>("exec");
	extension.add(std::move(replace));

	////////////////////////////////////////////////////////////////////////////
	Php::Class <Pcre2::HasMatch> hasMatch("Diskerror\\Pcre2\\HasMatch");
	hasMatch.method<&Pcre2::HasMatch::__construct>("__construct");
	hasMatch.method<&Pcre2::HasMatch::exec>("exec");
	extension.add(std::move(hasMatch));

	////////////////////////////////////////////////////////////////////////////
	Php::Class <Pcre2::Match> match("Diskerror\\Pcre2\\Match");
	match.method<&Pcre2::Match::__construct>("__construct");
	match.method<&Pcre2::Match::exec>("exec");
	extension.add(std::move(match));
	
	// return the extension
	return extension;
}


}    //	extern "C"
