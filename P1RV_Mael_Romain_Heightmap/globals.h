#pragma once

// Inclusion de GLUT
#include <Windows.h>
#include <iostream>
#include <GL/gl.h>
#include <glcorearb.h>
#include <GL/glut.h>
#include <SOIL.h>
#include <vector>

// Inclusion des autres librairies
#include "vertex.h"
#include "vector3D.h"

// Définition des variables globales
	// Concernant la fenêtre d'affichage
	int winW = 500;
	int winH = 400;
	float focale = 65.0f;
	float nr = 0.1f;
	float fr = 100.0f;
	float tailleRelief = 1.0;  // dimension du plus petit coté de la Heightmap
	float intensiteRelief = 0.3;  // heuteur max du relief

	// Concernant la camera et la vue
	Vector3D forwardView;
	Vector3D leftView;
	Vector3D upView(0.0, 0.0, 1.0);
	Vector3D camPos{ 1.2, 0.0, 0.0 };
	Vector3D tarPos{ 0.0f, 0.0f, 0.0f };
	GLfloat anglePhi = 0.0f;
	GLfloat angleTheta = 0.0f;
	GLfloat deltaPhi = 0.0f;
	GLfloat deltaTheta = 0.0f;
	GLint oldX = 0;
	GLint oldY = 0;
	GLboolean boutonClick = false;

// Concernant la position de la souris
int xMouse = -1;
int yMouse = -1;