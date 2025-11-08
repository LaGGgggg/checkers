#include <iostream>
#include <SFML/Graphics.hpp>
#include "storage.h"

int selected_num = -1;

int selected_x = -1; 
int selected_y = -1;
bool is_selected_flag = false;

bool now_color = true;

// functions ========================================================================================================================

//from main
sf::Vector2f calculate_checker_position(int i, int j) { // function from main()
	return {static_cast<float>(FIELD_OFFSET_X + (SQ_SIZE * ((i + 1) % 2)) + (j * 2 * SQ_SIZE)),
			static_cast<float>(FIELD_OFFSET_Y + (i * SQ_SIZE))};
}
sf::Vector2f calculate_position(int x, int y) {
	return{ static_cast<float>(FIELD_OFFSET_X + SQ_SIZE * x + CHECKER_OFFSET),
			static_cast<float>(FIELD_OFFSET_Y + SQ_SIZE * y + CHECKER_OFFSET) };
}


// вычисление номера черной клетки по индексам массива
inline int to_sq_num (int x, int y) { 
	return y * 4 + (x - ((y + 1) % 2)) / 2;
}

bool is_empty_in_radius(int x, int y, int r) {
	return ((selected_x + r == x) && (selected_y + r == y || selected_y - r == y)) ||
		   ((selected_x - r == x) && (selected_y + r == y || selected_y - r == y));
}

void select(int x, int y, int num, sf::RectangleShape* sq) {

	sq[num].setOutlineColor(selected_sq);
	sq[num].setOutlineThickness(-4);

	is_selected_flag = true;
	selected_num = num;
	selected_x = x;
	selected_y = y;
}
void remove_selection(sf::RectangleShape* sq) {

	sq[selected_num].setOutlineColor(black_sq);
	sq[selected_num].setOutlineThickness(-2);
	is_selected_flag = false;
	selected_num = -1;
	selected_x = -1;
	selected_y = -1;
}

// Обработка событий ================================================================================================================


void type(int click_x, int click_y, sf::RectangleShape* sq, mySqare sqare[][8]) {

	int x = (click_x - FIELD_OFFSET_X) / SQ_SIZE;
	int y = (click_y - FIELD_OFFSET_Y) / SQ_SIZE;


	if ((x + y) % 2 == 0) { // попали не на черную
		return;
	}

	//выделение
	if ((!sqare[x][y].is_empty())) {  

		int num = to_sq_num(x, y);  

		if (now_color != sqare[x][y].get_color()) {
			return;
		}


		if (!is_selected_flag) { // выделение
			select(x, y, num, sq);
		}
		else if (num == selected_num) { // снятие выделения
			remove_selection(sq);
		}
		else { // перевыделение 

			remove_selection(sq);
			select(x, y, num, sq);
		}
	}

	// ход
	else if (is_selected_flag) { 

		// обычный ход
		if (is_empty_in_radius(x, y, 1)) {

			sqare[selected_x][selected_y].get_checker()->setPosition(calculate_position(x, y));
			mySqare::move_from_to(sqare[selected_x][selected_y], sqare[x][y]);

			remove_selection(sq);
			now_color = !now_color;
			
		}

		// съесть шашку
		else if (is_empty_in_radius(x,y,2)){

			int tmp_x = (x + selected_x) / 2;
			int tmp_y = (y + selected_y) / 2;

			if (sqare[tmp_x][tmp_y].is_empty() ||
				sqare[tmp_x][tmp_y].get_color() == now_color) {
				return;
			}

				sqare[selected_x][selected_y].get_checker()->setPosition(calculate_position(x,y));
				mySqare::move_from_to(sqare[selected_x][selected_y], sqare[x][y]);

				sqare[tmp_x][tmp_y].delete_cheker();
				remove_selection(sq);
				now_color = !now_color;

		}
	}
}


// class my_sqare ===================================================================================================================
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

