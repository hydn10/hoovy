#include <hff/stream_info.hpp>

#include <stdexcept>


namespace hff
{

stream_info::stream_info(AVStream *stream, AVCodec const *codec, detail_::raii::av_codec_context &&codec_context)
    : stream_{stream}
    , codec_{codec}
    , codec_context_{std::move(codec_context)}
{
}


frame_info
stream_info::create_frames()
{
  // Open the codec.
  int ret = avcodec_open2(&codec_context_.get(), codec_, nullptr);
  if (ret < 0)
  {
    throw std::runtime_error("Could not open video codec.");
  }

  // Copy the stream parameters to the muxer.
  ret = avcodec_parameters_from_context(stream_->codecpar, &codec_context_.get());
  if (ret < 0)
  {
    throw std::runtime_error("Could not copy the stream parameters.");
  }

  // Allocate the frames.
  return frame_info(codec_context_.get().width, codec_context_.get().height, codec_context_.get().pix_fmt);
}


AVCodecContext &
stream_info::get_codec_context()
{
  return codec_context_.get();
}


AVStream &
stream_info::get_stream()
{
  return *stream_;
}


AVPacket &
stream_info::get_packet()
{
  return packet_.get();
}


int
stream_info::get_width() const
{
  return codec_context_.get().width;
}


int
stream_info::get_height() const
{
  return codec_context_.get().height;
}


AVPixelFormat
stream_info::get_pixel_format() const
{
  return codec_context_.get().pix_fmt;
}


AVRational
stream_info::get_time_base() const
{
  return codec_context_.get().time_base;
}

} // namespace hff
