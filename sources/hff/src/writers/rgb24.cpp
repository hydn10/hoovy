#include <hff/writers/rgb24.hpp>


namespace hff::writers
{

rgb24::rgb24(uint8_t *data, uint64_t stride)
    : data_{data}
    , stride_{stride}
{
}


auto
rgb24::red() -> color_plane<0>
{
  return color_plane<0>(*this);
}


auto
rgb24::green() -> color_plane<1>
{
  return color_plane<1>(*this);
}


auto
rgb24::blue() -> color_plane<2>
{
  return color_plane<2>(*this);
}

} // namespace hff::writers