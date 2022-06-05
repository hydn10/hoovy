set(target hoovy)

add_executable(${target}
"src/main.cpp"
"src/puppeteer.cpp")

target_include_directories(${target} PRIVATE ./include)

target_compile_features(${target} PRIVATE cxx_std_20)

target_link_libraries(${target} PRIVATE hff)

find_path(HTRACER_INCLUDE_DIRS "htracer/vector.hpp")
target_include_directories(${target} PRIVATE ${HTRACER_INCLUDE_DIRS})

find_package(FFMPEG COMPONENTS AVCODEC AVFORMAT AVUTIL SWSCALE REQUIRED)
target_include_directories(${target} PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_link_libraries(${target} PRIVATE ${FFMPEG_LIBRARIES})

install(TARGETS ${target})