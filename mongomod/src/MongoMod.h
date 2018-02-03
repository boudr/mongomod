#ifndef MONGO_MOD
#define MONGO_MOD
#include "GarrysMod/Lua/Interface.h"
#include <stdio.h>

#include <cstdlib>
#include <iostream>
#include <vector>
#include "mongo/client/dbclient.h" // for the driver
#include "mongo/bson/bson.h"

#include "Connection.h"

using mongo::BSONObj;
using mongo::BSONObjBuilder;
using mongo::DBClientCursor;
using namespace std;
using namespace GarrysMod::Lua;
#endif