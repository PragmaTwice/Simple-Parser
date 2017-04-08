// Liu M , 2017.4.4

#include <iostream>
#include <cctype>
#include <algorithm>

#include "scanner.hpp"

#define _DEBUG_1

using namespace std;

ostream& operator<<(ostream& out, const any& in)
{
	if (in.type() == typeid(int)) out << any_cast<int>(in);
	else if (in.type() == typeid(string)) out << any_cast<string>(in);

	return out;
}

int  main()
{
	scanner lex{ \
		new_tag("number", \
			scan_process<int>{ isdigit, [](int& v, char c) { v *= 10; v += c - '0'; }, 0 }), \
		new_tag("letter", \
			scan_process<string>{ isalpha, [](string& v, char c) { v.push_back(c); } }), \
		new_tag("space", \
			scan_ignore{ isspace }), \
		new_tag("count-dot", \
			scan_process<int>{ [](char c) { return c == '.'; }, [](int& v) { ++v; }, 0 }), \
		new_tag("smile-symbol", \
			scan_match<nullptr_t>{ "^-^", nullptr }), \
		new_tag("exception-example", \
			scan_process<nullptr_t>{[](cscan_iterator& it) { return *it == '~'; }, [](nullptr_t&, cscan_iterator& it) { throw scan_error(it, "scanner meet '~'."); } }), \
	};

	scanner::input_type input;
	scanner::output_type output;
	scanner::iterators_type outpos;

#if  defined _DEBUG_1

	cout << (input = "berfub 1234 ... ...... .q 233 ^-^ 1 6....653254 rrr212 t") << endl;

#elif defined _DEBUG_2

	std::getline(cin, input);

#endif

	try 
	{
		if (lex.scan(input, output, outpos))
		{
			cout << "success" << endl;

			auto pos_iterator = outpos.cbegin();
			for (const auto& elem : output)
			{
				const string tag_name = elem.get_tag().cref_name();
				cout << "<token pos = '" << distance(input.cbegin(), *(pos_iterator++)) << "' ";
				if(elem.cref_value().type() != typeid(nullptr_t)) cout <<"tag = '" << tag_name << "'> " << elem.cref_value() << " </token>" <<endl;
				else cout << "tag = '" << tag_name << "' />" << endl;
			}
		}
	}
	catch (const scan_error& exception)
	{
		cout << "exception in pos" << distance(input.cbegin(), exception.error_pos) << " : " << exception.reason;
	}

	cin.get();
	return 0;
}



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