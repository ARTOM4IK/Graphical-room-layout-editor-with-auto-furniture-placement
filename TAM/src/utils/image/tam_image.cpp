/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_image.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Image store handle implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 22.01.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Load image from file function.
 * ARGUMENTS:
 *   - image file name:
 *       const std::string &FileName;
 * RETURNS:
 *   (tam::image &) self reference.
 */
tam::image & tam::image::Load( const std::string &FileName )
{
  BOOL isok = FALSE;

  if (WicFactory == nullptr)
  {
    CoInitialize(nullptr);
    CoCreateInstance(CLSID_WICImagingFactory2, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&WicFactory));
  }
  // Load through Window Image Component
  if (WicFactory != nullptr)
  {
    IWICBitmapDecoder *Decoder {};
    std::wstring FileNameW = StringToWString(FileName); // {FileName.begin(), FileName.end()};

    // Create decoder (loader)
    WicFactory->CreateDecoderFromFilename(FileNameW.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &Decoder);
    if (Decoder != nullptr)
    {
      IWICBitmapFrameDecode *Frame {};
      Decoder->GetFrame(0, &Frame);
      UINT fw = 0, fh = 0;

      if (Frame != nullptr)
      {
        Frame->GetSize(&fw, &fh);
        if (fw != 0 && fh != 0)
        {
          // Converting image format to standard - RGBA with 8-bits per pixel
          IWICFormatConverter *Convert {};
          WicFactory->CreateFormatConverter(&Convert);
          if (Convert != nullptr)
          {
            // Converter initialization
            Convert->Initialize(Frame, GUID_WICPixelFormat32bppBGRA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);

            // Size checking
            Convert->GetSize(&fw, &fh);
            Create(fw, fh);
            if (fw != 0 && fh != 0)
            {
              // Copying image pixels to container pixel array
              WICRect Rect {0, 0, static_cast<INT>(fw), static_cast<INT>(fh)};
              Convert->CopyPixels(&Rect, fw * 4, fw * fh * 4, (BYTE *)Pixels.data());
              isok = TRUE;
            }
            Convert->Release();
          }
        }
        Frame->Release();
      }
      Decoder->Release();
    }
  }
  if (!isok)
  {
    HBITMAP hBm;
    if ((hBm = (HBITMAP)LoadImage(nullptr, FileName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)) != nullptr)
    {
      // Case of BMP file
      // Create contextes
      HDC hDC = GetDC(nullptr);
      HDC hMemDC = CreateCompatibleDC(hDC);
      HDC hMemDC1 = CreateCompatibleDC(hDC);
      SelectObject(hMemDC, hBm);
      ReleaseDC(nullptr, hDC);

      BITMAP bm;
      GetObject(hBm, sizeof(bm), &bm);

      // Build DIB section
      Create(bm.bmWidth, bm.bmHeight);
      BITMAPINFOHEADER bmih {};
      bmih.biSize = sizeof(BITMAPINFOHEADER);
      bmih.biBitCount = 32;                   // BGRx - DWORD
      bmih.biPlanes = 1;                      // always
      bmih.biWidth = bm.bmWidth;              // width
      bmih.biHeight = -bm.bmHeight;           // height + first coded row is 0
      bmih.biSizeImage = bm.bmWidth * bm.bmHeight * 4;  // image size in bytes

      VOID *Bits;
      HBITMAP hImage = CreateDIBSection(nullptr, (BITMAPINFO *)&bmih, DIB_RGB_COLORS,
                          (VOID **)&Bits, nullptr, 0);
      SelectObject(hMemDC1, hImage);

      // Draw load image to DIB
      BitBlt(hMemDC1, 0, 0, Width, Height, hMemDC, 0, 0, SRCCOPY);
      DeleteDC(hMemDC);
      DeleteDC(hMemDC1);

      memcpy(&Pixels[0], Bits, bmih.biSizeImage);
      DeleteObject(hImage);

      // Make alpha channel
      for (INT i = 3; i < Width * Height * 4; i += 4)
        Pixels[i] = 255;
    }
    else
    {
      std::fstream f(FileName, std::fstream::in | std::fstream::binary);

      if (f.is_open())
      {
        // Case of G24/G32
        INT fw = 0, fh = 0;
        f.read(reinterpret_cast<CHAR *>(&fw), 2);
        f.read(reinterpret_cast<CHAR *>(&fh), 2);
        f.seekg(0, std::fstream::end);
        INT flen = f.tellg();
        if (flen == 4 + fw * fh * 3)
        {
          // G24
          f.seekg(4, std::fstream::beg);
          Create(fw, fh);
          for (INT r = 0, p = 0; r < fh; r++)
            for (INT c = 0; c < fw; c++)
            {
              BYTE rgb[3];
              f.read(reinterpret_cast<CHAR *>(rgb), 3);
              Pixels[p++] = rgb[0];
              Pixels[p++] = rgb[1];
              Pixels[p++] = rgb[2];
              Pixels[p++] = 255;
            }
        }
        else if (flen == 4 + fw * fh * 4)
        {
          // G32
          f.seekg(4, std::fstream::beg);
          Create(fw, fh);
          f.read((CHAR *)Pixels.data(), 4 * fw * fh);
        }
        f.close();
      }
    }
  }
  return *this;
} /* End of 'tam::image::Load' function */

/* Create image from memory image data function.
 * ARGUMENTS:
 *   - image data byte stream:
 *       const std::vector<BYTE> &ImageData;
 * RETURNS:
 *   (tam::image &) self reference.
 */
tam::image & tam::image::Set( const std::vector<BYTE> &ImageData )
{
  BOOL isok = FALSE;

  if (ImageData.size() < 4)
    return *this;

  if (WicFactory == nullptr)
  {
    CoInitialize(nullptr);
    CoCreateInstance(CLSID_WICImagingFactory2, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&WicFactory));
  }
  // Load through Window Image Component
  if (WicFactory != nullptr)
  {
    IWICStream *wicStream = nullptr;
    HRESULT hr = WicFactory->CreateStream(&wicStream);
    if (!FAILED(hr) && wicStream != nullptr)
    {
      hr = wicStream->InitializeFromMemory(const_cast<BYTE *>(ImageData.data()), static_cast<DWORD>(ImageData.size()));
      if (!FAILED(hr))
      {
        IWICBitmapDecoder *Decoder {};

        // Create decoder (loader)
        WicFactory->CreateDecoderFromStream(wicStream, nullptr, WICDecodeMetadataCacheOnDemand, &Decoder);
        if (Decoder != nullptr)
        {
          IWICBitmapFrameDecode *Frame {};
          Decoder->GetFrame(0, &Frame);
          UINT fw = 0, fh = 0;

          if (Frame != nullptr)
          {
            Frame->GetSize(&fw, &fh);
            if (fw != 0 && fh != 0)
            {
              // Converting image format to standard - RGBA with 8-bits per pixel
              IWICFormatConverter *Convert {};
              WicFactory->CreateFormatConverter(&Convert);
              if (Convert != nullptr)
              {
                // Converter initialization
                Convert->Initialize(Frame, GUID_WICPixelFormat32bppBGRA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);

                // Size checking
                Convert->GetSize(&fw, &fh);
                Create(fw, fh);
                if (fw != 0 && fh != 0)
                {
                  // Copying image pixels to container pixel array
                  WICRect Rect {0, 0, static_cast<INT>(fw), static_cast<INT>(fh)};
                  Convert->CopyPixels(&Rect, fw * 4, fw * fh * 4, (BYTE *)Pixels.data());
                  isok = TRUE;
                }
                Convert->Release();
              }
            }
            Frame->Release();
          }
          Decoder->Release();
        }
      }
    }
    wicStream->Release();
  }
  if (!isok)
  {
    // Case of G24/G32
    INT fw = 0, fh = 0;
    const BYTE *ptr = ImageData.data();
    fw = *reinterpret_cast<const WORD *>(ptr), ptr += 2;
    fh = *reinterpret_cast<const WORD *>(ptr), ptr += 2;
    INT flen = static_cast<INT>(ImageData.size());
    if (flen == 4 + fw * fh * 3)
    {
      // G24
      Create(fw, fh);
      for (INT r = 0, p = 0; r < fh; r++)
        for (INT c = 0; c < fw; c++)
        {
          Pixels[p++] = *ptr++;
          Pixels[p++] = *ptr++;
          Pixels[p++] = *ptr++;
          Pixels[p++] = 255;
        }
    }
    else if (flen == 4 + fw * fh * 4)
    {
      // G32
      Create(fw, fh);
      memcpy(Pixels.data(), ptr, 4 * fw * fh);
    }
  }
  return *this;
} /* End of 'tam::image::Set' function */

/* END OF 'tam_image.cpp' FILE */
