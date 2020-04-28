#include "Book.h"

Book::Book(
  std::wstring registrationNumber, 
  std::wstring authorFirstName,
  std::wstring authorMiddleName, 
  std::wstring authorLastName,
  std::wstring bookName, 
  size_t year,
  std::wstring publisher,
  size_t pagesCount)
{ 
  _registrationNumber = registrationNumber;
  _authorFirstName = authorFirstName;
  _authorMiddleName = authorMiddleName;
  _authorLastName = authorLastName;
  _bookName = bookName;
  _year = year;
  _publisher = publisher;
  _pagesCount = pagesCount;  
}

std::wstring Book::GetRegistrationNumber()
{
  return _registrationNumber;
}

std::wstring Book::GetAuthorFirstName()
{
  return _authorFirstName;
}

std::wstring Book::GetAuthorMiddleName()
{
  return _authorMiddleName;
}

std::wstring Book::GetAuthorLastName()
{
  return _authorLastName;
}

std::wstring Book::GetBookName()
{
  return _bookName;
}

size_t Book::GetYear()
{
  return _year;
}

std::wstring Book::GetPublisher()
{
  return _publisher;
}

size_t Book::GetPagesCount()
{
  return _pagesCount;
}

std::wstring Book::GetAuthorAbbreviation()
{
  const std::wstring EMPTY_STRING(L" ");
  return ((_authorLastName.empty() || _authorLastName.length() == 0) ? EMPTY_STRING : std::wstring(1, _authorLastName[0]))
    + ((_authorFirstName.empty() || _authorFirstName.length() == 0) ? EMPTY_STRING : std::wstring(1, _authorFirstName[0]))
    + ((_authorMiddleName.empty() || _authorMiddleName.length() == 0) ? EMPTY_STRING : std::wstring(1, _authorMiddleName[0]));
}
