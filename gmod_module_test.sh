#!/bin/bash

docker run -it \
    -p 27016:27016/udp \
    -e "GAMEMODE=sandbox" \
    -e "PORT=27016" \
    -e "MAP=gm_flatgrass" \
    -v ${PWD}/module:/opt/garrysmod/garrysmod/lua/bin:rw \
    hackebein/garrysmod