// Spaceship-Game-2.cpp - Using MultiSprite for our project baseline title screen
//multiple movable, selectable, layered sprites

#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "GLXtras.h"
#include "Misc.h"
#include "Sprite.h"

//Application Display, modify size here
GLFWwindow* w;
int winWidth = 1000, winHeight = 600;
float splashDuration = 8;								//in seconds
time_t startTime = clock();								//start time of application

//Sprite section, the sprite locations I need to fix
Sprite background, splash, spaceship;
string dir = "C:/dev/Graphics/IMG/"; //sprite directory
string splashTex = dir + "splashPlaceholder.png"; //title sprite
string spaceshipTex = dir+"spaceship-32.png";  
string backgroundTex = dir+"space-backdrop.jpg"; //ALWAYS in the back. Sprites have z values
//New sprites must be 32 bit OR use a mat if not possible, unless BACKGROUND
//Remember, you must declare at the top, initialize in main, and remember to release, just like these.

//Keyboard Interaction
/*
int key = 0;
time_t keyDownTime = 0;		//key delay might be important, too difficult to be exact
float keyDelay = .1f;		//in seconds
*/

// Display

void Display() {
	glEnable(GL_BLEND); //this line and below is openGL automatic blending, smoothing
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClear(GL_DEPTH_BUFFER_BIT); //Z buffer refers to layers, how we control what is in front
	glEnable(GL_DEPTH_TEST);
	background.Display();	
	float elapsed = (float)(clock() - startTime) / CLOCKS_PER_SEC;
	if (elapsed < splashDuration) //If X time has passed, skip title screen move to game
		splash.Display();
	else
		spaceship.Display();

	glFlush();
}

// Interaction

void Animate() {
	float f = .02f;
	vec3 move;
	if (KeyDown('W') || glfwGetKey(w, GLFW_KEY_UP)) move.y += f;
	if (KeyDown('A') || glfwGetKey(w, GLFW_KEY_LEFT)) move.x -= f;
	if (KeyDown('S') || glfwGetKey(w, GLFW_KEY_DOWN)) move.y -= f;
	if (KeyDown('D') || glfwGetKey(w, GLFW_KEY_RIGHT)) move.x += f;
	spaceship.ptTransform = Translate(move) * spaceship.ptTransform;
}

// Application

void Resize(GLFWwindow *w, int width, int height) {
	glViewport(0, 0, winWidth = width, winHeight = height);
}

int main(int ac, char **av) {
	// init app window and GL context
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	w = glfwCreateWindow(winWidth, winHeight, "Spaceship-Fighter GGEZ", NULL, NULL);
	glfwSetWindowPos(w, 100, 100);
	glfwMakeContextCurrent(w);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	// initialize sprites here, FINAL parameter in .Initialize() is the Z value!! Greater Z value = further
	background.Initialize(backgroundTex, 0, .7f); //calls sprite.h
	splash.Initialize(splashTex, 1, .1f);
//	sprite1.Initialize(sprite1Tex, sprite1Mat, 1, .2f);
	spaceship.Initialize(spaceshipTex, 2, .5f);
	spaceship.ptTransform = Scale(.2f, .2f, 1);//making smaller, too big
	// callbacks
	glfwSetWindowSizeCallback(w, Resize);
	// event loop
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(w)) {
		Animate();
		Display();
		glfwSwapBuffers(w);
		glfwPollEvents();
	}
	// terminate
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	background.Release();
	splash.Release();
	spaceship.Release();
	glfwDestroyWindow(w);
	glfwTerminate();
}
