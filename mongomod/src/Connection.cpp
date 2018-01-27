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
	const char* database = "garrysmod";
public:

	Connection(const char* ip);
	~Connection();

	const char* GetActiveDatabase();
	void Insert(char* collection, BSONObj b);
};

//Needs to include credentials later as well as port
Connection::Connection(const char* ip){

	this->ip = ip;

	try{
		dbc.connect(this->ip);
		printf("\nConnection ok to %s\n", this->ip);
	}catch(const mongo::DBException &e){
		printf("\nCaught Exception: %s\n", e.toString().c_str());
	}
}

Connection::~Connection(){
	
}

void Connection::Insert(char* collection, BSONObj b){
	this->dbc.insert(collection, b);
}

const char* Connection::GetActiveDatabase(){
	return this->database;
}
// int Get(std::string collection, std::string key);
// void SetWorkingCollection(std::string collection);
// int SetWorkingCollection(std::string collection);