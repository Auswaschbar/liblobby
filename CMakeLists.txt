project(LibLobby)
cmake_minimum_required(VERSION 2.6)

FIND_PACKAGE(Boost 1.35.0 COMPONENTS thread system REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXXFLAGS_DEBUG} -ggdb -DDEBUG")
set(EXECUTABLE_OUTPUT_PATH bin)

aux_source_directory(src libfiles)
add_library(lobby SHARED ${libfiles})
target_link_libraries(lobby ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})

include_directories(src)
aux_source_directory(Examples/Clanbot cbfiles)
add_executable(clanbot ${cbfiles})
target_link_libraries(clanbot lobby)

add_executable(fortunebot Examples/FortuneBot.cpp)
target_link_libraries(fortunebot lobby)