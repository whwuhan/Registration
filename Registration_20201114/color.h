#pragma once

#include "properties.h"
#include <iostream>

typedef struct Color
{
	int R;
	int G;
	int B;

	Color();
	Color(int R, int G, int B);
} COLOR;

//ͨ��bias��ȡ��Ӧ����ɫ
Color get_color(Properties &prop ,float bias);