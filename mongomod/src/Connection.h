#include "mongo/client/dbclient.h"
#include <stdio.h>
#include <cstdlib>

#include "mongo/bson/bson.h"
#include "GarrysMod/Lua/Interface.h"

using mongo::BSONObjBuilder;
using mongo::BSONObj;
using namespace GarrysMod::Lua;

class Connection {
private:
	mongo::DBClientConnection dbc;
	std::string workingCollection;

	const char* ip;
	const char* database;
public:
	const char* GetActiveDatabase();
	void Insert(char* collection, BSONObj b);
	bool Connect(const char* ip, const char* database);
	std::auto_ptr<mongo::DBClientCursor> Query(const char* collection, mongo::BSONObj q);
};