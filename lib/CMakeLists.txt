set(target_name hoovy)

add_library(${target_name})
add_library(hoovy::hoovy ALIAS ${target_name})

target_sources(
	${target_name}
	PUBLIC
		FILE_SET HEADERS
		BASE_DIRS
			"${CMAKE_CURRENT_SOURCE_DIR}/include"
		FILES
			"${CMAKE_CURRENT_SOURCE_DIR}/include/hff/detail_/av/avcodec.hpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/include/hff/detail_/av/avformat.hpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/include/hff/detail_/av/avutil.hpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/include/hff/detail_/av/swscale.hpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/include/hff/detail_/raii/objects.hpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/include/hff/detail_/raii/raii_for_args.hpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/include/hff/detail_/averror.hpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/include/hff/writers/rgb24.hpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/include/hff/format.hpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/include/hff/frame_info.hpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/include/hff/picture.hpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/include/hff/pixel_format.hpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/include/hff/stream_info.hpp"
			"${CMAKE_CURRENT_SOURCE_DIR}/include/hff/writer.hpp"
	PRIVATE
		"src/picture.cpp"
		"src/format.cpp"
		"src/stream_info.cpp"
		"src/frame_info.cpp"
		"src/writer.cpp"
		"src/writers/rgb24.cpp"
)

if(BUILD_SHARED_LIBS)
	set_target_properties(
		${target_name}
		PROPERTIES
			WINDOWS_EXPORT_ALL_SYMBOLS ON
	)
endif()

target_compile_features (${target_name} PUBLIC cxx_std_20)

set_target_properties(
    ${target_name}
    PROPERTIES
        VERIFY_INTERFACE_HEADER_SETS ON
        EXPORT_NAME hoovy
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
	libavformat
	libavcodec
	libswscale
	libavutil
)

target_link_libraries(${target_name} PkgConfig::LIBAV)

install(
    TARGETS ${target_name}
    EXPORT hoovy-targets
    FILE_SET HEADERS
)

message(STATUS "Added library: ${target_name}")