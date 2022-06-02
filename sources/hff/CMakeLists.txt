set(target hff)

add_library(${target}
  "src/picture.cpp"
  "src/format.cpp"
  "src/stream_info.cpp"
  "src/frame_info.cpp"
  "src/writer.cpp"
  "src/writers/rgb24.cpp")

target_include_directories(${target} PUBLIC ./include)
target_compile_features (${target} PUBLIC cxx_std_17)

find_package(FFMPEG COMPONENTS AVCODEC AVFORMAT AVUTIL AVDEVICE REQUIRED)
target_include_directories(${target} PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_link_libraries(${target} PRIVATE ${FFMPEG_LIBRARIES})