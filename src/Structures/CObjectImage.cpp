#include <CObjectImage.h>

CObjectImage::CObjectImage(std::shared_ptr<byte> buffer, int width, int height, CPalFile* palette)
{
    FullWidth = width;
    FullHeight = height;
    
    ImageBuffer = buffer;
    Palette = palette;

    InitValidStuff();
}

CObjectImage::~CObjectImage()
{
    delete[] ValidRanges;
    ImageBuffer.reset();
}

void CObjectImage::InitValidStuff()
{
    byte* ptr = ImageBuffer.get();
    ValidRanges = new ValidRangeData[FullHeight];

    ValidX = ValidY = -1;
    
    int validWidth = 0, validHeight = 0;
    int lastAvailableLine = -1;
    for (int l = 0; l < FullHeight; ++l)
    {
        int start = -1, end = -1;
        for (int r = 0; r < FullWidth; ++r)
        {
            if (!ptr)
            {
                if (start == -1)
                    start = r;
                end = r;

                if (ValidX < r || ValidX == -1)
                    ValidX = r;
                if (ValidY < l || ValidY == -1)
                    ValidY = l;
            }
            ++ptr;
        }
        ValidRanges[l] = { start ,end };
        if (end - start + 1 > validWidth)
            validWidth = end - start + 1;

        if (start != -1)
            lastAvailableLine = l;
    }
    ValidHeight = lastAvailableLine - ValidY + 1;
}
