#pragma once

#include "Frames.h"

struct Animation
{
    const unsigned char *const *frames; // pointer to frame array
    uint16_t frameCount;                // number of frames
};

Animation animations[] = {
    {
        .frames = star_bitmap_allArray,
        .frameCount = star_bitmap_allArray_LEN,
    },
    {
        .frames = nyancat_bitmap_allArray,
        .frameCount = nyancat_bitmap_allArray_LEN,
    },
    {
        .frames = bango_bitmap_allArray,
        .frameCount = bango_bitmap_allArray_LEN,
    },
    {
        .frames = jake_bitmap_allArray,
        .frameCount = jake_bitmap_allArray_LEN,
    },
    {
        .frames = bmo_bitmap_allArray,
        .frameCount = bmo_bitmap_allArray_LEN,
    },
};
const uint8_t animationCount = sizeof(animations) / sizeof(animations[0]);