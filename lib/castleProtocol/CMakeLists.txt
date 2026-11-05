add_library(nbt STATIC
    src/protocol.cpp
)

target_include_directories(protocol
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(protocol PUBLIC cxx_std_23)
