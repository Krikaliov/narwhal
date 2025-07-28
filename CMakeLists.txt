cmake_minimum_required(VERSION 3.15)
project(narwhal LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.6.2
  GIT_SHALLOW ON)
FetchContent_MakeAvailable(SFML)

include_directories(${SFML_INCLUDE_DIR})

file(GLOB_RECURSE SOURCE_DIRECTORY "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_DIRECTORY})

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-network sfml-graphics sfml-audio sfml-window sfml-system)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
