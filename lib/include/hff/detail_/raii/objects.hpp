#ifndef HOOVY_HFF_DETAIL__RAII_OBJECTS_HPP
#define HOOVY_HFF_DETAIL__RAII_OBJECTS_HPP


extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libswscale/swscale.h>
}

#include <hff/detail_/raii/raii_for_args.hpp>


namespace hff::detail_::raii
{

namespace abbr
{

using av_format_context = raii_for_args<AVFormatContext, AVOutputFormat const *, char const *, char const *>;

using avio = raii_for_args<AVIOContext, char const *, int>;

using sws_context = raii_for_args<
    SwsContext,
    int,
    int,
    AVPixelFormat,
    int,
    int,
    AVPixelFormat,
    int,
    SwsFilter *,
    SwsFilter *,
    double const *>;

} // namespace abbr


using av_format_context_for_output =
    abbr::av_format_context::type_ac_af<avformat_alloc_output_context2, avformat_free_context>;

using avio = abbr::avio::type<abbr::avio::to_p_ret_create<avio_open>, abbr::avio::to_p_free_ignore_ret<avio_closep>>;

using av_codec_context =
    raii_for_args<AVCodecContext, AVCodec const *>::type<avcodec_alloc_context3, avcodec_free_context>;

using av_frame = raii_for_args<AVFrame>::type<av_frame_alloc, av_frame_free>;

using av_packet = raii_for_args<AVPacket>::type<av_packet_alloc, av_packet_free>;

using sws_context = abbr::sws_context::type_af<sws_getContext, sws_freeContext>;

} // namespace hff::detail_::raii

#endif
