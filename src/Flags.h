
#ifndef DISKERROR_PCRE2_FLAGS_H
#define DISKERROR_PCRE2_FLAGS_H

class Flags : public Php::Base
{
	uint64_t _flags;
	bool _hasChanged;

	inline virtual void _setChanged(uint64_t flags) const;

public:
	Flags()
	{};

	virtual void __construct(Php::Parameters &);

	virtual Php::Value add(Php::Parameters &) const;
	virtual Php::Value remove(Php::Parameters &) const;
	virtual Php::Value clear(Php::Parameters &) const;
	virtual Php::Value set(Php::Parameters &) const;
	virtual Php::Value hasFlag(Php::Parameters &) const;
	virtual Php::Value get(Php::Parameters &) const;
	virtual Php::Value getChanged(Php::Parameters &) const;
	virtual Php::Value clearChanged(Php::Parameters &) const;

};

#endif    //	DISKERROR_PCRE2_FLAGS_H
