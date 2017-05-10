#include "Vector3D.h"
#include "Octree.h"

#include "Shader.h"
#include "Program.h"

#include <random>
#include <string>
#include <iostream>
#include <cmath>
#include <cfloat>
#include <chrono>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#define G 6.6e-11
#define THETA 1.0
#define N 4096
#define STEP 100

float frand () {
	return (float)rand() / RAND_MAX;
}

void pairwise (Body* a, const Body* b) {
	if (a == nullptr || b == nullptr) return;

	/*	F = ma
		a = F/m
		r = rx * rx + ry * ry + rz * rz
		Let m = 1
		a = F
		F = GMm / (r^2) => a = G / (r^2)
		Fx = F * rx/r = G * rx / (r^3)		*/

	Vector3D rel = a->getPosition() - b->getPosition();
	float r = rel.getX() * rel.getX() + rel.getY() * rel.getY() + rel.getZ() * rel.getZ();

	if (r == 0.0) r = 1e-6;

	float invrcub = std::pow(r, -1.5f);
	float coef = -G * invrcub;

	Vector3D F = rel * coef;
	a->setAcceleration(a->getAcceleration() + F);
}

void pairwise (Body* body, const Octree* tree) {
	if (body == nullptr || tree == nullptr) return;

	if (tree->isLeaf()) {

		Body * other = tree->getBody();
		if (other == body) return;
		pairwise(body, other);
		
	}
	else {

		BoundingVolume bounds = tree->getBounds();
		float s = std::abs(bounds.getMax().getX() - bounds.getMin().getX());
		float d = (body->getPosition() - tree->getCentreOfMass()).magnitude();

		if (s / d < THETA) {
			Body other = Body(tree->getCentreOfMass(), tree->getTotalMass());
			pairwise(body, &other);
		}
		else {
			for (int i = 0; i < 8; ++i) {
				pairwise(body, tree->getChild(i));
			}
		}
		
	}
}

void constructAndDrawTree (const Octree& tree, glm::mat4 viewProj, GLuint mvpLoc, const int vertexCount) {

	BoundingVolume bounds = tree.getBounds();
	Vector3D min = bounds.getMin();
	Vector3D max = bounds.getMax();

	glm::vec3 centre (
		(min.getX() + max.getX()) / 2,
		(min.getY() + max.getY()) / 2,
		(min.getZ() + max.getZ()) / 2
	);

	glm::vec3 size (
		max.getX() - min.getX(),
		max.getY() - min.getY(),
		max.getZ() - min.getZ()
	);

	glm::mat4 modelMatrix = glm::translate(glm::mat4(1), centre) * glm::scale(glm::mat4(1), size);

	glm::mat4 mvp = viewProj * modelMatrix;
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glDrawArrays(GL_LINES, 0, vertexCount);

	if (!tree.isLeaf()) {
		for (int i = 0; i < 8; ++i) {
			Octree* child = tree.getChild(i);
			if (child != nullptr) {
				constructAndDrawTree(*child, viewProj, mvpLoc, vertexCount);
			}
		}
	}

}

glm::mat4 matProj = glm::perspective((float)M_PI / 4.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
void frameBufferSizeCallback (GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
	matProj = glm::perspective((float)M_PI / 4.0f, (float)width / height, 0.1f, 1000.0f);
}

static const std::array<GLfloat, 24 * 3> boundingData = {
	-0.5, -0.5, -0.5,
	-0.5, -0.5,  0.5,
	-0.5, -0.5, -0.5,
	-0.5,  0.5, -0.5,
	-0.5, -0.5, -0.5,
	 0.5, -0.5, -0.5,

	 0.5,  0.5,  0.5,
	 0.5,  0.5, -0.5,
	 0.5,  0.5,  0.5,
	 0.5, -0.5,  0.5,
	 0.5,  0.5,  0.5,
	-0.5,  0.5,  0.5,

	 0.5, -0.5,  0.5,
	-0.5, -0.5,  0.5,
	-0.5, -0.5,  0.5,
	-0.5,  0.5,  0.5,
	-0.5,  0.5,  0.5,
	-0.5,  0.5, -0.5,
	-0.5,  0.5, -0.5,
	 0.5,  0.5, -0.5,
	 0.5,  0.5, -0.5,
	 0.5, -0.5, -0.5,
	 0.5, -0.5, -0.5,
	 0.5, -0.5,  0.5
};

int main () {
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialise GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "Tutorial 01", nullptr, nullptr);
	if (window == nullptr) {
		fprintf(stderr, "Failed to open GLFW window\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialise GLEW\n");
		glfwTerminate();
		return -1;
	}

	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::array<Body, N> bodies = {};
	for (int i = 0; i < bodies.size(); ++i) {
		bodies[i] = Body(Vector3D(frand() - 0.5, frand() - 0.5, frand() - 0.5) * 20);
	}
	
	std::array<Body, N> naiiveBodies = bodies;
	std::array<Body, N> barnesHutBodies = bodies;

	GLuint vertexArray;
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);

	GLuint boundsBuffer;
	glGenBuffers(1, &boundsBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, boundsBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boundingData), boundingData.data(), GL_STATIC_DRAW);

	Program program;
	{
		Shader vertexShader(GL_VERTEX_SHADER, "shaders/vert.glsl");
		Shader fragmentShader(GL_FRAGMENT_SHADER, "shaders/frag.glsl");
		program.attach(vertexShader);
		program.attach(fragmentShader);
		program.link();
		program.detach(vertexShader);
		program.detach(vertexShader);
	}

	GLuint loc_MVP = program.getUniformLocation("MVP");
	GLuint loc_colour = program.getUniformLocation("colour");

	program.use();

	float angle = 0.0f;

	
	glm::mat4 origView = glm::lookAt(glm::vec3(30, 10, 30), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 matModl = glm::mat4(1.0f);

	Vector3D min = Vector3D(-10, -10, -10);
	Vector3D max = Vector3D(10, 10, 10);

	bool running = true;
	while (running) {

		/*// Naiive implementation

		for (unsigned int i = 0; i < naiiveBodies.size(); ++i) {
			Body &body = naiiveBodies[i];
			for (unsigned int j = 0; j < naiiveBodies.size(); ++j) {
				pairwise(&body, &naiiveBodies[j]);
			}
			body.update(STEP);
		}*/

		// Barnes-Hut implmentation

		for (int i = 0; i < barnesHutBodies.size(); ++i) {
			Vector3D pos = barnesHutBodies[i].getPosition();

			if (pos.getX() < min.getX()) {
				min.setX(2 * min.getX() - max.getX());
			}
			else if (pos.getX() > max.getX()) {
				max.setX(2 * max.getX() - min.getX());
			}

			if (pos.getY() < min.getY()) {
				min.setY(2 * min.getY() - max.getY());
			}
			else if (pos.getY() > max.getY()) {
				max.setY(2 * max.getY() - min.getY());
			}

			if (pos.getZ() < min.getZ()) {
				min.setZ(2 * min.getZ() - max.getZ());
			}
			else if (pos.getZ() > max.getZ()) {
				max.setZ(2 * max.getZ() - min.getZ());
			}
		}

		Octree tree = Octree(min, max);

		for (int i = 0; i < barnesHutBodies.size(); ++i) {
			if (!tree.insert(&barnesHutBodies[i])) {
				//std::cout << "Couldn't insert element " << i << '\n';
			}
		}
		
		for (unsigned int i = 0; i < barnesHutBodies.size(); ++i) {
			Body &body = barnesHutBodies[i];
			pairwise(&body, &tree);
			body.update(STEP);
		}

		// Render

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(barnesHutBodies),
			barnesHutBodies.data(),
			GL_STATIC_DRAW	
		);

		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		angle += 0.01f;
		glm::mat4 matView = origView * glm::rotate(angle, glm::vec3(0, 1, 0));

		// Render bodies

		glm::vec4 colour (1.0f, 1.0f, 1.0f, 1.0f);
		glUniform4fv(loc_colour, 1, glm::value_ptr(colour));

		glm::mat4 mvp = matProj * matView * matModl;
		glUniformMatrix4fv(loc_MVP, 1, GL_FALSE, glm::value_ptr(mvp));

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Body), nullptr);

		glDrawArrays(GL_POINTS, 0, naiiveBodies.size());

		// Render bounding volumes

		colour = glm::vec4(1.0f, 1.0f, 1.0f, 0.1f);
		glUniform4fv(loc_colour, 1, glm::value_ptr(colour));

		glBindBuffer(GL_ARRAY_BUFFER, boundsBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		constructAndDrawTree(tree, matProj * matView, loc_MVP, boundingData.size());

		//glDrawArrays(GL_LINES, 0, boundingData.size());

		glfwSwapBuffers(window);

		//std::this_thread::sleep_for(std::chrono::milliseconds(10));

		glfwPollEvents();

		if (glfwWindowShouldClose(window)) {
			running = false;
		}

	}

}