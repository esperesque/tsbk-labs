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

// Add more globals as needed

// Example of user controllable parameter
float someValue = 1.0;
float move_speed = 1.0;

// Weights
float cohesion_w = 1.0;
float separation_w = 0.5;
float alignment_w = 1.0;

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
    int sprite_count = 0;

    // FIRST LOOP
	sp = gSpriteRoot;
	do
	{
	    sprite_count++;
	    // Boids are assigned to one of two groups depending on whether they are nearer to p0 or p1

	    // Step 1: Cohesion
	    // Figure out the average position of all boids
	    vec3 d0 = p0 - sp->position;
	    vec3 d1 = p1 - sp->position;
	    float l0 = sqrt(pow(d0.x, 2) + pow(d0.y, 2));
	    float l1 = sqrt(pow(d1.x, 2) + pow(d1.y, 2));
        std::cout << "hi";
	    if(l0 < l1){
            g0_count++;
            sp->group = 0;
            g0_avg_pos += sp->position;
	    }
	    else{
	        g1_count++;
            sp->group = 1;
            g1_avg_pos += sp->position;
	    }

	    avg_pos += sp->position;

	    // Step 2: Separation
	    float closest_d = 9999;
	    SpritePtr sp2 = gSpriteRoot;
	    do{
            if(sp != sp2){
                vec3 d = sp2->position - sp->position;
                float l = sqrt(pow(d.x, 2) + pow(d.y, 2));
                if(l < closest_d){
                    closest_d = l;
                    sp->nearest_pos = sp2->position;
                }
            }
            sp2 = sp2->next;
	    } while (sp2 != NULL);

		sp = sp->next;
	} while (sp != NULL);

    avg_pos = avg_pos / float(sprite_count);
    if(g0_count != 0) g0_avg_pos = g0_avg_pos / float(g0_count);
    if(g1_count != 0) g1_avg_pos = g1_avg_pos / float(g1_count);

    // SECOND LOOP

    sp = gSpriteRoot;

    do
	{
		// Your code
		// Example affecting sprites by a controllable parameter
		vec3 co_dir;
		vec3 se_dir = vec3(0,0,0);
		vec3 al_dir = vec3(0,0,0);
		if(sp->group == 0){
            co_dir = g0_avg_pos - sp->position;
		}
		else{
            co_dir = g1_avg_pos - sp->position;
		}
		co_dir = normalize(co_dir);
		se_dir = sp->nearest_pos - sp->position;
		float se_len = sqrt(pow(se_dir.x, 2) + pow(se_dir.y, 2));
		se_dir = -normalize(se_dir);

		vec3 dir = co_dir*cohesion_w + al_dir*alignment_w;
		if(se_len < 100){
            dir += se_dir*separation_w;
		}
		//vec3 dir = avg_pos - sp->position;
		sp->speed = normalize(dir) * move_speed;

		HandleSprite(sp); // Default movement my speed. Callback in a real engine
		DrawSprite(sp);
		sp = sp->next;
	} while (sp != NULL);

    sgDraw();
	glutSwapBuffers();
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

	sgCreateStaticString(20, 40, "Slider and float display");
	sgCreateSlider(-1, -1, 200, &someValue, 0.5, 5);
	sgCreateDisplayFloat(-1, -1, "Value: ", &someValue);

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
