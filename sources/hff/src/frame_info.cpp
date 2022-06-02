#include <hff/frame_info.hpp>

extern "C"
{
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

#include <stdexcept>


namespace hff
{

namespace
{

inline constexpr auto TMP_FRAME_PIX_FMT = AV_PIX_FMT_RGB24;

}


frame_info::frame_info(int width, int height, AVPixelFormat final_pixel_format)
    : frame_{width, height, final_pixel_format}
    , tmp_frame_{width, height, TMP_FRAME_PIX_FMT}
    , converter_{
          width,
          height,
          TMP_FRAME_PIX_FMT,
          width,
          height,
          final_pixel_format,
          SWS_LANCZOS | SWS_ACCURATE_RND,
          nullptr,
          nullptr,
          nullptr}
{
}

} // namespace hff