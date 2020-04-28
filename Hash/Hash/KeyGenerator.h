#pragma once
#include "Book.h"

template<class T>
class KeyGenerator
{
public:
	int Generate(T) { return 0; }	
};

template<>
class KeyGenerator<Book>
{
public:
	int Generate(Book item)
	{
		auto sum = 0;
		for (auto var : item.GetAuthorAbbreviation())
		{
			sum += var;
		}
		return sum;
	}
};

