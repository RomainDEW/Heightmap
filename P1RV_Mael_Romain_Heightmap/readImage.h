#pragma once
#include <Windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>
#include <SOIL.h>
#include<string>
#include<vector>
#include<SOIL.h>


using namespace std;

//Lecture d'image par SOIL, retourne un tableau d'entiers � 2 dimensions contenant les niveaux de gris de chaque pixel. La derni�re ligne donne la largeur puis la hauteur de l'image
vector<vector<int>> readImage(const char* nomFichier);