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

	std::string password;
	std::string username;
	std::string port;
	std::string ip;
public:
	Connection(const char* ip, const char* port);
	Connection(const char* ip);
	~Connection();
	int Insert(const char* collection);
};

//Needs to include credentials later..
Connection::Connection(const char* ip, const char* port){

	try{
		dbc.connect(ip);
		printf("Connection ok to, %s\n", (this->ip + ":" + this->port).c_str());
	}catch(const mongo::DBException &e){
		printf("Caught Exception: %s\n", e.toString().c_str());
	}
}
//Needs to include credentials later..
Connection::Connection(const char* ip){

	try{
		dbc.connect(ip);
		printf("Connection ok to, %s\n", (this->ip).c_str());
	}catch(const mongo::DBException &e){
		printf("Caught Exception: %s\n", e.toString().c_str());
	}
}

Connection::~Connection(){
	
}

int Connection::Insert(const char* collection){ //Needs to also take the table.

	BSONObjBuilder b;

	b.genOID();

	b.append("key", "value");

	BSONObj p = b.obj();

	printf("Insert call: %s\n", collection);

	this->dbc.insert(collection, p);

	return 1;
} 
// int Get(std::string collection, std::string key);
// void SetWorkingCollection(std::string collection);