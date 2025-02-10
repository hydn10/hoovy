#ifndef HOOVY_HFF_STREAM_INFO_HPP
#define HOOVY_HFF_STREAM_INFO_HPP


extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include <hff/detail_/raii/objects.hpp>
#include <hff/frame_info.hpp>


namespace hff
{

class stream_info
{
  AVStream *stream_;
  AVCodec const *codec_;
  detail_::raii::av_codec_context codec_context_;
  detail_::raii::av_packet packet_;

public:
  stream_info(AVStream *stream, AVCodec const *codec, detail_::raii::av_codec_context &&codec_context);

  frame_info
  create_frames();

  AVCodecContext &
  get_codec_context();
  AVStream &
  get_stream();
  AVPacket &
  get_packet();

  int
  get_width() const;
  int
  get_height() const;
  AVPixelFormat
  get_pixel_format() const;
  AVRational
  get_time_base() const;
};

} // namespace hff

#endif