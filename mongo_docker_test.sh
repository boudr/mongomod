#!/bin/bash

#DO NOT MAKE THIS INTERNET FACING OTHERWISE YOU WILL EXPOSE THE DB WITH DEFAILT CREDENTIALS.
#THIS IS FOR TESTING ONLY

#This will not run in detached mode for debugging. Please feel free to remove -it and add -d for detached execution
docker run -it -p 27017:27017/tcp -v ${PWD}/data/:/data/db mongo
