#version 150

// Simplified Phong: No materials, only one, hard coded light source
// (in view coordinates) and no ambient

// Note: Simplified! In particular, the light source is given in view
// coordinates, which means that it will follow the camera.
// You usually give light sources in world coordinates.

// convolution implementation from https://www.ozone3d.net/tutorials/image_filtering_p2.php

#define KERNEL_SIZE 25

// 1 4 6 4 1
// 4 16 24 16 4
// 6 24 36 24 6
// 4 16 24 16 4
// 1 4 6 4 1

const float kernel[KERNEL_SIZE] = float[](1, 4, 6, 4, 1,
4, 16, 24, 16, 4,
6, 24, 36, 24, 6,
4, 16, 24, 16, 4,
1, 4, 6, 4, 1);

const vec2 offs[KERNEL_SIZE] = vec2[]( vec2(-2, -2), vec2(-1, -2), vec2(0, -2), vec2(1, -2), vec2(2, -2),
vec2(-2, -1), vec2(-1, -1), vec2(0, -1), vec2(1, -1), vec2(2, -1),
vec2(-2, 0), vec2(-1, 0), vec2(0, 0), vec2(1, 0), vec2(2, 0),
vec2(-2, 1), vec2(-1, 1), vec2(0, 1), vec2(1, 1), vec2(2, 1),
vec2(-2, 2), vec2(-1, 2), vec2(0, 2), vec2(1, 2), vec2(2, 2));

in vec2 outTexCoord;
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
    float tx = 1.0 / textureSize(texUnit, 0)[0];

    vec4 sum = vec4(0,0,0,0);
    int i = 0;
    for(i = 0; i < KERNEL_SIZE; i++){
        vec4 tmp = texture2D(texUnit, outTexCoord+(offs[i]*tx));
        sum += tmp * (kernel[i] / 256.0);
    }

    out_Color = sum;
}
