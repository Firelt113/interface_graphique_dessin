#pragma once
#include <Windows.h>

#define  WINHIGHT 1919
#define  WINWIDE 1008

typedef struct Image {
	int** tab;
	int pixheigt;
	int pixwide;
};

typedef struct Noeud {
	Noeud* suivant;
	void* donner;
};

typedef struct ChaineStack {
	Noeud* premier;
	int nb_noeud;
};

static int** cree_tableau(void);

Image cree_image(int pix_whide, int pix_heigt);

bool colision_point_carrer(tagRECT rect, tagPOINT point);

bool enchainer(ChaineStack chaine, Noeud* new_noeud);

Noeud dechainer(ChaineStack chaine);