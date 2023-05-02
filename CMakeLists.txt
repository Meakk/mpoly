cmake_minimum_required(VERSION 3.0)

project(mpoly VERSION 0.1)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

add_library(mpoly INTERFACE)

set_target_properties(mpoly PROPERTIES PUBLIC_HEADER "${CMAKE_SOURCE_DIR}/mpoly.h")

target_include_directories(mpoly INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(TARGETS mpoly EXPORT mpoly-targets PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT mpoly-targets NAMESPACE mpoly:: DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/cmake/mpoly")

configure_package_config_file(
    "${CMAKE_SOURCE_DIR}/cmake/mpoly-config.cmake.in"
    "${CMAKE_BINARY_DIR}/mpoly-config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/cmake/mpoly"
)
write_basic_package_version_file(
    "${CMAKE_BINARY_DIR}/mpoly-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMinorVersion
)
install(
    FILES "${CMAKE_BINARY_DIR}/mpoly-config.cmake" "${CMAKE_BINARY_DIR}/mpoly-config-version.cmake"
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/cmake/mpoly"
)

# Testing
option(BUILD_TESTING "Enable testing" OFF)

if (BUILD_TESTING)
    enable_testing()

    add_executable(main_test test.cxx)
    target_include_directories(main_test PRIVATE ${CMAKE_SOURCE_DIR})

    add_test(NAME mpoly::test COMMAND main_test)
endif()
