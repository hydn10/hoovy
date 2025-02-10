#ifndef HOOVY_PUPPETEER_HPP
#define HOOVY_PUPPETEER_HPP


#include <htracer/rendering/image.hpp>


namespace hoovy
{

htracer::rendering::image<double>
get_image(uint32_t width, uint32_t height, double lam);

} // namespace hoovy

#endif