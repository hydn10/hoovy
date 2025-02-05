#ifndef HOOVY_HFF_FRAME_INFO_HPP
#define HOOVY_HFF_FRAME_INFO_HPP


#include <hff/picture.hpp>
#include <hff/writers/rgb24.hpp>


namespace hff
{

class frame_info
{
  picture frame_;

  // If codec format and input format are the same, I do not need the temp frame and conversion context.
  // Right now they will always be different.
  picture tmp_frame_;
  detail_::raii::sws_context converter_;

  int64_t next_pts_ = 0;

public:
  frame_info(int width, int height, AVPixelFormat final_pixel_format);

  template<typename FrameWriter>
  AVFrame const &
  produce_frame(FrameWriter &&frame_writer);
};


template<typename FrameWriter>
AVFrame const &
frame_info::produce_frame(FrameWriter &&frame_writer)
{
  int ret = av_frame_make_writable(&frame_.get());
  if (ret < 0)
    throw std::runtime_error("Could not make frame writable.");

  auto &f = frame_.get();
  auto &tf = tmp_frame_.get();

  int height = f.height;
  int width = f.width;

  auto *rgbplane = tf.data[0];
  auto linesize = tf.linesize[0];

  hff::writers::rgb24 rgb24(rgbplane, linesize);

  frame_writer(rgb24, width, height, next_pts_);

  sws_scale(&converter_.get(), tf.data, tf.linesize, 0, height, f.data, f.linesize);

  f.pts = next_pts_++;

  return f;
}

} // namespace hff

#endif