#ifndef MONGO_MOD_CONNECTION
#define MONGO_MOD_CONNECTION
#include <iostream>

#include<mongocxx/uri.hpp>
#include<mongocxx/instance.hpp>
#include<mongocxx/client.hpp>
#include "GarrysMod/Lua/Interface.h"
#include <mongocxx/exception/exception.hpp>
#include <bsoncxx/builder/basic/document.hpp>

using namespace GarrysMod::Lua;
using namespace bsoncxx;

class Connection {
private:
    mongocxx::database database;
    std::string databaseName;
    std::string ip;
public:
    std::string GetActiveDatabase();
	/* void Insert(string collection, BSONObj b); */
	bool Connect(std::string ip, std::string database);
    /* void Update(string collection, BSONObj q, BSONObj update); */
    /* void Remove(string collection, BSONObj q, bool justOne); */
    mongocxx::cursor Query(std::string collection, builder::basic::document d);
};
#endif