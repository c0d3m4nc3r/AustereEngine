
set(ENGINE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")
set(ENGINE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Include")
set(ENGINE_PCH "${ENGINE_INCLUDE_DIR}/AE/PCH.hpp")

file(GLOB_RECURSE ENGINE_SOURCES "${ENGINE_SOURCE_DIR}/*.cpp")

add_library(Engine STATIC ${ENGINE_SOURCES})

target_include_directories(Engine
    PUBLIC ${ENGINE_INCLUDE_DIR}
    PUBLIC ${ENGINE_INCLUDE_DIR}/AE
)

target_precompile_headers(Engine PRIVATE ${ENGINE_PCH})

if(USE_SYSTEM_LIBS)
    target_link_libraries(Engine PUBLIC 
        SDL3::SDL3 assimp::assimp glm::glm glad stb
    )
else()
    target_link_libraries(Engine PUBLIC 
        SDL3-static assimp glm glad stb
    )
endif()

