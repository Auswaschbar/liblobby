project(LibLobby)
cmake_minimum_required(VERSION 2.6)

set(EXECUTABLE_OUTPUT_PATH bin)

aux_source_directory(src srcfiles)
add_executable(lobby ${srcfiles})
target_link_libraries(lobby boost_system-mt boost_thread-mt)
