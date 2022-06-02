#ifndef HOOVY_HFF_WRITERS_RGB24_HPP
#define HOOVY_HFF_WRITERS_RGB24_HPP


#include <cstdint>


namespace hff::writers
{

class rgb24
{
  template<uint8_t idx>
  class color_plane;

  template<uint8_t idx>
  friend class color_plane;

  uint8_t *data_;
  uint64_t stride_;

public:
  rgb24(uint8_t *data, uint64_t stride);

  color_plane<0>
  red();
  color_plane<1>
  green();
  color_plane<2>
  blue();
};


template<uint8_t plane_idx>
class rgb24::color_plane
{
  class color_row;

  friend class color_row;

  rgb24 &rgb24_;

public:
  color_plane(rgb24 &rgb24);

  color_row
  operator[](uint64_t idx);
};


template<uint8_t plane_idx>
class rgb24::color_plane<plane_idx>::color_row
{
  color_plane<plane_idx> &plane_;
  uint64_t row_idx_;

public:
  color_row(color_plane<plane_idx> &plane, uint64_t column_idx);

  uint8_t &
  operator[](uint64_t idx);
};


template<uint8_t plane_idx>
rgb24::color_plane<plane_idx>::color_plane(rgb24 &rgb24)
    : rgb24_{rgb24}

{
}


template<uint8_t plane_idx>
auto
rgb24::color_plane<plane_idx>::operator[](uint64_t idx) -> color_row
{
  return color_row(*this, idx);
}


template<uint8_t plane_idx>
rgb24::color_plane<plane_idx>::color_row::color_row(color_plane<plane_idx> &plane, uint64_t row_idx)
    : plane_{plane}
    , row_idx_{row_idx}
{
}


template<uint8_t plane_idx>
uint8_t &
rgb24::color_plane<plane_idx>::color_row::operator[](uint64_t idx)
{
  return plane_.rgb24_.data_[row_idx_ * plane_.rgb24_.stride_ + 3 * idx + plane_idx];
}

} // namespace hff::writers

#endif