#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include "storage.h"
#include <SFML/Window.hpp>
#include <optional>

#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>

bool at_home = false;


// creating fied [y][x]
mySqare sqare[8][8];

int main() {

	


	// window =========================================================================================================
	sf::RenderWindow window(sf::VideoMode({ WINDOW_X, WINDOW_Y }), "Checks");

	// Background
	sf::RectangleShape background({ WINDOW_X, WINDOW_Y });
	background.setFillColor(back_color);

	// Home screen ====================================================================================================

	
	sf::CircleShape white_example;
	white_example.setRadius(CHECKER_RADIUS * 3);
	white_example.setFillColor(wite_checker);
	white_example.setOutlineThickness(-1);
	white_example.setOutlineColor(white_sq);

	white_example.setPosition({ WINDOW_X / 2 - 100, WINDOW_Y/2 - 100 });

	sf::CircleShape black_example;
	black_example.setRadius(CHECKER_RADIUS * 3);
	black_example.setFillColor(black_checker);
	black_example.setOutlineThickness(-1);
	black_example.setOutlineColor(outline);

	black_example.setPosition({ WINDOW_X/2 + 300, WINDOW_Y / 2 - 100 });

	
	
	bool MY_COLOR = false;
	int pos = MY_COLOR ? 5 : 0;  // чтобы снизу были шашки данного игрока, а сверху противника


	// Field screen ===================================================================================================

	// Field (white sqares)
	sf::RectangleShape field({ FIELD_SIZE, FIELD_SIZE });
	field.setFillColor(white_sq);
	field.setPosition({ FIELD_OFFSET_X, FIELD_OFFSET_Y });
	field.setOutlineThickness(3);
	field.setOutlineColor(outline);

	// Black sqares
	sf::RectangleShape sq[32];
	for (int i = 0; i < 32; ++i) {

		sq[i].setSize({ SQ_SIZE, SQ_SIZE });
		sq[i].setFillColor(black_sq);
		sq[i].setOutlineThickness(-2);
		sq[i].setOutlineColor(black_sq);

	}
	int k = 0;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 4; ++j) {

			sq[k].setPosition(calculate_checker_position(i, j));
			++k;
		}
	}

	// White checkers
	sf::CircleShape w_checker[12];
	for (int i = 0; i < 12; ++i) {

		w_checker[i].setRadius(CHECKER_RADIUS);
		w_checker[i].setFillColor(wite_checker);
		w_checker[i].setOutlineThickness(-1);
		w_checker[i].setOutlineColor(white_sq);

	}
	k = 0;
	for (int i = 0 + pos; i < 3 + pos; ++i) {
		for (int j = 0; j < 4; ++j) {

			w_checker[k].setPosition(calculate_checker_position(i, j) + sf::Vector2f{ CHECKER_OFFSET, CHECKER_OFFSET });
			++k;
		}
	}


	// Black checkers
	sf::CircleShape b_checker[12];
	for (int i = 0; i < 12; ++i) {

		b_checker[i].setRadius(CHECKER_RADIUS);
		b_checker[i].setFillColor(black_checker);
		b_checker[i].setOutlineThickness(-1);
		b_checker[i].setOutlineColor(outline);

	}
	k = 0;
	for (int i = 5 - pos; i < 8 - pos; ++i) {
		for (int j = 0; j < 4; ++j) {

			b_checker[k].setPosition(calculate_checker_position(i, j) + sf::Vector2f{ CHECKER_OFFSET, CHECKER_OFFSET });
			++k;

		}
	}


	// binding checkers and sqares
	k = 0;
	for (int i = 0 + pos; i < 3 + pos; ++i) {                //y
		for (int j = 0; j < 8; ++j) {                        //x
			if ((i + j) % 2 == 1) {

				sqare[j][i].set_checker(true, false, &w_checker[k]);
				++k;
			}
		}
	}
	k = 0;
	for (int i = 5 - pos; i < 8 - pos; ++i) {                //y
		for (int j = 0; j < 8; ++j) {                        //x
			if ((i + j) % 2 == 1) {

				sqare[j][i].set_checker(false, false, &b_checker[k]);
				++k;
			}
		}
	}

	//set color 
	





	while (window.isOpen()) { //    Window    =================================================================


		while (const std::optional event = window.pollEvent()) {  // pollEvent - возвращает событие, если оно ожидает обработки
			
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}


			// tape
			if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {

				int click_x = mouseButton->position.x;
				int click_y = mouseButton->position.y;

				// calculate point if click
				sf::Vector2f point{ static_cast<float>(click_x), static_cast<float>(click_y)}; 

				if (field.getGlobalBounds().contains(point)) {

					int x = (click_x - FIELD_OFFSET_X) / SQ_SIZE;
					int y = (click_y - FIELD_OFFSET_Y) / SQ_SIZE;

					type(x, y, sq, sqare);
				}



			}

			
			if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {

				if (textEntered -> unicode >= 0 && textEntered -> unicode <= 9)
				std::cout << "ASCII character typed: " << static_cast<char>(textEntered->unicode) << std::endl;
			

			}
		}



		window.clear();
		window.draw(background);
		if (at_home) {

			window.draw(white_example);
			window.draw(black_example);

		}
		else {

			window.draw(field);
			for (int i = 0; i < 32; ++i) {
				window.draw(sq[i]);
			}
			for (int i = 0; i < 12; ++i) {
				window.draw(w_checker[i]);
				window.draw(b_checker[i]);
			}

		}


		


		window.display();

	}

	return 0;
}
