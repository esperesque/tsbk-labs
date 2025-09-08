#version 150

in vec2 outTexCoord;
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
    float kernel = mat3(1/9, 1/9, 1/9, 1/9, 1/9, 1/9, 1/9, 1/9, 1/9);

    vec4 clr = vec4(0.0, 0.0, 0.0, 0.0); //texture(texUnit, outTexCoord) / 9;
    vec2 sample_loc = outTexCoord;
    for(int i = 0; i < 9; i++){
        sample_loc.y += 0.03;
        clr += texture(texUnit, sample_loc) / 9;
    }

    //out_Color = vec4(1.0, 1.0, 0.0, 1.0);
    out_Color = clr; //texture(texUnit, outTexCoord);
}
