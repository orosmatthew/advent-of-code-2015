add_library(hash-library STATIC)
target_sources(hash-library PRIVATE source/md5.cpp)
target_include_directories(hash-library PUBLIC include)