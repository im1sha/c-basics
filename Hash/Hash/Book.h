#pragma once
#include <string>

class Book
{
private:
  std::wstring _registrationNumber;
  std::wstring _authorFirstName;
  std::wstring _authorMiddleName;
  std::wstring _authorLastName;
  std::wstring _bookName;
  size_t _year;
  std::wstring _publisher;
  size_t _pagesCount;
public:
  Book(
    std::wstring registrationNumber,
    std::wstring authorFirstName,
    std::wstring authorMiddleName,
    std::wstring authorLastName,
    std::wstring bookName,
    size_t year,
    std::wstring publisher,
    size_t pagesCount);
  
  std::wstring GetRegistrationNumber();
  std::wstring GetAuthorFirstName();
  std::wstring GetAuthorMiddleName();
  std::wstring GetAuthorLastName();
  std::wstring GetBookName();
  size_t GetYear();
  std::wstring GetPublisher();
  size_t GetPagesCount();
  std::wstring GetAuthorAbbreviation(); // key
};

