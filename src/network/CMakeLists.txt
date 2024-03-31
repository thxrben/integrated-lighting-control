set(network_src NetworkManager.cpp)

message("-- Adding target \"network\"")

add_library(network STATIC ${network_src})