#version 150

in vec2 outTexCoord;
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
    float sample_distance = 0.003; // TODO: Figure out how to make this exactly one texel
    vec2 offsets[9];
    offsets[0] = vec2(-1,0);
    offsets[1] = normalize(vec2(-1,1));
    offsets[2] = vec2(0,1);
    offsets[3] = normalize(vec2(1,1));
    offsets[4] = vec2(1,0);
    offsets[5] = normalize(vec2(1,-1));
    offsets[6] = vec2(0,-1);
    offsets[7] = normalize(vec2(-1,-1));
    offsets[8] = vec2(0,0);

    vec4 clr = vec4(0.0, 0.0, 0.0, 0.0); //texture(texUnit, outTexCoord) / 9;
    vec2 sample_loc = outTexCoord;
    for(int i = 0; i < 9; i++){
        sample_loc += offsets[i]*sample_distance;
        clr += texture(texUnit, sample_loc) / 9;
    }

    //out_Color = vec4(1.0, 1.0, 0.0, 1.0);
    out_Color = clr; //texture(texUnit, outTexCoord);
}
