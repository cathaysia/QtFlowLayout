cmake_minimum_required(VERSION 3.2)
project(FlowLayout
    VERSION 0.0.1
    DESCRIPTION "a flow widget"
    LANGUAGES CXX
    )
set(CMAKE_CXX_STANDARD 17)

set(AUTOMOC ON)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_library(FlowLayout STATIC flowlayout.cpp)

target_link_libraries(FlowLayout PRIVATE Qt5::Widgets)
target_include_directories(FlowLayout PUBLIC INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    )

if(NOT ${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
    message(AUTHOR_WARNING "Project treated as a subproject. No packing and installing")
else()
    # 安装
    include(CMakePackageConfigHelpers)
    install(TARGETS FlowLayout
        DESTINATION lib
        EXPORT FlowLayoutTargets)
    install(FILES flowlayout.h DESTINATION include)
    install(EXPORT FlowLayoutTargets
        FILE FlowLayoutTargets.cmake
        DESTINATION lib/cmake/FlowLayout)

    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/FlowLayoutConfig.cmake"
        INSTALL_DESTINATION "lib/cmake/FlowLayout"
        NO_SET_AND_CHECK_MACRO
        NO_CHECK_REQUIRED_COMPONENTS_MACRO
    )

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/FlowLayoutConfigVersion.cmake"
        VERSION "${FlowLayout_VERSION_MAJOR}.${FlowLayout_VERSION_MINOR}"
        COMPATIBILITY AnyNewerVersion
    )

    install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/FlowLayoutConfig.cmake
        DESTINATION lib/cmake/FlowLayout)

    # 打包
    include(InstallRequiredSystemLibraries)
    set(CPACK_PACKAGE_VENDOR Z)
    set(CPACK_GENERATOR TGZ)
    set(CPACK_PACKAGE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/pack)
    include(CPack)
    set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_SYSTEM_NAME})
    include(CMakePrintHelpers)
    cmake_print_variables(CPACK_PACKAGE_FILE_NAME)
endif()
