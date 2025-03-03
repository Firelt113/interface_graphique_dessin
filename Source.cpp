#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "affichage.h"
#include <iostream>
#include "bouton.h"
#include <stdint.h>

bool running = true;

BUFFER buffer;

BITMAPINFO buffer_bitmap_info;


LRESULT CALLBACK window_callback(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
) {
	LRESULT result = 0;		//resultat du traitement des user inpute
	switch (uMsg)
	{
	case WM_CLOSE:
	case WM_DESTROY: {
		running = false;

	}break;
	
	case WM_SIZE: {
		RECT rect;
		GetClientRect(hwnd, &rect);
		buffer.width = rect.right - rect.left;
		buffer.height = rect.bottom - rect.top;

		int buffer_size = buffer.width * buffer.height * sizeof(unsigned int);

		if (buffer.memory)
			VirtualFree(buffer.memory, 0, MEM_RELEASE);
		buffer.memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		buffer_bitmap_info.bmiHeader.biSize = sizeof(buffer_bitmap_info.bmiHeader);
		buffer_bitmap_info.bmiHeader.biWidth = buffer.width;
		buffer_bitmap_info.bmiHeader.biHeight = buffer.height;
		buffer_bitmap_info.bmiHeader.biPlanes = 1;
		buffer_bitmap_info.bmiHeader.biBitCount = 32;
		buffer_bitmap_info.bmiHeader.biCompression = BI_RGB;

	}break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}




int WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
) {
	//window class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = "Game Window classe";
	window_class.lpfnWndProc = window_callback;

	//regester it 
	RegisterClass(&window_class);
	
	//create window
	HWND window = CreateWindow(window_class.lpszClassName, "game",//<= nom au dessu de la fenetre
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);

	//==================Gestion des touches=======================

	Input input = { 0 };
	typedef long u32;

	//=========================================
	tagPOINT lpPoint;

	ClearScreen(buffer, 0xffffff);
	unsigned int couleur = 0xffffff;
	bool textout = false;

	Image image = cree_image(10,10);
	for(int j =0; j < HAUTEUR_DESSIN; j++)
		for (int i = 0; i < LARGEUR_DESSIN; i++)
		{
			image.tab[j][i] = 0x0000ff;
		}

	tagRECT rec_palette;

	FILE* add = fopen("dessin.txt", "w");

	tagRECT rect_window;

	tagPOINT point_indicateur_couleur = {buffer.width-(GRANDEUR_TABLE_COULEUR/2), buffer.height - (GRANDEUR_TABLE_COULEUR / 2) };
	
	
	while (running) {

		//inpute
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);

			switch (message.message)
			{
			case WM_KEYUP:case WM_KEYDOWN:
			{
				u32 vk_code = (u32)message.wParam;
				bool is_down = ((message.lParam & (1 << 31)) == 0);

				switch (vk_code)
				{
				case VK_UP:
				{
					input.button[BUTTON_UP].is_down = is_down;
					input.button[BUTTON_UP].changed = true;
				}break;
				case VK_DOWN:
				{
					input.button[BUTTON_DOWN].is_down = is_down;
					input.button[BUTTON_DOWN].changed = true;
				}break;
				case VK_RIGHT:
				{
					input.button[BUTTON_RIGHT].is_down = is_down;
					input.button[BUTTON_RIGHT].changed = true;
				}break;
				case VK_LEFT:
				{
					input.button[BUTTON_LEFT].is_down = is_down;
					input.button[BUTTON_LEFT].changed = true;
				}break;
				case 0x32: {
					input.button[BUTTON_2].is_down = is_down;
					input.button[BUTTON_2].changed = true;
				}break;
				case 0x33: {
					input.button[BUTTON_3].is_down = is_down;
					input.button[BUTTON_3].changed = true;
				}break;

				default:
					break;
				}
			}
			default:
				break;
			}
		}
		GetCursorPos(&lpPoint);
		
		GetWindowRect(window, &rect_window);

		//simulate
		
		rec_palette.left = buffer.width - GRANDEUR_TABLE_COULEUR;
		rec_palette.right = buffer.width ;
		rec_palette.bottom = GRANDEUR_TABLE_COULEUR;
		rec_palette.top = 0;

		lpPoint.x -= rect_window.left;
		lpPoint.y -= rect_window.top;

		if (input.button[BUTTON_2].is_down) {
			ecrire_point(buffer, image, lpPoint, couleur);
		}
		if (input.button[BUTTON_3].is_down) {
			tagPOINT pt;
			pt.x = lpPoint.x / image.pixwide;
			pt.y = lpPoint.y / image.pixheigt;
			pot_de_peinture(buffer, image, pt, couleur);
		}


    	afficher_palette(buffer, 1, 1, rec_palette);
		
		
		if (input.button[BUTTON_RIGHT].is_down && point_indicateur_couleur.x < buffer.width - LARGEUR_POINT_TABLE_COULEUR) {
			point_indicateur_couleur.x++;
			couleur = QuoiDansBuffer(buffer, OuDansBuffer(buffer, point_indicateur_couleur.x, point_indicateur_couleur.y));
		}
		if (input.button[BUTTON_LEFT].is_down && point_indicateur_couleur.x > buffer.width - GRANDEUR_TABLE_COULEUR + LARGEUR_POINT_TABLE_COULEUR) {
			point_indicateur_couleur.x--;
			couleur = QuoiDansBuffer(buffer, OuDansBuffer(buffer, point_indicateur_couleur.x, point_indicateur_couleur.y));
		}
		if (input.button[BUTTON_UP].is_down && point_indicateur_couleur.y < buffer.height - LARGEUR_POINT_TABLE_COULEUR) {
			point_indicateur_couleur.y++;
			couleur = QuoiDansBuffer(buffer, OuDansBuffer(buffer, point_indicateur_couleur.x, point_indicateur_couleur.y));
		}
		if (input.button[BUTTON_DOWN].is_down && point_indicateur_couleur.y > buffer.height - GRANDEUR_TABLE_COULEUR + LARGEUR_POINT_TABLE_COULEUR) {
			point_indicateur_couleur.y--;
			couleur = QuoiDansBuffer(buffer, OuDansBuffer(buffer, point_indicateur_couleur.x, point_indicateur_couleur.y));
		}

		ecrire_point_sans_image(buffer, point_indicateur_couleur, couleur);

		rectangle(buffer, VIDE, 0, 0,   LARGEUR_DESSIN * image.pixwide,  HAUTEUR_DESSIN * image.pixheigt, 0);
		
		//render
		StretchDIBits(hdc, 0, 0, buffer.width, buffer.height, 0, 0, buffer.width, buffer.height, buffer.memory, &buffer_bitmap_info, DIB_RGB_COLORS, SRCCOPY);
		
	}


	/* je vais devoire initialiser image.tab en blanc puis mettre if(image.tab[i][j] != 0xffffff) fprintf...*/
	for (int i = 0; i < (buffer.width / image.pixwide);i++)
		for (int j = 0; j < (buffer.height / image.pixheigt);j++)
			if (image.tab[i][j] >= 0)
				fprintf(add, "%d %d %d  ", j, i, image.tab[i][j]);
	
}