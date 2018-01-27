# MongoMod

Garry's Mod Lua implementation on MongoDB (https://www.mongodb.com)

This binary module will enable a Garry's Mod NoSQL option.

## Getting Started

Please see the release section for the redistribution packages for your respective operating system.

Releases: https://github.com/ceeit0/mongomod/releases

If you choose to build this yourself; please see the installation section below.


### Installation
Warning: I have not built this for windows and do not intend to. If you are running a windows server, please contact me and I will attempt to work it out.

Note: I am currently building and testing this against the Ubuntu environment. MMV..

Steps:
* Step 1: Download the libmongoclient.so and gmsv_mongomod_linux.dll
* Step 2: sudo apt-get install libboost-all-dev OR sudo apt-get install libboost-all-dev:i386 (Boost 1.58.0 is required)
* Step 3: Place the libmongoclient.so is the root of the steamapps folder. (Same place as srcds_run)
* Step 4: Place the gmsv_mongomod_linux.dll in the garrysmod/lua/bin folder.
* Step 5: Profit

## Available Functions

Connect:
```lua
local database = mongo.connect("localhost:27017")
```
Insert a new collection:
```lua
database:insert("collectionName", {this = "Is", a = "Table", yes = true, wow = 1})
```

