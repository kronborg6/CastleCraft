cmake_minimum_required(VERSION 3.24)

project(
    castleProtocol
    VERSION 0.1.0
    DESCRIPTION "Minecraft protocol"
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library
add_library(castleProtocol STATIC
    src/protocol/varint.cpp
    src/uuid.cpp
    src/package.cpp
)

target_include_directories(castleProtocol
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(castleProtocol PUBLIC cxx_std_23)

# if(PROJECT_IS_TOP_LEVEL)
#     add_executable(castleNBTTool
#         tools/main.cpp
#     )
#
#     target_link_libraries(castleNBTTool
#         PRIVATE castleNBT
#     )
#
#     include(CTest)
#
#     if(BUILD_TESTING)
#         add_subdirectory(tests)
#     endif()
# endif()
