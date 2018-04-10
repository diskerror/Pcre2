
#include "Pcre2/Replacer.h"
#include "Pcre2/HasMatch.h"
#include "Pcre2/Matcher.h"

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
	Php::Class <Pcre2::Replacer> replacer("Diskerror\\Pcre2\\Replacer");

	replacer.method<&Pcre2::Replacer::__construct>("__construct", {
		Php::ByVal("expression", Php::Type::String, true),
		Php::ByVal("replacement", Php::Type::String, false),
		Php::ByVal("compileOptions", Php::Type::Numeric, false),
		Php::ByVal("matchOptions", Php::Type::Numeric, false)
	});

	replacer.method<&Pcre2::Replacer::replace>("replace", {
		Php::ByVal("subject", Php::Type::String, true),
		Php::ByVal("offset", Php::Type::Numeric, false)
	});

	extension.add(std::move(replacer));

	////////////////////////////////////////////////////////////////////////////
	Php::Class <Pcre2::Matcher> matcher("Diskerror\\Pcre2\\Matcher");

	matcher.method<&Pcre2::Matcher::__construct>("__construct", {
		Php::ByVal("expression", Php::Type::String, true),
		Php::ByVal("compileOptions", Php::Type::Numeric, false),
		Php::ByVal("matchOptions", Php::Type::Numeric, false)
	});

	hasMatch.method<&Pcre2::Matcher::hasMatch>("hasMatch", {
		Php::ByVal("subject", Php::Type::String, true),
		Php::ByVal("offset", Php::Type::Numeric, false)
	});

	matcher.method<&Pcre2::Matcher::match>("match", {
		Php::ByVal("subject", Php::Type::String, true),
		Php::ByVal("offset", Php::Type::Numeric, false)
	});

	matcher.method<&Pcre2::Matcher::matchAll>("matchAll", {
		Php::ByVal("subject", Php::Type::String, true),
		Php::ByVal("offset", Php::Type::Numeric, false)
	});

	extension.add(std::move(matcher));
	
	// return the extension
	return extension;
}


}    //	extern "C"
