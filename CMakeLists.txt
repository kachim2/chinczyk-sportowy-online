cmake_minimum_required(VERSION 3.10)
project(chinczyk_sportowy_online LANGUAGES C CXX)
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  MESSAGE("Clang")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  MESSAGE("GNU")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  MESSAGE("Intel")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
  MESSAGE("MSVC")
endif()
option(USE_SFML "USE SFML" ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set (outname ${PROJECT_NAME})
if (EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EMSCRIPTEN_ARGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -sPTHREAD_POOL_SIZE=6 -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=0 --std=c++14 -s ASYNCIFY -lwebsocket.js -sPROXY_POSIX_SOCKETS -pthread")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
    set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so that you can directly open it.
    #configure_file(${CMAKE_SOURCE_DIR}/html/shell.html ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/index.html COPYONLY)
    configure_file(${CMAKE_SOURCE_DIR}/4deploy/staticwebapp.config.json ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/staticwebapp.config.json COPYONLY)
    set (outname index)
endif ()
include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
set(CUSTOMIZE_BUILD on CACHE BOOL "" FORCE)
#set(SUPPORT_PARTIALBUSY_WAIT_LOOP OFF CACHE BOOL "" FORCE)
set(FETCHCONTENT_QUIET FALSE )

add_definitions(-DUSE_SFML)
set(SFML_BUILD_AUDIO    OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_GRAPHICS    OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_WINDOW    OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS    OFF CACHE BOOL "" FORCE)
FetchContent_Declare(sfml-net GIT_REPOSITORY https://github.com/SFML/SFML GIT_TAG 2.6.x GIT_PROGRESS TRUE)
FetchContent_MakeAvailable(sfml-net)




FetchContent_Declare(raylib GIT_REPOSITORY https://github.com/raysan5/raylib.git GIT_TAG master GIT_PROGRESS TRUE)
FetchContent_MakeAvailable(raylib)


add_executable(${outname} src/main.cpp src/net.cpp)
#add_executable(server src/server.cpp)
target_include_directories(${outname} PRIVATE include)
target_link_libraries(${outname} PRIVATE raylib)
target_link_libraries(${outname} PRIVATE sfml-network)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
