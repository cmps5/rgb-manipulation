#include "Image.hpp"

namespace prog
{
  // Create image with width w, height h, and all pixels set to color fill
  Image::Image(int w, int h, const Color &fill)
  {
    this->_width = w;
    this->_height = h;
    this->_color = new Color *[w];
    for (int i = 0; i < w; i++)
    {
      _color[i] = new Color[h];
      for (int j = 0; j < h; j++)
        _color[i][j] = fill;
    }
  }
  // Destructor
  Image::~Image()
  {
    for (int i = 0; i < this->_width; i++)
      delete[] this->_color[i];
    delete[] this->_color;
  }

  // Getters
  // Get image width
  int Image::width() const
  {
    return this->_width;
  }
  // Get image height
  int Image::height() const
  {
    return this->_height;
  }
  // Get mutable reference to the value of pixel (x, y), where 0 <= x < width() and 0 <= y < height()
  Color &Image::at(int x, int y)
  {
    return _color[x][y];
  }
  // Get read-only reference to the value of pixel (x, y).
  const Color &Image::at(int x, int y) const
  {
    return _color[x][y];
  }
}
