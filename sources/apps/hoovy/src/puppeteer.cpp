#include <puppeteer.hpp>

#include <htracer/htracer.hpp>

#include <numbers>


namespace hoovy
{

namespace
{

template<typename Float>
using sphere_t = htracer::geometries::sphere<Float>;
template<typename Float>
using plane_t = htracer::geometries::plane<Float>;


constexpr htracer::staging::material<double>
get_sphere_material(double hue)
{
  using htracer::colors::hsl;

  auto const color = hsl<double>{hue, 0.7, 0.5}.to_srgb().to_linear();
  return htracer::staging::make_solid<double>(color, 0.125, 0.05, 200, .4);
}

} // namespace


htracer::rendering::image<double>
get_image(uint32_t width, uint32_t height, double lam)
{
  using v3 = htracer::v3<double>;

  htracer::staging::scene_with<double, sphere_t, plane_t> scene;

  scene.add_light({{-3., 6., 0.}, {1., 1., 1.}, 20});
  scene.add_light({{3., 6., 0.}, {1., 1., 1.}, 10});

  auto const floor_material = htracer::staging::make_solid<double>({0.2, 0.2, 0.2}, 0.125, 0, 200, .2);
  auto const mirror_material = htracer::staging::make_mirror<double>(0, 200, 0.92);

  scene.emplace_object<sphere_t>({{-8.5, 2.0, -5.0}, 2.0}, get_sphere_material(310));
  scene.emplace_object<sphere_t>({{-1.5, 1.0, 0.0}, 1.0}, get_sphere_material(42));
  scene.emplace_object<sphere_t>({{1.5, 1.0, 0.0}, 1.0}, get_sphere_material(110));
  scene.emplace_object<sphere_t>({{6.8, 1.0, -3.0}, 1.0}, get_sphere_material(200));

  scene.emplace_object<plane_t>({{0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}}, floor_material);
  scene.emplace_object<plane_t>({{0.0, 0.0, -11.0}, {0.0, -0.2, 1.0}}, mirror_material);

  v3 const camera_begin(0, 3.7, 2.3);
  v3 const camera_move(0, 0, -2.5);

  auto const camera_pos = camera_begin + lam * camera_move;

  auto const camera_pan = std::sin(2 * std::numbers::pi_v<double> * lam) * 0.18;

  v3 const camera_dir(camera_pan, -0.9, -1.);
  v3 const camera_up(0, 1, 0);

  auto const focal_vec = v3(0.0, 1.0, 0.0) - camera_pos;
  auto const focal_distance = std::sqrt(dot(focal_vec, focal_vec));
  auto const aperture_radius = .05;

  auto const fov_rads = 60 * std::numbers::pi_v<double> / 180;

  htracer::rendering::camera const camera(camera_pos, camera_dir, camera_up, width, height, fov_rads);

  htracer::rendering::batchers::column_batcher const batcher;

  htracer::rendering::sensors::uniform_sensor<double> const sensor;
  htracer::rendering::lenses::thin_lens<double> const lens(aperture_radius, focal_distance);

  auto const renderer = htracer::rendering::make_renderer(batcher, scene, camera, sensor, lens);
  return renderer.render(htracer::rendering::par_unseq, 76);
}

} // namespace hoovy
