#pragma once

#include "token.hpp"

using namespace std;

using terminator = token;

shared_ptr<token_tag> unterminator_tag = new_tag(nullptr, nullptr);

class unterminator : private token
{
public /*con/de-structor*/ :

	unterminator(const string& _name) :
		token(unterminator_tag, _name)
	{
	}

	unterminator(const unterminator& another) :
		token(another)
	{
	}

	unterminator(unterminator&& another) :
		token(move(another))
	{
	}

public /*operator*/ :

	unterminator& operator=(const unterminator& another)
	{
		return static_cast<unterminator&>(token::operator=(another));
	}

	unterminator& operator=(unterminator&& another)
	{
		return static_cast<unterminator&>(token::operator=(move(another)));
	}

public /*function*/:

	inline void swap(unterminator& another)
	{
		token::swap(another);
	}

	inline void reset(const string& _name)
	{
		token::reset<string>(_name);
	}

	inline const any& cref_value() const
	{
		return token::cref_value();
	}

	inline string get_value() const
	{
		return token::get_value<string>();
	}
};