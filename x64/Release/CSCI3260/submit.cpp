/*********************************************************
FILE : submit.cpp (csci3260 2016-2017 Assignment 2)
*********************************************************/
/*********************************************************
Student Information
Student ID: 1155049055
Student Name: Siu Long Wa
Credit: All the extra objects come from TF3dm.com
*********************************************************/

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include "custom_vertex.h"
#include "resourceLoader.h"
#include "interactive.h"
#include "shaderInstall.h"
using namespace std;

GLfloat earth_selfRotate = 0;
GLfloat planet_selfRotate = 0;
GLfloat moon_Rotate = 0;
GLfloat moon_selfRotate = 0.1 * 100 / 0.1;
GLfloat lightbox_selfRotate = 0;
GLfloat spaceship_selfRotate = 0;
GLfloat spaceship_Rotate = 0;
GLfloat spaceship_Orientation = 45.0f;

GLfloat fovy = +90.0f;
GLfloat aspect = 1024.0f/512.0f;
GLfloat zNear = +0.1f;
GLfloat zFar = +300.0f;

const int numberOfRock = 6000;
glm::mat4 rockInstancedMatrix[numberOfRock];
GLfloat rockSelfRotate = 0;
GLfloat rockRotate = 0;

GLuint floor_vao, planet_vao, skybox_vao, lightbox_vao, rock_vao, spaceship_vao;
GLuint floor_vbo[3], planet_vbo[3], skybox_vbo[3], lightbox_vbo[3], rock_vbo[4], spaceship_vbo[3];
int planet_size, airbus_size, lightbox_size, rock_size, skybox_size, spaceship_size;
GLuint earth_texture[2], moon_texture, lightbox_texture, planet2_texture[3], rock_texture, skybox_texture[2], spaceship_texture;

glm::mat4 transMatrix, transMatrix2, rotMatrix, rotMatrix2, rotMatrix3, scaleMatrix, projMatrix, AniRotMatrix, 
			modelTransformMatrix, viewMatrix, AniRotMatrix2, AniRotMatrix3, orientMatirx, orientMatirx2, orientMatirx3;;
glm::vec3 tranvec3, scalevec3;

const glm::vec3 worldUp = glm::vec3(0.0f,1.0f,0.0f);
glm::vec3 camLocation = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camTarget = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 camUp = glm::vec3(0.0f,1.0f,0.0f);
glm::vec3 spaceLocation = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 spaceTarget = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 spaceUp = glm::vec3(0.0f, 0.0f, 1.0f);

// Light related 
GLfloat lightx, lighty, lightz;
GLfloat lightbox_rotate = 0.01f;
GLfloat lightbox_required_movement = 30.0f;
int moveDirection = 1;
glm::vec3 lightbox_position, lightbox_position2, lightbox_position3;
glm::vec3 ambientLight;

GLint modelTransformMatrixUniformLocation, viewMatrixUniformLocation, projectionMatrixUniformLocation, textureLocation, normalTextureLocation,
lightPositionWorldLocation, ambientLightLocation, eyePositionLocation, selfIlluminated, diffuseControl, specularControl, normalMapping, tempLocation;

void spaceshipInEarth() {
	if (spaceTravel == 0) {
		AniRotMatrix = glm::rotate(glm::mat4(), glm::radians(spaceship_selfRotate), glm::vec3(0, 1, 0));
		transMatrix = glm::translate(glm::mat4(), glm::vec3(8.0f, 0.0f, 0.0f));
		AniRotMatrix2 = glm::rotate(glm::mat4(), glm::radians(spaceship_Rotate), glm::vec3(0, 1, 0));
		rotMatrix2 = glm::rotate(glm::mat4(), glm::radians(45.0f), glm::vec3(0, 0, 1));
		transMatrix2 = glm::translate(glm::mat4(), glm::vec3(50.0f + zMouse, 2.0f + yMouse, -40.0f + xMouse));
		spaceship_selfRotate += spaceshipSpeed*0.0025f / 2.0f; //6.750131
		spaceship_Rotate += spaceshipSpeed*0.1f / 2.0f; // 270.1
		modelTransformMatrix = transMatrix2*rotMatrix2*AniRotMatrix2*transMatrix*AniRotMatrix*rotMatrix*scaleMatrix;
		if (spaceship_Rotate > 360.0) {
			spaceship_Rotate = 0;
			spaceship_selfRotate = 0;
		}
	}
	else {
		if (spaceTravelInProgress == 0) {
			spaceship_Rotate = 0;
			spaceship_selfRotate = 270.0f;
		}
		// Travel from planet to Earth
		AniRotMatrix = glm::rotate(glm::mat4(), glm::radians(spaceship_selfRotate), glm::vec3(0, 1, 0));
		transMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -30.0f));
		AniRotMatrix2 = glm::rotate(glm::mat4(), glm::radians(spaceship_Rotate), glm::vec3(0, 1, 0));
		rotMatrix2 = glm::rotate(glm::mat4(), glm::radians(45.0f), glm::vec3(0, 0, 1));
		transMatrix2 = glm::translate(glm::mat4(), glm::vec3(50.0f + zMouse, 2.0f + yMouse, -2.0f + xMouse));
		modelTransformMatrix = transMatrix2*rotMatrix2*AniRotMatrix2*transMatrix*AniRotMatrix*rotMatrix*scaleMatrix;
		// First circular motion
		if (spaceTravelInProgress < 180) {
			spaceship_selfRotate -= spaceshipSpeed*0.0005f; //6.750131
			spaceship_Rotate -= spaceshipSpeed*0.02f; // 270.1
			spaceTravelInProgress += spaceshipSpeed*0.02f;
		}
		else {
			spaceTravel = 0;
			spaceshipOnEarth = 0;
			spaceship_Rotate = 90.0f;
			spaceship_selfRotate = 2.25f;
			spaceTravelInProgress = 0;
		}
	}
}

void spaceshipInPlanet() {
	if (spaceTravel == 0) {
		AniRotMatrix = glm::rotate(glm::mat4(), glm::radians(spaceship_selfRotate), glm::vec3(0, 1, 0));
		transMatrix = glm::translate(glm::mat4(), glm::vec3(12.0f, 0.0f, 0.0f));
		AniRotMatrix2 = glm::rotate(glm::mat4(), glm::radians(spaceship_Rotate), glm::vec3(0, 1, 0));
		rotMatrix2 = glm::rotate(glm::mat4(), glm::radians(45.0f), glm::vec3(0, 0, 1));
		transMatrix2 = glm::translate(glm::mat4(), glm::vec3(50.0f + zMouse, 2.0f + yMouse, 40.0f + xMouse));
		spaceship_selfRotate += spaceshipSpeed*0.0025f; //6.750131
		spaceship_Rotate += spaceshipSpeed*0.1f; // 270.1
		modelTransformMatrix = transMatrix2*rotMatrix2*AniRotMatrix2*transMatrix*AniRotMatrix*rotMatrix*scaleMatrix;
		if (spaceship_Rotate > 360.0) {
			spaceship_Rotate = 0;
			spaceship_selfRotate = 0;
		}
	}
	else {
		if (spaceTravelInProgress == 0) {
			spaceship_Rotate = 0;
			spaceship_selfRotate = 270.0f;
		}
		// Travel from planet to Earth
		AniRotMatrix = glm::rotate(glm::mat4(), glm::radians(spaceship_selfRotate), glm::vec3(0, 1, 0));
		transMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 50.0f));
		AniRotMatrix2 = glm::rotate(glm::mat4(), glm::radians(spaceship_Rotate), glm::vec3(0, 1, 0));
		rotMatrix2 = glm::rotate(glm::mat4(), glm::radians(45.0f), glm::vec3(0, 0, 1));
		transMatrix2 = glm::translate(glm::mat4(), glm::vec3(50.0f + zMouse, 2.0f + yMouse, 2.0f + xMouse));
		modelTransformMatrix = transMatrix2*rotMatrix2*AniRotMatrix2*transMatrix*AniRotMatrix*rotMatrix*scaleMatrix;
		// First circular motion
		if (spaceTravelInProgress < 180) {
			spaceship_selfRotate += spaceshipSpeed*0.0005f; //6.750131
			spaceship_Rotate += spaceshipSpeed*0.02f; // 270.1
			spaceTravelInProgress += spaceshipSpeed*0.02f;
		}
		else {
			spaceTravel = 0;
			spaceshipOnEarth = 1;
			spaceship_Rotate = 90.0f;
			spaceship_selfRotate = 2.25f;
			spaceTravelInProgress = 0;
		}
	}
}

void getRock() {
	GLfloat randNum[10];
	glm::mat4 orientMatirx, orientMatirx2, orientMatirx3;
	for (int i = 0; i < numberOfRock; i++) {
		// Scale of the rock
		randNum[0] = ((GLfloat)rand() / (RAND_MAX + 1.0))/5.0f + 0.1f;
		// Position of the rock
		GLfloat radius = 5*((GLfloat)rand() / (RAND_MAX + 1.0)) + 15.0f;
		GLfloat angle = ((GLfloat)rand() / (RAND_MAX + 1.0)) * 360.0f;
		randNum[1] = radius*sin(angle);
		randNum[2] = radius*cos(angle);
		randNum[3] = (GLfloat)rand() / (RAND_MAX + 1.0);
		// Rotation of the rock
		randNum[4] = ((GLfloat)rand() / (RAND_MAX + 1.0)) * 360.0f; // x
		randNum[5] = ((GLfloat)rand() / (RAND_MAX + 1.0)) * 360.0f; // y
		randNum[6] = ((GLfloat)rand() / (RAND_MAX + 1.0)) * 360.0f; // z

		scaleMatrix = glm::scale(glm::mat4(), glm::vec3(randNum[0], randNum[0], randNum[0]));
		orientMatirx = glm::rotate(glm::mat4(), glm::radians(randNum[4]), glm::vec3(1, 0, 0));
		orientMatirx2 = glm::rotate(glm::mat4(), glm::radians(randNum[5]), glm::vec3(0, 1, 0));
		orientMatirx3 = glm::rotate(glm::mat4(), glm::radians(randNum[6]), glm::vec3(0, 0, 1));
		transMatrix = glm::translate(glm::mat4(), glm::vec3(randNum[1], randNum[3], randNum[2]));
		rockInstancedMatrix[i] = transMatrix*orientMatirx3*orientMatirx2*orientMatirx*scaleMatrix;
	}
}

void sendDataToOpenGL()
{
	bool res;

	// Skybox data
	glGenVertexArrays(1, &skybox_vao);
	glBindVertexArray(skybox_vao);
	// Vertex data
	glGenBuffers(1, &skybox_vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_ver), skybox_ver, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,0, 0);
	//Getting the texture
	getSkyboxTexture();
	skybox_texture[0] = loadCubeMap(skyboxTexture);
	getAnotherSkyboxTexture();
	skybox_texture[1] = loadCubeMap(skyboxTexture2);
	skybox_size = 108;
	// Disable the binding
	glBindVertexArray(0);

	//Data for the planet
	std::vector<glm::vec3> planet_vertices, planet_normals;
	std::vector<glm::vec2> planet_uvs;
	res = loadOBJ("obj/planet.obj", planet_vertices, planet_uvs, planet_normals);
	glGenVertexArrays(1, &planet_vao);
	glBindVertexArray(planet_vao);
	//Vertex
	glGenBuffers(1, &planet_vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, planet_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, planet_vertices.size() * sizeof(glm::vec3), &planet_vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, planet_vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//UV
	glGenBuffers(1, &planet_vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, planet_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, planet_uvs.size() * sizeof(glm::vec2), &planet_uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, planet_vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//Normal
	glGenBuffers(1, &planet_vbo[2]);
	glBindBuffer(GL_ARRAY_BUFFER, planet_vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, planet_normals.size() * sizeof(glm::vec3), &planet_normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, planet_vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	planet_size = planet_vertices.size();
	earth_texture[0] = loadBMP_custom("texture/earth.bmp");
	earth_texture[1] = loadBMP_custom("normal_map/earth_normal.bmp");
	moon_texture = loadBMP_custom("texture/moon.bmp");
	planet2_texture[0] = loadBMP_custom("texture/wired_planet.bmp");
	planet2_texture[1] = loadBMP_custom("texture/wired_planet2.bmp");
	planet2_texture[2] = loadBMP_custom("texture/wired_planet3.bmp");

	//Data for the lightbox
	std::vector<glm::vec3> lightbox_vertices, lightbox_normals;
	std::vector<glm::vec2> lightbox_uvs;
	res = loadOBJ("obj/lightbox.obj", lightbox_vertices, lightbox_uvs, lightbox_normals);
	glGenVertexArrays(1, &lightbox_vao);
	glBindVertexArray(lightbox_vao);
	//Vertex
	glGenBuffers(1, &lightbox_vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, lightbox_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, lightbox_vertices.size() * sizeof(glm::vec3), &lightbox_vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, lightbox_vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//UV
	glGenBuffers(1, &lightbox_vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, lightbox_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, lightbox_uvs.size() * sizeof(glm::vec2), &lightbox_uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, lightbox_vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//Normal
	glGenBuffers(1, &lightbox_vbo[2]);
	glBindBuffer(GL_ARRAY_BUFFER, lightbox_vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, lightbox_normals.size() * sizeof(glm::vec3), &lightbox_normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, lightbox_vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	lightbox_size = lightbox_vertices.size();
	lightbox_texture = loadBMP_custom("texture/lightbox.bmp");

	//Data for the rock
	std::vector<glm::vec3> rock_vertices, rock_normals;
	std::vector<glm::vec2> rock_uvs;
	res = loadOBJ("obj/rock.obj", rock_vertices, rock_uvs, rock_normals);
	getRock();
	glGenVertexArrays(1, &rock_vao);
	glBindVertexArray(rock_vao);
	//Vertex
	glGenBuffers(1, &rock_vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, rock_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, rock_vertices.size() * sizeof(glm::vec3), &rock_vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, rock_vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//UV
	glGenBuffers(1, &rock_vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, rock_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, rock_uvs.size() * sizeof(glm::vec2), &rock_uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, rock_vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//Normal
	glGenBuffers(1, &rock_vbo[2]);
	glBindBuffer(GL_ARRAY_BUFFER, rock_vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, rock_normals.size() * sizeof(glm::vec3), &rock_normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, rock_vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &rock_vbo[3]);
	glBindBuffer(GL_ARRAY_BUFFER, rock_vbo[3]);
	GLsizei vec4Size = sizeof(glm::vec4);
	glBufferData(GL_ARRAY_BUFFER, numberOfRock * sizeof(glm::mat4), &rockInstancedMatrix[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(3 * vec4Size));
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	rock_size = rock_vertices.size();
	rock_texture = loadBMP_custom("texture/meteor.bmp");


	//Data for the planet
	std::vector<glm::vec3> spaceship_vertices, spaceship_normals;
	std::vector<glm::vec2> spaceship_uvs;
	res = loadOBJ("obj/spaceship.obj", spaceship_vertices, spaceship_uvs, spaceship_normals);
	glGenVertexArrays(1, &spaceship_vao);
	glBindVertexArray(spaceship_vao);
	//Vertex
	glGenBuffers(1, &spaceship_vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, spaceship_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, spaceship_vertices.size() * sizeof(glm::vec3), &spaceship_vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, spaceship_vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//UV
	glGenBuffers(1, &spaceship_vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, spaceship_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, spaceship_uvs.size() * sizeof(glm::vec2), &spaceship_uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, spaceship_vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//Normal
	glGenBuffers(1, &spaceship_vbo[2]);
	glBindBuffer(GL_ARRAY_BUFFER, spaceship_vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, spaceship_normals.size() * sizeof(glm::vec3), &spaceship_normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, spaceship_vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	spaceship_size = spaceship_vertices.size();
	spaceship_texture = loadBMP_custom("texture/spaceship.bmp");

}

void initializedGL(void) //run only once
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	sendDataToOpenGL();
	installGeneralShaders();
	installSkyboxShaders();
	installLightBoxShaders();
	installRockShaders();
}

void paintGL(void)  //always run
{
	// Setting up the universal matrix
	camTarget = glm::vec3(camTarget_x, camTarget_y, camTarget_z);
	camLocation = glm::vec3(camLocation_x, camLocation_y, camLocation_z);
	camUp = glm::vec3(camUp_x, camUp_y, camUp_z);
	viewMatrix = glm::lookAt(camLocation, camTarget, camUp);

	projMatrix = glm::perspective(glm::radians(fovy), aspect, zNear, zFar);

	glClearColor(0.0f, 0.0, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw skybox
	glUseProgram(skyBoxID);
	glDepthMask(GL_FALSE);
	glBindVertexArray(skybox_vao);
	// need to change the value of view matrix
	viewMatrix = glm::mat4(glm::mat3(viewMatrix));
	rotMatrix = glm::rotate(glm::mat4(), glm::radians(180.0f), glm::vec3(1, 0, 0));
	modelTransformMatrix = projMatrix*viewMatrix*rotMatrix;
	modelTransformMatrixUniformLocation = glGetUniformLocation(skyBoxID, "modelTransformMatrix");
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);

	textureLocation = glGetUniformLocation(skyBoxID, "skybox");
	glActiveTexture(GL_TEXTURE0);
	if (trueUniverse == 1) 
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture[1]);
	else
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture[0]);
	glUniform1i(textureLocation, 0);
	glDrawArrays(GL_TRIANGLES, 0, 12*3);
	glDepthMask(GL_TRUE);

	// Restore the value of view matrix
	viewMatrix = glm::lookAt(camLocation, camTarget, camUp);


	// Draw lightbox
	glUseProgram(lightBoxID);

	glBindVertexArray(lightbox_vao);
	scaleMatrix = glm::scale(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f));
	// Need to extract the lightbox position for locating the light source
	transMatrix = glm::translate(glm::mat4(), glm::vec3(25.0f , 12.0f, 0.0f));
	rotMatrix = glm::rotate(glm::mat4(), glm::radians(lightbox_rotate), glm::vec3(0, 1, 0));
	transMatrix2 = glm::translate(glm::mat4(), glm::vec3(50.0f + zMouse, 12.0f + yMouse, 0.0f + xMouse));
	glm::vec4 tempMatrix = transMatrix2* rotMatrix* glm::vec4(glm::vec3(25.0f, 12.0f, 0.0f), 1.0f);
	lightx = tempMatrix.x;
	lighty = tempMatrix.y;
	lightz = tempMatrix.z;
	lightbox_position = glm::vec3(lightx, lighty, lightz);
	// Ordinary Matirx
	AniRotMatrix = glm::rotate(glm::mat4(), glm::radians(lightbox_selfRotate), glm::vec3(0, 1, 0));
	modelTransformMatrix = transMatrix2*rotMatrix*transMatrix*AniRotMatrix*scaleMatrix;
	modelTransformMatrixUniformLocation = glGetUniformLocation(lightBoxID, "modelTransformMatrix");
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	viewMatrixUniformLocation = glGetUniformLocation(lightBoxID, "viewMatrix");
	glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	projectionMatrixUniformLocation = glGetUniformLocation(lightBoxID, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, &projMatrix[0][0]);

	textureLocation = glGetUniformLocation(lightBoxID, "textureSampler1");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lightbox_texture);
	glUniform1i(textureLocation, 0);
	tempLocation = glGetUniformLocation(lightBoxID, "colorDecision");
	glUniform1i(tempLocation, 1);

	glDrawArrays(GL_TRIANGLES, 0, lightbox_size);

	// Draw the second lightbox
	scaleMatrix = glm::scale(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f));
	// Need to extract the lightbox position for locating the light source
	transMatrix = glm::translate(glm::mat4(), glm::vec3(-100.0f, 0.0f, 0.0f));
	rotMatrix = glm::rotate(glm::mat4(), glm::radians(lightbox_rotate), glm::vec3(0, 0, 1));
	transMatrix2 = glm::translate(glm::mat4(), glm::vec3(50.0f + zMouse, 0.0f + yMouse, 0.0f + xMouse));
	tempMatrix = transMatrix2* rotMatrix* glm::vec4(glm::vec3(-100.0f, 0.0f, 0.0f), 1.0f);
	lightx = tempMatrix.x;
	lighty = tempMatrix.y;
	lightz = tempMatrix.z;
	lightbox_position2 = glm::vec3(lightx, lighty, lightz);
	// Ordinary Matirx
	AniRotMatrix = glm::rotate(glm::mat4(), glm::radians(lightbox_selfRotate), glm::vec3(0, 1, 0));
	modelTransformMatrix = transMatrix2*rotMatrix*transMatrix*AniRotMatrix*scaleMatrix;
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glUniform1i(tempLocation, 2);
	
	glDrawArrays(GL_TRIANGLES, 0, lightbox_size);

	// Draw the third lightbox
	scaleMatrix = glm::scale(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f));
	// Need to extract the lightbox position for locating the light source
	transMatrix = glm::translate(glm::mat4(), glm::vec3(lightbox_required_movement + zMouse, 0.0f + yMouse, 0.0f + xMouse));
	tempMatrix = transMatrix*glm::vec4(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
	lightx = tempMatrix.x;
	lighty = tempMatrix.y;
	lightz = tempMatrix.z;
	lightbox_position3 = glm::vec3(lightx, lighty, lightz);
	// Ordinary Matirx
	AniRotMatrix = glm::rotate(glm::mat4(), glm::radians(lightbox_selfRotate), glm::vec3(0, 1, 0));
	modelTransformMatrix = transMatrix*scaleMatrix;
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glUniform1i(tempLocation, 3);

	glDrawArrays(GL_TRIANGLES, 0, lightbox_size);

	if (lightbox_required_movement > 100 && moveDirection == 1) {
		lightbox_required_movement -= 0.1;
		moveDirection = 0;
	}
	else
	if (lightbox_required_movement >= 30 && moveDirection == 0) {
		lightbox_required_movement -= 0.1;
	}
	else
	if (lightbox_required_movement >= 30 && moveDirection == 1) {
		lightbox_required_movement += 0.1;
	}
	else
	if (lightbox_required_movement < 30 && moveDirection == 0) {
		lightbox_required_movement += 0.1;
		moveDirection = 1;
	}

	lightbox_selfRotate += 0.1;
	lightbox_rotate += 0.1;
	// Avoid Overflow for long run
	if (lightbox_selfRotate > 360)
		lightbox_selfRotate -= 360;
	if (lightbox_rotate > 360)
		lightbox_rotate -= 360;
	

	// Draw Earth
	glUseProgram(generalID);

	// Light information 
	lightPositionWorldLocation = glGetUniformLocation(generalID, "lightPositionWorld");
	glUniform3f(lightPositionWorldLocation, lightbox_position.x, lightbox_position.y, lightbox_position.z);
	lightPositionWorldLocation = glGetUniformLocation(generalID, "lightPositionWorld2");
	glUniform3f(lightPositionWorldLocation, lightbox_position2.x, lightbox_position2.y, lightbox_position2.z);
	lightPositionWorldLocation = glGetUniformLocation(generalID, "lightPositionWorld3");
	glUniform3f(lightPositionWorldLocation, lightbox_position3.x, lightbox_position3.y, lightbox_position3.z);
	selfIlluminated = glGetUniformLocation(generalID, "selfIlluminated");
	glUniform1i(selfIlluminated, trueUniverse);
	eyePositionLocation = glGetUniformLocation(generalID, "eyePositionWorld");
	glUniform3f(eyePositionLocation, camLocation.x, camLocation.y, camLocation.z);
	normalMapping = glGetUniformLocation(generalID, "normalMapping_flag");
	if (trueUniverse == 1)
		glUniform1i(normalMapping, 0);
	else
		glUniform1i(normalMapping, 1);


	glBindVertexArray(planet_vao);
	scaleMatrix = glm::scale(glm::mat4(), glm::vec3(6.0f, 6.0f, 6.0f));
	transMatrix = glm::translate(glm::mat4(), glm::vec3(50.0f+ zMouse, 2.0f+yMouse, -40.0f+xMouse));
	rotMatrix = glm::rotate(glm::mat4(), glm::radians(180.0f), glm::vec3(1, 0, 0));
	rotMatrix2 = glm::rotate(glm::mat4(), glm::radians(23.4f), glm::vec3(-1, 0, 0));
	rotMatrix3 = glm::rotate(glm::mat4(), glm::radians(23.4f), glm::vec3(0, 0, -1));
	AniRotMatrix = glm::rotate(glm::mat4(), glm::radians(earth_selfRotate), glm::vec3(0, 1, 0));
	modelTransformMatrix = transMatrix*rotMatrix3*rotMatrix2*AniRotMatrix*rotMatrix*scaleMatrix;
	modelTransformMatrixUniformLocation = glGetUniformLocation(generalID, "modelTransformMatrix");
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	viewMatrixUniformLocation = glGetUniformLocation(generalID, "viewMatrix");
	glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	projectionMatrixUniformLocation = glGetUniformLocation(generalID, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, &projMatrix[0][0]);
	tempLocation = glGetUniformLocation(generalID, "multipleTexture");
	glUniform1i(tempLocation, 0);

	textureLocation = glGetUniformLocation(generalID, "textureSampler1");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earth_texture[0]);
	glUniform1i(textureLocation, 0);
	textureLocation = glGetUniformLocation(generalID, "textureSampler2");
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, earth_texture[1]);
	glUniform1i(textureLocation, 1);

	glDrawArrays(GL_TRIANGLES, 0, planet_size);
	earth_selfRotate += 0.01;
	// Avoid Overflow for long run
	if (earth_selfRotate > 360)
		earth_selfRotate -= 360;

	// Draw Moon
	glUseProgram(generalID);
	// Light information 
	lightPositionWorldLocation = glGetUniformLocation(generalID, "lightPositionWorld");
	glUniform3f(lightPositionWorldLocation, lightbox_position.x, lightbox_position.y, lightbox_position.z);
	lightPositionWorldLocation = glGetUniformLocation(generalID, "lightPositionWorld2");
	glUniform3f(lightPositionWorldLocation, lightbox_position2.x, lightbox_position2.y, lightbox_position2.z);
	lightPositionWorldLocation = glGetUniformLocation(generalID, "lightPositionWorld3");
	glUniform3f(lightPositionWorldLocation, lightbox_position3.x, lightbox_position3.y, lightbox_position3.z);
	selfIlluminated = glGetUniformLocation(generalID, "selfIlluminated");
	glUniform1i(selfIlluminated, trueUniverse);
	eyePositionLocation = glGetUniformLocation(generalID, "eyePositionWorld");
	glUniform3f(eyePositionLocation, camLocation.x, camLocation.y, camLocation.z);
	normalMapping = glGetUniformLocation(generalID, "normalMapping_flag");
	glUniform1i(normalMapping, 0);

	glBindVertexArray(planet_vao);
	scaleMatrix = glm::scale(glm::mat4(), glm::vec3(2.0f, 2.0f, 2.0f));
	rotMatrix = glm::rotate(glm::mat4(), glm::radians(180.0f), glm::vec3(1, 0, 0));
	transMatrix = glm::translate(glm::mat4(), glm::vec3(50.0f+ zMouse, 2.0f+yMouse, -40.0f+xMouse));
	transMatrix2 = glm::translate(glm::mat4(), glm::vec3(18.0f, 0.0f, 0.0f));
	rotMatrix2 = glm::rotate(glm::mat4(), glm::radians(moon_Rotate), glm::vec3(0, 1, 0));
	rotMatrix3 = glm::rotate(glm::mat4(), glm::radians(5.145f), glm::vec3(0, 0, -1));
	AniRotMatrix = glm::rotate(glm::mat4(), glm::radians(moon_selfRotate), glm::vec3(0, 1, 0));
	modelTransformMatrix = transMatrix* rotMatrix3*rotMatrix2* transMatrix2* AniRotMatrix*rotMatrix*scaleMatrix;
	modelTransformMatrixUniformLocation = glGetUniformLocation(generalID, "modelTransformMatrix");
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	viewMatrixUniformLocation = glGetUniformLocation(generalID, "viewMatrix");
	glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	projectionMatrixUniformLocation = glGetUniformLocation(generalID, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, &projMatrix[0][0]);
	tempLocation = glGetUniformLocation(generalID, "multipleTexture");
	glUniform1i(tempLocation, 0);

	textureLocation = glGetUniformLocation(generalID, "textureSampler1");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, moon_texture);
	glUniform1i(textureLocation, 0);

	glDrawArrays(GL_TRIANGLES, 0, planet_size);
	moon_selfRotate += 0.0025;
	moon_Rotate += 0.1;
	// Avoid Overflow for long run
	if (moon_Rotate > 360) {
		moon_Rotate = 0;
		moon_selfRotate = 0.1*100/0.1;
	}

	// Draw Wired Planet
	glUseProgram(generalID);
	// Light information 
	lightPositionWorldLocation = glGetUniformLocation(generalID, "lightPositionWorld");
	glUniform3f(lightPositionWorldLocation, lightbox_position.x, lightbox_position.y, lightbox_position.z);
	lightPositionWorldLocation = glGetUniformLocation(generalID, "lightPositionWorld2");
	glUniform3f(lightPositionWorldLocation, lightbox_position2.x, lightbox_position2.y, lightbox_position2.z);
	lightPositionWorldLocation = glGetUniformLocation(generalID, "lightPositionWorld3");
	glUniform3f(lightPositionWorldLocation, lightbox_position3.x, lightbox_position3.y, lightbox_position3.z);
	selfIlluminated = glGetUniformLocation(generalID, "selfIlluminated");
	glUniform1i(selfIlluminated, trueUniverse);
	eyePositionLocation = glGetUniformLocation(generalID, "eyePositionWorld");
	glUniform3f(eyePositionLocation, camLocation.x, camLocation.y, camLocation.z);
	normalMapping = glGetUniformLocation(generalID, "normalMapping_flag");
	glUniform1i(normalMapping, 0);

	glBindVertexArray(planet_vao);
	scaleMatrix = glm::scale(glm::mat4(), glm::vec3(4.0f, 4.0f, 4.0f));
	rotMatrix = glm::rotate(glm::mat4(), glm::radians(72.3f), glm::vec3(1, 0, 0));
	transMatrix = glm::translate(glm::mat4(), glm::vec3(50.0f+ zMouse, 2.0f+yMouse, 40.0f+xMouse));
	rotMatrix2 = glm::rotate(glm::mat4(), glm::radians(40.3f), glm::vec3(0, 0, 1));
	AniRotMatrix = glm::rotate(glm::mat4(), glm::radians(planet_selfRotate), glm::vec3(0, 1, 0));
	modelTransformMatrix = transMatrix* rotMatrix2* AniRotMatrix*rotMatrix*scaleMatrix;
	modelTransformMatrixUniformLocation = glGetUniformLocation(generalID, "modelTransformMatrix");
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	viewMatrixUniformLocation = glGetUniformLocation(generalID, "viewMatrix");
	glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	projectionMatrixUniformLocation = glGetUniformLocation(generalID, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, &projMatrix[0][0]);
	tempLocation = glGetUniformLocation(generalID, "multipleTexture");
	glUniform1i(tempLocation, 1);

	textureLocation = glGetUniformLocation(generalID, "textureSampler1");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, planet2_texture[0]);
	glUniform1i(textureLocation, 0);
	textureLocation = glGetUniformLocation(generalID, "textureSampler2");
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, planet2_texture[1]);
	glUniform1i(textureLocation, 1);
	textureLocation = glGetUniformLocation(generalID, "textureSampler3");
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, planet2_texture[2]);
	glUniform1i(textureLocation, 2);

	planet_selfRotate += 1;
	if (planet_selfRotate > 360) {
		planet_selfRotate -= 360;
	}

	glDrawArrays(GL_TRIANGLES, 0, planet_size);
	
	// Draw ring
	glUseProgram(rockID);
	// Light information 
	lightPositionWorldLocation = glGetUniformLocation(rockID, "lightPositionWorld");
	glUniform3f(lightPositionWorldLocation, lightbox_position.x, lightbox_position.y, lightbox_position.z);
	lightPositionWorldLocation = glGetUniformLocation(generalID, "lightPositionWorld2");
	glUniform3f(lightPositionWorldLocation, lightbox_position2.x, lightbox_position2.y, lightbox_position2.z);
	lightPositionWorldLocation = glGetUniformLocation(generalID, "lightPositionWorld3");
	glUniform3f(lightPositionWorldLocation, lightbox_position3.x, lightbox_position3.y, lightbox_position3.z);
	eyePositionLocation = glGetUniformLocation(rockID, "eyePositionWorld");
	glUniform3f(eyePositionLocation, camLocation.x, camLocation.y, camLocation.z);
	selfIlluminated = glGetUniformLocation(rockID, "selfIlluminated");
	glUniform1i(selfIlluminated, trueUniverse);
	normalMapping = glGetUniformLocation(rockID, "normalMapping_flag");
	glUniform1i(normalMapping, 0);

	glBindVertexArray(rock_vao);
	textureLocation = glGetUniformLocation(rockID, "textureSampler1");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rock_texture);
	glUniform1i(textureLocation, 0);
	
	
	glBindVertexArray(rock_vao);
	projMatrix = glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
	rotMatrix = glm::rotate(glm::mat4(), glm::radians(rockRotate), glm::vec3(0, 1, 0));
	rotMatrix2 = glm::rotate(glm::mat4(), glm::radians(50.0f), glm::vec3(0, 0, 1));
	transMatrix = glm::translate(glm::mat4(), glm::vec3(50.0f + zMouse, 2.0f + yMouse, 40.0f + xMouse));
	modelTransformMatrix = transMatrix*rotMatrix2*rotMatrix;
	modelTransformMatrixUniformLocation = glGetUniformLocation(rockID, "modelTransformMatrix");
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	viewMatrixUniformLocation = glGetUniformLocation(generalID, "viewMatrix");
	glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	projectionMatrixUniformLocation = glGetUniformLocation(generalID, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, &projMatrix[0][0]);
	glDrawArraysInstanced(GL_TRIANGLES, 0, rock_size, numberOfRock);
	rockRotate+= 0.05;

	scaleMatrix = glm::scale(glm::mat4(), glm::vec3(0.4f, 0.4f, 0.4f));
	rotMatrix = glm::rotate(glm::mat4(), glm::radians(180.0f), glm::vec3(0, 0, 1));
	if (spaceshipOnEarth == 1)
		spaceshipInEarth();
	else
		spaceshipInPlanet();
	if (spaceCam == 0) {
		// Draw spaceship
		glUseProgram(generalID);

		// Light information 
		lightPositionWorldLocation = glGetUniformLocation(generalID, "lightPositionWorld");
		glUniform3f(lightPositionWorldLocation, lightbox_position.x, lightbox_position.y, lightbox_position.z);
		lightPositionWorldLocation = glGetUniformLocation(generalID, "lightPositionWorld2");
		glUniform3f(lightPositionWorldLocation, lightbox_position2.x, lightbox_position2.y, lightbox_position2.z);
		lightPositionWorldLocation = glGetUniformLocation(generalID, "lightPositionWorld3");
		glUniform3f(lightPositionWorldLocation, lightbox_position3.x, lightbox_position3.y, lightbox_position3.z);
		selfIlluminated = glGetUniformLocation(generalID, "selfIlluminated");
		glUniform1i(selfIlluminated, trueUniverse);
		eyePositionLocation = glGetUniformLocation(generalID, "eyePositionWorld");
		glUniform3f(eyePositionLocation, camLocation.x, camLocation.y, camLocation.z);
		normalMapping = glGetUniformLocation(generalID, "normalMapping_flag");
		glUniform1i(normalMapping, 0);

		glBindVertexArray(spaceship_vao);
		modelTransformMatrixUniformLocation = glGetUniformLocation(generalID, "modelTransformMatrix");
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
		viewMatrixUniformLocation = glGetUniformLocation(generalID, "viewMatrix");
		glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, &viewMatrix[0][0]);
		projectionMatrixUniformLocation = glGetUniformLocation(generalID, "projectionMatrix");
		glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, &projMatrix[0][0]);

		textureLocation = glGetUniformLocation(generalID, "textureSampler1");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, spaceship_texture);
		glUniform1i(textureLocation, 0);

		glDrawArrays(GL_TRIANGLES, 0, spaceship_size);
	}
	else {
		camLocation = glm::vec3(modelTransformMatrix*glm::vec4(spaceLocation, 1.0f));
		camLocation_x = camLocation.x;
		camLocation_y = camLocation.y;
		camLocation_z = camLocation.z;
		if (spaceTravel == 1) {
			camTarget_y = camLocation_y;
			camTarget_x = camLocation_x - sin(glm::radians(spaceship_Rotate-90.0f));
			camTarget_z = camLocation_z - cos(glm::radians(spaceship_Rotate-90.0f));
		}
		else {
			camTarget_y = camLocation_y;
			camTarget_x = camLocation_x - sin(glm::radians(spaceship_Rotate ));
			camTarget_z = camLocation_z - cos(glm::radians(spaceship_Rotate));
		}
	}

	glFlush(); //force execution of GL commands
	glutPostRedisplay();
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(1024, 512);
	glutCreateWindow(argv[0]);
	glutSetCursor(GLUT_CURSOR_NONE);

	initializedGL();

	glutDisplayFunc(paintGL);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(move);
	glutPassiveMotionFunc(PassiveMouse);
	glutMouseFunc(mouse_scroll);

	glutMainLoop(); //call display callback over and over
}