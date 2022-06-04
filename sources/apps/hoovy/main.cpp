#include <hff/writer.hpp>

#include <htracer/scene/scene.hpp>

#include <iostream>


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

  htracer::scene::scene<double> scene;

  auto writer = hff::writer(argv[1], 1920, 1080, 60, 8000000, 12, hff::pixel_format::YUV444P);

  writer.write_header();

  for (int i = 0; i < 600; ++i)
    writer.write_next_frame(
        [](auto &rgbplane, auto width, auto height, auto time_index)
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
              r_col[x] = static_cast<uint8_t>(10 + y + time_index * 2);
              g_col[x] = 100;
              b_col[x] = static_cast<uint8_t>(200 + time_index + x);
            }
          }
        });

  writer.write_trailer();
}
catch (std::runtime_error const &e)
{
  std::cout << "MYERROR: " << e.what() << std::endl;
  return -1;
}