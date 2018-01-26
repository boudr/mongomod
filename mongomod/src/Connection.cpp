#include "mongo/client/dbclient.h"
#include <stdio.h>
#include <cstdlib>

#include "mongo/bson/bson.h"

using mongo::BSONObjBuilder;
using mongo::BSONObj;

class Connection {
private:
	mongo::DBClientConnection dbc;
	std::string workingCollection;

	const char* ip;
public:
	Connection(const char* ip);
	~Connection();
	int Insert(const char* collection);
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

int Connection::Insert(const char* collection){ //Needs to also take the table.

	BSONObjBuilder b;

	b.genOID();

	b.append("key", "value");

	BSONObj p = b.obj();

	this->dbc.insert(collection, p);

	return 1;
} 
// int Get(std::string collection, std::string key);
// void SetWorkingCollection(std::string collection);