// Liu M , 2017.4.1

#pragma once

#include <string>
#include <functional>
#include <memory>

#include "any.hpp"

#define scan_lambda(_input_string, _input_iterator, _output_value) \
 [](const string& _input_string, string::const_iterator& _input_iterator, any& _output_value)->bool

using namespace std;

class token
{
public /*class*/ :

	class tag_type
	{
	public /*type*/ :

		using name_type = string;
		using scan_type = function<bool(const string&, string::const_iterator&, any&)>;

	private /*member*/:

		name_type name;
		scan_type scan;

	public /*con/de-structor*/:

		tag_type(const name_type& _name,const scan_type& _scan) :
			name(_name), scan(_scan)
		{
		}

		tag_type(const tag_type& another) :
			name(another.name), scan(another.scan)
		{
		}

		tag_type(tag_type&& another) :
			name(move(another.name)), scan(move(another.scan))
		{
		}

	public /*operator*/:

		tag_type& operator=(const tag_type& another)
		{
			name = another.name;
			scan = another.scan;

			return *this;
		}

		tag_type& operator=(tag_type&& another)
		{
			name = move(another.name);
			scan = move(another.scan);

			return *this;
		}

		bool operator==(const tag_type& another)
		{
			return name == another.name;
		}

		bool operator!=(const tag_type& another)
		{
			return name != another.name;
		}

		bool operator<(const tag_type& another)
		{
			return name < another.name;
		}

		bool operator>(const tag_type& another)
		{
			return name > another.name;
		}

		bool operator<=(const tag_type& another)
		{
			return name < another.name;
		}

		bool operator>=(const tag_type& another)
		{
			return name >= another.name;
		}

	public /*function*/ :

		inline void swap(tag_type& another)
		{
			name.swap(another.name);
			scan.swap(another.scan);
		}

		inline void reset(const name_type& _name, const scan_type& _scan)
		{
			name = _name;
			scan = _scan;
		}

		inline void reset(const scan_type& _scan)
		{
			scan = _scan;
		}

		inline const name_type& cref_name() const
		{
			return name;
		}

		inline const scan_type &cref_scan() const
		{
			return scan;
		}
	};

private /*member*/ :

	shared_ptr<tag_type> tag;
	any value;

public /*con/de-structor*/:

	token(const shared_ptr<tag_type>& _tag, const any& _val) :
		tag(_tag), value(_val)
	{
	}

	token(const token& another) :
		tag(another.tag),value(another.value)
	{
	}

	token(token&& another) :
		tag(another.tag), value(move(another.value))
	{
	}

public /*operator*/ :

	token& operator=(const token& another)
	{
		tag = another.tag;
		value = another.value;
	}

	token& operator=(token&& another)
	{
		tag = move(another.tag);
		value = move(another.value);
	}

public /*function*/ :

	inline void swap(token& another)
	{
		std::swap(tag, another.tag);
		value.swap(another.value);
	}

	template <typename T>
	inline void reset(const shared_ptr<tag_type>& _tag, const T& _val)
	{
		tag = _tag;
		value = _val;
	}

	template <typename T>
	inline void reset(const T& _val)
	{
		value = _val;
	}

	inline const type_info& value_type() const
	{
		return value.type();
	}

	inline const shared_ptr<tag_type>& cref_tag() const
	{
		return tag;
	}

	inline tag_type get_tag() const
	{
		return *tag;
	}

	inline const any& cref_value() const
	{
		return value;
	}

	template<typename T>
	inline T get_value() const
	{
		return any_cast<T>(value);
	}
};

using token_tag = token::tag_type;

#define new_tag(...) shared_ptr<token_tag>{ new token_tag{ __VA_ARGS__ } }

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