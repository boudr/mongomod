#!/bin/bash

docker run -it \
	-p 27016:27016/udp \
	-e "port=27016" \
	-v ${PWD}/module:/opt/garrysmod/garrysmod/lua/bin:rw \
	kalka/gmod-mongo-dev