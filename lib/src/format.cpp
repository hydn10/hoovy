#include <hff/format.hpp>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
}

#include <stdexcept>
#include <format>
#include <thread>


namespace hff
{

format::format(std::string_view filename)
    : format_context_(nullptr, nullptr, filename.data())
{
}


stream_info
format::create_video_stream(
    uint16_t width,
    uint16_t height,
    uint16_t frame_rate,
    int64_t bit_rate,
    uint16_t gop_size,
    pixel_format pixel_format)
{
  auto codec_id = AV_CODEC_ID_AV1;

  // Freed when format_context_ is freed, no need to call anything extra.
  auto *stream_ptr = avformat_new_stream(&format_context_.get(), nullptr);
  if (stream_ptr == nullptr)
  {
    throw std::runtime_error("Could not allocate stream.");
  }

  auto &stream = *stream_ptr;

  stream.id = format_context_.get().nb_streams - 1;

  AVCodec const *codec = avcodec_find_encoder(codec_id);
  if (codec == nullptr)
  {
    throw std::runtime_error(std::format("Could not find encoder for {}", avcodec_get_name(codec_id)));
  }

  detail_::raii::av_codec_context codec_context(codec);
  auto &cc = codec_context.get();

  cc.codec_id = codec_id;
  cc.bit_rate = bit_rate;
  cc.width = width;
  cc.height = height;

  stream.time_base = AVRational{1, static_cast<int>(frame_rate)};
  stream.codecpar->codec_id = codec_id;

  cc.time_base = stream.time_base;
  cc.gop_size = gop_size;
  cc.pix_fmt = static_cast<AVPixelFormat>(pixel_format);

  av_opt_set(cc.priv_data, "cpu-used", "4", 0);
  cc.thread_count = std::thread::hardware_concurrency();

  // Some formats want stream headers to be separate.
  if (format_context_.get().oformat->flags & AVFMT_GLOBALHEADER)
  {
    cc.flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
  }

  return stream_info{stream_ptr, codec, std::move(codec_context)};
}


AVFormatContext &
format::oc()
{
  return format_context_.get();
}


AVFormatContext const &
format::oc() const
{
  return format_context_.get();
}


bool
format::should_open_file() const
{
  return !(format_context_.get().oformat->flags & AVFMT_NOFILE);
}

} // namespace hff
