#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"

using namespace std;

namespace prog
{
    // Use to read color values from a script file.
    istream &operator>>(istream &input, Color &c)
    {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    Script::Script(const string &filename) : image(nullptr), input(filename)
    {
    }
    void Script::clear_image_if_any()
    {
        if (image != nullptr)
        {
            delete image;
            image = nullptr;
        }
    }
    Script::~Script()
    {
        clear_image_if_any();
    }

    void Script::run()
    {
        string command;
        while (input >> command)
        {
            cout << "Executing command '" << command << "' ..." << endl;
            if (command == "open")
            {
                open();
                continue;
            }
            if (command == "blank")
            {
                blank();
                continue;
            }
            if (command == "save")
            {
                save();
                continue;
            }
            if (command == "invert")
            {
                invert();
                continue;
            }
            if (command == "to_gray_scale")
            {
                to_gray_scale();
                continue;
            }
            if (command == "replace")
            {
                replace();
                continue;
            }
            if (command == "fill")
            {
                fill();
                continue;
            }
            if (command == "h_mirror")
            {
                h_mirror();
                continue;
            }
            if (command == "v_mirror")
            {
                v_mirror();
                continue;
            }
            if (command == "add")
            {
                add();
                continue;
            }
            if (command == "crop")
            {
                crop();
                continue;
            }
            if (command == "rotate_left")
            {
                rotate_left();
                continue;
            }
            if (command == "rotate_right")
            {
                rotate_right();
                continue;
            }
            if (command == "median_filter")
            {
                median_filter();
                continue;
            }
            if (command == "xpm2_open")
            {
                xpm2_open();
                continue;
            }
            if (command == "xpm2_save")
            {
                xpm2_save();
                continue;
            }
        }
    }
    void Script::open()
    {
        // Replace current image (if any) with image read from PNG file.
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }
    void Script::blank()
    {
        // Replace current image (if any) with blank image.
        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }
    void Script::save()
    {
        // Save current image to PNG file.
        string filename;
        input >> filename;
        saveToPNG(filename, image);
    }

    // simple image manipulations
    // Transforms each individual pixel (r, g, b) to (255-r, 255-g, 255-b)
    void Script::invert()
    {
        for (int i = 0; i < image->width(); i++)
        {
            for (int j = 0; j < image->height(); j++)
            {
                Color _inverted = image->at(i, j);
                _inverted.red() = 255 - _inverted.red();
                _inverted.green() = 255 - _inverted.green();
                _inverted.blue() = 255 - _inverted.blue();
                image->at(i, j) = _inverted;
            }
        }
    }
    // Transforms each individual pixel (r, g, b) to (v, v, v) where v = (r + g + b)/3
    void Script::to_gray_scale()
    {
        for (int i = 0; i < image->width(); i++)
        {
            for (int j = 0; j < image->height(); j++)
            {
                rgb_value r = image->at(i, j).red();
                rgb_value g = image->at(i, j).green();
                rgb_value b = image->at(i, j).blue();
                rgb_value gray = (r + g + b) / 3;

                image->at(i, j).red() = gray;
                image->at(i, j).green() = gray;
                image->at(i, j).blue() = gray;
            }
        }
    }
    // Replaces all rgb1 pixels by given rgb2
    void Script::replace()
    {
        // rgb_value r1, g1, b1, r2, g2, b2;
        int r1, g1, b1, r2, g2, b2;
        input >> r1 >> g1 >> b1 >> r2 >> g2 >> b2;

        for (int i = 0; i < image->width(); i++)
        {
            for (int j = 0; j < image->height(); j++)
            {
                if ((image->at(i, j).red() == r1) && (image->at(i, j).green() == g1) && (image->at(i, j).blue() == b1))
                {
                    image->at(i, j).red() = r2;
                    image->at(i, j).green() = g2;
                    image->at(i, j).blue() = b2;
                }
            }
        }
    }
    // Fill the rectangle defined by top-left corner (x, y), width w, and height h with the given rgb color
    void Script::fill()
    {
        int x, y, w, h;
        // rgb_value r, g, b;
        int r, g, b;
        input >> x >> y >> w >> h >> r >> g >> b;

        for (int i = x; i < x + w; i++)
        {
            for (int j = y; j < y + h; j++)
            {
                image->at(i, j).red() = r;
                image->at(i, j).green() = g;
                image->at(i, j).blue() = b;
            }
        }
    }
    // Mirror image horizontally
    void Script::h_mirror()
    {
        // store the data from the current image
        int w = image->width(), h = image->height();
        Color **_mirror = new Color *[w];
        for (int i = 0; i < w; i++)
        {
            _mirror[i] = new Color[h];
            for (int j = 0; j < h; j++)
            {
                _mirror[i][j].red() = image->at(i, j).red();
                _mirror[i][j].green() = image->at(i, j).green();
                _mirror[i][j].blue() = image->at(i, j).blue();
            }
        }

        // mirror image horizontally
        for (int i = 0; i < image->width(); i++)
        {
            for (int j = 0; j < image->height(); j++)
            {
                image->at(i, j).red() = _mirror[image->width() - 1 - i][j].red();
                image->at(i, j).green() = _mirror[image->width() - 1 - i][j].green();
                image->at(i, j).blue() = _mirror[image->width() - 1 - i][j].blue();
            }
        }

        for (int i = 0; i < w; i++)
            delete[] _mirror[i];
        delete[] _mirror;
    }
    // Mirror image vertically
    void Script::v_mirror()
    {
        // store the data from the current image
        int w = image->width(), h = image->height();
        Color **_mirror = new Color *[w];
        for (int i = 0; i < w; i++)
        {
            _mirror[i] = new Color[h];
            for (int j = 0; j < h; j++)
            {
                _mirror[i][j].red() = image->at(i, j).red();
                _mirror[i][j].green() = image->at(i, j).green();
                _mirror[i][j].blue() = image->at(i, j).blue();
            }
        }

        // mirror image vertically
        for (int i = 0; i < image->width(); i++)
        {
            for (int j = 0; j < image->height(); j++)
            {
                image->at(i, j).red() = _mirror[i][image->height() - 1 - j].red();
                image->at(i, j).green() = _mirror[i][image->height() - 1 - j].green();
                image->at(i, j).blue() = _mirror[i][image->height() - 1 - j].blue();
            }
        }

        for (int i = 0; i < w; i++)
            delete[] _mirror[i];
        delete[] _mirror;
    }
    /*
        Copy all pixels from image stored in PNG file filename, except pixels in that image with “neutral” color
        to the rectangle of the current image with top-left corner (x, y) of the current image
     */
    void Script::add()
    {
        // store the data from the current image
        int w = image->width(), h = image->height();
        Color **_add = new Color *[w];
        for (int i = 0; i < w; i++)
        {
            _add[i] = new Color[h];
            for (int j = 0; j < h; j++)
            {
                _add[i][j].red() = image->at(i, j).red();
                _add[i][j].green() = image->at(i, j).green();
                _add[i][j].blue() = image->at(i, j).blue();
            }
        }

        open(); // replace current image with stored image to be added

        int neutral_r, neutral_g, neutral_b;
        int x, y;
        input >> neutral_r >> neutral_g >> neutral_b >> x >> y;

        for (int i = x; i < x + image->width(); i++)
        {
            for (int j = y; j < y + image->height(); j++)
            {
                if (image->at(i - x, j - y).red() != neutral_r ||
                    image->at(i - x, j - y).green() != neutral_g ||
                    image->at(i - x, j - y).blue() != neutral_b)
                {
                    _add[i][j].red() = image->at(i - x, j - y).red();
                    _add[i][j].green() = image->at(i - x, j - y).green();
                    _add[i][j].blue() = image->at(i - x, j - y).blue();
                }
            }
        }

        // creat new image and fill with added pixels
        clear_image_if_any();
        image = new Image(w, h);
        for (int i = 0; i < w; i++)
        {
            for (int j = 0; j < h; j++)
            {
                image->at(i, j).red() = _add[i][j].red();
                image->at(i, j).green() = _add[i][j].green();
                image->at(i, j).blue() = _add[i][j].blue();
            }
        }

        for (int i = 0; i < w; i++)
            delete[] _add[i];
        delete[] _add;
    }

    // dimension-changing operations
    // Crop the image, reducing it to all pixels contained in the rectangle defined by top-left corner (x, y), width w, and height h
    void Script::crop()
    {
        int x, y, w, h;
        input >> x >> y >> w >> h;
        Color **_crop = new Color *[w];

        for (int i = 0; i < w; i++)
        {
            _crop[i] = new Color[h];
            for (int j = 0; j < h; j++)
            {
                _crop[i][j].red() = image->at(i + x, j + y).red();
                _crop[i][j].green() = image->at(i + x, j + y).green();
                _crop[i][j].blue() = image->at(i + x, j + y).blue();
            }
        }

        clear_image_if_any();
        image = new Image(w, h);
        for (int i = 0; i < w; i++)
        {
            for (int j = 0; j < h; j++)
            {
                image->at(i, j).red() = _crop[i][j].red();
                image->at(i, j).green() = _crop[i][j].green();
                image->at(i, j).blue() = _crop[i][j].blue();
            }
        }
        for (int i = 0; i < w; i++)
            delete[] _crop[i];
        delete[] _crop;
    }
    // Rotate image left by 90 degrees
    void Script::rotate_left()
    {
        rotate(LEFT);
    }
    // Rotate image right by 90 degrees
    void Script::rotate_right()
    {
        rotate(RIGHT);
    }
    // Rotate image left or right by 90 degrees, according to given direction
    void Script::rotate(int direction)
    {
        int w, h;
        w = image->height();
        h = image->width();
        Color **_rotate = new Color *[w];

        if (direction == LEFT) // Rotate image left by 90 degrees
        {
            for (int i = 0; i < w; i++)
            {
                _rotate[i] = new Color[h];
                for (int j = h - 1; j >= 0; j--)
                {
                    _rotate[i][j].red() = image->at(h - 1 - j, i).red();
                    _rotate[i][j].green() = image->at(h - 1 - j, i).green();
                    _rotate[i][j].blue() = image->at(h - 1 - j, i).blue();
                }
            }
        }
        else // Rotate image right by 90 degrees
        {
            for (int i = w - 1; i >= 0; i--)
            {
                _rotate[i] = new Color[h];
                for (int j = 0; j < h; j++)
                {
                    _rotate[i][j].red() = image->at(j, w - 1 - i).red();
                    _rotate[i][j].green() = image->at(j, w - 1 - i).green();
                    _rotate[i][j].blue() = image->at(j, w - 1 - i).blue();
                }
            }
        }

        clear_image_if_any();
        image = new Image(w, h);
        for (int i = 0; i < w; i++)
        {
            for (int j = 0; j < h; j++)
            {
                image->at(i, j).red() = _rotate[i][j].red();
                image->at(i, j).green() = _rotate[i][j].green();
                image->at(i, j).blue() = _rotate[i][j].blue();
            }
        }
        for (int i = 0; i < w; i++)
            delete[] _rotate[i];
        delete[] _rotate;
    }
    // Advanced functionality
    // Apply a median filter with window size ws >= 3 to the current image
    void Script::median_filter()
    {
        int ws;
        input >> ws;

        // store the data from the current image
        int w = image->width(), h = image->height();
        Color **_median = new Color *[w];
        for (int i = 0; i < w; i++)
        {
            _median[i] = new Color[h];
            for (int j = 0; j < h; j++)
            {
                _median[i][j].red() = image->at(i, j).red();
                _median[i][j].green() = image->at(i, j).green();
                _median[i][j].blue() = image->at(i, j).blue();
            }
        }

        for (int i = 0; i < image->width(); i++)
        {
            for (int j = 0; j < image->height(); j++)
            {
                // TODO
            }
        }

        // TODO

        for (int i = 0; i < w; i++)
            delete[] _median[i];
        delete[] _median;
    }
    // Read image stored in the XPM2 file format
    void Script::xpm2_open()
    {
        // TODO
    }
    // Saves current image in XPM2 format to filename
    void Script::xpm2_save()
    {
        // TODO
    }
}