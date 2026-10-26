cmake_minimum_required(VERSION 4.2)

project(
    castleCraft
    VERSION 0.1.0
    DESCRIPTION "C++ minecraft server"
    LANGUAGES CXX
)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(
        CMAKE_BUILD_TYPE Debug
        CACHE STRING "Choose the build type"
        FORCE
    )

    set_property(
        CACHE CMAKE_BUILD_TYPE
        PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel
    )
endif()

option(DEV_MODE "Enable debug printing" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

####################
# Compiler warnings
####################

add_library(project_warnings INTERFACE)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(project_warnings INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2
        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Debug>:-g3>
    )
endif()

####################
# Application logic
####################

add_subdirectory(lib/castleNBT)


add_library(minecraft_server_lib STATIC
    src/protocol/protocol.cpp
    src/handlers/handshake.cpp
    src/handlers/status.cpp
    src/handlers/login.cpp
    src/handlers/configuration.cpp
    src/generators/registries/dimension.cpp
    src/generators/registries/timeline.cpp
    src/generators/registries/worldClock.cpp
    src/generators/biome.cpp
    src/tcp.cpp
)

target_compile_features(minecraft_server_lib
    PUBLIC
        cxx_std_23
)

target_include_directories(minecraft_server_lib
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
        # "${PROJECT_SOURCE_DIR}/include/"
)

target_compile_definitions(minecraft_server_lib
    PRIVATE
        $<$<BOOL:${DEV_MODE}>:DEV_MODE>
)

target_link_libraries(minecraft_server_lib
    PUBLIC
    castleNBT
    PRIVATE
        project_warnings
)

############
# Executable
############

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_compile_features(${PROJECT_NAME}
    PRIVATE
        cxx_std_23
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        $<$<BOOL:${DEV_MODE}>:DEV_MODE>
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        minecraft_server_lib
       castleNBT 
        project_warnings
)

########
# Tests
########

include(CTest)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

##############
# Installation
##############

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

###########
# Packaging
###########

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR "Kronborg")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")

set(
    CPACK_PACKAGE_FILE_NAME
    "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}"
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CPACK_GENERATOR "TGZ;ZIP;DEB")
else()
    set(CPACK_GENERATOR "TGZ;ZIP")
endif()

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Kronborg")
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)
