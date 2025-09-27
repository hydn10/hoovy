set(target_name hoovy.apps.hoovy)

add_executable(${target_name})
  
target_sources(
    ${target_name}
    PRIVATE
        FILE_SET HEADERS
		BASE_DIRS
			"${CMAKE_CURRENT_SOURCE_DIR}/include"
		FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/include/puppeteer.hpp"
    PRIVATE
        "src/main.cpp"
        "src/puppeteer.cpp"
)

target_compile_features(${target_name} PRIVATE cxx_std_23)

target_link_libraries(${target_name} PRIVATE hoovy::hoovy)

find_package(htracer CONFIG REQUIRED)
target_link_libraries(${target_name} PRIVATE htracer::htracer)

set_target_properties(
    ${target_name}
    PROPERTIES
        OUTPUT_NAME hoovy-app
        EXPORT_NAME hoovy-app
)

install(
    TARGETS ${target_name}
    EXPORT hoovy-targets
    RUNTIME_DEPENDENCY_SET ${target_name}_RUNTIME_DEPS
)

# See [1].
# https://github.com/microsoft/vcpkg/issues/13797
if (WIN32)
    install(
        RUNTIME_DEPENDENCY_SET ${target_name}_RUNTIME_DEPS
            DIRECTORIES "$<TARGET_FILE_DIR:hoovy::hoovy>"
            PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
            POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
    )
endif()

message(STATUS "Added app: ${target_name}")
