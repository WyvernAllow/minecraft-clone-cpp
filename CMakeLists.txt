cmake_minimum_required(VERSION 3.13)
project(minecraft-clone VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp")
add_executable(${PROJECT_NAME} ${sources})

add_subdirectory(deps/glfw)
add_subdirectory(deps/glm)

target_include_directories(${PROJECT_NAME}
    PRIVATE deps/glfw/include
    PRIVATE deps/glm
    PRIVATE deps/glad/include
    PRIVATE deps/stb/include
)

target_sources(${PROJECT_NAME}
    PRIVATE deps/glad/src/gl.c
    PRIVATE deps/stb/include/stb_image.c
)

target_link_directories(${PROJECT_NAME}
    PRIVATE deps/glfw/src
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE glfw
)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res
)

add_dependencies(${PROJECT_NAME} copy_assets)
