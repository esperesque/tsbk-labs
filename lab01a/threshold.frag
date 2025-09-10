#version 150

in vec2 outTexCoord;
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
    float threshold = 1.0;
    vec4 samp = texture(texUnit, outTexCoord);
    if(samp.x > threshold || samp.y > threshold || samp.z > threshold){
        out_Color = samp;
    }
    else{
        out_Color = vec4(0, 0, 0, 0);
    }
}
