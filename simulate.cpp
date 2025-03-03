#include "simulate.h"
#include<stdlib.h>

static int** cree_tableau(void)
{
	int** tab = (int**)malloc(WINHIGHT * sizeof(int*));

	for (int  i = 0; i < WINHIGHT; i++)
	{
		tab[i] = (int*)malloc(WINWIDE * sizeof(int));
	} 

	return tab;
}

Image cree_image(int pix_whide, int pix_heigt)
{
	Image image;
	image.pixheigt = pix_heigt;
	image.pixwide = pix_whide;
	image.tab = cree_tableau();
	return image;
}

bool colision_point_carrer(tagRECT rect, tagPOINT point) {

	if (point.x > rect.left && point.x < rect.right)
		if (point.y < rect.bottom && point.y > rect.top)
			return true;
	return false;
}

bool enchainer(ChaineStack chaine, Noeud* new_noeud) {
	chaine.premier->suivant = new_noeud;
	chaine.premier = new_noeud;
	return true;
}

Noeud dechainer(ChaineStack chaine) {

	Noeud noeud = *chaine.premier;
	free(chaine.premier);
	return noeud;
}