// Liu M , 2017.4.1

#pragma once

#include <string>
#include <vector>
#include <functional>

#include "token.hpp"
#include "scanerror.hpp"

using namespace std;

class scanner
{
public /*type*/ :

	using input_type = string;
	using output_type = vector<token>;

private /*member*/ :

	vector<shared_ptr<token_tag>> tags;

public /*con/de-structor*/ :

	scanner(const vector<shared_ptr<token_tag>>& _tags) :
		tags(_tags)
	{
	}

	scanner(initializer_list<shared_ptr<token_tag>> _tags) :
		tags(_tags)
	{
	}

	scanner(const scanner& another) :
		tags(another.tags)
	{
	}

	scanner(scanner&& another) :
		tags(move(another.tags))
	{
	}

public /*operator*/ :

	scanner &operator=(const scanner& another)
	{
		tags = another.tags;
	}

	scanner &operator=(scanner&& another)
	{
		tags = move(another.tags);
	}

public /*function*/ :

	bool scan(const input_type& input, output_type& output)
	{
		string::const_iterator input_iterator = input.cbegin();
		any temp_value;

		while (input_iterator != input.cend())
		{
			vector<shared_ptr<token_tag>>::const_iterator tag_iterator;
			for (tag_iterator = tags.cbegin(); tag_iterator != tags.cend(); ++tag_iterator)
			{
				if ((*tag_iterator)->cref_scan()(input, input_iterator, temp_value))
				{
					output.emplace_back(*tag_iterator, temp_value);
					break;
				}
			}
			if (tag_iterator == tags.cend())
			{
				throw scan_error(input_iterator,"unknow characters");
				return false;
			}
		}

		return true;
	}

	void swap(scanner& another)
	{
		tags.swap(another.tags);
	}

	void push_tag_back(const shared_ptr<token_tag>& tag_ptr)
	{
		tags.push_back(tag_ptr);
	}

};

class scan_ignore
{
private :

	const function<bool(string::value_type)> be_ignored;

public :

	scan_ignore(const function<bool(char)>& _be_ignored) :be_ignored(_be_ignored)
	{
	}

	bool operator()(const string& _input_string, string::const_iterator& _input_iterator, any& _output_value)
	{
		auto begin_iterator = _input_iterator; 

		while (_input_iterator != _input_string.cend() && be_ignored(*_input_iterator)) ++_input_iterator;

		_output_value = any(nullptr);
		return begin_iterator != _input_iterator;
	}
};

template<typename T>
class scan_process
{
private:

	const function<bool(const string::const_iterator&, const T&)> be_processed;
	const function<void(T&, const string::const_iterator&)> process;
	const T init_value;

public:

	scan_process(const function<bool(string::value_type)>& _be_processed, const function<void(T&)>& _process, const T& _init_value = T{}) :
		be_processed([_be_processed](const string::const_iterator& __value, const T&) { return _be_processed(*__value); }),
		process( [_process] (T& __T, const string::const_iterator&) { return _process(__T); }),
		init_value(_init_value)
	{
	}

	scan_process(const function<bool(string::value_type)>& _be_processed, const function<void(T&, string::value_type)>& _process, const T& _init_value = T{}) :
		be_processed( [_be_processed] (const string::const_iterator& __value, const T&) { return _be_processed(*__value); } ),
		process( [_process] (T& __T, const string::const_iterator& __value) { return _process(__T, *__value); }),
		init_value(_init_value)
	{
	}

	scan_process(const function<bool(string::value_type,const T&)>& _be_processed, const function<void(T&)>& _process, const T& _init_value = T{}) :
		be_processed( [_be_processed] (const string::const_iterator& __value, const T& __T) { return _be_processed(*__value, __T); }),
		process( [_process] (T& __T, const string::const_iterator&) { return _process(__T); } ),
		init_value(_init_value)
	{
	}

	scan_process(const function<bool(string::value_type, const T&)>& _be_processed, const function<void(T&, string::value_type)>& _process, const T& _init_value = T{}) :
		be_processed([_be_processed](const string::const_iterator& __value, const T& __T) { return _be_processed(*__value, __T); }),
		process([_process](T& __T, const string::const_iterator& __value) { return _process(__T, *__value); }),
		init_value(_init_value)
	{
	}

	scan_process(const function<bool(const string::const_iterator&)>& _be_processed, const function<void(T&)>& _process, const T& _init_value = T{}) :
		be_processed([_be_processed](const string::const_iterator& __value, const T& __T) { return _be_processed(__value); }),
		process([_process](T& __T, const string::const_iterator& __value) { return _process(__T); }),
		init_value(_init_value)
	{
	}

	scan_process(const function<bool(const string::const_iterator&)>& _be_processed, const function<void(T&, const string::const_iterator&)>& _process, const T& _init_value = T{}) :
		be_processed([_be_processed](const string::const_iterator& __value, const T& __T) { return _be_processed(__value); }),
		process(_process), 
		init_value(_init_value)
	{
	}

	scan_process(const function<bool(const string::const_iterator&, const T&)>& _be_processed, const function<void(T&)>& _process, const T& _init_value = T{}) :
		be_processed(_be_processed), 
		process([_process](T& __T, const string::const_iterator& __value) { return _process(__T); }),
		init_value(_init_value)
	{
	}

	scan_process(const function<bool(const string::const_iterator&,const T&)>& _be_processed, const function<void(T&, const string::const_iterator&)>& _process, const T& _init_value = T{}) :
		be_processed(_be_processed), process(_process), init_value(_init_value)
	{
	}

	bool operator()(const string& _input_string, string::const_iterator& _input_iterator, any& _output_value)
	{
		auto begin_iterator = _input_iterator;
		T value { init_value };

		while (_input_iterator != _input_string.cend() && be_processed(_input_iterator, value))
		{
			process(value,_input_iterator);
			_input_iterator += 1;
		}

		_output_value = any(value);
		return begin_iterator != _input_iterator;
	}
};

template<typename T>
class scan_match
{
private :

	const T init_value;
	const function<size_t(const string::const_iterator&,T&,const string::difference_type&)> match;

public :

	scan_match(const function<size_t(const string::const_iterator&, T&, const size_t&)>& _match, const T& _init_value = T{}) :
		match(_match), init_value(_init_value)
	{
	}

	scan_match(size_t scan_length, const function<bool(const string&, T&)>& _match, const T& _init_value = T{}) :
		match([scan_length, _match](const string::const_iterator& _it, T& _value, const size_t& _rest) { \
			if (scan_length <= _rest && _match(string(_it, _it + scan_length), _value)) return scan_length; else return (size_t)0; }),
		init_value(_init_value)
	{
	}

	scan_match(const string& to_match, const T& _init_value = T{}) :
		scan_match(to_match.size(), [to_match](const string& _be_matched, T&) { return to_match == _be_matched; }, _init_value)
	{
	}

	bool operator()(const string& _input_string, string::const_iterator& _input_iterator, any& _output_value)
	{
		T value { init_value };

		size_t  diff = match(_input_iterator, value, _input_string.cend() - _input_iterator);

		_input_iterator += diff;
		_output_value = any(value);

		return diff != 0;
	}
};


using scan_iterator = string::const_iterator;
using cscan_iterator = const string::const_iterator;

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