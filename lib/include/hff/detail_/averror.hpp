#ifndef HFF_DETAIL__AVERROR_HPP
#define HFF_DETAIL__AVERROR_HPP


#ifdef av_err2str
#undef av_err2str
#include <array>
#include <string>
// TODO: mark str as static and then return as string_view?
av_always_inline std::string
av_err2string(int errnum)
{
  std::array<char, AV_ERROR_MAX_STRING_SIZE> str;
  return av_make_error_string(str.data(), AV_ERROR_MAX_STRING_SIZE, errnum);
}
#define av_err2str(err) av_err2string(err).c_str()
#endif // av_err2str

#endif
