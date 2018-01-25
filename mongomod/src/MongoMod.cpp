#include "MongoMod.h"

using namespace GarrysMod::Lua;

using mongo::BSONObjBuilder;
using mongo::BSONObj;

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
function insert(collection, table)
--Collection: The name of the collection to store into ("gmod.players")
--Table: Table to store (local tab = {})

Example:
    Mongo.insert("gmod", "random", {[1]="Hello, ", [2]="World!"})
*/
int Connect( lua_State* state ){

    GarrysMod::Lua::ILuaBase* LUA = state->luabase;

    LUA->CheckString(1);
    LUA->CheckString(2);
    LUA->CheckString(3);

    printf("Lua Call: %s\n",LUA->GetString(-1) );

    Connection* c = new Connection(LUA->GetString(-3), LUA->GetString(-2));

    c->Insert(LUA->GetString(-1));

    LUA->Pop(3);

    delete c;

    return 0;

    // GarrysMod::Lua::ILuaBase* LUA = state->luabase;

    // mongo::client::initialize();

    // mongo::DBClientConnection dbcon;

    // try {
    //     dbcon.connect("10.0.0.30");
    //     printf("Did this work?\n");
    // } catch( const mongo::DBException &e ) {
    //     printf("Connection could not be made..\n")
    //     return 0;
    // }

    // LUA->CheckType(1, Type::TABLE); // -1

    // BSONObjBuilder b;

    // b.genOID();

    // int j = 1;

    // //Key: -2 ; Value: -1
    // for(LUA->PushNil(); LUA->Next(-2); LUA->Pop()){
    //     if(LUA->IsType(-1, Type::STRING)){

    //         const char* element = LUA->GetString(-1);

    //         const std::string buffer(element);

    //         b.append(buffer, buffer);
    //     }
    // }

    // BSONObj p = b.obj();

    // dbcon.insert("gmod.test", p);

    // return 0;
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
        LUA->SetField(-2, "Mongo");
    }

    LUA->Pop();

    return 0;
}

GMOD_MODULE_CLOSE(){

    return 0;
}