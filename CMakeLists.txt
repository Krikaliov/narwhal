cmake_minimum_required(VERSION 3.14)
set(VERSION 1.0)
project(narwhal VERSION ${VERSION})

list(APPEND CMAKE_PREFIX_PATH "C:/SFML-2.4.2/cmake")
find_package(SFML 2 COMPONENTS window graphics audio network REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME}-v${VERSION} ${SOURCES})

target_include_directories(${PROJECT_NAME}-v${VERSION} ${SOURCES})*
target_link_libraries(${PROJECT_NAME}-v${VERSION} ${SFML_LIBRAIRIES})
