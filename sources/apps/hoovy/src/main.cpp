#include <hff/writer.hpp>

#include <puppeteer.hpp>

#include <iostream>
#include <print>
#include <ranges>


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


int
main(int argc, char **argv)
try
{
  if (argc < 2)
  {
    std::println("Usage: {} <output_file>", argv[0]);
    return 1;
  }

  uint_fast16_t constexpr W = 1280;
  uint_fast16_t constexpr H = 720;
  uint_fast16_t constexpr FPS = 24;
  double constexpr DURATION = 8;

  auto writer = hff::writer(argv[1], W, H, FPS, 8'000'000, 12, hff::pixel_format::YUV444P);

  writer.write_header();

  auto constexpr STEPS = static_cast<uint_fast64_t>(FPS * DURATION);
  decltype(STEPS) constexpr ZERO = 0;

  for (auto i : std::views::iota(ZERO, STEPS))
  {
    double step = static_cast<double>(i);
    auto image = hoovy::get_image(W, H, step / STEPS);

    std::println("{} / {}", i + 1, STEPS);

    writer.write_next_frame([&image](auto &rgbplane, int width, int height, uint64_t time_index)
    {
      auto red = rgbplane.red();
      auto green = rgbplane.green();
      auto blue = rgbplane.blue();

      for (auto [x, y] : std::views::cartesian_product(std::views::iota(0, width), std::views::iota(0, height)))
      {
        auto r_col = red[y];
        auto g_col = green[y];
        auto b_col = blue[y];

        auto pixel = image.pixels()[y * width + x].to_srgb();
        (void)time_index;

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
  std::println(std::cerr, "MYERROR: {}", e.what());
  return -1;
}
