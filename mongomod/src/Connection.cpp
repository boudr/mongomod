#include "Connection.h"

bool Connection::Connect(string ip, string database){
	this->ip = ip;
	this->database = database;

	try{
		dbc.connect(this->ip);
	}catch(DBException &e){
		printf("\n[MongoMod] ERROR: Connection Failed:  %s\n", e.toString().c_str());
		return false;
	}

	return true;
}

void Connection::Insert(string collection, BSONObj b){

	try{
		dbc.insert(collection, b);
	}catch(DBException &e){
		printf("\n[MongoMod] ERROR: Insert Failed:  %s\n", e.toString().c_str());
	}
}

//Need to handle exception for this...
auto_ptr<DBClientCursor> Connection::Query(string collection, BSONObj q){
	return dbc.query(collection, q);
}

void Connection::Update(string collection, BSONObj q, BSONObj update){
    try{
        dbc.update(collection, q, update, false, true);
    }catch(DBException &e){
        printf("\n[MongoMod] ERROR: Update Failed: %s\n", e.toString().c_str());
    }
}

void Connection::Remove(string collection, BSONObj q, bool justOne){
   try{
       dbc.remove(collection, q, justOne);
   }catch(DBException &e){
       printf("\n[MongoMod] ERROR: Remove Failed: %s\n", e.toString().c_str());
   }
}

string Connection::GetActiveDatabase(){
	return database;
}