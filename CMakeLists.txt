project(Lobby)

aux_source_directory(src srcfiles)
add_executable(lobby ${srcfiles})
target_link_libraries(lobby boost_system-mt)
