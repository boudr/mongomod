#include "MongoMod.h"

using namespace GarrysMod::Lua;

using mongo::BSONObjBuilder;
using mongo::BSONObj;

#define CONNECTION "Connection"

//Connection iType ID. Returned from created the new meta table of type CONNECTION
int ConTypeID;

int TableScanRef;

//Used to concat the database and collection into one c string
char* concatDB(const char* a, const char* b){
    char* final = new char[strlen(a) + strlen(b) + 2];
    strcpy(final, a);
    strcat(final, ".");
    strcat(final, b);

    return final;
}

//Pass a stack position for a table and this function will build a BSONObj that is query ready.
BSONObj query_builder(ILuaBase* LUA, int iStackPos, bool update=false){
    int i = 1;

    BSONObjBuilder qBuilder;
    for(LUA->PushNil(); LUA->Next(iStackPos) != 0; LUA->Pop()){
        if(LUA->IsType(-2, Type::STRING)){
            if(LUA->IsType(-1, Type::BOOL)){
                bool lbool= LUA->GetBool(-1);
                qBuilder.append(LUA->GetString(-2), lbool);
                i++;
                continue;
            }else if(LUA->IsType(-1, Type::NUMBER)){
                double lnum = LUA->GetNumber(-1);
                qBuilder.append(LUA->GetString(-2), lnum);
                i++;
                continue;
            }else if(LUA->IsType(-1, Type::STRING)){
                qBuilder.append(LUA->GetString(-2), LUA->GetString(-1));
                i++;
                continue;
            }else if(LUA->IsType(-1, Type::TABLE)){
                printf("\n%s\n", LUA->GetString(-2));
                if(update){
                    qBuilder.append(LUA->GetString(-2),query_builder(LUA, -2));
                }else{
                    qBuilder.appendArray(LUA->GetString(-2), query_builder(LUA, -2));
                }
                i++;
                continue;
            }else{
                printf("\n[MongoMod] ERROR: Datatype provided as value is not supported: %s\n", LUA->GetTypeName(-1));
            }
        }else if(LUA->IsType(-2, Type::NUMBER)){
            if(LUA->IsType(-1, Type::BOOL)){
                bool lbool= LUA->GetBool(-1);
                qBuilder.append(std::to_string(i).c_str(), lbool);
                i++;
                continue;
            }else if(LUA->IsType(-1, Type::NUMBER)){
                double lnum = LUA->GetNumber(-1);
                qBuilder.append(std::to_string(i).c_str(), lnum);
                i++;
                continue;
            }else if(LUA->IsType(-1, Type::STRING)){
                qBuilder.append(std::to_string(i).c_str(), LUA->GetString(-1));
                i++;
                continue;
            }else if(LUA->IsType(-1, Type::TABLE)){
                if(update){
                    printf("\n[MongoMod] ERROR: Key to second table MUST be an Update Operator!\n");
                }else{
                    qBuilder.appendArray(LUA->GetString(-2), query_builder(LUA, -2));
                }
                i++;
                continue;
            }else{
                printf("\n[MongoMod] ERROR: Datatype provided as value is not supported: %s\n", LUA->GetTypeName(-1));
            }
        }else{
            printf("\n[MongoMod] ERROR: Mongomod only supports strings as key values or no key specified.\n");
        }
    }
    return qBuilder.obj();
}

//Hacky workaround.
//This is supposed to build sub tables out of a bson obj since we can't call lua_table_builder without building more vectors..
void lua_sub_table_builder(ILuaBase* LUA, BSONObj b, const char* fieldName){
    //Pull fields out of the objects and build a feild vector.
    BSONObj::iterator i = b.begin();

    //-2
    LUA->CreateTable();
    {
        while(i.more()){
            mongo::BSONElement e = i.next();
            if(e.type() == mongo::BSONType::Bool){
                LUA->PushBool(e.boolean());
                LUA->SetField(-2, e.fieldName());
            }else if(e.type() == mongo::BSONType::NumberDouble){
                LUA->PushNumber(e._numberDouble());
                LUA->SetField(-2, e.fieldName());
            }else if(e.type() == mongo::BSONType::String){
                LUA->PushString(e.valuestr());
                LUA->SetField(-2, e.fieldName());
            }else if(e.type() == mongo::BSONType::Array){

                BSONObj newObject = (BSONObj)e.embeddedObject();

                lua_sub_table_builder(LUA, newObject, e.fieldName());
            }
        }
    }
    LUA->SetField(-2, fieldName);
}

int lua_table_builder(ILuaBase* LUA, std::vector<BSONObj>* elements){

    printf("\nValues on stack: %s\n", std::to_string(LUA->Top()).c_str());
    int j = 0;
    LUA->CreateTable();
    {
        //Pull all the BSONObjs
        for(std::vector<mongo::BSONObj>::iterator it = elements->begin(); it != elements->end(); it++){

            //Pull fields out of the objects and build a feild vector.
            BSONObj::iterator i = it->begin();

            //-2
            LUA->CreateTable();
            {
                while(i.more()){
                    mongo::BSONElement e = i.next();
                    if(e.type() == mongo::BSONType::Bool){
                        LUA->PushBool(e.boolean());
                        LUA->SetField(-2, e.fieldName());
                    }else if(e.type() == mongo::BSONType::NumberDouble){
                        LUA->PushNumber(e._numberDouble());
                        LUA->SetField(-2, e.fieldName());
                    }else if(e.type() == mongo::BSONType::String){
                        LUA->PushString(e.valuestr());
                        LUA->SetField(-2, e.fieldName());
                    }else if(e.type() == mongo::BSONType::Array){

                        BSONObj newObject = (BSONObj)e.embeddedObject();

                        lua_sub_table_builder(LUA, newObject, e.fieldName());

                    }
                }
            }
            LUA->SetField(-2, std::to_string(j + 1).c_str());
            j++;
        }
    }

    return j;
}


LUA_FUNCTION(New){
    if(!ConTypeID){
        printf("\n[MongoMod] ERROR: Connection Type not initialized!\n");
        return 0;
    }

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

LUA_FUNCTION(Connect){

    if(!ConTypeID){
        printf("\n[MongoMod] ERROR: Connection Type not initialized!\n");
        return 0;
    }

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
LUA_FUNCTION(Disconnect){

    if(!ConTypeID){
        printf("\n[MongoMod] ERROR: Connection Type not initialized!\n");
        return 0;
    }

    LUA->CheckType(1, ConTypeID);

    ILuaBase::UserData* userdata = (ILuaBase::UserData*) LUA->GetUserdata(1);
    Connection* c = (Connection*)userdata->data;

    delete c;

    return 0;
}

LUA_FUNCTION(Insert){

    if(!ConTypeID){
        printf("\n[MongoMod] ERROR: Connection Type not initialized!\n");
        return 0;
    }

    LUA->CheckType(1, ConTypeID);

    ILuaBase::UserData* userdata = (ILuaBase::UserData*) LUA->GetUserdata(1);
    Connection* c = (Connection*)userdata->data;

    if(!c){
        printf("\n[MongoMod] ERROR: Connection is invalid! Unable to insert requested data.\n");
        return 0;
    }

    LUA->CheckString(2);

    const char* collection = LUA->GetString(2);

    LUA->CheckType(3, Type::TABLE);

    BSONObj b = query_builder(LUA, 3);

    LUA->Pop();

    char* final = concatDB(c->GetActiveDatabase(), collection);

    printf("\n%s\n", final);

    c->Insert(final, b);

    return 0;
}

LUA_FUNCTION( Query ){

    if(!ConTypeID){
        printf("\n[MongoMod] ERROR: Connection Type not initialized!\n");
        return 0;
    }

    LUA->CheckType(1, ConTypeID);

    ILuaBase::UserData* userdata = (ILuaBase::UserData*) LUA->GetUserdata(1);
    Connection* c = (Connection*)userdata->data;

    if(!c){
        printf("\n[MongoMod] ERROR: Connection is invalid! Unable to query on requested database.\n");
        return 0;
    }

    LUA->CheckString(2);

    const char* collection = LUA->GetString(2);

    char* final = concatDB(c->GetActiveDatabase(), collection);

    printf("\n%s\n", final);

    LUA->CheckType(3, Type::TABLE);

    std::vector<mongo::BSONObj> elements;

    BSONObj q = query_builder(LUA, 3);

    LUA->Pop();

    std::auto_ptr<mongo::DBClientCursor> cursor = c->Query(final, q);

    while(cursor->more()){
        elements.push_back(cursor->next());
    }

    LUA->PushNumber(lua_table_builder(LUA, &elements));

    return 2;
}

LUA_FUNCTION(Update){
    if(!ConTypeID){
        printf("\n[MongoMod] ERROR: Connection Type not initialized!\n");
        return 0;
    }

    LUA->CheckType(1, ConTypeID);

    ILuaBase::UserData* userdata = (ILuaBase::UserData*) LUA->GetUserdata(1);
    Connection* c = (Connection*)userdata->data;

    if(!c){
        printf("\n[MongoMod] ERROR: Connection is invalid! Unable to query on requested database.\n");
        return 0;
    }

    LUA->CheckString(2);

    const char* collection = LUA->GetString(2);

    char* final = concatDB(c->GetActiveDatabase(), collection);

    printf("\n%s\n", final);

    LUA->CheckType(3, Type::TABLE);

    std::vector<mongo::BSONObj> elements;

    BSONObj q = query_builder(LUA, 3);

    LUA->CheckType(4, Type::TABLE);

    BSONObj update = query_builder(LUA, 4, true);

    c->Update(final, q, update);

    return 0;
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

        LUA->PushCFunction(Update);
        LUA->SetField(-2, "update");
    }

    LUA->Pop();

    return 0;
}

GMOD_MODULE_CLOSE(){

    ConTypeID = -1;

    return 0;
}