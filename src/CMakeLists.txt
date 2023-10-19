add_library(
  FlowLayout
  STATIC
  flowlayout.cpp
  flowlayout.hpp
)

if(BUILD_WITH_QT6)

    find_package(Qt6 COMPONENTS Widgets REQUIRED)
    target_include_directories(FlowLayout PRIVATE Qt6::Widgets)
    target_link_libraries(FlowLayout PRIVATE Qt6::Widgets)

else()

    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    target_include_directories(FlowLayout PRIVATE Qt5::Widgets)
    target_link_libraries(FlowLayout PRIVATE Qt5::Widgets)

endif()