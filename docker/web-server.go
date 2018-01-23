//Web server for serving the local lib modules to the docker container

//Use this to curl from inside the container if for some reason the mongocxx drivers have been recompiled.

package main

import(
	"net/http"
	"fmt"
)

func main() {
	fmt.Println("Starting web server...")
	panic(http.ListenAndServe(":80", http.FileServer(http.Dir("./wsm/"))))
}
