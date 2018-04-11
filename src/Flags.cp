#include "Flags.h"

void Flags::__construct(Php::Parameters &)
{
	_flags = p[0];
	_hasChanged = true;
}

inline void Flags::_setChanged(uint64_t flags) const
{
	if (flags != _flags) {
		_flags = flags;
		_hasChanged = true;
	}
}

Php::Value Flags::add(Php::Parameters &) const
{
	_setChanged(_flags | p[0]);
	return this;
}

Php::Value Flags::remove(Php::Parameters &) const
{
	_setChanged(_flags & ~p[0]);
	return $this;
}

Php::Value Flags::clear() const
{
	_setChanged(0);
	return $this;
}

Php::Value Flags::set(Php::Parameters &) const
{
	_setChanged(p[0]);
	return $this;
}

Php::Value Flags::hasFlag(Php::Parameters &) const
{
	return (bool) (_flags & p[0]);
}

Php::Value Flags::get(Php::Parameters &) const
{
	return _flags & p[0];
}

Php::Value Flags::getChanged() const
{
	return _hasChanged;
}

Php::Value Flags::clearChanged() const
{
	_hasChanged = false;
	return $this;
}
