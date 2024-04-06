#include "database.h"
#include <iostream>

void delete_array(Array *array); // 재귀적으로 Array 의 items 가 가리키는 곳을 지워준다.

// 엔트리를 생성한다.
Entry *create(Type type, std::string key, void *value)
{
    Entry *entry = new Entry;
    entry->type = type;
    entry->key = key;
    entry->value = value;
    return entry;
}

// 데이터베이스를 초기화한다.
void init(Database &database)
{
    database.entries = nullptr;
    database.size = 0;
}

// 데이터베이스에 엔트리를 추가한다.
void add(Database &database, Entry *entry)
{
    Entry **tmp = new Entry *[database.size + 1];
    for (int i = 0; i < database.size; i++)
    {
        tmp[i] = database.entries[i];
    }
    tmp[database.size] = entry;
    database.entries = tmp;
    database.size++;
}

// 데이터베이스에서 키에 해당하는 엔트리를 찾는다.
Entry *get(Database &database, std::string &key)
{
    for (int i = 0; i < database.size; i++)
    {
        if (database.entries[i]->key == key)
        {
            return database.entries[i];
        }
    }
    std::cout << "not found" << std::endl;
    return nullptr; // 못찾을경우
}

// 데이터베이스에서 키에 해당하는 엔트리를 제거한다.
void remove(Database &database, std::string &key)
{
    if (get(database, key) == nullptr)
    {
        return;
    }

    Entry **tmp = new Entry *[database.size - 1];
    for (int i = 0, j = 0; i < database.size; i++)
    {
        if (database.entries[i]->key == key)
        {
            switch (database.entries[i]->type)
            {
            case INT:
                delete static_cast<int *>(database.entries[i]->value);
                break;
            case DOUBLE:
                delete static_cast<double *>(database.entries[i]->value);
                break;
            case STRING:
                delete static_cast<std::string *>(database.entries[i]->value);
                break;
            case ARRAY:
                delete_array(static_cast<Array *>(database.entries[i]->value));
                delete static_cast<Array *>(database.entries[i]->value);
                break;
            }
        }
        else
        {
            tmp[j++] = database.entries[i];
        }
    }
    delete[] database.entries;
    database.size--;
    database.entries = tmp;
}

// 데이터베이스를 해제한다.
void destroy(Database &database)
{
    while (database.size != 0)
    {
        remove(database, database.entries[0]->key);
    }
    database.entries = nullptr; // 데이터베이스는 이제 초기 상태이다.
}

void delete_array(Array *array) // 재귀적으로 Array 의 items 가 가리키는 곳을 지워준다.
{
    switch (array->type)
    {
    case INT:
        delete[] static_cast<int *>(array->items);
        break;
    case DOUBLE:
        delete[] static_cast<double *>(array->items);
        break;
    case STRING:
        delete[] static_cast<std::string *>(array->items);
        break;
    case ARRAY:
        delete_array(static_cast<Array *>(array->items));
        delete[] static_cast<Array *>(array->items);
        break;
    }
}
