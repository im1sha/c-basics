#pragma once
#include <vector>
#include <iostream>
#include "Book.h"

template<class T>
class Writer
{
public:
	bool Write(std::vector<T> items, std::wstring filename) { return false; }
};

template<>
class Writer<Book>
{
public:
	bool Write(std::vector<Book> items, std::wstring filename)
	{
		std::wofstream out(filename);
		for (auto b : items)
		{		
			out << b.GetRegistrationNumber() << std::endl;
			out << b.GetAuthorFirstName() << std::endl;
			out << b.GetAuthorMiddleName() << std::endl;
			out << b.GetAuthorLastName() << std::endl;
			out << b.GetBookName() << std::endl;	
			out << b.GetYear() << std::endl;
			out << b.GetPublisher() << std::endl;
			out << b.GetPagesCount() << std::endl;
			out << std::endl;
		}		
		out.close();

		return true;
	}
};

