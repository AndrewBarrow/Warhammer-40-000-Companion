/* 
	dice_demo.cpp
*/

#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

class dice {
public:
	dice() { }

	void seed() {
		srand((unsigned)time(0));
	}

	void roll(int& dice) {
		dice = (rand() % 6) + 1;
	}
};

int main() {
	int dice_roll[10];
	dice dice;
	dice.seed();
	
	for (int i = 0; i < 10; i++) {
		dice.roll(dice_roll[i]);
	}

	for (int i = 0; i < 10; i++) {
		std::cout << "Dice[" << i << "]: " << dice_roll[i] << '\n';
	}

	system("pause");
	return 0;
}
