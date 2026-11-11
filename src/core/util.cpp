#include "util.h"
#include <iostream>
// Primitive helper utilities, clamping, and screen control

int clampInt(int val, int minVal, int maxVal)
{

	if (val > maxVal)
		return maxVal;
	if (val < minVal)
		return minVal;

	return val;

}

void trimWhitespace(char* str)
{
	int index = 0;
	int start = 0;
	
	int last = 0;

	while (str[index] == ' ')
		index++;

	while (str[index] != '\0')
	{
		str[start] = str[index];

		if (str[index] != ' ')
			last = start + 1;

		index++;
		start++;
	}

	str[last] = '\0';

	

}

bool stringEquals(const char* a, const char* b)
{
	int same = 1;
	int i = 0;
	while (a[i] != '\0' && b[i] != '\0')
	{
		if (a[i] != b[i])
			return false;
		i++;
		

	}

	if (a[i] == b[i])
		return true;

	return false;

}

void printHeader(const char* title)
{

	std::cout << "==============================" << std::endl;
	std::cout << title << std::endl;
	std::cout << "==============================" << std::endl;

}

void clearScreen()
{

	system("cls");

}