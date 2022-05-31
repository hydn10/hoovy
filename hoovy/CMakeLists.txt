set(target hoovy)

add_executable (${target} "main.cpp")
target_compile_features(${target} PRIVATE cxx_std_17)

find_package(FFMPEG COMPONENTS AVCODEC AVFORMAT AVUTIL AVDEVICE REQUIRED)
target_include_directories(${target} PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_link_libraries(${target} PRIVATE ${FFMPEG_LIBRARIES})

install(TARGETS ${target})