
#ifndef HSV2RGB_H_INCLUDED
#define HSV2RGB_H_INCLUDED

#include <math.h>
#include <cstdint>
struct hsv_t
{
    float H;
    float S;
    float V;
};

struct rgb_t
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
};

void HSVtoRGB(hsv_t in,rgb_t* out);

#endif // HSV2RGB_H_INCLUDED
