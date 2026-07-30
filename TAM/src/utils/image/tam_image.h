/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_image.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Image handle declaration module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 29.01.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_image_h_
#define __tam_image_h_

#include <wincodec.h>

#include "tam_def.h"

/* 'tam' namespace */
namespace tam
{
  /* Image representation class */
  class image
  {
  private:
    // Windows Imaging Component factory
    inline static IWICImagingFactory2 *WicFactory;

    // Image size in pixels
    INT Width = 0, Height = 0;
    // Image pixel data
    std::vector<BYTE> Pixels;

  public:

    // Rows access pointer by DWORD
    std::vector<DWORD *> RowsD;
    // Rows access pointer by BYTE quads
    std::vector<BYTE (*)[4]> RowsB;

    // Image size references
    const INT &W = Width, &H = Height;

    /* Class default constructor */
    image( VOID )
    {
    } /* End of 'image' function */

    /* Class from image file construtor.
     * ARGUMENTS:
     *   - image file name:
     *       const std::string &FileName;
     */
    image( const std::string &FileName )
    {
      Load(FileName);
    } /* End of 'image' function */

    /* Class from memory image data construtor.
     * ARGUMENTS:
     *   - load image data byte stream:
     *       const std::vector<BYTE> &ImageData;
     */
    image( const std::vector<BYTE> &ImageData )
    {
      Set(ImageData);
    } /* End of 'image' function */

    /* Class destructor */
    ~image( VOID )
    {
    } /* End of '~image' function */

    /*** Image handle functions ***/

    /* Create image function.
     * ARGUMENTS:
     *   - image size:
     *       INT NewW, NewH;
     * RETURNS:
     *   (image &) self reference.
     */
    image & Create( INT NewW, INT NewH )
    {
      Pixels.resize(NewW * NewH * 4);
      Width = NewW;
      Height = NewH;

      // Setup row pointers
      INT i;
      RowsD.resize(Height);
      i = 0;
      for (auto &r : RowsD)
        r = (DWORD *)&Pixels[i++ * Width * 4];
      RowsB.resize(Height);
      i = 0;
      for (auto &r : RowsB)
        r = (BYTE (*)[4])&Pixels[i++ * Width * 4];
      return *this;
    } /* End of 'Create' function */

    /* Vertical flip image function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (image &) self reference.
     */
    image & FlipY( VOID )
    {
      for (INT y = 0; y < Height / 2; y++)
      {
        for (INT x = 0; x < Width; x++)
        {
          DWORD tmp;

          COM_SWAP(RowsD[y][x], RowsD[Height - 1 - y][x], tmp);
        }
      }
      return *this;
    } /* End of 'FlipY' function */

    /* Load image from file function.
     * ARGUMENTS:
     *   - image file name:
     *       const std::string &FileName;
     * RETURNS:
     *   (image &) self reference.
     */
    image & Load( const std::string &FileName );

    /* Create image from memory image data function.
     * ARGUMENTS:
     *   - image data byte stream:
     *       const std::vector<BYTE> &ImageData;
     * RETURNS:
     *   (image &) self reference.
     */
    image & Set( const std::vector<BYTE> &ImageData );

    /***
     * Save image function group
     ***/

    /* Format specifier type */
    enum class format
    {
      eEXT, // Determine by extension
      eG24, // .G24 CGSG image type
      eG32, // .G32 CGSG image type
      eTGA, // Truevision Targe Image File Format
      eBMP, // MS-Windows Device Independed Bitmap
      eJPG, // Joint Photographics Expret Group File Interchange Format
      ePNG, // Portable Network Graphics
    };

    /* Save image to file function.
     * ARGUMENTS:
     *   - file name:
     *       const std::string &FileName;
     *   - image type:
     *       format Fmt;
     * RETURNS:
     *   (BOOL) TRUE if successful, FALSE otherwise.
     */
    BOOL Save( const std::string &FileName, format Fmt = format::eEXT );

  }; /* End of 'image' class */
} /* end of 'tam' namespace */

#endif /* __tam_image_h_ */

/* END OF 'tam_image.h' FILE */
