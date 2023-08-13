
// Inclusion de librairies
#include<iostream>
#include<vector>
#include<SOIL.h>
#include "readImage.h"


//namespace
using namespace std;


vector<vector<int>> readImage(const char* nomFichier) {
	// Lecture de l'image via SOIL

	// Paramètres de SOIL:
	int force_channels = SOIL_LOAD_L;   // forcer le chargement en lightmap
	int* channels = new int;            // nombre de "canaux", i.e 
	int* width = new int;               // stockage de la largeur de l'image 
	int* height = new int;              // stockage de la hauteur de l'image

	// stockage des données de l'image sous forme d'un tableau de niveaux de gris
	unsigned char* imageLoad;           

	// appel à SOIL et récupération des données
	imageLoad = SOIL_load_image(nomFichier, width, height, channels, force_channels);

	// stockage des niveaux de gris dans un tableau à 2 dimensions pour l'affichage ultérieur
	vector<vector<int>> image{};
	vector<int> ligne;
	int pixel;
	for (int i = 0; i < *height; i++) {
		ligne = {};
		for (int j = 0; j < *width; j++) {
			pixel = *imageLoad;
			ligne.push_back(pixel);
			imageLoad++;
		}
		image.push_back(ligne);
	}
	ligne = {};
	ligne.push_back(*width);
	ligne.push_back(*height);
	image.push_back(ligne);
	return image;
}