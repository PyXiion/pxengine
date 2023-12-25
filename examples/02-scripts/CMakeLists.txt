add_executable(02_scripts
    main.cpp
    some_manager.cpp
    some_manager.hpp
    entity.cpp
    entity.hpp
    player.cpp
    player.hpp
)
target_link_libraries(02_scripts px_engine px easyloggingpp)