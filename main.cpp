#include "database.h"
#include <iostream>
#include <string>

void command_list(Database &database);
void command_add(Database &database);
void command_get(Database &database);
void command_del(Database &database);

void print_entry(Entry *entry);  // Entry 를 출력한다
void print_array(Array *array);  // 재귀적으로 Array 를 출력한다
Array *add_array(void);          // 재귀적으로 Array 를 만든다
void delete_array(Array *array); // 재귀적으로 Array 의 items 가 가리키는 곳을 지워준다.

int main(void)
{
    Database database;
    init(database);
    std::string command_string;

    while (true)
    {
        std::cout << "command (list, add, get, del, exit): ";
        std::cin >> command_string;

        if (command_string == "exit")
        {
            break;
        }
        else if (command_string == "list")
        {
            command_list(database);
        }
        else if (command_string == "add")
        {
            command_add(database);
        }
        else if (command_string == "get")
        {
            command_get(database);
        }
        else if (command_string == "del")
        {
            command_del(database);
        }
        else
        {
            std::cout << "invalid command" << std::endl;
        }
    }

    destroy(database);
}

void command_list(Database &database)
{
    for (int i = 0; i < database.size; i++)
    {
        print_entry(database.entries[i]);
    }
}
void command_add(Database &database)
{
    std::cout << "key: ";
    std::string key;
    std::cin >> key;

    for (int i = 0; i < database.size; i++)
    {
        if (database.entries[i]->key == key)
        {
            remove(database, key);
        }
    }

    std::cout << "type: (int, double, string, array): ";
    std::string type;
    std::cin >> type;

    if (type == "array")
    {
        std::cout << "value: ";
        Array *value = add_array();
        add(database, create(ARRAY, key, value));
    }
    else if (type == "int")
    {
        std::cout << "value: ";
        int *value = new int;
        std::cin >> *value;

        add(database, create(INT, key, value));
    }
    else if (type == "double")
    {
        std::cout << "value: ";
        double *value = new double;
        std::cin >> *value;

        add(database, create(DOUBLE, key, value));
    }
    else if (type == "string")
    {
        std::cout << "value: ";
        std::string *value = new std::string;
        std::cin.ignore();
        std::getline(std::cin, *value);

        add(database, create(STRING, key, value));
    }
    else
    {
        std::cout << "invalid type" << std::endl;
    }
}
void command_get(Database &database)
{
    std::cout << "key: ";
    std::string key;
    std::cin >> key;

    if (get(database, key) != nullptr)
    {
        print_entry(get(database, key));
    }
}
void command_del(Database &database)
{
    std::cout << "key: ";
    std::string key;
    std::cin >> key;

    remove(database, key);
}

void print_entry(Entry *entry)
{
    if (entry == nullptr)
    {
    }
    std::cout << entry->key << ": ";
    switch (entry->type)
    {
    case INT:
        std::cout << *static_cast<int *>(entry->value);
        break;
    case DOUBLE:
        std::cout << *static_cast<double *>(entry->value);
        break;
    case STRING:
        std::cout << "\"" << *static_cast<std::string *>(entry->value) << "\"";
        break;
    case ARRAY:
        print_array(static_cast<Array *>(entry->value));
        break;
    }
    std::cout << std::endl;
}
void print_array(Array *array)
{
    std::cout << "[";
    switch (array->type)
    {
    case INT:
        for (int i = 0; i < array->size; i++)
        {
            std::cout << static_cast<int *>(array->items)[i];
            if (i + 1 != array->size)
            {
                std::cout << ", ";
            }
        }
        break;
    case DOUBLE:
        for (int i = 0; i < array->size; i++)
        {
            std::cout << static_cast<double *>(array->items)[i];
            if (i + 1 != array->size)
            {
                std::cout << ", ";
            }
        }
        break;
    case STRING:
        for (int i = 0; i < array->size; i++)
        {
            std::cout << "\"" << static_cast<std::string *>(array->items)[i] << "\"";
            if (i + 1 != array->size)
            {
                std::cout << ", ";
            }
        }
        break;
    case ARRAY:
        for (int i = 0; i < array->size; i++)
        {
            print_array(&static_cast<Array *>(array->items)[i]);
            if (i + 1 != array->size)
            {
                std::cout << ", ";
            }
        }
        break;
    }
    std::cout << "]";
}
Array *add_array(void)
{
    std::cout << "type: (int, double, string, array): ";
    std::string type;
    std::cin >> type;

    std::cout << "size: ";
    int size;
    std::cin >> size;

    Array *array = new Array;
    array->size = size;
    if (type == "array")
    {
        array->type = ARRAY;
        array->items = new Array[size];
        for (int i = 0; i < array->size; i++)
        {
            std::cout << "item[" << i << "]: ";
            static_cast<Array *>(array->items)[i] = *static_cast<Array*>(add_array());
        }
    }
    else if (type == "int")
    {
        array->type = INT;
        array->items = new int[size];
        for (int i = 0; i < array->size; i++)
        {
            std::cout << "item[" << i << "]: ";
            std::cin >> static_cast<int *>(array->items)[i];
        }
    }
    else if (type == "double")
    {
        array->type = DOUBLE;
        array->items = new double[size];
        for (int i = 0; i < array->size; i++)
        {
            std::cout << "item[" << i << "]: ";
            std::cin >> static_cast<double *>(array->items)[i];
        }
    }
    else if (type == "string")
    {
        std::cin.ignore();
        array->type = STRING;
        array->items = new std::string[size];
        for (int i = 0; i < size; i++)
        {
            std::cout << "item[" << i << "]: ";
            std::getline(std::cin, static_cast<std::string *>(array->items)[i]);
        }
    }
    else
    {
        delete_array(array);
        std::cout << "invalid type" << std::endl;
        return nullptr;
    }
    return array;
}
