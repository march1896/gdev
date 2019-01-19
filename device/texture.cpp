#include "texture.h"
#include "bitmap_image.h"

namespace Device {
    void saveAsBmp(std::string const& filename, Surface<Vec3f> colorTarget)
    {
        U32 width = colorTarget.getWidth();
        U32 height = colorTarget.getHeight();

        bitmap_image image(width, height);
        for (U32 x = 0; x < width; x ++)
        {
            for (U32 y = 0; y < height; y ++)
            {
                U32 const screen_x = x;
                U32 const screen_y = height - y - 1;

                Vec3f const& color = colorTarget.getPixel(x, y);
                image.set_pixel(screen_x, screen_y,
                    color.x * 255, color.y * 255, color.z * 255);
            }
        }

        image.save_image(filename.c_str());
    }

    void saveAsBmp(std::string const& filename, Surface<float> depthTarget)
    {
        U32 width = depthTarget.getWidth();
        U32 height = depthTarget.getHeight();

        bitmap_image image(width, height);
        for (U32 x = 0; x < width; x ++)
        {
            for (U32 y = 0; y < height; y ++)
            {
                U32 const screen_x = x;
                U32 const screen_y = height - y - 1;

                float depth = depthTarget.getPixel(x, y);
                U8 gray = depth * 256;
                image.set_pixel(screen_x, screen_y, gray, gray, gray);
            }
        }

        image.save_image(filename.c_str());
    }
}
