add_library(console_worker console_worker.h console_worker.cpp)
add_library(api_getter api_getter.h api_getter.cpp)
add_library(jsons_parse jsons_parse.h jsons_parse.cpp)

target_link_libraries(console_worker PRIVATE api_getter)
target_link_libraries(console_worker PRIVATE jsons_parse)
target_link_libraries(console_worker PRIVATE cpr::cpr)
target_link_libraries(console_worker PRIVATE nlohmann_json::nlohmann_json)

target_link_libraries(api_getter PRIVATE jsons_parse)
target_link_libraries(api_getter PRIVATE cpr::cpr)
target_link_libraries(api_getter PRIVATE nlohmann_json::nlohmann_json)

target_link_libraries(jsons_parse PRIVATE nlohmann_json::nlohmann_json)
