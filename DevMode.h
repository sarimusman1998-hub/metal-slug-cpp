#pragma once
#include "Character.h"
#include "Weapon.h"
#include <SFML/Graphics.hpp>


class DevMode {
	private:
	bool active;
	int key_count;
	float key_timer;
	bool key_press;

	//Devmode application in Game.h
	void applyEffects(Character** characters, int num_chars);
	void removeEffects(Character** characters, int num_chars);


	public:
		DevMode();
		~DevMode() {
		
		}
		bool isActive() const;

		bool handleInput(bool d_key_now, float dt,Character** characters, int num_chars);

		void renderHUD(sf::RenderWindow& window, sf::Text& mode_text);

		void reset(Character** characters, int num_chars);
};