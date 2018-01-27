#include "MongoMod.h"

using namespace GarrysMod::Lua;

using mongo::BSONObjBuilder;
using mongo::BSONObj;

#define CONNECTION "Connection"

//Connection iType ID. Returned from created the new meta table of type CONNECTION
int ConTypeID;

/*
function mongo.connect(ipandport)
Returns: Userdata with type CONNECTION

Example: local db = mongo.connect("10.0.0.30:27017")
*/
int Connect( lua_State* state ){

    GarrysMod::Lua::ILuaBase* LUA = state->luabase;

    LUA->CheckString(1);

    Connection* c = new Connection(LUA->GetString(1));

    LUA->Pop();

    //Push UserType connection and the connection object to the stack.
    LUA->PushUserType(c, ConTypeID);

    return 1;
}

//Garbage collection deconstructor
int Disconnect( lua_State* state ){

    if(!ConTypeID){
        printf("\n[MongoMod] ERROR: Connection Type not initialized!\n");
        return 0;
    }

    GarrysMod::Lua::ILuaBase* LUA = state->luabase;

    LUA->CheckType(1, ConTypeID);

    ILuaBase::UserData* userdata = (ILuaBase::UserData*) LUA->GetUserdata(1);
    Connection* c = (Connection*)userdata->data;

    delete c;

    return 0;
}

int Insert(lua_State* state){

    if(!ConTypeID){
        printf("\n[MongoMod] ERROR: Connection Type not initialized!\n");
        return 0;
    }

    GarrysMod::Lua::ILuaBase* LUA = state->luabase;

    LUA->CheckType(1, ConTypeID);

    ILuaBase::UserData* userdata = (ILuaBase::UserData*) LUA->GetUserdata(1);
    Connection* c = (Connection*)userdata->data;

    if(!c){
        printf("\n[MongoMod] ERROR: Connection is invalid! Unable to insert requested data.\n");
        return 0;
    }

    LUA->CheckString(2);

    const char* collection = LUA->GetString(2);

    BSONObjBuilder builder;

    builder.genOID();

    LUA->CheckType(3, Type::TABLE);

    for(LUA->PushNil(); LUA->Next(3); LUA->Pop()){
        if(LUA->IsType(-1, Type::BOOL)){
            bool lbool= LUA->GetBool(-1);
            builder.append(LUA->GetString(-2), lbool);
            continue;
        }else if(LUA->IsType(-1, Type::NUMBER)){
            double lnum = LUA->GetNumber(-1);
            builder.append(LUA->GetString(-2), lnum);
            continue;
        }else{
            builder.append(LUA->GetString(-2), LUA->GetString(-1));
        }
    }

    //+2 for the additional '.' character
    char* final = new char[strlen(c->GetActiveDatabase()) + strlen(collection) + 2];
    strcpy(final, c->GetActiveDatabase());
    strcat(final, ".");
    strcat(final, collection);

    printf("\n%s\n", final);

    c->Insert(final, builder.obj());
}


GMOD_MODULE_OPEN(){

    //Initialize the mongo client
    mongo::client::initialize();

    //Push Global table onto the stack -1
    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    {
        //Create table -2
        LUA->CreateTable();
        {
            //Push Function onto the table -3 (POP AFTER);
            LUA->PushCFunction(Connect);
            LUA->SetField(-2, "connect");
        }
        LUA->SetField(-2, "mongo");
    }

    LUA->Pop();

    //Meta table for connection userdata
    ConTypeID = LUA->CreateMetaTable(CONNECTION);
    {
        //Table index
        LUA->Push(-1);
        LUA->SetField(-2, "__index");

        //Attach disconnection function to garbage collection
        LUA->PushCFunction(Disconnect);
        LUA->SetField(-2, "__gc");

        //Insert Function
        LUA->PushCFunction(Insert);
        LUA->SetField(-2, "insert");
    }

    LUA->Pop();

    return 0;
}

GMOD_MODULE_CLOSE(){

    ConTypeID = -1;

    return 0;
}