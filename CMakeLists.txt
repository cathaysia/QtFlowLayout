cmake_minimum_required(VERSION 3.2)
project(
  FlowLayout
  VERSION 0.0.1
  DESCRIPTION "a flow widget"
  LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

add_subdirectory(src)

include(CTest)

add_subdirectory(test)

