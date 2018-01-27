#include "MongoMod.h"

using namespace GarrysMod::Lua;

using mongo::BSONObjBuilder;
using mongo::BSONObj;

#define CONNECTION "Connection"

//Connection iType ID. Returned from created the new meta table of type CONNECTION
int ConTypeID;

int New( lua_State* state ){
    if(!ConTypeID){
        printf("\n[MongoMod] ERROR: Connection Type not initialized!\n");
        return 0;
    }

    GarrysMod::Lua::ILuaBase* LUA = state->luabase;

    Connection* c = new Connection();

    if(!c){
        printf("\nThis is failing\n");
        return 0;
    }else{
        //Push UserType connection and the connection object to the stack.
        LUA->PushUserType(c, ConTypeID);
    }

    return 1;
}

int Connect( lua_State* state ){

    if(!ConTypeID){
        printf("\n[MongoMod] ERROR: Connection Type not initialized!\n");
        return 0;
    }

    GarrysMod::Lua::ILuaBase* LUA = state->luabase;

    LUA->CheckType(1, ConTypeID);

    ILuaBase::UserData* userdata = (ILuaBase::UserData*) LUA->GetUserdata(1);
    Connection* c = (Connection*)userdata->data;

    LUA->CheckString(2);
    LUA->CheckString(3);

    if(!c){
        printf("\n[MongoMod] ERROR: Connection is invalid! Unable to insert requested data.\n");
        return 0;
    }

    bool success = c->Connect(LUA->GetString(2), LUA->GetString(3));

    if(!success){
        printf("\n[MongoMod] ERROR: Unable to connect to the database!\n");
    }

    return 0;
    
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

    //For element key count
    int i = 1;

    for(LUA->PushNil(); LUA->Next(3) != 0; LUA->Pop()){
        if(LUA->IsType(-2, Type::NUMBER)){
            printf("\nYes, the index is a number.\n");
            printf("\n%s\n", LUA->GetString(-1));
            if(LUA->IsType(-1, Type::BOOL)){
                bool lbool= LUA->GetBool(-1);
                builder.append(std::to_string(i).c_str(), lbool);
                i++;
                continue;
            }else if(LUA->IsType(-1, Type::NUMBER)){
                double lnum = LUA->GetNumber(-1);
                builder.append(std::to_string(i).c_str(), lnum);
                i++;
                continue;
            }else{
                builder.append(std::to_string(i).c_str(), LUA->GetString(-1));
            }
        }else if(LUA->IsType(-2, Type::STRING)){
            if(LUA->IsType(-1, Type::BOOL)){
                bool lbool= LUA->GetBool(-1);
                builder.append(LUA->GetString(-2), lbool);
                i++;
                continue;
            }else if(LUA->IsType(-1, Type::NUMBER)){
                double lnum = LUA->GetNumber(-1);
                builder.append(LUA->GetString(-2), lnum);
                i++;
                continue;
            }else{
                builder.append(LUA->GetString(-2), LUA->GetString(-1));
            }
        }
        i++;
    }

    //+2 for the additional '.' character
    char* final = new char[strlen(c->GetActiveDatabase()) + strlen(collection) + 2];
    strcpy(final, c->GetActiveDatabase());
    strcat(final, ".");
    strcat(final, collection);

    printf("\n%s\n", final);

    c->Insert(final, builder.obj());
}

int Query(lua_State* state){

    if(!ConTypeID){
        printf("\n[MongoMod] ERROR: Connection Type not initialized!\n");
        return 0;
    }

    GarrysMod::Lua::ILuaBase* LUA = state->luabase;

    LUA->CheckType(1, ConTypeID);

    ILuaBase::UserData* userdata = (ILuaBase::UserData*) LUA->GetUserdata(1);
    Connection* c = (Connection*)userdata->data;

    if(!c){
        printf("\n[MongoMod] ERROR: Connection is invalid! Unable to query on requested database.\n");
        return 0;
    }

    LUA->CheckString(2);

    const char* collection = LUA->GetString(2);

    LUA->CheckType(3, Type::TABLE);

    for(LUA->PushNil(); LUA->Next(3) != 0; LUA->Pop()){
        if(LUA->IsType(-2, Type::STRING)){
            // if(LUA->IsType(-1, Type::BOOL)){
            //     bool lbool= LUA->GetBool(-1);


            //     continue;
            // }else if(LUA->IsType(-1, Type::NUMBER)){
            //     double lnum = LUA->GetNumber(-1);


            //     continue;
            // }else{
                
            // }

            //+2 for the additional '.' character
            char* final = new char[strlen(c->GetActiveDatabase()) + strlen(collection) + 2];
            strcpy(final, c->GetActiveDatabase());
            strcat(final, ".");
            strcat(final, collection);

            printf("\n%s\n", LUA->GetString(-2));
            printf("\n%s\n", LUA->GetString(-1));

            std::auto_ptr<mongo::DBClientCursor> cursor = c->Query(final, MONGO_QUERY(LUA->GetString(-2) << LUA->GetNumber(-1)));
            while(cursor->more()){
                std::cout << cursor->next().toString() << std::endl;
            }
        }
    }
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
            LUA->PushCFunction(New);
            LUA->SetField(-2, "new");
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

        LUA->PushCFunction(Connect);
        LUA->SetField(-2, "connect");

        LUA->PushCFunction(Query);
        LUA->SetField(-2, "query");
    }

    LUA->Pop();

    return 0;
}

GMOD_MODULE_CLOSE(){

    ConTypeID = -1;

    return 0;
}