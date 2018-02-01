#include "Connection.h"

bool Connection::Connect(const char* ip, const char* database){
	this->ip = ip;
	this->database = database;

	try{
		dbc.connect(this->ip);
	}catch(mongo::DBException &e){
		printf("\n[MongoMod] ERROR: Connection failed .. %s\n", e.toString().c_str());
		return false;
	}

	return true;
}

void Connection::Insert(char* collection, BSONObj b){

	try{
		this->dbc.insert(collection, b);
	}catch(mongo::DBException &e){
		printf("\n[MongoMod] ERROR: Unable to insert new data .. %s\n", e.toString().c_str());
	}
}

//Need to handle exception for this...
std::auto_ptr<mongo::DBClientCursor> Connection::Query(const char* collection, mongo::BSONObj q){
	return this->dbc.query(collection, q);
}

void Connection::Update(const char* collection, BSONObj q, BSONObj update){
    try{
        this->dbc.update(collection, q, update, false, true);
    }catch(mongo::DBException &e){
        printf("\n[MongoMod] ERROR: %s\n", e.toString().c_str());
    }
}

const char* Connection::GetActiveDatabase(){
	return this->database;
}
// int Get(std::string collection, std::string key);
// void SetWorkingCollection(std::string collection);
// int SetWorkingCollection(std::string collection);