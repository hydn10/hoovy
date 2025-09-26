#include <hff/writers/rgb24.hpp>

#include <cstdint>


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
  return {*this};
}


auto
rgb24::green() -> color_plane<1>
{
  return {*this};
}


auto
rgb24::blue() -> color_plane<2>
{
  return {*this};
}

} // namespace hff::writers
