// Liu M , 2017.4.6

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



//Copyright 2017 Liu Mingyang.
//This source file is part of SimpleParser.

//SimpleParser is free software : you can redistribute it and / or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//SimpleParser is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with SimpleParser.If not, see <http://www.gnu.org/licenses/>.