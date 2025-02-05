[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)

# Hoovy

A modern C++23 video generation library that bridges the gap between raytraced image sequences and video encoding.
Hoovy provides a simple, efficient interface to create high-quality videos from programmatically generated images using FFmpeg, with a focus on performance and ease of use.



## Features

- **Modern C++ Design**: Leverages C++23 features for clean, efficient code.
- **FFmpeg Integration**: Direct access to FFmpeg's powerful encoding capabilities.
- **Raytracing Integration**: Works directly with [htracer](https://github.com/hydn10/htracer) for raytracing.
- **Type Safe**: Strong typing and compile-time checks.
- **AV1 Encoding**: Leverage the modern AV1 ecoder using libaom.



## Examples

Below is a basic example to demonstrate encoding a short video:

```cpp
#include <hff/writer.hpp>

#include <print>
#include <ranges>


uint8_t
to_pixel_value(double x)
{
  auto constexpr NUM_COLORS = 256;
  auto scaled = NUM_COLORS * x;

  if (scaled >= NUM_COLORS) [[unlikely]]
  {
    scaled = NUM_COLORS - 1;
  }

  return static_cast<uint8_t>(scaled);
};


int
main(int argc, char **argv)
{
  uint_fast16_t constexpr W = 1920;
  uint_fast16_t constexpr H = 1080;
  uint_fast16_t constexpr FPS = 60;
  double constexpr DURATION = 12;

  auto writer = hff::writer("out.mp4", W, H, FPS, 8'000'000, 12, hff::pixel_format::YUV444P);

  writer.write_header();

  auto constexpr STEPS = static_cast<uint_fast64_t>(FPS * DURATION);
  decltype(STEPS) constexpr ZERO = 0;

  for (auto idx : std::views::iota(ZERO, STEPS))
  {
    double step = static_cast<double>(i);
    auto image = hoovy::get_image(W, H, step / STEPS);

    std::println("Progress: frame {} of {}", idx + 1, STEPS);

    writer.write_next_frame([&image](auto &rgbplane, int width, int height, int64_t time_index)
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
```

Running the code produces an mp4 video in `out.mp4`.


## Dependencies
- **FFmpeg** for video encoding.
- **[htracer](https://github.com/hydn10/htracer)** for generating raytraced images.
- C++23-compatible compiler (GCC 13+, Clang 16+).



## Usage

### Using Nix

Hoovy is packaged as a Nix Flake:

```bash
nix build
```


### Using Vcpkg

1.  Clone the repository and navigate to its root directory.
2.  Install [vcpkg](https://vcpkg.io/en/) if you don't have it already.
3.  Make sure `VCPKG_ROOT` points to the vcpkg install folder or set `CMAKE_TOOLCHAIN_FILE` to point to `<VCPKG_ROOT>/scripts/buildsystems/vcpkg.cmake"`.
4.  Configure, build and install using CMake:
    
    ```bash
    # List available presets
    cmake --list-presets
    
    # Or set options manually
    cmake -S . -B out -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake

    # Build
    cmake --build out

    # Install
    cmake --build out --target install
    ```



## License

This project is licensed under the [MIT License](LICENSE).

----------

Enjoy the videomaking! 🎥
