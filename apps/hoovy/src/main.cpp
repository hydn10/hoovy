#include <puppeteer.hpp>

#include <hff/pixel_format.hpp>
#include <hff/writer.hpp>

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <print>
#include <ranges>
#include <span>
#include <stdexcept>


namespace
{

uint8_t
to_pixel_value(double x)
{
  auto constexpr NUM_COLORS = 256;

  auto scaled = NUM_COLORS * x;

  // Handle case where x == 1.0.
  // Another option is scaling by (NUM_COLORS - .000001), but I feel it is more correct this way.
  // The branch predictor should almost always take the other path anyway.
  if (scaled >= NUM_COLORS) [[unlikely]]
  {
    scaled = NUM_COLORS - 1;
  }

  return static_cast<uint8_t>(scaled);
};

} // namespace


int
main(int argc, char **argv)
try
{
  std::span<char const *const> const args{argv, static_cast<std::size_t>(argc)};

  if (argc < 2)
  {
    std::println("Usage: {} <output_file>", args[0]);
    return 1;
  }

  try
  {
    constexpr uint_fast16_t W = 1280;
    constexpr uint_fast16_t H = 720;
    constexpr uint_fast16_t FPS = 24;
    constexpr double DURATION = 8;

    auto writer = hff::writer(args[1], W, H, FPS, 8'000'000, 12, hff::pixel_format::YUV444P);

    writer.write_header();

    constexpr auto STEPS = static_cast<uint_fast64_t>(FPS * DURATION);

    for (auto idx : std::views::iota(decltype(STEPS){0}, STEPS))
    {
      auto step = static_cast<double>(idx);
      auto image = hoovy::get_image(W, H, step / STEPS);

      std::println("Frames: {} / {}", idx + 1, STEPS);

      writer.write_next_frame([&image](auto &rgbplane, int width, int height, uint64_t)
      {
        auto red = rgbplane.red();
        auto green = rgbplane.green();
        auto blue = rgbplane.blue();

        for (auto const &[x, y] :
             std::views::cartesian_product(std::views::iota(0, width), std::views::iota(0, height)))
        {
          auto r_col = red[y];
          auto g_col = green[y];
          auto b_col = blue[y];

          auto pixel = image.pixels()[y * width + x].to_srgb();

          r_col[x] = to_pixel_value(pixel.r());
          g_col[x] = to_pixel_value(pixel.g());
          b_col[x] = to_pixel_value(pixel.b());
        }
      });
    }

    writer.write_trailer();
  }
  catch (std::runtime_error const &e)
  {
    std::println(std::cerr, "RUNTIME_ERROR: {}", e.what());
    return 2;
  }
}
catch (...)
{
  return -1;
}
