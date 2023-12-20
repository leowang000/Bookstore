cmake_minimum_required(VERSION 3.22)
project(Bookstore_2023)

set(CMAKE_CXX_STANDARD 17)

add_executable(code
        src/main.cpp
        src/include/BookStore.cpp
        src/include/Books.cpp
        src/include/Accounts.cpp
        src/include/Log.cpp
        src/include/Instruction.cpp
        src/include/Tools/error.cpp
        src/include/Tools/read_string.cpp
        src/include/Tools/File.cpp)