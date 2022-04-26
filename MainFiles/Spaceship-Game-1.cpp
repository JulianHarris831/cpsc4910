// Demo-4-MatrixAndMore.cpp: same as Demo-4-Matrix.cpp, but with:
//     matrix controlled by mouse drag or WASD
//     4 byte images supported (change to pixel shader and Display)

#include <glad.h>                                         
#include <glfw3.h>                                        
#include "GLXtras.h"  
#include "Misc.h"														// LoadTexture

mat4 m = Scale(1, .5f, 0);												// transformation matrix
GLuint program = 0, textureName = 0, textureUnit = 0;					// OpenGL identifiers
const char *pixFile = "C:/dev/Graphics/IMG/spaceship-32.png";
int nChannels = 0;														// 3 if RGB, 4 if RGBA

int key = 0, keyCount = 0;
time_t keyDownTime = clock();											//KeyTest.cpp globals

const char *vertexShader = R"(
	#version 130
	uniform mat4 m;														// from CPU
	in vec2 point, uv;													// from GPU
	out vec2 vuv;           
	void main() {
		vec4 v = vec4(point, 0, 1);
		gl_Position = m*v;												// transform
		vuv = uv;														// send to pixel shader
	}
)";

const char *pixelShader = R"(
	#version 130
	in vec2 vuv;														// from vertex shader
	uniform sampler2D textureImage;										// from CPU
	uniform int nChannels = 3;
	out vec4 pColor;                                       
	void main() {
		pColor = texture(textureImage, vuv);
		if (nChannels == 3)
			pColor.a = 1;
		if (pColor.a < .02)												// if nearly full matte,
			discard;													// don't tag z-buffer
	}
)";

void Display() {
	glClearColor(0, 0, 0, 1);											// set background color
	glClear(GL_COLOR_BUFFER_BIT);										// clear background
	SetUniform(program, "m", m);										// set vertex shader transform
	SetUniform(program, "textureImage", (int) textureUnit);				// set texture map for pixel shader
	SetUniform(program, "nChannels", nChannels);
	glDrawArrays(GL_QUADS, 0, 4);										// draw object                     
	glFlush();															// finish scene
}

void MouseWheel(GLFWwindow *w, double ignore, double spin) {
	m = m*RotateZ((float)spin);											// add rotation
}

vec2 mouseRef;

void MouseButton(GLFWwindow *w, int butn, int action, int mods) {
	double x, y;
	glfwGetCursorPos(w, &x, &y);
	if (action == GLFW_PRESS)
		mouseRef = vec2(x, y);
}

void MouseMove(GLFWwindow *w, double x, double y) {
	if (glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		vec2 mDif(x-mouseRef.x, mouseRef.y-y);							// invert vertical
		mat4 tDif = Translate(mDif.x/1000, mDif.y/1000, 0);
		m = tDif*m;														// add translation
		mouseRef = vec2(x, y);
	}
}
void Resize(GLFWwindow *w, int width, int height) {
	glViewport(0, 0, width, height);
}

void Keyboard(GLFWwindow *w, int k, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		key = k;
		keyCount = 0;													//keyboard input from KeyTest.cpp
		keyDownTime = clock();
		float d = .02f;
		vec3 move = k=='W'? vec3(0,d,0) : k=='A'? vec3(-d,0,0) : k=='S'? vec3(0,-d,0) : k =='D'? vec3(d,0,0) : vec3(0,0,0);
		m = Translate(move)*m;
	}
}

void Animate() {														//Animate from KeyTest.cpp
	float keyTime = (float)(clock() - keyDownTime) / CLOCKS_PER_SEC;
	float d = .02f;
	if (KeyDown(key) && keyTime > .25f) {
		vec3 move = key == 'W' ? vec3(0, d, 0) : key == 'A' ? vec3(-d, 0, 0) : key == 'S' ? vec3(0, -d, 0) : key == 'D' ? vec3(d, 0, 0) : vec3(0, 0, 0); 
		m = Translate(move) * m;										//Replaced if statement with KeyTest.cpp equivalent
	}
}

int main() {
	glfwInit();
	GLFWwindow *w = glfwCreateWindow(600, 600, "Demo-4-Matrix", NULL, NULL);
	glfwSetWindowPos(w, 200, 200);
	glfwMakeContextCurrent(w);
	glfwSetScrollCallback(w, MouseWheel);
	glfwSetMouseButtonCallback(w, MouseButton);
	glfwSetCursorPosCallback(w, MouseMove);
	glfwSetWindowSizeCallback(w, Resize);
	glfwSetKeyCallback(w, Keyboard);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	program = LinkProgramViaCode(&vertexShader, &pixelShader);
	glUseProgram(program);
	GLuint vBuffer = 0;
	glGenBuffers(1, &vBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	vec2 pts[] = { {-1,-1}, {-1,1}, {1,1}, {1,-1} };					// vertex geometric location
	vec2 uvs[] = { {0, 0}, {0, 1}, {1, 1}, {1, 0} };					// vertex texture location
	int spts = sizeof(pts), suvs = sizeof(uvs);							// array sizes
	glBufferData(GL_ARRAY_BUFFER, spts+suvs, NULL, GL_STATIC_DRAW);		// allocate GPU buffer
	glBufferSubData(GL_ARRAY_BUFFER, 0, spts, pts);						// store pts
	glBufferSubData(GL_ARRAY_BUFFER, spts, suvs, uvs);					// store uvs after pts in mem
	textureName = LoadTexture(pixFile, textureUnit, true, &nChannels);	// store texture image
	glActiveTexture(GL_TEXTURE0+textureUnit);							// make texture active
	glBindTexture(GL_TEXTURE_2D, textureName);
	VertexAttribPointer(program, "point", 2, 0, (void *) 0);			// set feed for pts
	VertexAttribPointer(program, "uv", 2, 0, (void *) spts);			// set feed for uvs
	while (!glfwWindowShouldClose(w)) {
		Display();
		Animate();
		glfwSwapBuffers(w);                          
		glfwPollEvents();
	}
	glfwDestroyWindow(w);
	glfwTerminate();
}
