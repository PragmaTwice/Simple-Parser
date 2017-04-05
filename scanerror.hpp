// Liu M , 2017.4.4

#pragma once

#include <map>
#include <string>

using namespace std;

class scan_error : public exception
{
public /*member*/ :

	const string::const_iterator error_pos;
	const string reason;

public /*con/de-structor*/ :

	scan_error(const string::const_iterator& _error_pos,const string& _reason) :
		error_pos(_error_pos), reason(_reason)
	{
	}

	scan_error(const scan_error& another) :
		reason(another.reason)
	{
	}

	virtual ~scan_error() {}

public /*function*/:

	virtual const char* what() const
	{
		return reason.c_str();
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