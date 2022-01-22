add_library(
  FlowLayout
  STATIC
  flowlayout.cpp
  flowlayout.hpp
)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
target_link_libraries(FlowLayout PRIVATE Qt5::Widgets)
