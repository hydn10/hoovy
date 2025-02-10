#ifndef HOOVY_HFF_WRITER_HPP
#define HOOVY_HFF_WRITER_HPP


#include <hff/format.hpp>
#include <hff/frame_info.hpp>
#include <hff/pixel_format.hpp>
#include <hff/stream_info.hpp>

#include <cstdint>
#include <string_view>


namespace hff
{

class writer
{
  format format_;
  stream_info stream_info_;
  frame_info frame_info_;

  detail_::raii::avio avio_;

  // static int
  // write_frame(AVFormatContext *oc, const AVRational *time_base, AVStream *st, AVPacket *pkt);
  static bool
  write_video_frame(AVFormatContext &oc, hff::stream_info &sti, AVFrame const *frame);

public:
  writer(
      std::string_view filename,
      uint16_t width,
      uint16_t height,
      uint16_t frame_rate,
      int64_t bit_rate,
      uint16_t gop_size,
      pixel_format pixel_format);

  void
  write_header();
  template<typename FrameWriter>
  void
  write_next_frame(FrameWriter &&frame_writer);
  void
  write_trailer();
};


template<typename FrameWriter>
void
writer::write_next_frame(FrameWriter &&frame_writer)
{
  write_video_frame(format_.oc(), stream_info_, &frame_info_.produce_frame(std::forward<FrameWriter>(frame_writer)));
}

} // namespace hff

#endif