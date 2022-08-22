cmake_minimum_required(VERSION 3.2)
	 
project(server_1)			
set(SOURCE_EXE server.cpp)
set(SOURCE_LIB ServerSide.cpp
		ServerSide.hpp)	
add_library(lib STATIC ${SOURCE_LIB})
add_executable(main ${SOURCE_EXE})
target_link_libraries(main lib)
