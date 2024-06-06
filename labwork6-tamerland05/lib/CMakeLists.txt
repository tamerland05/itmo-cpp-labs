#add_library(triple_array triple_array.h triple_array.cpp
#)
add_library(argparser parser/ArgParser.h parser/ArgParser.cpp
                      parser/Arguments.h parser/Arguments.cpp)

add_library(hamming_code hamming_code.h hamming_code.cpp)
add_library(archiver archiver.h archiver.cpp)