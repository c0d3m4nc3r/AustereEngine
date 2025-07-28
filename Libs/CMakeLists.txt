
# GLAD

set(GLAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glad")
add_library(glad STATIC "${GLAD_DIR}/src/gl.c")
target_include_directories(glad PUBLIC "${GLAD_DIR}/include")

set(STB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/stb")
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE "${STB_DIR}")
