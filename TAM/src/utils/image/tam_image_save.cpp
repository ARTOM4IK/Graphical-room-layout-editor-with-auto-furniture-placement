/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_image_save.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Image store handle implementation module.
 *               Image save function group implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 29.01.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <cstring>

#include "tam.h"

/* Save image to file function.
 * ARGUMENTS:
 *   - file name:
 *       const std::string &FileName;
 *   - image type:
 *       format Fmt;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL tam::image::Save( const std::string &FileName, format Fmt )
{
  if (Width == 0 || Height == 0)
    return FALSE;

  if (Fmt == format::eEXT)
  {
    std::map<std::string, format> assoc
    {
      {".g24", format::eG24},
      {".g32", format::eG32},
      {".tga", format::eTGA},
      {".bmp", format::eBMP},
      {".dib", format::eBMP},
      {".jpg", format::eJPG},
      {".jpeg", format::eJPG},
      {".png", format::ePNG},
    };
    std::string ext = std::filesystem::path(FileName).extension().string();
    for (auto &as : assoc)
      if (_stricmp(ext.c_str(), as.first.c_str()) == 0)
      {
        Fmt = as.second;
        break;
      }
    if (Fmt == format::eEXT)
      return FALSE;
  }

  if (WicFactory == nullptr)
  {
    CoInitialize(nullptr);
    CoCreateInstance(CLSID_WICImagingFactory2, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&WicFactory));
  }
  BOOL isok = FALSE;
  // Load through Window Image Component
  if (Fmt == format::eBMP || Fmt == format::eJPG || Fmt == format::ePNG ||
      WicFactory != nullptr)
  {
    IWICBitmap *Bitmap;
    WicFactory->CreateBitmapFromMemory(Width, Height, GUID_WICPixelFormat32bppBGRA,
      Width * 4, Width * Height * 4, RowsB[0][0], &Bitmap);
    if (Bitmap != nullptr)
    {
      // Create file stream
      if (IWICStream *Stream = nullptr;
          SUCCEEDED(WicFactory->CreateStream(&Stream)) && Stream != nullptr)
      {
        if (SUCCEEDED(Stream->InitializeFromFilename(StringToWString(FileName).c_str(), GENERIC_WRITE)))
        {
          // Create encoder
          IWICBitmapEncoder *Encoder = nullptr;
          auto type =
            Fmt == format::ePNG ? GUID_ContainerFormatPng :
            Fmt == format::eJPG ? GUID_ContainerFormatJpeg :
            Fmt == format::eBMP ? GUID_ContainerFormatBmp : GUID_ContainerFormatRaw;
          if (SUCCEEDED(WicFactory->CreateEncoder(type, nullptr, &Encoder)))
          {
            if (SUCCEEDED(Encoder->Initialize(Stream, WICBitmapEncoderNoCache)))
            {
              // Create new image frame
              IPropertyBag2 *Props = nullptr;
              IWICBitmapFrameEncode *Frame = nullptr;
              if (SUCCEEDED(Encoder->CreateNewFrame(&Frame, &Props)))
              {
                if (type == GUID_ContainerFormatJpeg && Props != nullptr)
                {
                  PROPBAG2 option {0};
                  option.dwType = PROPBAG2_TYPE_DATA;
                  option.pstrName = const_cast<WCHAR *>(L"ImageQuality");
                  VARIANT value;
                  VariantInit(&value);
                  value.vt = VT_R4;
                  value.fltVal = 1;
                  Props->Write(1, &option, &value);
                  VariantClear(&value);
                }

                if (WICPixelFormatGUID pixelFormat = type == GUID_ContainerFormatJpeg ? GUID_WICPixelFormat24bppBGR : GUID_WICPixelFormat32bppBGRA;
                    SUCCEEDED(Frame->Initialize(Props)) &&
                    SUCCEEDED(Frame->SetSize(Width, Height)) &&       // Set image size and pixel format
                    SUCCEEDED(Frame->SetPixelFormat(&pixelFormat)) &&
                    SUCCEEDED(Frame->WriteSource(Bitmap, nullptr)) && // Copy pixel data from image to frame
                    SUCCEEDED(Frame->Commit()) &&
                    SUCCEEDED(Encoder->Commit()))
                  isok = TRUE;
                Frame->Release();
              }
              Props->Release();
            }
            Encoder->Release();
          }
        }
        Stream->Release();
      }
      Bitmap->Release();
    }
    return isok;
  }
  if (Fmt == format::eG24 || Fmt == format::eG32)
  {
    std::fstream f(FileName, std::fstream::out | std::fstream::binary);
    if (f.is_open())
    {
      // Case of G24/G32
      f.write(reinterpret_cast<const CHAR *>(&Width), 2);
      f.write(reinterpret_cast<const CHAR *>(&Height), 2);
      if (Fmt == format::eG24)
      {
        BYTE *ptr = Pixels.data();
        for (INT r = 0; r < Height; r++)
          for (INT c = 0; c < Width; c++, ptr += 3)
            f.write(reinterpret_cast<const CHAR *>(ptr), 3);
      }
      else
        f.write(reinterpret_cast<const CHAR *>(Pixels.data()), Width * Height * 4);
      f.close();
      return TRUE;
    }
  }
  return FALSE;
} /* End of 'tam::image::Save' function */

/* END OF 'tam_image_save.cpp' FILE */
