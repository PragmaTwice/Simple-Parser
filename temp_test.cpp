#include <iostream>
#include <string>

#define main _main

int main()
{
	std::string s("12345");
	std::cout << s.cend() - s.cbegin() << std::endl;

	std::cin.get();

	return 0;
}