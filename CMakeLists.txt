cmake_minimum_required(VERSION 3.20)
project(CMP VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_definitions(WIN32)

add_executable(${PROJECT_NAME} 
  src/main.cpp 
  src/pipe.cpp
  src/pipe_server.cpp 
  src/win_process.cpp
)
add_executable(${PROJECT_NAME}_sub 
  src/subprocess.cpp 
  src/pipe_client.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(${PROJECT_NAME}_sub PUBLIC ${CMAKE_SOURCE_DIR})