#version 150

//in vec3 in_Color;
in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
uniform mat4 matrix;

uniform mat4 bone_pos0; // Translation matrix of bone 0
uniform mat4 bone_pos1; // Translation matrix of bone 1
uniform mat4 bone_rot0; // Rotation matrix of bone 0
uniform mat4 bone_rot1;

out vec4 g_color;
const vec3 lightDir = normalize(vec3(0.3, 0.5, 1.0));

// Uppgift 3: Soft-skinning på GPU
//
// Flytta över din implementation av soft skinning från CPU-sidan
// till vertexshadern. Mer info finns på hemsidan.

void main(void)
{
	// transformera resultatet med ModelView- och Projection-matriserna
	//gl_Position = matrix * vec4(in_Position, 1.0);

	// sätt röd+grön färgkanal till vertex Weights
	vec4 color = vec4(in_TexCoord.x, in_TexCoord.y, 0.0, 1.0);
	float weight = (1.0-in_TexCoord.y); // Weight goes from 0.0 to 1.0, I think
	//vec4 color = vec4(weight, 0.0, 0.0, 1.0);

	// Do matrix stuff
	mat4 m0t_inv = inverse(bone_pos0);
	mat4 m1t_inv = inverse(bone_pos1);

	mat4 m0p = bone_pos0 * bone_rot0;
	mat4 m1p = bone_pos1 * bone_rot1;

	mat4 m0 = m0p*m0t_inv;
	mat4 m1 = m1p*m1t_inv;

	vec4 v0 = m0*vec4(in_Position, 1.0);
	vec4 v1 = m1*vec4(in_Position, 1.0);

    vec4 new_vec = vec4(0,0,0, 0.0);
    new_vec += v0*weight;
    new_vec += v1*(1.0-weight);

	gl_Position = matrix * new_vec;

	// Lägg på en enkel ljussättning på vertexarna
	float intensity = dot(in_Normal, lightDir);
	color.xyz *= intensity;

	g_color = color;
}

