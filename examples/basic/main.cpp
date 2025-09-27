#include <hff/pixel_format.hpp>
#include <hff/writer.hpp>

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <print>
#include <ranges>


namespace
{

constexpr uint16_t WIDTH = 640;
constexpr uint16_t HEIGHT = 360;
constexpr uint16_t FPS = 144;
constexpr double DURATION_SECONDS = 4.0;
constexpr int64_t BIT_RATE = 2'000'000;
constexpr uint16_t GOP_SIZE = 12;


constexpr uint8_t
lerp_px(double a, double b, double t)
{
  return static_cast<uint8_t>(a + (b - a) * t);
}


void
paint_frame(auto &rgb, int width, int height, double progress)
{
  int const square_size = HEIGHT / 5;
  int const square_left = static_cast<int>(std::round((width - square_size) * progress));
  int const square_top = (HEIGHT - square_size) / 2;

  for (int y = 0; y < height; ++y)
  {
    double const backdrop = static_cast<double>(y) / (HEIGHT - 1);
    auto const bg_col = lerp_px(60.0, 200.0, backdrop);

    auto r_row = rgb.red()[y];
    auto g_row = rgb.green()[y];
    auto b_row = rgb.blue()[y];

    for (int x = 0; x < width; ++x)
    {
      bool const inside_square =
          x >= square_left && x < square_left + square_size && y >= square_top && y < square_top + square_size;

      if (inside_square)
      {
        r_row[x] = lerp_px(230.0, 0.0, progress);
        g_row[x] = 20;
        b_row[x] = lerp_px(0.0, 230.0, progress);
      }
      else
      {
        r_row[x] = g_row[x] = b_row[x] = bg_col;
      }
    }
  }
}

} // namespace


int
main()
try
{
  hff::writer writer("basic.mp4", WIDTH, HEIGHT, FPS, BIT_RATE, GOP_SIZE, hff::pixel_format::YUV444P);

  writer.write_header();

  constexpr auto total_frames = static_cast<std::uint64_t>(FPS * DURATION_SECONDS);

  for (auto frame_number : std::views::iota(decltype(total_frames){0}, total_frames))
  {
    writer.write_next_frame([](auto &rgb, int width, int height, std::uint64_t idx)
    {
      auto const progress = (total_frames > 1) ? static_cast<double>(idx) / (total_frames - 1) : 0.0;
      paint_frame(rgb, width, height, progress);
    });

    std::println("Frames: {} / {}", frame_number + 1, total_frames);
  }

  std::println("Done.");

  writer.write_trailer();

  return EXIT_SUCCESS;
}
catch (...)
{
  return EXIT_FAILURE;
}
