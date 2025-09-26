cmake_minimum_required(VERSION 3.25)

file(READ vcpkg.json VCPKG_JSON_STRING)
string(JSON PROJ_NAME_FROM_JSON GET ${VCPKG_JSON_STRING} name)
string(JSON VERSION_FROM_JSON GET ${VCPKG_JSON_STRING} version)

project(${PROJ_NAME_FROM_JSON} LANGUAGES CXX VERSION ${VERSION_FROM_JSON})

option(
    HOOVY_BUILD_TESTS
    "Build the tests. Default: ${PROJECT_IS_TOP_LEVEL}."
    ${PROJECT_IS_TOP_LEVEL})

option(
    HOOVY_BUILD_APP
    "Build the hoovy app. Default: ${PROJECT_IS_TOP_LEVEL}."
    ${PROJECT_IS_TOP_LEVEL})

enable_testing()


add_subdirectory(lib)

if(HOOVY_BUILD_APP)
    add_subdirectory(apps)
endif()


include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(
    EXPORT hoovy-targets
    NAMESPACE hoovy::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/hoovy"
    FILE hoovy-targets.cmake
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/hoovy-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/lib/hoovy-config.cmake
    INSTALL_DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/hoovy
    NO_SET_AND_CHECK_MACRO
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/lib/hoovy-version.cmake
    COMPATIBILITY ExactVersion
) 

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/lib/hoovy-version.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/lib/hoovy-config.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/hoovy"
)