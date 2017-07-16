#include "tiffio.h"
#include<iostream>
using namespace std;
main()
{
    uint32 height,width;
    TIFF* tif = TIFFOpen("../../data/6040_2_2_A.tif", "r");
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);           // uint32 width;
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    cout<<height<<" "<<width;
  //  uint32 npixels=width*height;
    uint16 raster;//=(uint16 *) _TIFFmalloc(npixels *sizeof(uint32));
    //buf = _TIFFmalloc(TIFFScanlineSize(tif)*width);
    for(uint16 row;row<width;row++)
    {
    TIFFReadScanline(tif, &raster, row);
    cout<<raster<<"\n";
    }

}
