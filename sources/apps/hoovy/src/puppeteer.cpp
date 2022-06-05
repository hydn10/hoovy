#include <puppeteer.hpp>

#include <htracer/geometries/plane.hpp>
#include <htracer/geometries/sphere.hpp>
#include <htracer/raytracing/camera.hpp>

#include <numbers>


namespace hoovy
{

namespace
{

using Float = double;

template<typename Float>
using sphere_t = htracer::geometries::sphere<Float>;
template<typename Float>
using plane_t = htracer::geometries::plane<Float>;


constexpr htracer::scene::material<Float>
get_sphere_material(Float hue)
{
  using htracer::colors::hsl;

  const auto color = hsl<Float>{hue, 0.7, 0.5}.to_srgb().to_linear();
  return htracer::scene::make_solid<Float>(color, 0.125, 0.05, 200, .4);
}

} // namespace


htracer::raytracing::image<double>
get_image(uint32_t width, uint32_t height, double lam)
{
  using v3 = htracer::v3<Float>;

  htracer::scene::scene<Float, sphere_t, plane_t> scene;

  scene.add_light({{-3., 6., 0.}, {1., 1., 1.}, 20});
  scene.add_light({{3., 6., 0.}, {1., 1., 1.}, 10});

  auto const floor_material = htracer::scene::make_solid<Float>({0.2, 0.2, 0.2}, 0.125, 0, 200, .2);
  auto const mirror_material = htracer::scene::make_mirror<Float>(0, 200, 0.92);

  scene.emplace_object<sphere_t>({{-8.5, 2.0, -5.0}, 2.0}, get_sphere_material(310));
  scene.emplace_object<sphere_t>({{-1.5, 1.0, 0.0}, 1.0}, get_sphere_material(42));
  scene.emplace_object<sphere_t>({{1.5, 1.0, 0.0}, 1.0}, get_sphere_material(110));
  scene.emplace_object<sphere_t>({{6.8, 1.0, -3.0}, 1.0}, get_sphere_material(200));

  scene.emplace_object<plane_t>({{0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}}, floor_material);
  scene.emplace_object<plane_t>({{0.0, 0.0, -11.0}, {0.0, -0.2, 1.0}}, mirror_material);

  v3 const camera_begin(0, 2.7, 3.1);
  v3 const camera_move(0, 0, -1.5);

  auto const camera_pos = camera_begin + lam * camera_move;

  v3 const camera_dir(0, -0.4, -1.);
  v3 const camera_up(0, 1, 0);

  auto const focal_vec = v3(0.0, 1.0, 0.0) - camera_pos;
  auto const focal_distance = std::sqrt(dot(focal_vec, focal_vec));

  // htracer::v3<Float> const camera_pos(8, 4, 0.4);
  // htracer::v3<Float> const camera_dir = htracer::v3<Float>(1.5, 1.0, -3.0) - camera_pos;
  // htracer::v3<Float> const camera_up(0, 1, 0);

  htracer::raytracing::camera<Float> const cam(
      camera_pos, camera_dir, camera_up, width, height, 60 * std::numbers::pi_v<Float> / 180, 0.05, focal_distance);

  auto const image = cam.render(std::execution::par_unseq, htracer::scene::scene_view(scene), 40);

  return image;
}

} // namespace hoovy