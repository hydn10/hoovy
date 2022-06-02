#ifndef HOOVY_HFF_PIXEL_FORMAT_HPP
#define HOOVY_HFF_PIXEL_FORMAT_HPP

extern "C"
{
#include <libavformat/avformat.h>
}


namespace hff
{

enum class pixel_format
{
  YUV444P = AV_PIX_FMT_YUV444P,
  YUV420P = AV_PIX_FMT_YUV420P,
};

}

#endif