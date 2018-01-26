#include "MongoMod.h"

using namespace GarrysMod::Lua;

using mongo::BSONObjBuilder;
using mongo::BSONObj;

#define CONNECTION "Connection"

int ConTypeID;

int MyExampleFunction( lua_State* state ){
    GarrysMod::Lua::ILuaBase* LUA = state->luabase;
    if ( LUA->IsType( 1, Type::NUMBER ) )
    {
        char strOut[512];
        float fNumber = LUA->GetNumber( 1 );
        sprintf( strOut, "Thanks for the number - I love %f!!", fNumber );
        LUA->PushString( strOut );
        return 1;
    }

    LUA->PushString( "This string is returned" );
    return 1;
}

/*
function mongo.connect(ipandport)
Returns: Userdata with type CONNECTION

Example: local db = mongo.connect("10.0.0.30:27017")
*/
int Connect( lua_State* state ){

    GarrysMod::Lua::ILuaBase* LUA = state->luabase;

    LUA->CheckString(1); //-1

    Connection* c = new Connection(LUA->GetString(-1));

    LUA->Pop();

    //Push UserType connection and the connection object to the stack.
    LUA->PushUserType(c, ConTypeID);

    return 1;
}

//Garbage collection deconstructor
int Disconnect( lua_State* state ){

    if(!ConTypeID){
        printf("\nConnection Type Metatable not initialized!\n");
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
        printf("\nConnection Type Metatable not initialized!\n");
        return 0;
    }

    GarrysMod::Lua::ILuaBase* LUA = state->luabase;

    LUA->CheckType(1, ConTypeID);

    ILuaBase::UserData* userdata = (ILuaBase::UserData*) LUA->GetUserdata(1);
    Connection* c = (Connection*)userdata->data;

    if(!c){
        printf("\nThis inserting didn't work.\n");
        return 0;
    }

    c->Insert("this.isworking");

    return 0;
}


GMOD_MODULE_OPEN(){

    mongo::client::initialize();

    //Push Global table onto the stack -1
    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    {
        //Create table -2
        LUA->CreateTable();
        {
            //Push Function onto the table -3 (POP AFTER)
            LUA->PushCFunction(MyExampleFunction);
            LUA->SetField(-2, "example");
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

        LUA->PushCFunction(Insert);
        LUA->SetField(-2, "insert");
    }

    LUA->Pop();

    return 0;
}

GMOD_MODULE_CLOSE(){
    return 0;
}