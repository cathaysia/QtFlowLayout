add_executable(layoutTest main.cpp mainwindow.cpp mainwindow.hpp)

if(BUILD_WITH_QT6)

    find_package(Qt6 COMPONENTS Widgets REQUIRED)
    target_include_directories(layoutTest PRIVATE Qt6::Widgets ../src)
    target_link_libraries(layoutTest PRIVATE Qt6::Widgets FlowLayout)

else()

    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    target_include_directories(layoutTest PRIVATE Qt5::Widgets ../src)
    target_link_libraries(layoutTest PRIVATE Qt5::Widgets FlowLayout)

endif()

add_test(TestLayout layoutTest)
