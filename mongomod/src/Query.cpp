
//How do we represent this data so that we don't have to keep calling it?
//Function needs to return two things, the table holding all found elements and the number of total tables returned.
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

    char* final = new char[strlen(c->GetActiveDatabase()) + strlen(collection) + 2];
    strcpy(final, c->GetActiveDatabase());
    strcat(final, ".");
    strcat(final, collection);

    LUA->CheckType(3, Type::TABLE);

    std::vector<mongo::BSONObj> elements;

    BSONObjBuilder qBuilder;

    for(LUA->PushNil(); LUA->Next(3) != 0; LUA->Pop()){
        if(LUA->IsType(-2, Type::STRING)){
            if(LUA->IsType(-1, Type::BOOL)){
                bool lbool= LUA->GetBool(-1);
                qBuilder.append(LUA->GetString(-2), lbool);
                continue;
            }else if(LUA->IsType(-1, Type::NUMBER)){
                double lnum = LUA->GetNumber(-1);
                qBuilder.append(LUA->GetString(-2), lnum);
                continue;
            }else{
                qBuilder.append(LUA->GetString(-2), LUA->GetString(-1));
            }
        }
    }

    LUA->Pop();

    std::auto_ptr<mongo::DBClientCursor> cursor = c->Query(final, qBuilder.obj());

    while(cursor->more()){
        elements.push_back(cursor->next());
    }

    int j = 0;

    //-1
    LUA->CreateTable();
    {
        //Pull all the BSONObjs
        for(std::vector<mongo::BSONObj>::iterator it = elements.begin(); it != elements.end(); it++){

            std::vector<const char*> fieldNames;
            //Pull fields out of the objects and build a feild vector.
            BSONObj::iterator i = it->begin();

            //-2
            LUA->CreateTable();
            {
                while(i.more()){
                    mongo::BSONElement e = i.next();
                    fieldNames.push_back(e.fieldName());

                    if(e.type() == mongo::BSONType::Bool){
                        LUA->PushBool(e.boolean());
                        LUA->SetField(-2, e.fieldName());
                    }else if(e.type() == mongo::BSONType::NumberDouble){
                        LUA->PushNumber(e._numberDouble());
                        LUA->SetField(-2, e.fieldName());
                    }else if(e.type() == mongo::BSONType::String){
                        LUA->PushString(e.valuestr());
                        LUA->SetField(-2, e.fieldName());
                    }
                }
            }
            LUA->SetField(-2, std::to_string(j + 1).c_str());
            j++;
        }
    }

    LUA->PushNumber(j);

    return 2;
}


