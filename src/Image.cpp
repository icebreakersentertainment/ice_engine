#include <FreeImage.h>

#include "Image.hpp"

#include "exceptions/RuntimeException.hpp"

namespace ice_engine
{
namespace
{
inline void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message)
{
    std::cerr << message << std::endl;
    //throw std::runtime_error(message);
}
}

void Image::importImage(const std::vector<byte>& data, bool hasAlpha)
{
    FIMEMORY* stream = nullptr;
    FIBITMAP* bitmap = nullptr;

    try
    {
        FreeImage_SetOutputMessage(FreeImageErrorHandler);

        stream = FreeImage_OpenMemory((BYTE*)&data[0], static_cast<DWORD>(data.size() * sizeof(byte)));
        FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeFromMemory(stream, 0);
        bitmap =  FreeImage_LoadFromMemory(format, stream);

        if (bitmap == nullptr)
        {
            throw RuntimeException("Unable to load image data");
        }

        FIBITMAP* temp = bitmap;
        if ( hasAlpha )
        {
            bitmap = FreeImage_ConvertTo32Bits(bitmap);
        }
        else
        {
            bitmap = FreeImage_ConvertTo24Bits(bitmap);
        }

        FreeImage_Unload(temp);

        int w = FreeImage_GetWidth(bitmap);
        int h = FreeImage_GetHeight(bitmap);

        byte* pixels = FreeImage_GetBits(bitmap);

        if ( pixels != nullptr )
        {
            int pixelSize = 3;
            if ( hasAlpha )
            {
                pixelSize = 4;
            }

            // FreeImage loads in BGR format, so you need to swap some bytes (Or use GL_BGR)
            for ( int j = 0; j < w * h; j++ )
            {
                byte swap = pixels[j * pixelSize + 2];
                pixels[j * pixelSize + 2] = pixels[j * pixelSize + 0];
                pixels[j * pixelSize + 0] = swap;
            }

            int imageBytesLength = pixelSize * w * h;

            // Transfer raw data into a vector
            data_ = std::vector<byte>(pixels, pixels+imageBytesLength);

            width_ = w;
            height_ = h;
            if (hasAlpha)
            {
                format_ = IImage::Format::FORMAT_RGBA;
            }
            else
            {
                format_ = IImage::Format::FORMAT_RGB;
            }
        }

        FreeImage_Unload(bitmap);
        FreeImage_CloseMemory(stream);
    }
    catch (const std::exception& e)
    {
        if (bitmap != nullptr)
        {
            FreeImage_Unload(bitmap);
        }
        if (stream != nullptr)
        {
            FreeImage_CloseMemory(stream);
        }

        throw e;
    }
}

}
