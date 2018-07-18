# MongoMod

Garry's Mod Lua implementation on MongoDB (https://www.mongodb.com)

## Important Note
I haven't had much time to do a re-write of this project. This master branch uses the legacy driver for MongoDB. While this project does work, it requires a very specific environment and setup. Needless to say, don't use this module. I will eventually do a re-write or just wait for Sandbox to be released and do something similar for that. This was a fun learning experience for stack programming and some deeper C concepts. If you have questions, make an issue and I will try my best to answer. :)

## Getting Started

Please see the release section for the redistribution packages for your respective operating system.

Releases: https://github.com/boudr/mongomod/releases

If you choose to build this yourself; please see the installation section below.

### Installation
Warning: I have not built this for windows and do not intend to. If you are running a windows server, please contact me and I will attempt to work it out.

Note: I am currently building and testing this against the Ubuntu environment. MMV..

Steps:
* Step 1: Download the libmongoclient.so and gmsv_mongomod_linux.dll
* Step 2: sudo apt-get install 
- libboost-system-dev
- libboost-thread-dev
- libboost-regex-dev
* Step 3: Place the libmongoclient.so is the root of the steamapps folder. (Same place as srcds_run)
* Step 4: Place the gmsv_mongomod_linux.dll in the garrysmod/lua/bin folder.
* Step 5: Profit

## Available Functions

### MongoDB Resources:
Update Conditions(https://docs.mongodb.com/manual/reference/operator/update/)

### New Database Object:
```lua
local database = mongo.new()
```
### Connect:
```lua
--Takes IP and port combo as well as the name of the database
db:connect("localhost:27017", "garrysmod")
```
### Insert a new collection:
```lua
--ONLY use insert for new items. See the 'update' function to modify or add to existing data.
database:insert("collectionName", {this = "Is", a = "Table", yes = true, wow = 1})
```
### Query:
```lua
--Returns the table of elements found and the number of elements.
--To query on multiple cases, just keep adding to the table.
--[[
	Example:
	{ banned = true, name="dril" }
]]--

--For conditional queries: See the query_conditional function.

local tab, elementCount = database:query("players", {banned = true})

print(elementCount)

for _, v in pairs(tab) do
	print(v.name .. " is banned")
end

--OR--

PrintTable(tab)
```
### Update:
```lua
--To update, the first table will be the same as query.
--[[
	Example:
	{ banned = true, name="dril" }
]]--
--The second table will use MongoDB's Update conditions as the key.
--[[
    Example:
    { ["$set"] = {
        name = "crown"
    }
]]--

database:update("players", {banned = true}, {["$set"] = {banned=false}})
```
