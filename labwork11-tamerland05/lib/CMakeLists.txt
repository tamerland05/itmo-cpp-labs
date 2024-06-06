add_subdirectory(argparser)

add_library(search_engine search_engine.h
        search_engine.cpp
)

target_link_libraries(search_engine PUBLIC argparser)
