#include "Color.hpp"

namespace prog
{

    // Default constructor
    Color::Color()
    {
        this->_red = 0;
        this->_green = 0;
        this->_blue = 0;
    }
    // Copy constructor
    Color::Color(const Color &other)
    {
        this->_red = other._red;
        this->_green = other._green;
        this->_blue = other._blue;
    }
    // Constructor using supplied (r, g, b) values
    Color::Color(rgb_value red, rgb_value green, rgb_value blue)
    {
        this->_red = red;
        this->_green = green;
        this->_blue = blue;
    }

    // Getters
    // Get values for individual RGB color channels
    rgb_value Color::red() const
    {
        return _red;
    }
    rgb_value Color::green() const
    {
        return _green;
    }
    rgb_value Color::blue() const
    {
        return _blue;
    }

    // Get (mutable) references for individual RGB color channels.
    rgb_value &Color::red()
    {
        return _red;
    }
    rgb_value &Color::green()
    {
        return _green;
    }
    rgb_value &Color::blue()
    {
        return _blue;
    }
}
