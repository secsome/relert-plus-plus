#pragma once

#include <Always.h>

#include <unordered_map>
#include <vector>

class CPalFile;

class CObjectImage
{
public:
    CObjectImage(std::shared_ptr<byte> buffer, int width, int height, CPalFile* palette);
    ~CObjectImage();

private:
    void InitValidStuff();

private:
    /// <summary>
    /// Well, let me explain what's this map being used for.
    /// The inner vector is used to save different frames or facing.
    /// The outer vector is used to save different part of your object.
    /// The map's key is just an object's name.
    /// </summary>
    static std::unordered_map<CString, std::vector<std::vector<std::shared_ptr<CObjectImage>>>> Datas;

private:
    std::shared_ptr<byte> ImageBuffer; // we use a shared_ptr to manage it
    int FullWidth;
    int FullHeight;
    int ValidX;
    int ValidY;
    int ValidWidth;
    int ValidHeight;
    struct ValidRangeData
    {
        int First; 
        int Last; 
    }* ValidRanges;
    CPalFile* Palette;
};