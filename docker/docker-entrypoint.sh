#!/bin/bash

cp /opt/garrysmod/garrysmod/lua/bin/libmongoclient.so /opt/garrysmod/bin/

exec /opt/garrysmod/srcds_run -steamdir /opt/steamcmd -port "$port" "$@"