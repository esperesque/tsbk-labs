// Variant with SimpleGUI

#include <iostream>
#include <stdlib.h>
#include "MicroGlut.h"
#include "LoadTGA.h"
#include "SpriteLight.h"
#include "GL_utilities.h"
#define MAIN
#include "VectorUtils4.h"
#include "SimpleGUI.h"

vec3 rotate_towards(vec3 a, vec3 b, float angle);

// Add more globals as needed

// Example of user controllable parameter
float someValue = 1.0;
float move_speed = 1.0;
float turn_rate = 0.25;

float separation_range = 400; // Distance at which separation vector becomes 0

// Weights
float cohesion_w = 1.0;
float separation_w = 0.6;
float alignment_w = 0.7;

vec3 p0 = vec3(0, 0, 0);
vec3 p1 = vec3(800, 600, 0);

void SpriteBehavior() // Your code!
{
// Add your lab code here. You may edit anywhere you want, but most of it goes here.
// You can start from the global list gSpriteRoot.
}

// Drawing routine
void Display()
{
	SpritePtr sp;

	glClearColor(0, 0, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//DrawBackground();

	SpriteBehavior(); // Your code

// Loop though all sprites. (Several loops in real engine.)
    vec3 avg_pos = vec3(0,0,0);
    int g0_count = 0;
    int g1_count = 0;
    vec3 g1_avg_pos = vec3(0,0,0);
    vec3 g0_avg_pos = vec3(0,0,0);
    vec3 g0_avg_dir = vec3(0,0,0);
    vec3 g1_avg_dir = vec3(0,0,0);
    int sprite_count = 0;

    // FIRST LOOP
	sp = gSpriteRoot;
	do
	{
	    sprite_count++;
	    // Boids are assigned to one of two groups depending on whether they are nearer to p0 or p1

	    // Step 1: Cohesion
	    // Figure out the average position of all boids

	    /*
	    vec3 d0 = p0 - sp->position;
	    vec3 d1 = p1 - sp->position;
	    float l0 = sqrt(pow(d0.x, 2) + pow(d0.y, 2));
	    float l1 = sqrt(pow(d1.x, 2) + pow(d1.y, 2));


	    if(l0 < l1){
            g0_count++;
            sp->group = 0;
            g0_avg_pos += sp->position;
            g0_avg_dir += sp->dir;
	    }
	    else{
	        g1_count++;
            sp->group = 1;
            g1_avg_pos += sp->position;
            g1_avg_dir += sp->dir;
	    }
	    */

	    if(sp->group == 0){
            g0_count++;
            g0_avg_pos += sp->position;
            g0_avg_dir += sp->dir;
	    }
	    else{
            g1_count++;
            g1_avg_pos += sp->position;
            g1_avg_dir += sp->dir;
	    }

	    avg_pos += sp->position;

	    // Step 2: Separation + Step 3: Alignment

	    sp->avoid_vec = vec3(0,0,0);

	    // Version 2: Each other boid has a weighted influence depending on how close it is
	    // No, you take the average of all their positions and then weight the separation to closeness to that position
	    SpritePtr sp_other = gSpriteRoot;
	    do{
            if(sp == sp_other){
                sp_other = sp_other->next;
                continue;
            }

            vec3 d_vec = sp_other->position - sp->position;
            float distance = sqrt(pow(d_vec.x, 2) + pow(d_vec.y, 2));
            float influence = 1.0 - (distance / separation_range);
            influence = std::max(influence, 0.0f);

            sp->avoid_vec -= normalize(d_vec)*influence;


            sp_other = sp_other->next;
	    } while(sp_other != NULL);

	    // Old ver
	    /*
	    float closest_d = 9999;
	    SpritePtr sp2 = gSpriteRoot;
	    vec3 avg_dir = vec3(0,0,0);
	    int group_count = 0;
	    do{
            if(sp != sp2){
                vec3 d = sp2->position - sp->position;
                float l = sqrt(pow(d.x, 2) + pow(d.y, 2));
                if(l < closest_d){
                    closest_d = l;
                    sp->nearest_pos = sp2->position;
                }

                // Alignment
                if(sp->group == sp2->group){
                    avg_dir += sp2->dir;
                    group_count++;
                }
            }
            sp2 = sp2->next;
	    } while (sp2 != NULL);*/



	    // Step 3: Alignment
	    /*
        if(group_count > 0){
            avg_dir = avg_dir / float(group_count);
            sp->avg_dir = normalize(avg_dir); // normalize?
	    }
	    else{
            sp->avg_dir = vec3(0,0,0);
	    }*/

		sp = sp->next;
	} while (sp != NULL);

    avg_pos = avg_pos / float(sprite_count);
    if(g0_count != 0) g0_avg_pos = g0_avg_pos / float(g0_count);
    if(g1_count != 0) g1_avg_pos = g1_avg_pos / float(g1_count);

    g0_avg_dir = normalize(g0_avg_dir);
    g1_avg_dir = normalize(g1_avg_dir);

    // SECOND LOOP

    sp = gSpriteRoot;

    do
	{
		// Your code
		// Example affecting sprites by a controllable parameter
		vec3 co_dir;
		vec3 se_dir = vec3(0,0,0);
		vec3 al_dir = vec3(0,0,0);

		// Calculate cohesion vector
		if(sp->group == 0){
            co_dir = g0_avg_pos - sp->position;
            al_dir = g0_avg_dir;
		}
		else{
            co_dir = g1_avg_pos - sp->position;
            al_dir = g1_avg_dir;
		}
		co_dir = normalize(co_dir);

		// Calculate separation vector
		se_dir = sp->nearest_pos - sp->position;
		float se_len = sqrt(pow(se_dir.x, 2) + pow(se_dir.y, 2));
		se_dir = -normalize(se_dir);

		// Lerp strength of separation vector
		float strength = 1.0 - (se_len / separation_range);
		if(strength < 0) strength = 0;

		// Calculate alignment
		//al_dir = sp->avg_dir;

		se_dir = sp->avoid_vec;

		vec3 dir = co_dir*cohesion_w + se_dir*separation_w + al_dir*alignment_w;
		//if(se_len < 100){
        //    dir += se_dir*separation_w;
		//}
		//vec3 dir = avg_pos - sp->position;

		// Smooth turning
		vec3 prev_dir = sp->dir;
		vec3 new_dir = dir;

		dir = rotate_towards(prev_dir, new_dir, turn_rate);
		// Rotate prev_dir just a little toward new_dir

		sp->speed = normalize(dir) * move_speed;
		sp->dir = normalize(dir);

		HandleSprite(sp); // Default movement my speed. Callback in a real engine
		DrawSprite(sp);
		sp = sp->next;
	} while (sp != NULL);

    sgDraw();
	glutSwapBuffers();
}

vec3 rotate_towards(vec3 a, vec3 b, float angle){
    a = normalize(a);
    b = normalize(b);
    if(DotProduct(a,b) == 0){
        return(a); // Parallel vectors
    }
    float theta = acos(DotProduct(a,b));
    vec3 ax = CrossProduct(a, b);
    vec3 rot_a = a*cos(angle) + CrossProduct(ax,a)*sin(angle) + DotProduct(ax,DotProduct(ax,a))*(1-cos(angle));

    return(rot_a);
}

void Reshape(int h, int v)
{
	glViewport(0, 0, h, v);
	gWidth = h;
	gHeight = v;
}

void Key(unsigned char key,
         __attribute__((unused)) int x,
         __attribute__((unused)) int y)
{
  switch (key)
  {
    case '+':
    	someValue += 0.1;
    	printf("someValue = %f\n", someValue);
    	break;
    case '-':
    	if (someValue > 0.5) someValue -= 0.1;
    	printf("someValue = %f\n", someValue);
    	break;
    case 0x1b:
      exit(0);
  }
}

void Init()
{
	TextureData *sheepFace, *blackieFace, *dogFace, *foodFace;

	LoadTGATextureSimple("bilder/leaves.tga", &backgroundTexID); // Background

	sheepFace = GetFace("bilder/sheep.tga"); // A sheep
	blackieFace = GetFace("bilder/blackie.tga"); // A black sheep
	dogFace = GetFace("bilder/dog.tga"); // A dog
	foodFace = GetFace("bilder/mat.tga"); // Food

	NewSprite(sheepFace, 100, 200, 1, 1);
	NewSprite(sheepFace, 200, 100, 1.5, -1);
	NewSprite(sheepFace, 250, 200, -1, 1.5);
	NewSprite(sheepFace, 400, 400, -1, 1.5);
	NewSprite(dogFace, 0, 300, 1, 1);
	NewSprite(dogFace, 600, 400, 0.1, -1);
	NewSprite(dogFace, 600, 480, 0.1, -1);
	NewSprite(dogFace, 700, 50, 0.1, -1);

	// Determine groups
    SpritePtr sp = gSpriteRoot;
    do{
        int g = rand() % 1;
        sp->group = g;
        sp = sp->next;
    }
    while(sp != NULL);

	sgCreateStaticString(20, 40, "Slider and float display");
	sgCreateSlider(-1, -1, 200, &cohesion_w, 0.1, 2.0);
	sgCreateDisplayFloat(-1, -1, "Cohesion: ", &cohesion_w);
    sgCreateSlider(-1, -1, 200, &separation_w, 0.1, 2.0);
	sgCreateDisplayFloat(-1, -1, "Separation: ", &separation_w);
	sgCreateSlider(-1, -1, 200, &alignment_w, 0.1, 2.0);
	sgCreateDisplayFloat(-1, -1, "Alignment: ", &alignment_w);

	// Always fix the colors if it looks bad.
        sgSetFrameColor(0,0,0);
        sgSetBackgroundColor(1, 1, 1, 0.5);
        sgSetTextColor(0, 0, 0);
}

void mouse(int button, int state, int x, int y)
{
	sgMouse(state, x, y);
	glutPostRedisplay();
}

void drag(int x, int y)
{
	sgMouseDrag(x, y);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 2);
	glutCreateWindow("Lab 4 Flocking");

	glutDisplayFunc(Display);
	glutRepeatingTimer(20); // Should match the screen synch
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);

	InitSpriteLight();
	Init();

	glutMainLoop();
	return 0;
}
