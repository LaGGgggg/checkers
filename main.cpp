#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "storage.h"

//#include <SFML/Audio.hpp>
//#include <cmath>
//#include <ctime>
//#include <cstdlib>

// создание поля [x][y]
mySqare sqare[8][8];

int main(){

	bool MY_COLOR = false;
	int pos = MY_COLOR ? 5 : 0;  // чтобы снизу были шашки данного игрока, а сверху противника


	// window =========================================================================================================
	sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Checks");
	

	// Задний фон
	sf::RectangleShape background({ WINDOW_X, WINDOW_Y });
	background.setFillColor(back_color);
	
	// Поле (белые клетки)
	sf::RectangleShape field({ FIELD_SIZE, FIELD_SIZE });
	field.setFillColor(white_sq);
	field.setPosition({ FIELD_OFFSET_X, FIELD_OFFSET_Y });
	field.setOutlineThickness(3);
	field.setOutlineColor(boulders);

	// Черные клетки
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

			sq[k].setPosition(calculate_checker_position (i,j));
			++k;
		}
	}

	// Белые шашки
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

			w_checker[k].setPosition(calculate_checker_position(i, j) + sf::Vector2f{CHECKER_OFFSET, CHECKER_OFFSET});
			++k;
		}
	}


	// Черные шашки
	sf::CircleShape b_checker[12];
	for (int i = 0; i < 12; ++i) {

		b_checker[i].setRadius(CHECKER_RADIUS);
		b_checker[i].setFillColor(black_checker);
		b_checker[i].setOutlineThickness(-1);
		b_checker[i].setOutlineColor(boulders);

	}
	k = 0;
	for (int i = 5 - pos; i < 8 - pos; ++i) {
		for (int j = 0; j < 4; ++j) {

			b_checker[k].setPosition(calculate_checker_position(i, j) + sf::Vector2f{ CHECKER_OFFSET, CHECKER_OFFSET });
			++k;

		}
	}

	
	// привязка шашек к клеткам
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





	while (window.isOpen()) { //    Работа с окном    =================================================================

 
		sf::Event event;
		while (window.pollEvent(event)) {  // pollEvent - возвращает событие, если оно ожидает обработки
			// закрытие окна
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			
			// нажатие на поле
			if (event.type == sf::Event::MouseButtonPressed &&
				field.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
				
				int click_x = event.mouseButton.x;
				int click_y = event.mouseButton.y;

				type(click_x, click_y, sq, sqare);
			}
		}



		window.clear();

		// отрисовка объектов ----------------------------------
		window.draw(background);
		window.draw(field);
		for (int i = 0; i < 32; ++i) {
			window.draw(sq[i]);
		}
		for (int i = 0; i < 12; ++i) {
			window.draw(w_checker[i]);
			window.draw(b_checker[i]);
		}


		window.display();

	}

	return 0;
}
