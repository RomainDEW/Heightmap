/*

main.cpp
P1RV : Heightmap

Maël CHAKMA, Romain DEWULF


*/

// Inclusion de GLUT
#include <Windows.h>
#include <iostream>
#include <GL/gl.h>
#include <glcorearb.h>
#include <GL/glut.h>
#include <SOIL.h>
#include <vector>

// Inclusion des bibliothèques personnelles
#include "readImage.h"
#include "vertex.h"
#include "globals.h"

// Inclusion des autres bibliothèques nécessaires
#include "vector3D.h" //Pour les vecteurs 3D

// Concernant la lecture de l'image

// Données de l'image, tableau d'entiers à 2 dimensions contenant les niveaux de gris de chaque pixel. La dernière ligne contient la largeur puis la hauteur de l'image.
vector<vector<int>> imageData;
// Tableau de points à 2 dimensions, contient position dans l'espace et couleur de chaque point
vector<vector<vertex>> tableauPoints{};
int largeur;
int hauteur;
float coeffL;
float coeffH;


// Déclarations des fonctions de rappel (callbacks)
GLvoid affichage();
GLvoid clicSouris(int bouton, int etat, int x, int y);
GLvoid deplacementSouris(int x, int y);
GLvoid redimensionner(int w, int h);

// Deplacement de la souris
void deplacementCamera(GLfloat anglePhi, GLfloat angleTheta) {
	// set new camera  coordinates
	camPos.setVx(cos(anglePhi));
	camPos.setVy(sin(anglePhi));
	camPos.setVz(cos(angleTheta));
	camPos.normalize();
	camPos = camPos * 1.2;
}

// Callback d'affichage
GLvoid affichage() {
	//deplacement
	deplacementCamera(anglePhi, angleTheta);

	// Effacement du frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Passage en mode ModelView
	glMatrixMode(GL_MODELVIEW);

	// Chargement de l'identité
	glLoadIdentity();

	// Placement de la caméra
	gluLookAt(camPos.getVx(), camPos.getVy(), camPos.getVz(),
		0, 0, 0,
		0, 0, 1);

	//Dessin de la Heightmap
	for (int i = 0; i < hauteur - 1; i++) {   // On ne fait pas d'affichage de triangles en utilisant la dernière ligne de points car on créé uniquement les triangles "sous" le point considéré
		for (int j = 0; j < largeur; j++) {
			if (j != largeur - 1) {
				glBegin(GL_TRIANGLES);
				glColor3f(tableauPoints[i][j].l, tableauPoints[i][j].l, tableauPoints[i][j].l);
				glVertex3f(tableauPoints[i][j].x, tableauPoints[i][j].y, tableauPoints[i][j].z);
				glColor3f(tableauPoints[i + 1][j].l, tableauPoints[i + 1][j].l, tableauPoints[i + 1][j].l);
				glVertex3f(tableauPoints[i + 1][j].x, tableauPoints[i + 1][j].y, tableauPoints[i + 1][j].z);
				glColor3f(tableauPoints[i][j + 1].l, tableauPoints[i][j + 1].l, tableauPoints[i][j + 1].l);
				glVertex3f(tableauPoints[i][j + 1].x, tableauPoints[i][j + 1].y, tableauPoints[i][j + 1].z);
				glEnd();
			}
			if (j != 0) {
				glBegin(GL_TRIANGLES);
				glColor3f(tableauPoints[i][j].l, tableauPoints[i][j].l, tableauPoints[i][j].l);
				glVertex3f(tableauPoints[i][j].x, tableauPoints[i][j].y, tableauPoints[i][j].z);
				glColor3f(tableauPoints[i + 1][j - 1].l, tableauPoints[i + 1][j - 1].l, tableauPoints[i + 1][j - 1].l);
				glVertex3f(tableauPoints[i + 1][j - 1].x, tableauPoints[i + 1][j - 1].y, tableauPoints[i + 1][j - 1].z);
				glColor3f(tableauPoints[i + 1][j].l, tableauPoints[i + 1][j].l, tableauPoints[i + 1][j].l);
				glVertex3f(tableauPoints[i + 1][j].x, tableauPoints[i + 1][j].y, tableauPoints[i + 1][j].z);
				glEnd();
			}
		}
	}
	// Affichage des côtés du bloc de relief
	float bas = -intensiteRelief / 2;
	// "fond" du bloc de relief
	glBegin(GL_QUADS);
	glColor4f(0.0, 0.0, 0.0, 0.5);
	glVertex3f(0.5 * coeffH, 0.5 * coeffL, bas);
	glVertex3f(0.5 * coeffH, - 0.5 * coeffL, bas);
	glVertex3f(- 0.5 * coeffH, - 0.5 * coeffL, bas);
	glVertex3f(- 0.5 * coeffH, 0.5 * coeffL, bas);
	glEnd();

	// côtés du bloc de relief
	glBegin(GL_POLYGON);
	glVertex3f(0.5 * coeffH, 0.5 * coeffL, bas);
	glVertex3f(0.5 * coeffH, -0.5 * coeffL, bas);
	for (int j = largeur - 1; j >= 0; j--) {
		glColor3f(tableauPoints[0][j].l, tableauPoints[0][j].l, tableauPoints[0][j].l); 
		glVertex3f(tableauPoints[0][j].x, tableauPoints[0][j].y, tableauPoints[0][j].z);
	}
	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(0.5 * coeffH, -0.5 * coeffL, bas);
	glVertex3f(-0.5 * coeffH, -0.5 * coeffL, bas);
	for (int j = hauteur - 1; j >= 0; j--) {
		glColor3f(tableauPoints[j][largeur - 1].l, tableauPoints[j][largeur - 1].l, tableauPoints[j][largeur - 1].l);
		glVertex3f(tableauPoints[j][largeur - 1].x, tableauPoints[j][largeur - 1].y, tableauPoints[j][largeur - 1].z);
	}
	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(- 0.5 * coeffH, 0.5 * coeffL, bas);
	glVertex3f(- 0.5 * coeffH, -0.5 * coeffL, bas);
	for (int j = largeur - 1; j >= 0; j--) {
		glColor3f(tableauPoints[hauteur - 1][j].l, tableauPoints[hauteur - 1][j].l, tableauPoints[hauteur - 1][j].l);
		glVertex3f(tableauPoints[hauteur - 1][j].x, tableauPoints[hauteur - 1][j].y, tableauPoints[hauteur - 1][j].z);
	}
	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(0.5 * coeffH, 0.5 * coeffL, bas);
	glVertex3f(-0.5 * coeffH, 0.5 * coeffL, bas);
	for (int j = hauteur - 1; j >= 0; j--) {
		glColor3f(tableauPoints[j][0].l, tableauPoints[j][0].l, tableauPoints[j][0].l);
		glVertex3f(tableauPoints[j][0].x, tableauPoints[j][0].y, tableauPoints[j][0].z);
	}
	glEnd();

	glFlush();
	glutSwapBuffers();
}

// Fonction de gestion du deplacement de la souris
void deplacementSouris(int x, int y) {

	// On ne fait quelque chose que si l'utilisateur
	// a deja clique quelque part avec le bouton gauche
	if (xMouse >= 0 || yMouse >= 0) {

		// mise a jour des deltas des angles theta et phi
		deltaPhi = (x - xMouse) * 3.14 / winW;
		deltaTheta = (y - yMouse) * 3.14 / winH;
		anglePhi = anglePhi - deltaPhi;
		angleTheta = angleTheta + deltaTheta;
		xMouse = x;
		yMouse = y;
	}
	glutPostRedisplay();
}

// Fonction de gestion des clics souris
void clicSouris(int button, int state, int x, int y) {

	// On ne fait quelque chose que sur le bouton gauche de la souris
	if (button == GLUT_LEFT_BUTTON) {
		// si on relache le bouton on met a jour les deltas d'angles theta et phi
		// et on dit que l'on a pas clique
		if (state == GLUT_UP) {
			deltaPhi = 0.0f;
			deltaTheta = 0.0f;
			xMouse = -1;
			yMouse = -1;
		}
		else {
		   // si l'on a clique sur le bouton gauche
		   // on garde les positions de la souris au moment du clic gauche
			xMouse = x;
			yMouse = y;
		}
	}
}

// Callback de redimensionnement de la fenêtre
GLvoid redimensionner(int w, int h) {
	// Garde les valeurs
	winW = w;
	winH = h;
	// eviter une division par 0
	if (winH == 0)
		winH = 1;

	float ratio = (float)winW / (float)winH;

	// Projection
	glMatrixMode(GL_PROJECTION);

	// Resetting matrix
	glLoadIdentity();

	// Viewport
	glViewport(0, 0, winW, winH);

	// Mise en place de la perspective
	gluPerspective(focale, ratio, nr, fr);

	// Retourne a la pile modelview
	glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char** argv) {
	// Initialisation de GLUT
	glutInit(&argc, argv);
	// Choix du mode d'affichage (ici RVB)
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	// Position initiale de la fenetre GLUT
	glutInitWindowPosition(200, 200);
	// Taille initiale de la fenetre GLUT
	glutInitWindowSize(winW, winH);
	// Creation de la fenetre GLUT
	glutCreateWindow("Heightmap");
	// Définition de la couleur d'effacement du framebuffer pour créer un "ciel"
	glClearColor(154.0/256, 204.0/256, 188.0/256, 0.5f);
	//// Affichage de points
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Activation du Zbuffer
	glEnable(GL_DEPTH_TEST);

	//// Correction de la perspective
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Paramètres de lecture
	const char* nomFichier = "C:\\Users\\Eleve\\source\\repos\\P1RV_Mael_Romain_Heightmap\\test2v3.png";   // Nom du fichier

	// Lecture de l'image
	imageData = readImage(nomFichier);

	// dimensions de l'image
	largeur = imageData.back()[0]; // largeur de l'image
	hauteur = imageData.back()[1]; // hauteur de l'image

	// dimensionnement de l'image: on affiche le plus petit des cotés avec une longueur de taille "tailleRelief"
	if (hauteur < largeur) {
		coeffH = tailleRelief;
		coeffL = largeur * tailleRelief / hauteur;
	}
	else {
		coeffL = tailleRelief;
		coeffH = hauteur * tailleRelief / largeur;
	}

	// Placement des points, on centre la Heightmap en (0, 0, 0)
	vector<vertex> lignePoints;
	float x;
	float y;
	float z;
	float l;
	for (int i = 0; i < hauteur; i++) {
		lignePoints = {};
		for (int j = 0; j < largeur; j++) {
			x = (0.5 - i * 1.0 / hauteur) * coeffH;  
			y = (0.5 - j * 1.0 / largeur) * coeffL;
			l = imageData[i][j] * 1.0 / 255;
			z = (l - 0.5) * intensiteRelief;
			lignePoints.push_back({ x, y, z, l });
		}
		tableauPoints.push_back(lignePoints);
	}

	// Définition des fonctions de callbacks
	glutDisplayFunc(affichage);
	glutMouseFunc(clicSouris);
	glutMotionFunc(deplacementSouris);
	glutReshapeFunc(redimensionner);

	// Lancement de la boucle infinie GLUT
	glutMainLoop();
	
	return 0;
}