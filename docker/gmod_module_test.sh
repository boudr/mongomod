#!/bin/bash

docker run -it \
	-p 27016:27016/udp \
	-e "port=27016" \
	-v ${PWD}/module:/opt/garrysmod/garrysmod/lua/bin:rw \
	hackebein/garrysmod /bin/bash #LD_LIBRARY_PATH=garrysmod/bin:bin:. LD_DEBUG=libs ./srcds_run -game garrysmod -port 27016 +gamemode sandbox +map gm_flatgrass

