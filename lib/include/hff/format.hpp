#ifndef HOOVY_HFF_FORMAT_HPP
#define HOOVY_HFF_FORMAT_HPP


#include <hff/detail_/raii/objects.hpp>
#include <hff/pixel_format.hpp>
#include <hff/stream_info.hpp>

#include <string_view>


namespace hff
{

class format
{
  detail_::raii::av_format_context_for_output format_context_;

public:
  format(std::string_view filename);

  stream_info
  create_video_stream(
      uint16_t width,
      uint16_t height,
      uint16_t frame_rate,
      int64_t bit_rate,
      uint16_t gop_size,
      pixel_format pixel_format);

  AVFormatContext &
  oc();
  AVFormatContext const &
  oc() const;

  bool
  should_open_file() const;
};

} // namespace hff

#endif