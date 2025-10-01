#version 150
// Simple shader for combining two textures

in vec2 outTexCoord;
uniform sampler2D texUnit;
uniform sampler2D texUnit2;
out vec4 out_Color;

void main(void)
{
    //float threshold = 1.0;
    //vec4 samp = texture(texUnit, outTexCoord);

    out_Color = texture(texUnit, outTexCoord) + texture(texUnit2, outTexCoord);
}
