#ifndef MONGO_MOD_CONNECTION
#define MONGO_MOD_CONNECTION
#include "mongo/client/dbclient.h"
#include <stdio.h>
#include <cstdlib>

#include "mongo/bson/bson.h"
#include "GarrysMod/Lua/Interface.h"

using namespace mongo;
using namespace GarrysMod::Lua;
using namespace std;

class Connection {
private:
	DBClientConnection dbc;

    string database;
    string ip;
public:
	string GetActiveDatabase();
	void Insert(string collection, BSONObj b);
	bool Connect(string ip, string database);
    void Update(string collection, BSONObj q, BSONObj update);
    void Remove(string collection, BSONObj q, bool justOne);
	auto_ptr<DBClientCursor> Query(string collection, BSONObj q);
};
#endif