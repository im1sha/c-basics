#pragma once
#include <map>
#include <fstream>
#include <stdexcept>
#include "Book.h"
#include "Reader.h"
#include "Writer.h"
#include "KeyGenerator.h"

template <class T>
class Hash
{
private:
  size_t TABLE_LENGTH; 
  KeyGenerator<T> _keyGenerator;
  Reader<T> _reader;
  Writer<T> _writer;
  std::map<int, T>* _table;

public:
  Hash(size_t, KeyGenerator<T>, Writer<T>, Reader<T>);
  ~Hash();  
  bool Add(T);
  bool Remove(int);
  T Find(int);  
  bool LoadFromFile(std::wstring path);
  bool SaveToFile(std::wstring path);
  int GetHashCode(int key)
  {
    return key % TABLE_LENGTH;
  }
};


template<class T>
Hash<T>::Hash(size_t length, KeyGenerator<T> generator, Writer<T> writer, Reader<T> reader)
{
  TABLE_LENGTH = length;
  _keyGenerator = generator;
  _reader = reader;
  _writer = writer;
  _table = new std::map<int, T>[length]; // have indexes from 0 to length -1
  for (size_t i = 0; i < length; i++)
  {
    _table[i] = std::map<int, T>();
  }
}

template<class T>
Hash<T>::~Hash()
{
  SaveToFile(L"filename.txt");
  delete[] _table;
}

template<class T>
bool Hash<T>::LoadFromFile(std::wstring path)
{
  std::vector<T> data = std::vector<T>();
  bool result = _reader.Read(&data, path);
  for (size_t i = 0; i < data.size(); i++)
  {
    Add(data.at(i));
  }
  return result;
}

template<class T>
bool Hash<T>::SaveToFile(std::wstring path)
{
  std::vector<T> data = std::vector<T>();

  for (size_t i = 0; i < TABLE_LENGTH; i++)
  {
    std::map<int, T> mapToIterate = _table[i];
    auto it = mapToIterate.begin();

    // Iterate over the map using Iterator till end.
    while (it != mapToIterate.end())
    {
      // Accessing VALUE from element pointed by it.
      T tmp = it->second;

      data.push_back(tmp);
      it++;
    }
  }
  return _writer.Write(data, path);
}

template<class T>
bool Hash<T>::Add(T item)
{
  int key = _keyGenerator.Generate(item);
  int index = GetHashCode(key);
  std::map<int, T> *mapAtIndex = &(_table[index]);

  if (mapAtIndex->count(key) > 0)
  {
    return false;
  }
  mapAtIndex->insert(std::pair<int, T>(key, item));
  return true;
}

template<class T>
bool Hash<T>::Remove(int key)
{
  int index = GetHashCode(key);
  std::map<int, T> *mapAtIndex = &(_table[index]);
  auto it = mapAtIndex->find(key);
  if (mapAtIndex->end() != it)
  {
    mapAtIndex->erase(it);
    return true;
  }
  return false;
}

template<class T>
T Hash<T>::Find(int key)
{
  int index = GetHashCode(key);
  std::map<int, T> * mapAtIndex = &(_table[index]);
  auto it = mapAtIndex->find(key);
  if (mapAtIndex->end() != it)
  {
    return mapAtIndex->at(key);
  }
  throw std::out_of_range("Hash<T>::Find: item not found");
}

