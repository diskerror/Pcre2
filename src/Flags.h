
#ifndef DISKERROR_PCRE2_FLAGS_H
#define DISKERROR_PCRE2_FLAGS_H

class Flags : public Php::Base
{
	int64_t _flags;
	bool _hasChanged;

	inline virtual void _setChanged(int64_t flags) const;

public:
	Flags();

	virtual void __construct(Php::Parameters &);

	virtual Php::Value add(Php::Parameters &) const;
	virtual Php::Value remove(Php::Parameters &) const;
	virtual Php::Value clear() const;

	virtual Php::Value set(Php::Parameters &) const;
	inline virtual void set(int64_t) const;

	virtual Php::Value hasFlag(Php::Parameters &) const;
	inline virtual bool hasFlag(int64_t) const;

	virtual Php::Value get(Php::Parameters &) const;
	inline virtual int64_t get(int64_t) const;

	virtual Php::Value getChanged() const;
	virtual Php::Value clearChanged() const;

};

#endif    //	DISKERROR_PCRE2_FLAGS_H
