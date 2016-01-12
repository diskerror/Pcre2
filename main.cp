
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
	static Php::Extension extension("diskerror_pcre2", "0.1");
	
	////////////////////////////////////////////////////////////////////////////
	Php::Class<Pcre2::Replace> replace("Diskerror\\Pcre2\\Replace");
	
	replace.method("__construct", &Pcre2::Replace::__construct, {
		Php::ByVal("expression", Php::Type::String, true),
		Php::ByVal("replacement", Php::Type::String, true),
		Php::ByVal("syntaxOption", Php::Type::Numeric, false)
	});
	
	//	the PHP "exec" method also calls the C++ __invoke
	replace.method("exec", &Pcre2::Replace::exec, {
		Php::ByRef("subject", Php::Type::String, true),
		Php::ByVal("offset", Php::Type::Numeric, false)
	});
	
	extension.add(std::move(replace));
	
	////////////////////////////////////////////////////////////////////////////
	Php::Class<Pcre2::HasMatch> hasMatch("Diskerror\\Pcre2\\HasMatch");
	
	hasMatch.method("__construct", &Pcre2::HasMatch::__construct, {
		Php::ByVal("expression", Php::Type::String, true),
		Php::ByVal("syntaxOption", Php::Type::Numeric, false)
	});
	
	hasMatch.method("exec", &Pcre2::HasMatch::exec, {
		Php::ByRef("subject", Php::Type::String, true),
		Php::ByVal("offset", Php::Type::Numeric, false)
	});
	
	extension.add(std::move(hasMatch));
	
	////////////////////////////////////////////////////////////////////////////
	Php::Class<Pcre2::Match> match("Diskerror\\Pcre2\\Match");
	
	match.method("__construct", &Pcre2::Match::__construct, {
		Php::ByVal("expression", Php::Type::String, true),
		Php::ByVal("syntaxOption", Php::Type::Numeric, false)
	});
	
	match.method("exec", &Pcre2::Match::exec, {
		Php::ByRef("subject", Php::Type::String, true),
		Php::ByRef("matches", Php::Type::Array, true),
		Php::ByVal("offset", Php::Type::Numeric, false)
	});
	
	extension.add(std::move(match));
	
	// return the extension
	return extension;
}


}
