set(target hff)

add_library(${target}
  "src/picture.cpp"
  "src/format.cpp"
  "src/stream_info.cpp"
  "src/frame_info.cpp"
  "src/writer.cpp"
  "src/writers/rgb24.cpp"
)

target_include_directories(${target} PUBLIC include)
target_compile_features (${target} PUBLIC cxx_std_17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
  libavformat
  libavcodec
  libswscale
  libavutil
)

target_link_libraries(${target} PkgConfig::LIBAV)