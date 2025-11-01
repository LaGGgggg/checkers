#include <iostream>
#include <SFML/Graphics.hpp>
#include "storage.h"

int selected_num = -1;
int selected_x = -1; 
int selected_y = -1;
bool is_selected_flag = false;

bool now_color = true;

// вычисление номера черной клетки по номерам из массива
inline int to_sq_num (int x, int y) { 

	return y * 4 + (x - ((y + 1) % 2)) / 2;
}



// Обработка событий ================================================================================================================


void type(int click_x, int click_y, sf::RectangleShape* sq, mySqare sqare[][8]) {

	int x = (click_x - FIELD_OFFSET_X) / SQ_SIZE;
	int y = (click_y - FIELD_OFFSET_Y) / SQ_SIZE;


	if ((x + y) % 2 == 0) { // попали не на черную
		return;
	}


	if ((!sqare[x][y].is_empty())) {  

		int num = to_sq_num(x, y);  
		
		if (now_color != sqare[x][y].get_color()) {
			return;
		}

		if (!is_selected_flag) { // выделение
			sq[num].setOutlineColor(sf::Color::Green);
			sq[num].setOutlineThickness(-4);

			is_selected_flag = true;
			selected_num = num;
			selected_x = x;
			selected_y = y;
		}
		else if (num == selected_num) { // снятие выделения
			sq[num].setOutlineColor(black_sq);
			sq[num].setOutlineThickness(-2);

			is_selected_flag = false;
			selected_num = -1;
			selected_x = -1;
			selected_y = -1;
		}
		else { // перевыделение 
			sq[selected_num].setOutlineColor(black_sq);
			sq[selected_num].setOutlineThickness(-2);
			selected_num = num;
			selected_x = x;
			selected_y = y;

			sq[num].setOutlineColor(sf::Color::Green);
			sq[num].setOutlineThickness(-4);

		}
	}

	else if (is_selected_flag) { // ход

		// обычный ход
		if (((selected_x + 1 == x) && (selected_y + 1 == y || selected_y - 1 == y)) ||
			((selected_x - 1 == x) && (selected_y + 1 == y || selected_y - 1 == y))) {

			// удаление обводки
			sq[selected_num].setOutlineColor(black_sq);
			sq[selected_num].setOutlineThickness(-2);

			sqare[selected_x][selected_y].get_checker()->setPosition({
				static_cast<float>(FIELD_OFFSET_X + SQ_SIZE * x + CHECKER_OFFSET),
				static_cast<float>(FIELD_OFFSET_Y + SQ_SIZE * y + CHECKER_OFFSET) });

			mySqare::move_from_to(sqare[selected_x][selected_y], sqare[x][y]);

			is_selected_flag = false;
			selected_num = -1;
			selected_x = -1;
			selected_y = -1;
			now_color = !now_color;
		}

		// съесть шашку
		else if (((selected_x + 2 == x) && (selected_y + 2 == y) && (!sqare[selected_x + 1][selected_y + 1].is_empty()) && (sqare[selected_x + 1][selected_y + 1].get_color() != now_color)) ||
				 ((selected_x + 2 == x) && (selected_y - 2 == y) && (!sqare[selected_x + 1][selected_y - 1].is_empty()) && (sqare[selected_x + 1][selected_y - 1].get_color() != now_color)) ||
				 ((selected_x - 2 == x) && (selected_y + 2 == y) && (!sqare[selected_x - 1][selected_y + 1].is_empty()) && (sqare[selected_x - 1][selected_y + 1].get_color() != now_color)) ||
				 ((selected_x - 2 == x) && (selected_y - 2 == y) && (!sqare[selected_x - 1][selected_y - 1].is_empty()) && (sqare[selected_x - 1][selected_y - 1].get_color() != now_color)))
		     {

				sq[selected_num].setOutlineColor(black_sq);
				sq[selected_num].setOutlineThickness(-2);

				sqare[selected_x][selected_y].get_checker()->setPosition({
				static_cast<float>(FIELD_OFFSET_X + SQ_SIZE * x + CHECKER_OFFSET),
				static_cast<float>(FIELD_OFFSET_Y + SQ_SIZE * y + CHECKER_OFFSET) });

				mySqare::move_from_to(sqare[selected_x][selected_y], sqare[x][y]);

				sqare[(x + selected_x)/2][(y + selected_y)/2].delete_cheker();


				is_selected_flag = false;
				selected_num = -1;
				selected_x = -1;
				selected_y = -1;
				now_color = !now_color;


			 }
		

		

	}
}


// class my_sqare =========================================================================================
void mySqare::move_from_to(mySqare& a, mySqare& b) {
	bool tmp;

	a.isEmpty = true;
	b.isEmpty = false;

	tmp = a.color;
	a.color = b.color;
	b.color = tmp;

	tmp = a.isQueen;
	a.isQueen = b.isQueen;
	b.isQueen = tmp;

	b.figure = a.figure;
	a.figure = nullptr;
}

