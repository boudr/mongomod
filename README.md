# MongoMod

Garry's Mod Lua implementation on MongoDB (https://www.mongodb.com)

This binary module will enable a Garry's Mod NoSQL option.

## Getting Started

Please see the release section for the redistribution packages for your respective operating system.

Releases: https://git.globius.org/cee/mongomod-build/tags

If you choose to build this yourself; please see the installation section below.


### Installation
Warning: I have not built this for windows and do not intend to. If you are running a windows server, please contact me and I will attempt to work it out.

Note: I am currently building and testing this against the Ubuntu environment. MMV..

DO NOT run this module on a 64 bit server UNLESS you know what you are doing with packages and have the correct libboost-all-dev packages installed.

Steps:
* Step 1: Download the libmongoclient.so and gmsv_mongomod_linux.dll
* Step 2: sudo apt-get install libboost-all-dev OR sudo apt-get install libboost-all-dev:i386 (Boost 1.58.0 is required)
* Step 3: Place the libmongoclient.so is the root of the steamapps folder. (Same place as srcds_run)
* Step 4: Place the gmsv_mongomod_linux.dll in the garrysmod/lua/bin folder.
* Step 5: Profit

## Available Functions
Coming soon to a town near you!
