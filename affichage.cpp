#include "affichage.h"
#include <math.h>

void ClearScreen(BUFFER buffer, unsigned int couleur)
{
	unsigned int* pixel = (unsigned int*)buffer.memory;
	for (int y = 0; y < buffer.height; y++) {
		for (int x = 0; x < buffer.width; x++) {
			*pixel++ = couleur;
		}
	}
}

int OuDansBuffer(BUFFER buffer, int x, int y)
{
	if (x >= buffer.width || y >= buffer.height)
		return -1;
	return (y * buffer.width) + x;
}

void EcrireDansBuffer(BUFFER buffer, int PosPixel, unsigned int couleur)
{
	if (PosPixel < buffer.height * buffer.width && PosPixel > 0) {
		unsigned int* pixel = (unsigned int*)buffer.memory + PosPixel;
		*pixel = couleur;
	}
}

unsigned int QuoiDansBuffer(BUFFER buffer, int PosPixel)
{
	if (PosPixel < buffer.width * buffer.height && PosPixel >= 0) {
		unsigned int* pixel = (unsigned int*)buffer.memory + PosPixel;
		return *pixel;
	}
	return 0;
}

void Line(BUFFER buffer, int x1, int y1, int x2, int y2, unsigned int couleur)
{
	if (abs(x1 - x2) >= abs(y1 - y2))
	{
		if (x2 - x1 > 0)
		{
			double a = ((double)y2 - (double)y1) / ((double)x2 - (double)x1);

			double b = (double)y2 - (a * (double)x2);

			for (int x = x1; x < x2; x++)
			{
				int y = a * x + b;
				if (y < buffer.height && x < buffer.width)
					EcrireDansBuffer(buffer, OuDansBuffer(buffer, x, y), couleur);
			}
		}
		else if (x1 - x2 > 0)
		{
			double a;
			if (x1 - x2 == 0)
				a = buffer.height;

			else
				a = (double)y1 - (double)y2 / ((double)x1 - (double)x2);

			double b = (double)y2 - (a * (double)x2);

			for (int x = x2; x < x1; x++)
			{
				int y = a * x + b;
				if (y < buffer.height && x < buffer.width)
					EcrireDansBuffer(buffer, OuDansBuffer(buffer, x, y), couleur);
			}

		}
	}
	else      //avec y comme independante
	{
		if (y2 - y1 > 0)
		{
			double a = ((double)x2 - (double)x1) / ((double)y2 - (double)y1);

			double b = (double)x2 - (a * (double)y2);

			for (int y = y1; y < y2; y++)
			{
				int x = a * y + b;
				if (y < buffer.height && x < buffer.width && y > 0 && x > 0)
					EcrireDansBuffer(buffer, OuDansBuffer(buffer, x, y), couleur);
			}
		}
		else if (y1 - y2 > 0)
		{
			double a = (double)x1 - (double)x2 / ((double)y1 - (double)y2);

			double b = (double)x2 - (a * (double)y2);

			for (int y = y2; y < y1; y++)
			{
				int x = a * y + b;
				if (y < buffer.height && x < buffer.width && y > 0 && x > 0)
					EcrireDansBuffer(buffer, OuDansBuffer(buffer, x, y), couleur);
			}

		}
	}
	
}

void rectangle(BUFFER buffer, RAMPLISSAGE ramplissage, int x1, int y1, int x2, int y2, unsigned int couleur)
{
	if (ramplissage == VIDE) {
		Line(buffer, x1, y1, x1, y2, couleur);
		Line(buffer, x1, y1, x2, y1, couleur);
		Line(buffer, x2, y1, x2, y2, couleur);
		Line(buffer, x1, y2, x2, y2, couleur);
	}
	else if(ramplissage == RAMPLIE)
	{
		unsigned int* pixel = (unsigned int*)buffer.memory;
		for (int y = y1; y < y2; y++) {
			for (int x = x1; x < x2; x++) {
				EcrireDansBuffer(buffer, OuDansBuffer(buffer, x, y), couleur);
			}
		}
	}
}

void cercle(BUFFER buffer, int x, int y, int r, unsigned int couleur)
{
	for (int i = x - r; i < x + r; i++)
		for (int j = y - r; j < y + r; j++)
		{
			if (sqrt(pow(j - y, 2) + pow(i - x, 2)) <= r)
				EcrireDansBuffer(buffer, OuDansBuffer(buffer, i, j), couleur);
		}
}

//====================Dessin===========================

void ecrire_point(BUFFER buffer, Image image, tagPOINT point, unsigned int couleur)
{
	int y = buffer.height - point.y;

	/*if (point.x >= buffer.width - image.pixwide)
		point.x = buffer.width - image.pixwide;
	else if (point.x <= 0)
		point.x = 1;
	if (y <= 1)
		y = 2;
	else if (buffer.height- (image.pixheigt * DECALAGE_Y+1) <= y)
		y = buffer.height - (image.pixheigt * (DECALAGE_Y+1))-1;
	*/
	/* le -1 et +3 ajuste lecriture exactement au bout du curseure */
	int x = (point.x / image.pixwide)-1;	
	y = (y / image.pixheigt)+3;

	if (x >= 0 && y >= 0 && x < LARGEUR_DESSIN && y < HAUTEUR_DESSIN) {
		image.tab[y][x] = couleur;

		rectangle(buffer, RAMPLIE, x * image.pixwide, y * image.pixheigt, x * image.pixwide + image.pixwide , y * image.pixheigt + image.pixheigt , couleur);
	}
}

void ecrire_point_sans_decalage(BUFFER buffer, Image image, tagPOINT point, unsigned int couleur)
{
	

	int x = (point.x );
	int y = (point.y );

	if (x >= 0 && y >= 0 && x < LARGEUR_DESSIN && y < HAUTEUR_DESSIN) {
		image.tab[y][x] = couleur;

		rectangle(buffer, RAMPLIE, x * image.pixwide, y * image.pixheigt, x * image.pixwide + image.pixwide , y * image.pixheigt + image.pixheigt, couleur);
	}
}

void ecrire_point_sans_image(BUFFER buffer, tagPOINT point, unsigned int couleur) {

	rectangle(buffer, RAMPLIE, point.x - LARGEUR_POINT_TABLE_COULEUR, point.y - LARGEUR_POINT_TABLE_COULEUR,
		point.x + LARGEUR_POINT_TABLE_COULEUR, point.y + LARGEUR_POINT_TABLE_COULEUR, couleur);

	rectangle(buffer, VIDE, point.x - LARGEUR_POINT_TABLE_COULEUR, point.y - LARGEUR_POINT_TABLE_COULEUR,
		point.x + LARGEUR_POINT_TABLE_COULEUR, point.y + LARGEUR_POINT_TABLE_COULEUR, 0);
}

static unsigned int gradiant_couleur_palette(int i, int j) {
	unsigned int blue, green, red;
	
	if (i >= 0 && i < 64) {
		i > 0 ? blue = i * 4 - 1 : blue = 0;
		green = 0;
		red = j;
	}
	if (i >= 64 && i < 128) {
		blue = 255;
		i > 64 ? green = (i - 64) * 4 - 1 : green = 0;
		red = j;
	}
	if (i >= 128 && i < 192) {
		blue = 255 - ((i - 128) * 4);
		if (blue < 0) { blue = 0; }
		green = 255;
		red = j;
	}
	if (i >= 192 && i < 256) {
		blue = 0;
		green = 255 - ((i - 192) * 4);
		if (green < 0) { green = 0; }
		red = j;
	}
	green = green * 0x000100;
	red = red * 0x010000;

	return blue + green + red;
}

void afficher_palette(BUFFER buffer,int x_point, int y_point, tagRECT rectanglee) {
	
	double couleur_palette;
	int green, blue, red;

	for (int j = 0; j < GRANDEUR_TABLE_COULEUR;j++)
		for (int i = 0; i < GRANDEUR_TABLE_COULEUR; i++) {

			couleur_palette = gradiant_couleur_palette(i,j);
			EcrireDansBuffer(buffer, OuDansBuffer(buffer, rectanglee.left + i,buffer.height - GRANDEUR_TABLE_COULEUR + j), couleur_palette);
		}
}

bool pot_de_peinture(BUFFER buffer, Image image, tagPOINT pt, unsigned int couleur_final) {
	int maxy = pt.y, count;
	int couleur = image.tab[pt.y][pt.x];
	
	if (couleur == couleur_final) {
		return true;
	}

	tagPOINT ptList[4096];
	tagPOINT ptListCopie[4096];

	ptListCopie[0] = pt;
	
	int indexMax = 1;
	do {
		count = 0;
		for (int i = 0; i < indexMax; i++) {
			
			if (ptListCopie[i].y > 0 && image.tab[ptListCopie[i].y - 1][ptListCopie[i].x] == couleur) {
				tagPOINT pointtmp;
				pointtmp.x = ptListCopie[i].x; pointtmp.y = ptListCopie[i].y - 1;
				ptList[count] = pointtmp;
				ecrire_point_sans_decalage(buffer, image, pointtmp, couleur_final);
				count++;
			}
			if (ptListCopie[i].x > 0 && image.tab[ptListCopie[i].y][ptListCopie[i].x - 1] == couleur) {
				tagPOINT pointtmp;
				pointtmp.x = ptListCopie[i].x - 1; pointtmp.y = ptListCopie[i].y;
				ptList[count] = pointtmp;
				ecrire_point_sans_decalage(buffer, image, pointtmp, couleur_final);
				count++;
			}
			if (ptListCopie[i].y < HAUTEUR_DESSIN && image.tab[ptListCopie[i].y + 1][ptListCopie[i].x] == couleur) {
				tagPOINT pointtmp;
				pointtmp.x = ptListCopie[i].x; pointtmp.y = ptListCopie[i].y + 1;
				ptList[count] = pointtmp;
				ecrire_point_sans_decalage(buffer, image, pointtmp, couleur_final);
				count++;
			}
			if (ptListCopie[i].x < LARGEUR_DESSIN && image.tab[ptListCopie[i].y][ptListCopie[i].x + 1] == couleur) {
				tagPOINT pointtmp;
				pointtmp.x = ptListCopie[i].x + 1; pointtmp.y = ptListCopie[i].y;
				ptList[count] = pointtmp;
				ecrire_point_sans_decalage(buffer, image, pointtmp, couleur_final);
				count++;
			}
		}
		for (int i = 0; i < count; i++) {
			ptListCopie[i] = ptList[i];
		}
		indexMax = count;
	} while (count > 0);
	
	return true;
}