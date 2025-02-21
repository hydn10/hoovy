﻿set(target hoovy)

add_executable(${target}
  "src/main.cpp"
  "src/puppeteer.cpp")

target_include_directories(${target} PRIVATE include)

target_compile_features(${target} PRIVATE cxx_std_23)

target_link_libraries(${target} PRIVATE hff)

find_package(HTracer CONFIG REQUIRED)
target_link_libraries(${target} PRIVATE HTracer::HTracer)


install(
  TARGETS ${target}
  RUNTIME_DEPENDENCY_SET ${target}_RUNTIME_DEPS
)

# See [1].
# https://github.com/microsoft/vcpkg/issues/13797
if (WIN32)
  install(
    RUNTIME_DEPENDENCY_SET ${target}_RUNTIME_DEPS
      PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
      POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
  )
endif()
