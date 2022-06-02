#include <hff/picture.hpp>

#include <stdexcept>


namespace hff
{

picture::picture(int width, int height, AVPixelFormat pixel_format)
{
  av_frame_.get().width = width;
  av_frame_.get().height = height;
  av_frame_.get().format = pixel_format;

  int ret = av_frame_get_buffer(&av_frame_.get(), 0);
  if (ret < 0)
    throw std::runtime_error("Could not allocate frame data.");
}


AVFrame &
picture::get()
{
  return av_frame_.get();
}

} // namespace hff