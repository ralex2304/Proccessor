#include <SFML/Graphics.hpp>

#include "lib_sfml.h"

bool sfmlWindow::ctor(const unsigned int pixel_width, const unsigned int pixel_height,
                      const unsigned int hor_num,     const unsigned int ver_num,
                      const char* header) {
    assert(pixel_width > 0);
    assert(pixel_height > 0);
    assert(hor_num > 0);
    assert(ver_num > 0);

    width = hor_num;
    height = ver_num;

    window = new(std::nothrow) sf::RenderWindow(sf::VideoMode(pixel_width * hor_num, pixel_height * ver_num), header);
    window->setPosition(sf::Vector2i(25, 25));


    pixels = new(std::nothrow) sf::RectangleShape[hor_num * ver_num];

    if (window == nullptr || pixels == nullptr)
        return false;

    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            pixels[i * width + j] = sf::RectangleShape(sf::Vector2f((float)pixel_width, (float)pixel_height));
            pixels[i * width + j].setPosition((float)(j * pixel_width), (float)(i * pixel_height));
            pixels[i * width + j].setFillColor(sf::Color(0, 0, 0));
        }
    }

    renew();

    return true;
}

void sfmlWindow::set_pixel_color(const size_t x, const size_t y, const uint32_t color) {
    assert(x < width);
    assert(y < height);

    pixels[x * width + y].setFillColor(sf::Color(((unsigned char) color >> 16),
                                                 ((unsigned char) color >> 8),
                                                 ((unsigned char) color >> 0)));
}

void sfmlWindow::set_pixel_color(const size_t i, const uint32_t color) {
    assert(i < height * width);

    pixels[i].setFillColor(sf::Color((unsigned char)(color >> 16),
                                     (unsigned char)(color >> 8),
                                     (unsigned char)(color >> 0)));
}

void sfmlWindow::renew() {
    window->clear();
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            window->draw(pixels[i * width + j]);
        }
    }
    window->display();
}

void sfmlWindow::dtor() {
    delete [] pixels;
    delete window;
}

bool sfmlWindow::is_closed() {
    sf::Event event;
    if (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
            return true;
        }
    }
    return false;
}
