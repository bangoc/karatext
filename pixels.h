#ifndef PIXELS_H_
#define PIXELS_H_

static inline BLRgba32 GetPixelColor(BLImage *img, long x, long y) {
  BLImageData tmp;
  img->getData(&tmp);
  uint8_t *data = (uint8_t*)tmp.pixelData;
  long idx = (y * tmp.size.w + x) * 4;
  return *((BLRgba32*)(data + idx));
}

static inline void SetPixelColor(BLImage *img, long x, long y, BLRgba32 *color) {
  BLImageData tmp;
  img->getData(&tmp);
  uint8_t *data = (uint8_t*)tmp.pixelData;
  long idx = (y * tmp.size.w + x) * 4;
  *((BLRgba32*)(data + idx)) = *color;
}

#endif  // PIXELS_H_