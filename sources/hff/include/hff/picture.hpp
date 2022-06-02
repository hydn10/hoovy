#ifndef HOOVY_HFF_PICTURE_HPP
#define HOOVY_HFF_PICTURE_HPP


#include <hff/detail_/raii/objects.hpp>


namespace hff
{

class picture
{
  detail_::raii::av_frame av_frame_;

public:
  picture(int width, int height, AVPixelFormat pixel_format);

  AVFrame &
  get();
};

} // namespace hff

#endif