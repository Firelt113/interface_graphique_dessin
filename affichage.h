#pragma once
#include "simulate.h"

#define DECALAGE_Y 3
#define DECALAGE_X 1
#define GRANDEUR_TABLE_COULEUR 256
#define LARGEUR_POINT_TABLE_COULEUR 5
#define LARGEUR_DESSIN 64
#define HAUTEUR_DESSIN 64

#define RAMPLIE 0x000000
#define VIDE 0x000001

#define WHITE 0xffffff
#define RED 0xff0000
#define GREEN 0x00ff00
#define BLUE 0x0000ff

#define is_Colision_On_Square(x1,x2,y1, y2,x,y)((x <= x2 && x >= x1 && y <= y2 && y >= y1)? 1 : 0)

typedef struct BUFFER {
	;
	void* memory;
	int width;
	int height;
}BUFFER;

//0x000000 => VIDE
//0x000000 => RAMPLIE
typedef int RAMPLISSAGE;

void ClearScreen(BUFFER buffer,unsigned int couleur);

int OuDansBuffer(BUFFER buffer, int x, int y);

void EcrireDansBuffer(BUFFER buffer, int PosPixel, unsigned int couleur);

unsigned int QuoiDansBuffer(BUFFER buffer, int PosPixel);

void Line(BUFFER buffer, int x1, int y1, int x2, int y2, unsigned int couleur);

void rectangle(BUFFER buffer ,RAMPLISSAGE ramplissage, int x1, int y1, int x2, int y2, unsigned int couleur);

void cercle(BUFFER buffer, int x, int y, int r, unsigned int couleur);

//-------------AFFICHAGE J-----------------------//

void ecrire_point(BUFFER buffer, Image image, tagPOINT point, unsigned int couleur);

void ecrire_point_sans_image(BUFFER buffer, tagPOINT point, unsigned int couleur);

void afficher_palette(BUFFER buffer, int x_point, int y_point, tagRECT rectanglee);

static unsigned int gradiant_couleur_palette(int i, int j);

bool pot_de_peinture(BUFFER buffer, Image image, tagPOINT pt, unsigned int couleur_final);