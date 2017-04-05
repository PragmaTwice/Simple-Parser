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

		_output_value = any(0);
		return begin_iterator != _input_iterator;
	}
};

template<typename T>
class scan_process
{
private:

	const function<bool(string::value_type)> be_processed;
	const function<void(T&, string::value_type)> process;
	const T init_value;

public:

	scan_process(const function<bool(char)>& _be_processed, const function<void(T&, string::value_type)>& _process, const T& _init_value = T{}) :
		be_processed(_be_processed), process(_process), init_value(_init_value)
	{
	}

	bool operator()(const string& _input_string, string::const_iterator& _input_iterator, any& _output_value)
	{
		auto begin_iterator = _input_iterator;
		T value { init_value };

		while (_input_iterator != _input_string.cend() && be_processed(*_input_iterator))
		{
			process(value,*_input_iterator);
			_input_iterator += 1;
		}

		_output_value = any(value);
		return begin_iterator != _input_iterator;
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