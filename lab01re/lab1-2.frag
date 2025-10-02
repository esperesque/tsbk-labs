#version 150
// bump mapping should be calculated
// 1) in view coordinates
// 2) in texture coordinates

in vec2 outTexCoord;
in vec3 out_Normal;
in vec3 Ps; // tangentvektor
in vec3 Pt; // bitangent
in vec3 pixPos;  // Needed for specular reflections
uniform sampler2D bumpMap;
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
    vec3 light = vec3(0.0, 0.7, 0.7); // Light source in view coordinates
	float offset = 1.0 / 256.0; // texture size, same in both directions
	vec3 normal = normalize(out_Normal);

	// 2a, bump mapping in view coordinates


    float bs = texture(bumpMap, outTexCoord - vec2(1, 0) * offset).r - texture(bumpMap, outTexCoord).r;
	float bt = texture(bumpMap, outTexCoord - vec2(0, 1) * offset).r - texture(bumpMap, outTexCoord).r;

	vec3 np = normal + 2*bs * Ps + 2*bt, Pt;
	np = normalize(np);

	// Simplified lighting calculation.
	// A full solution would include material, ambient, specular, light sources, multiply by texture.
    out_Color = texture(texUnit, outTexCoord) * vec4( max(0.0, dot(np, light))+0.2).r;


    // 2b
    /*
    mat3 mvt = transpose(mat3(Ps, Pt, normal));

	vec3 lp = mvt * light;
	lp = normalize(lp);

	float bs = texture(bumpMap, outTexCoord - vec2(1, 0) * offset).r - texture(bumpMap, outTexCoord).r;
	float bt = texture(bumpMap, outTexCoord - vec2(0, 1) * offset).r - texture(bumpMap, outTexCoord).r;

	vec3 np = vec3(2*bs, 2*bt, 1);
	np = normalize(np);

	// Simplified lighting calculation.
	// A full solution would include material, ambient, specular, light sources, multiply by texture.
	//out_Color = vec4( max(0.0, dot(np, lp))+0.2).r;


    out_Color = texture(texUnit, outTexCoord) * vec4( max(0.0, dot(np, lp))+0.2).r;
    */
}
