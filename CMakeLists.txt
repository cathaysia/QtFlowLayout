cmake_minimum_required(VERSION 3.2)
project(
  FlowLayout
  VERSION 0.0.1
  DESCRIPTION "a flow widget"
  LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)

add_library(FlowLayout STATIC flowlayout.cpp flowlayout.h flowlayout.hpp)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
target_link_libraries(FlowLayout PRIVATE Qt5::Widgets)
