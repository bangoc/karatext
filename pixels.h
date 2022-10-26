#ifndef PIXELS_H_
#define PIXELS_H_

#include <iostream>

static inline BLRgba32 GetPixelColor(BLImage *img, long x, long y) {
  BLRgba32 *data = (BLRgba32*)(img->_impl()->pixelData);
  return data[y * img->width() + x];
}

static inline void SetPixelColor(BLImage *img, long x, long y, BLRgba32 *color) {
  BLRgba32 *data = (BLRgba32*)(img->_impl()->pixelData);
  data[y * img->width() + x] = *color;
}

static inline void SetPixelColor(BLImage *img, long x, long y, BLRgba32 color) {
  SetPixelColor(img, x, y, &color);
}

#endif  // PIXELS_H_