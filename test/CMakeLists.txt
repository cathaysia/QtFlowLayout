set(CMAKE_AUTOMOC ON)

add_executable(layoutTest main.cpp mainwindow.cpp mainwindow.hpp)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
target_include_directories(layoutTest PRIVATE Qt5::Widgets ../src)
target_link_libraries(layoutTest PRIVATE Qt5::Widgets FlowLayout)

add_test(TestLayout layoutTest)
