#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "Book.h"

template<class T>
class Reader
{
public:
	bool Read(std::vector<T> * items, std::wstring filename) { return false; }
};

template<>
class Reader<Book>
{
public:
	bool Read(std::vector<Book> * items, std::wstring filename)
	{
		std::wifstream in(filename);

		while (true)
		{	
			std::wstring registrationNumber = std::wstring(),
				authorFirstName = std::wstring(),
				authorMiddleName = std::wstring(),
				authorLastName = std::wstring(),
				bookName = std::wstring(),
				year = std::wstring(),
				publisher = std::wstring(),
				pagesCount = std::wstring(),
				emptyLine = std::wstring();

			if (!std::getline(in, registrationNumber)) break;
			if (!std::getline(in, authorFirstName)) break;
			if (!std::getline(in, authorMiddleName)) break;
			if (!std::getline(in, authorLastName)) break;
			if (!std::getline(in, bookName)) break;
			if (!std::getline(in, year)) break;
			if (!std::getline(in, publisher)) break;
			if (!std::getline(in, pagesCount)) break;

			if (!std::getline(in, emptyLine)) break;
			 			
			items->push_back(Book(registrationNumber,
				authorFirstName,
				authorMiddleName,
				authorLastName,
				bookName,
				(size_t)std::stoi(year),
				publisher,
				(size_t)std::stoi(pagesCount)));
		}
			
	  in.close();
		return true;
	}
};

