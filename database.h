#ifndef DATABASE_H
#define DATABASE_H

#include <string>

enum Type
{
  INT,
  DOUBLE,
  STRING,
  ARRAY
};

struct Array
{
  int size;
  Type type;
  void *items;
};

struct Entry
{
  Type type;
  std::string key;
  void *value;
};

struct Database
{
  Entry **entries; // Entry형 포인터들을 저장하는 배열
  int size;        // 배열의 크기
  // 해당 부분만 수정하여 사용한다.
  // 다른 부분은 수정하면 안됨.
};

// 엔트리를 생성한다.
Entry *create(Type type, std::string key, void *value);

// 데이터베이스를 초기화한다.
void init(Database &database);

// 데이터베이스에 엔트리를 추가한다.
void add(Database &database, Entry *entry);

// 데이터베이스에서 키에 해당하는 엔트리를 찾는다.
Entry *get(Database &database, std::string &key);

// 데이터베이스에서 키에 해당하는 엔트리를 제거한다.
void remove(Database &database, std::string &key);

// 데이터베이스를 해제한다.
void destroy(Database &database);

#endif
