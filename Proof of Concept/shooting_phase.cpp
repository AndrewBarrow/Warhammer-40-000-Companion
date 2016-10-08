/*
	shooting_phase.cpp

	A proof of concept for the shooting phase.
*/

#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <cstdint>

class Troop_Datasheet {
private:
	std::string name_;
	uint16_t WS_;
	uint16_t BS_;
	uint16_t S_;
	uint16_t T_;
	uint16_t W_;
	uint16_t I_;
	uint16_t A_;
	uint16_t Ld_;
	uint16_t Sv_;

public:
	Troop_Datasheet() { }

	void build_profile(
		std::string troop_name,
		uint16_t weapon_skill,
		uint16_t ballistic_skill,
		uint16_t strength,
		uint16_t toughness,
		uint16_t wounds,
		uint16_t initiative,
		uint16_t attacks,
		uint16_t leadership,
		uint16_t armour_save
		) {
		name_ = troop_name;
		WS_ = weapon_skill;
		BS_ = ballistic_skill;
		S_ = strength;
		T_ = toughness;
		W_ = wounds;
		I_ = initiative;
		A_ = attacks;
		Ld_ = leadership;
		Sv_ = armour_save;
	}

	void print_profile() {
		std::cout << name_ << '\n'
			<< "-------------------------------------" << '\n'
			<< "|WS |BS | S | T | W | I | A |Ld |Sv |" << '\n'
			<< "-------------------------------------" << '\n'
			<< "| " << WS_ << " | " << BS_ << " | " << S_ << " | " << T_ << " | " << W_ << " | " << I_ << " | " << A_ << " | " << Ld_ << " | " << Sv_ << " |" << '\n'
			<< "-------------------------------------" << std::endl;
	}

	std::string name() {
		return name_;
	}

	uint16_t BS() {
		return BS_;
	}

	uint16_t S() {
		return S_;
	}

	uint16_t T() {
		return T_;
	}

	uint16_t Sv() {
		return Sv_;
	}
};

class Dice {
public:
	Dice() { }

	void seed() {
		srand((unsigned)time(0));
	}

	void roll(int& dice) {
		dice = (rand() % 6) + 1;
	}

};

class Rules {
private:
	Dice dice;
	int hit_;

	void roll_to_hit(Troop_Datasheet& attacker, int num_of_attackers) {
		hit_ = 0;
		int *rolls;
		rolls = new int[num_of_attackers];

		// roll dice
		for (int i = 0; i < num_of_attackers; i++) {
			dice.roll(rolls[i]);
		}

		for (int i = 0; i < num_of_attackers; i++) {
			if (attacker.BS() <= 5) {
				if (rolls[i] >= (7 - attacker.BS())) {
					hit_++;
				}
			}
			else if (attacker.BS() >= 6 && attacker.BS() <= 10) {
				// Ballistic Skill of 6 or Better reroll
				if (rolls[i] >= 2) {
					hit_++;
				}
				else if (rolls[i] == 1) {
					dice.roll(rolls[i]);
					if (rolls[i] >= (6 - (attacker.BS() - 6))) {
						hit_++;
					}
				}
			}
		}
		std::cout << attacker.name() << " fired " << num_of_attackers << " shots. " << hit_ << " hit." << std::endl;
		delete[] rolls;
	}

	void roll_to_wound(Troop_Datasheet& attacker, Troop_Datasheet& defender) {
		int num_of_rolls = hit_;
		hit_ = 0;
		int *rolls;
		rolls = new int[num_of_rolls];

		// roll dice
		for (int i = 0; i < num_of_rolls; i++) {
			dice.roll(rolls[i]);
		}

		for (int i = 0; i < num_of_rolls; i++) {
			if (attacker.S() == defender.T()) {
				// 4+
				if (rolls[i] >= 4) {
					hit_++;
				}
			}
			else if (attacker.S() > defender.T()) {
				if ((attacker.S() - defender.T()) == 1) {
					// 3+
					if (rolls[i] >= 3) {
						hit_++;
					}
				}
				else if ((attacker.S() - defender.T()) >= 2) {
					// 2+
					if (rolls[i] >= 2) {
						hit_++;
					}
				}
			}
			else if (attacker.S() < defender.T()) {
				if ((defender.T() - attacker.S()) == 1) {
					// 5+
					if (rolls[i] >= 5) {
						hit_++;
					}
				}
				else if (((defender.T() - attacker.S()) == 2) || ((defender.T() - attacker.S()) == 3)) {
					// 6+
					if (rolls[i] >= 6) {
						hit_++;
					}
				}
			}
		}
		std::cout << hit_ << " shots connect on " << defender.name() << "." << std::endl;
		delete[] rolls;
	}

	void roll_to_armour_save(Troop_Datasheet defender) {
		int num_of_rolls = hit_;
		int saves = 0;
		hit_ = 0;
		int *rolls;
		rolls = new int[num_of_rolls];

		// roll dice
		for (int i = 0; i < num_of_rolls; i++) {
			dice.roll(rolls[i]);
		}

		for (int i = 0; i < num_of_rolls; i++) {
			if (rolls[i] >= defender.Sv()) {
				saves++;
			}
			else {
				hit_++;
			}
		}

		std::cout << saves << " shots bounce off. " << hit_ << " shots kill." << std::endl;
		delete[] rolls;
	}

public:
	Rules() {
		dice.seed();
	}

	void shooting_phase(Troop_Datasheet& attacker, int num_of_attackers, Troop_Datasheet& defender) {
		roll_to_hit(attacker, num_of_attackers);
		roll_to_wound(attacker, defender);
		roll_to_armour_save(defender);
	}
};

int main() {
	Rules shooting;

	Troop_Datasheet fire_warrior;
	fire_warrior.build_profile("Fire Warrior Strike Team", 2, 3, 3, 3, 1, 2, 1, 7, 4);

	Troop_Datasheet terminator;
	terminator.build_profile("Terminator", 4, 4, 4, 4, 1, 4, 2, 9, 2);

	fire_warrior.print_profile();
	std::cout << std::endl;
	terminator.print_profile();
	std::cout << std::endl;

	shooting.shooting_phase(fire_warrior, 10, terminator);

	system("pause");
	return 0;
}