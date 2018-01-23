#include "GarrysMod/Lua/Interface.h"
#include <stdio.h>

#include <cstdlib>
#include <iostream>
#include "mongo/client/dbclient.h" // for the driver

using namespace GarrysMod::Lua;

void run( const char* url ) {

    mongo::DBClientConnection c;
    c.connect(url);
}

int init( lua_State* state ) {

    GarrysMod::Lua::ILuaBase* LUA = state->luabase;

    mongo::client::initialize();

    char strOut[512];
    try {
        run(LUA->CheckString(1));
        sprintf(strOut, "connected ok");
        LUA->PushString(strOut);
    } catch( const mongo::DBException &e ) {
        sprintf(strOut, "caught exception");
        LUA->PushString(strOut);
    }
    return 1;
}

int MyExampleFunction( lua_State* state )
{
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


//
// Called when you module is opened
//
GMOD_MODULE_OPEN()
{
    //
    // Set Global[ "TextFunction" ] = MyExampleFunction
    //
    LUA->PushSpecial( GarrysMod::Lua::SPECIAL_GLOB );   // Push global table
    LUA->PushString( "TestFunction" );                  // Push Name
    LUA->PushCFunction( MyExampleFunction );            // Push function
    LUA->SetTable(-3);

    LUA->PushSpecial( GarrysMod::Lua::SPECIAL_GLOB );
    LUA->PushString( "MongoInit" );
    LUA->PushCFunction( init );
    LUA->SetTable( -3 );                                // Set the table 

    return 0;
}

//
// Called when your module is closed
//
GMOD_MODULE_CLOSE()
{
    return 0;
}