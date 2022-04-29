// Spaceship-fighter-game.cpp - multiple layered sprites

#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "GLXtras.h"
#include "Misc.h"
#include "Sprite.h"

//Push test comment

// Application Display
GLFWwindow *w;
int winWidth = 1000, winHeight = 600;
float splashDuration = 4;										// in seconds (change screen)
time_t startTime = clock();										// start time of application

// Sprites
Sprite background, splash, spaceship, arrow, saucer, asteroid, laser;
string dir = "C:/dev/Graphics/IMG/";
string splashTex = dir + "space-start.png";						// png, tga ok; bmp, jpg do not support 32
string arrowTex = dir + "arrow-32.png";
string spaceshipTex = dir+"spaceship-32.png";
string saucerTex = dir + "saucer-32.png";
string asteroidTex = dir + "asteroid-32.png";
string laserTex = dir + "laser-32.png";
string backgroundTex = dir+"space.jpg";
// string otherSprites = dir+"Combined32.png";					// add more sprites (remember to init new sprites in main)

// Display, Animation (Boolean, etc.) Variables
bool startScreen = true;
bool playScreen = false;
int arrowPosition = 0;											// valid arrow position, 0: Start Game, 1: High Scores, 2: Credits
bool enemyDeployed = false;
bool asteroidDeployed = false;
bool shootLaser = false;

// Display

void Display() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	background.Display();
	float elapsed = (float)(clock() - startTime) / CLOCKS_PER_SEC;
	/*if (elapsed < splashDuration) {
		splash.Display();
		arrow.Display();
	}*/
	if (startScreen) {
		splash.Display();
		arrow.Display();
		if (glfwGetKey(w, GLFW_KEY_ENTER) && arrowPosition == 0) startScreen = false;
	}
	else {
		playScreen = true;
		spaceship.Display();
		asteroid.Display();			// how to make asteroid display after each interval? say every 4 seconds elapsed?
		asteroidDeployed = true;
		if (elapsed > splashDuration) {
			saucer.Display();		// how to display a lot of saucer at the same time (make copies of sprite)
			enemyDeployed = true;
		}
		if (glfwGetKey(w, GLFW_KEY_SPACE)) {
			shootLaser = true;		// similar to above, how to make every time user hit space, a new (another) laser is displayed
		}
		if (shootLaser) laser.Display();
	}

	glFlush();
}

// Interaction

void Animate() {
	float f = .03f;												// movement amount; object translation speed
	vec3 move;
	if (playScreen) {
		float play_elapsed = (float)(clock() - startTime) / CLOCKS_PER_SEC;
		if (KeyDown('W') || glfwGetKey(w, GLFW_KEY_UP)) move.y += f;
		if (KeyDown('A') || glfwGetKey(w, GLFW_KEY_LEFT)) move.x -= f;
		if (KeyDown('S') || glfwGetKey(w, GLFW_KEY_DOWN)) move.y -= f;
		if (KeyDown('D') || glfwGetKey(w, GLFW_KEY_RIGHT)) move.x += f;
		spaceship.ptTransform = Translate(move) * spaceship.ptTransform;
		if (asteroidDeployed) {
			asteroid.ptTransform = Translate(-f, 0, 0) * asteroid.ptTransform;
			// how to check asteroid is out of screen/hits player spaceship, asteroidDeployed = false
		}
		if (shootLaser) {
			laser.ptTransform = Translate(f, 0, 0) * laser.ptTransform;
		}
	}
}


void Keyboard(GLFWwindow* w, int k, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		float d = .2f;
		// vec3 arrowMove = k == 'W' ? vec3(0, d, 0) : k == 'S' ? vec3(0, -d, 0) : vec3(0, 0, 0);
		if (k == 'W' || glfwGetKey(w, GLFW_KEY_UP)) {
			arrowPosition -= 1;
			if (arrowPosition < 0) {
				arrow.ptTransform = Translate(0, 2 * -d, 0) * arrow.ptTransform;
				arrowPosition = 2;
			}
			else 
				arrow.ptTransform = Translate(0, d, 0) * arrow.ptTransform;
		}
		else if (k == 'S' || glfwGetKey(w, GLFW_KEY_DOWN)) {
			arrowPosition += 1;
			if (arrowPosition > 2) {
				arrow.ptTransform = Translate(0, 2 * d, 0) * arrow.ptTransform;
				arrowPosition = 0;
			}
			else 
				arrow.ptTransform = Translate(0, -d, 0) * arrow.ptTransform;
		}
	}
}

// Application

void Resize(GLFWwindow *w, int width, int height) {
	glViewport(0, 0, winWidth = width, winHeight = height);
}

int main(int ac, char **av) {
	// init app window and GL context
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	w = glfwCreateWindow(winWidth, winHeight, "Spaceship Fighter Game", NULL, NULL);
	glfwSetWindowPos(w, 100, 100);
	glfwMakeContextCurrent(w);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	// initialize sprites
	background.Initialize(backgroundTex, 0, .7f);
	splash.Initialize(splashTex, 1, .2f);
	arrow.Initialize(arrowTex, 1, .1f);
	arrow.ptTransform = Translate(-.35f, -.2f, 0)*Scale(.22f, .15f, 1);
	spaceship.Initialize(spaceshipTex, 2, .1f);
	spaceship.ptTransform = Translate(-.77f, 0, 0)*Scale(.2f, .2f, 1);
	saucer.Initialize(saucerTex, 2, .1f);
	saucer.ptTransform = Translate(.7f, .2f, 0) * Scale(.1f, .1f, 1);
	asteroid.Initialize(asteroidTex, 2, .1f);
	asteroid.ptTransform = Translate(.9f, 0, 0) * Scale(.1f, .2f, 1);
	laser.Initialize(laserTex, 2, .1f);
	laser.ptTransform = Translate(-.7f, 0, 0) * Scale(.1f, .2f, 1);	// note laser initial position depends on spaceship current pos (still wrong)
	// callbacks
	glfwSetWindowSizeCallback(w, Resize);
	glfwSetKeyCallback(w, Keyboard);
	// event loop
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(w)) {
		Display();
		Animate();
		glfwSwapBuffers(w);
		glfwPollEvents();
	}
	// terminate
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	background.Release();
	splash.Release();
	arrow.Release();
	spaceship.Release();
	saucer.Release();
	asteroid.Release();
	laser.Release();
	glfwDestroyWindow(w);
	glfwTerminate();
}
