#include "Flags.h"

Flags::Flags()
{
	_flags = 0;
	_hasChanged = true;
}

void Flags::__construct(Php::Parameters &)
{
	if(p.size() > 0)
		_flags = p[0];
}

inline void Flags::_setChanged(int64_t flags) const
{
	if (flags != _flags) {
		_flags = flags;
		_hasChanged = true;
	}
}

Php::Value Flags::add(Php::Parameters &) const
{
	_setChanged(_flags | (int64_t) p[0]);
	return this;
}

Php::Value Flags::remove(Php::Parameters &) const
{
	_setChanged(_flags & ~((int64_t)p[0]));
	return $this;
}

Php::Value Flags::clear() const
{
	_setChanged(0);
	return $this;
}

Php::Value Flags::set(Php::Parameters &) const
{
	_setChanged((int64_t) p[0]);
	return $this;
}

void Flags::set(int64_t flags) const
{
	_setChanged(flags);
}

Php::Value Flags::hasFlag(Php::Parameters &) const
{
	return (bool) (_flags & (int64_t) p[0]);
}

bool Flags::hasFlag(int64_t whichFlags) const
{
	return (bool) (_flags & whichFlags);
}

Php::Value Flags::get(Php::Parameters &) const
{
	return _flags & (int64_t) p[0];
}

int64_t Flags::get(int64_t whichFlags) const
{
	return _flags & whichFlags;
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
