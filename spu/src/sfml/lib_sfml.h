#ifndef LIB_SFML_H_
#define LIB_SFML_H_

#include <SFML/Graphics.hpp>

#include "lib/utils/statuses.h"

/**
 * @brief sfml library wrapper
 */
struct sfmlWindow {
    sf::RenderWindow* window = nullptr;     //< sfml window class

    sf::RectangleShape* pixels = nullptr;   //< array of "pixels" (rectangles)
    unsigned int width = 0;                 //< number of "pixels" in horizontal orientation
    unsigned int height = 0;                //< number of "pixels" in vertical orientation

    /**
     * @brief sfmlWindow constructor
     *
     * @param pixel_width width of one "pixel" (rectangle)
     * @param pixel_height height of one "pixel" (rectangle)
     * @param hor_num number of "pixels" in horizontal orientation
     * @param ver_num number of "pixels" in vertical orientation
     * @param header window header
     * @return true
     * @return false
     */
    bool ctor(const unsigned int pixel_width, const unsigned int pixel_height,
              const unsigned int hor_num,     const unsigned int ver_num,
              const char* header);
    void dtor();

    /**
     * @brief Sets the "pixel" color
     *
     * @param x coordinate
     * @param y coordinate
     * @param color
     */
    void set_pixel_color(const size_t x, const size_t y, const uint32_t color);

    /**
     * @brief Sets the "pixel" color
     *
     * @param i number of "pixel" in array
     * @param color
     */
    void set_pixel_color(const size_t i, const uint32_t color);

    /**
     * @brief Renew window (update all pixels)
     */
    void renew();

    /**
     * @brief Tells if window close button was pushed
     *
     * @return true
     * @return false
     */
    bool is_closed();

    /**
     * @brief Sets framerate limit
     *
     * @param fps
     */
    void set_fps(const unsigned int fps);
};

#endif //< #ifndef LIB_SFML_H_
