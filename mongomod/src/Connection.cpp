#include "Connection.h"

bool Connection::Connect(std::string ip, std::string database){
	this->ip = ip;
    databaseName = database;

    mongocxx::instance instance{};
    try{
        mongocxx::uri uri(ip);
        mongocxx::client client(uri);

        this->database = client[database];
    }catch(mongocxx::exception &e){
        std::cout << e.what() << std::endl;
    }

	return true;
}

/* void Connection::Insert(string collection, BSONObj b){ */

/* 	try{ */
/* 		dbc.insert(collection, b); */
/* 	}catch(DBException &e){ */
/* 		printf("\n[MongoMod] ERROR: Insert Failed:  %s\n", e.toString().c_str()); */
/* 	} */
/* } */

//Need to handle exception for this...
mongocxx::cursor Connection::Query(std::string collection, builder::basic::document d){
    mongocxx::collection col = database[collection];
    try{
        return col.find(d.view());
    }catch(mongocxx::exception &e){
        std::cout << "\n" << e.what() << std::endl;
    }
}

/* void Connection::Update(string collection, BSONObj q, BSONObj update){ */
/*     try{ */
/*         dbc.update(collection, q, update, false, true); */
/*     }catch(DBException &e){ */
/*         printf("\n[MongoMod] ERROR: Update Failed: %s\n", e.toString().c_str()); */
/*     } */
/* } */

/* void Connection::Remove(string collection, BSONObj q, bool justOne){ */
/*    try{ */
/*        dbc.remove(collection, q, justOne); */
/*    }catch(DBException &e){ */
/*        printf("\n[MongoMod] ERROR: Remove Failed: %s\n", e.toString().c_str()); */
/*    } */
/* } */
std::string Connection::GetActiveDatabase(){
    return databaseName;
}
