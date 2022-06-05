#include <hff/writer.hpp>

#include <puppeteer.hpp>

#include <iostream>

uint8_t
to_pixel_value(double x)
{
  auto constexpr NUM_COLORS = 256;

  auto scaled = NUM_COLORS * x;

  // Handle case where x == 1.0.
  // Another option is scaling by (NUM_COLORS - .000001), but I feel it is more correct this way.
  // The branch predictor should almost always take the other path anyway.
  if (scaled >= NUM_COLORS) [[unlikely]]
    scaled = NUM_COLORS - 1;

  return static_cast<uint8_t>(scaled);
};


int
main(int argc, char **argv)
try
{
  if (argc < 2)
  {
    printf(
        "usage: %s output_file\n"
        "API example program to output a media file with libavformat.\n"
        "This program generates a synthetic audio and video stream, encodes and\n"
        "muxes them into a file named output_file.\n"
        "The output format is automatically guessed according to the file extension.\n"
        "Raw images can also be output by using '%%d' in the filename.\n"
        "\n",
        argv[0]);
    return 1;
  }

  uint32_t constexpr W = 720;
  uint32_t constexpr H = 576;
  uint32_t constexpr FPS = 60;
  double constexpr DURATION = 5;

  auto writer = hff::writer(argv[1], W, H, FPS, 8000000, 12, hff::pixel_format::YUV444P);

  writer.write_header();

  auto constexpr STEPS = static_cast<uint64_t>(FPS * DURATION);

  for (uint64_t i = 0; i < STEPS; ++i)
  {
    double step = static_cast<double>(i);
    auto image = hoovy::get_image(W, H, step / STEPS);

    std::cout << i + 1 << " / " << STEPS << std::endl;

    writer.write_next_frame(
        [&image](auto &rgbplane, auto width, auto height, auto time_index)
        {
          auto red = rgbplane.red();
          auto green = rgbplane.green();
          auto blue = rgbplane.blue();

          for (int y = 0; y < height; ++y)
          {
            auto r_col = red[y];
            auto g_col = green[y];
            auto b_col = blue[y];

            for (int x = 0; x < width; ++x)
            {
              auto const &pixel = image.pixels()[y * width + x].to_srgb();
              (void)time_index;

              r_col[x] = to_pixel_value(pixel.r());
              g_col[x] = to_pixel_value(pixel.g());
              b_col[x] = to_pixel_value(pixel.b());
            }
          }
        });
  }

  writer.write_trailer();
}
catch (std::runtime_error const &e)
{
  std::cout << "MYERROR: " << e.what() << std::endl;
  return -1;
}