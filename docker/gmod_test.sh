#!/bin/bash

# Make the directories, fix permissions

mkdir -p ${PWD}/{garrysmod,css} && chmod 777 ${PWD}/{garrysmod,css}

# Get the requisites

mkdir -p ${PWD}/garrysmod/garrysmod/lua/bin

wget "https://github.com/ceeit0/mongomod/releases/download/v0.0.1/gmsv_mongomod_linux.dll" -O ${PWD}/garrysmod/garrysmod/lua/bin/gmsv_mongomod_linux.dll

wget "https://github.com/ceeit0/mongomod/releases/download/v0.0.1/libmongoclient.so" -O ${PWD}/garrysmod/libmongoclient.so

docker run -it --rm -v ${PWD}/garrysmod:/opt/garrysmod -v ${PWD}/css:/opt/css -p 27015:27015/udp kalka/gmod-mongo -game garrysmod -maxplayers 8 +gamemode sandbox +map gm_flatgrass