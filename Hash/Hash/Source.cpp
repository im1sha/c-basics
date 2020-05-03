#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <vector>
#include <iostream>
#include "Hash.h"
#include "Book.h"
#include "Reader.h"
#include "Writer.h"
#include "KeyGenerator.h"

#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

int main()
{
  auto gen = KeyGenerator<Book>();
  auto reader = Reader<Book>();
  auto writer = Writer<Book>();
  Hash<Book> * hash = new Hash<Book>(113, gen, writer, reader);

  while (true)
  {
    size_t feature = 0;
    std::wcout << L"\n\nENTER FEATURE NUMBER:\n"
      L"0 CREATE DEFAULT HASH TABLE\n"
      L"1 WRITE TO FILE\n"
      L"2 READ FROM FILE\n"
      L"3 ADD BOOK\n"
      L"4 FIND BOOK" << std::endl;
    std::wcin >> feature;
    std::wcout << feature << std::endl;
    switch (feature)
    {
    //0 CREATE DEFAULT HASH TABLE
    case 0:
    { 
      if (hash) delete hash;
      std::vector<Book> books = std::vector<Book>();
      books.push_back(Book(L"0", L"a", L"a", L"a", L"a", 2000, L"a", 25));
      books.push_back(Book(L"1", L"b", L"b", L"b", L"b", 2001, L"b", 405));
      books.push_back(Book(L"2", L"c", L"c", L"c", L"c", 2010, L"c", 278));
      books.push_back(Book(L"3", L"d", L"d", L"d", L"d", 2005, L"d", 527));
      books.push_back(Book(L"4", L"e", L"e", L"e", L"e", 1990, L"e", 102));
      hash = new Hash<Book>(101, gen, writer, reader);
      for (auto b : books)
      {
        std::wcout << (hash->Add(b) ? L"Add succeeded" : L"Add failed") << std::endl << std::flush;
      }
      break;
    }
    //1 WRITE TO FILE
    case 1:
    {
      if (hash)
      {      
        std::wcout << (hash->SaveToFile(L"filename.txt") ? L"Save succeeded" : L"Save failed") << std::endl << std::flush;
      }
      break;
    }
    //2 READ FROM FILE
    case 2:
    {
      if (hash)
      {
        std::wcout << (hash->LoadFromFile(L"filename.txt") ? L"Load succeeded" : L"Load failed") << std::endl << std::flush;
      }
      break;
    }
    //3 ADD BOOK
    case 3:
    {
      if (hash)
      {
        std::wstring registrationNumber = std::wstring(),
          authorFirstName = std::wstring(),
          authorMiddleName = std::wstring(),
          authorLastName = std::wstring(),
          bookName = std::wstring(),
          year = std::wstring(),
          publisher = std::wstring(),
          pagesCount = std::wstring();

        std::wcout << L"First name" << std::endl;       
        std::wcin >> authorFirstName;
        std::wcout << L"Last Name" << std::endl;       
        std::wcin >> authorLastName;
        std::wcout << L"Middle Name" << std::endl;       
        std::wcin >> authorMiddleName;
        std::wcout << L"Book Name" << std::endl;        
        std::wcin >> bookName;
        std::wcout << L"Pages Count" << std::endl;      
        std::wcin >> pagesCount;
        std::wcout << L"Publisher" << std::endl;       
        std::wcin >> publisher;
        std::wcout << L"Registration Number" << std::endl;  
        std::wcin >> registrationNumber;
        std::wcout << L"Year" << std::endl << std::flush;
        std::wcin >> year;
        try
        {
          size_t yearAsSizet = (size_t)std::stoi(year);
          size_t pagesCountAsSizet = (size_t)std::stoi(year);
          std::wcout <<
            (hash->Add(Book(registrationNumber,
              authorFirstName,
              authorMiddleName,
              authorLastName,
              bookName,
              yearAsSizet,
              publisher,
              pagesCountAsSizet))
              ? L"Add succeeded"
              : L"Add failed") << std::endl << std::flush;
        }
        catch (...)
        {
          std::wcout << "Invalid input" << std::endl << std::flush;
        }      
      }
      break;
    }
    //4 FIND BOOK
    case 4:
    {
      if (hash)
      {
        std::wstring authorFirstName = std::wstring(),
          authorMiddleName = std::wstring(),
          authorLastName = std::wstring();
        std::wcout << L"First name" << std::endl;
        std::wcin >> authorFirstName;
        std::wcout << L"Middle name" << std::endl;
        std::wcin >> authorMiddleName;
        std::wcout << L"Last name" << std::endl << std::flush;
        std::wcin >> authorLastName;
        try
        {
          auto b = hash->Find(gen.Generate(Book(L"", authorFirstName, authorMiddleName, authorLastName, L"", 0, L"", 0)));
          std::wcout << L"First name" << std::endl;
          std::wcout << b.GetAuthorFirstName() << std::endl;
          std::wcout << L"Last Name" << std::endl;
          std::wcout << b.GetAuthorLastName() << std::endl;
          std::wcout << L"Middle Name" << std::endl;
          std::wcout << b.GetAuthorMiddleName() << std::endl;
          std::wcout << L"Book Name" << std::endl;
          std::wcout << b.GetBookName() << std::endl;
          std::wcout << L"Pages Count" << std::endl;
          std::wcout << b.GetPagesCount() << std::endl;
          std::wcout << L"Publisher" << std::endl;
          std::wcout << b.GetPublisher() << std::endl;
          std::wcout << L"Registration Number" << std::endl;
          std::wcout << b.GetRegistrationNumber() << std::endl;
          std::wcout << L"Year" << std::endl;
          std::wcout << b.GetYear() << std::endl << std::flush;
        }
        catch (...)
        {
          std::wcout << L"Find failed" << std::endl << std::flush;
        }
      }
      break;
    }
    }
  }
  return 0;
}

