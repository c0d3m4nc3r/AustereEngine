set(GAME_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")
set(GAME_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Include")
set(GAME_ASSETS_DIR "${CMAKE_SOURCE_DIR}/Assets")

file(GLOB_RECURSE GAME_SOURCES "${GAME_SOURCE_DIR}/*.cpp")

add_executable(Game ${GAME_SOURCES})

target_include_directories(Game PRIVATE ${GAME_INCLUDE_DIR})
target_link_libraries(Game PRIVATE Engine)

add_custom_target(CopyAssets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Assets"
        "$<TARGET_FILE_DIR:Game>/Assets"
    COMMENT "Copying assets folder to build directory..."
    VERBATIM
)

if(TARGET Game)
    add_dependencies(CopyAssets Game)
endif()
