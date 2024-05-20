#include <iostream>
#include <glut.h>
#include <Windows.h>

#include "Game.h"

int main(int argc, char** argv) {

	srand(time(0));

	Game game(argc, argv, 1920, 1080);

}