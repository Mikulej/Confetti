#ifndef IMAGE_H
#define IMAGE_H
#include <ctime>
#include <cstdlib>
#include <glad/glad.h>
#include <iostream>
#include <cmath>
#ifndef STBI
#define STBI
#include "stb_image.h"
#endif
#include "glm-master/glm/glm.hpp"
#include "glm-master/glm/gtc/matrix_transform.hpp"
#include "glm-master/glm/gtc/type_ptr.hpp"
#include <GLFW/glfw3.h>//tylko do testow
#define PATH_RANGE_B 0.45f //maks. wys. paraboli - zalecane 0.45f
#define PATH_RANGE_S 100.0f //skalar tj. rozpietosc paraboli - zalecane 100.0f
#define PATH_RANGE_T 10.0f //minimalna wartosc rozpietosci paraboli - zalecane 10.0f
#define PATH_SPEED 0.003f //szybkosc poruszania czastek w osi X - zalecane 0.003f
#define PATH_ROTATE 32 //szybkosc obracania czastek - zalecane 32
#define PI 3.14159265358979323846 
class Image {
public:
	glm::vec4 Pos;
	glm::vec4 Scale;
	glm::vec4 Color;
	float angle;
	class ImagePath {
	public:
		float a,b,s,kierunek,speed,x,originx,originy;
		ImagePath(float sx,float sy); //losowa sciezka
	};
	ImagePath path;
	unsigned int texture;
	Image(unsigned int _texture,float _x = 0, float _y = 0);
	int updatePath();
	void rand_color();
};
#endif