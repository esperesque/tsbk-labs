#version 150
// bump mapping should be calculated
// 1) in view coordinates
// 2) in texture coordinates

in vec2 outTexCoord;
in vec3 out_Normal;
in vec3 Ps;
in vec3 Pt;
in vec3 pixPos;  // Needed for specular reflections
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
    vec3 light = vec3(0.0, 0.7, 0.7); // Light source in view coordinates

	// Calculate gradients here
	float offset = 1.0 / 256.0; // texture size, same in both directions

	// 2a, bump mapping in view coordinates

    float dx = (texture(texUnit, outTexCoord).x - texture(texUnit, outTexCoord+vec2(offset,0)).x)/offset;
	float dy = (texture(texUnit, outTexCoord).y - texture(texUnit, outTexCoord+vec2(0,offset)).y)/offset;
    vec3 normal = normalize(out_Normal); //normalize(vec3(dx, dy, out_Normal.z));
    out_Color = vec4( max(0.0, dot(normal, light))+0.4) * texture(texUnit, outTexCoord);
}
