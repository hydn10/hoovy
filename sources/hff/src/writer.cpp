#include <hff/writer.hpp>

#include <hff/detail_/averror.hpp>

#include <stdexcept>
#include <string>


namespace hff
{

bool
writer::write_video_frame(AVFormatContext &oc, hff::stream_info &sti, AVFrame const *frame)
{
  using namespace std::string_literals;

  auto &cc = sti.get_codec_context();

  int ret = avcodec_send_frame(&cc, frame);
  if (ret < 0)
    throw std::runtime_error("Error sending a frame to the encoder: "s + av_err2str(ret));

  auto &pkt = sti.get_packet();

  while (ret >= 0)
  {
    ret = avcodec_receive_packet(&cc, &pkt);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
      break;

    else if (ret < 0)
      throw std::runtime_error("Error encoding a frame: "s + av_err2str(ret));

    auto &st = sti.get_stream();

    // Rescale output packet timestamp values from codec to stream timebase
    av_packet_rescale_ts(&pkt, cc.time_base, st.time_base);
    pkt.stream_index = st.index;

    ret = av_interleaved_write_frame(&oc, &pkt);
    // pkt is now blank (av_interleaved_write_frame() takes ownership of
    // its contents and resets pkt), so that no unreferencing is necessary.
    // This would be different if one used av_write_frame().
    if (ret < 0)
      throw std::runtime_error("Error while writing output packet: "s + av_err2str(ret));
  }

  return ret != AVERROR_EOF;
}


writer::writer(
    std::string_view filename,
    uint16_t width,
    uint16_t height,
    uint16_t frame_rate,
    int64_t bit_rate,
    uint16_t gop_size,
    pixel_format pixel_format)
    : format_{filename}
    , stream_info_{format_.create_video_stream(width, height, frame_rate, bit_rate, gop_size, pixel_format)}
    , frame_info_{stream_info_.create_frames()}
    , avio_{detail_::raii::empty}
{
  if (format_.should_open_file())
  {
    avio_.reset(filename.data(), AVIO_FLAG_WRITE);

    // Careful, if I do it like this the `pb` pointer will not be set to
    // nullptr when avio_closep is called on avio_ destructor.
    // No idea if this has negative consequences.
    format_.oc().pb = &avio_.get();
  }
}


void
writer::write_header()
{
  int ret = avformat_write_header(&format_.oc(), nullptr);
  if (ret < 0)
    throw std::runtime_error("Error occurred when writing header.");
}


void
writer::write_trailer()
{
  // Flush encoder (I think?).
  while (write_video_frame(format_.oc(), stream_info_, nullptr))
  {
  }

  int ret = av_write_trailer(&format_.oc());
  if (ret < 0)
    throw std::runtime_error("Error ocurred when writing trailer.");
}

} // namespace hff