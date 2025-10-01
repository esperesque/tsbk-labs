// SpriteLight - Heavily simplified sprite engine
// by Ingemar Ragnemalm 2009

// What does a mogwai say when it sees a can of soda?
// Eeek! Sprite light! Sprite light!

#ifndef SPRITELIGHT_H
#define SPRITELIGHT_H

#include "LoadTGA.h"
#include "VectorUtils4.h"

typedef struct SpriteRec
{
	vec3 position; // OBS! Really 2D, declared as vec3 to make functions like normalizations compatible.
	TextureData *face;
	vec3 speed;
	GLfloat rotation;
	struct SpriteRec *next;
	int type; // 0 = sheep, 1 = black sheep

	// Add custom sprite data here as needed
	int group;
	vec3 nearest_pos; // position of nearest other boid
	vec3 dir = vec3(0,0,0); // last calculated direction vector
	vec3 avg_dir; // average direction of boids in the same group
	vec3 avoid_vec = vec3(0,0,0); // wants to move away from this position
	vec3 bias = vec3(1.0,1.0,1.0);
	float food_timer = 1000.0;
	vec3 food_vec = vec3(0,0,0); // Vector to food
} SpriteRec, *SpritePtr;

// Globals: The sprite list, background texture and viewport dimensions (virtual or real pixels)
extern SpritePtr gSpriteRoot;
extern GLuint backgroundTexID;
extern long gWidth, gHeight;

// Functions
TextureData *GetFace(const char *fileName);
struct SpriteRec *NewSprite(TextureData *f, GLfloat h, GLfloat v, GLfloat hs, GLfloat vs, int type);
void HandleSprite(SpritePtr sp);
void DrawSprite(SpritePtr sp);
void DrawBackground();

void InitSpriteLight();

#endif // SPRITELIGHT_H
