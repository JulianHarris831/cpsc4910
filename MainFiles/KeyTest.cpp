// KeyTest.cpp: is key held?

#include <glad.h>
#include <GLFW/glfw3.h>
#include <time.h>
#include "Misc.h"

int key = 0, keyCount = 0;
time_t keyDownTime = clock();

void Animate() {
	float keyTime = (float) (clock()-keyDownTime)/CLOCKS_PER_SEC;
	if (KeyDown(key) && keyTime > .25f) {
		keyCount++;
		printf("\rkey %c count = %i\t\t", key, keyCount);
	}
}

void Keyboard(GLFWwindow *w, int k, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		printf("\nkey %c count 1", k);
		key = k;
		keyCount = 0;
		keyDownTime = clock();
	}
}

int main(int ac, char **av) {
	if (!glfwInit()) { printf("Can't init GLFW\n"); return 1; }
	GLFWwindow *w = glfwCreateWindow(1, 1, "KeyTest", NULL, NULL);
	glfwSetKeyCallback(w, Keyboard);
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(w)) {
		Animate();
		glfwPollEvents();
		glfwSwapBuffers(w);
	}
	glfwDestroyWindow(w);
	glfwTerminate();
}
