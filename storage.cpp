#include <iostream>
#include <SFML/Graphics.hpp>
#include "storage.h"

int selected_num = -1;
int selected_x = -1; 
int selected_y = -1;

bool is_selected_flag = false;
bool is_double_jump_flag = false;

bool now_color = true;
bool MY_COLOR;
int my_counter = 0;
int other_counter = 0;


// Functions ========================================================================================================================

// func from main
sf::Vector2f calculate_checker_position(int i, int j) { // function from main()
	return {static_cast<float>(FIELD_OFFSET_X + (SQ_SIZE * ((i + 1) % 2)) + (j * 2 * SQ_SIZE)),
			static_cast<float>(FIELD_OFFSET_Y + (i * SQ_SIZE))};
}
sf::Vector2f calculate_position(int x, int y) {
	return{ static_cast<float>(FIELD_OFFSET_X + SQ_SIZE * x + CHECKER_OFFSET),
			static_cast<float>(FIELD_OFFSET_Y + SQ_SIZE * y + CHECKER_OFFSET) };
}
void set_my_color(bool color) {
	MY_COLOR = color;
}
void get_from(int& x, int& y) {
	x = selected_x;
	y = selected_y;
	std::cout << selected_x << "   " << selected_y << std::endl;
}
int get_status_of_play() { // 0 - contining,	1 - my win,		2 - other win

	if (my_counter == 12) {
		return 1;
	}
	if (other_counter == 12) {
		return 2;
	}

	return 0;
}



inline int to_sq_num (int x, int y) { // calculate index -> number of black sqare 
	return y * 4 + (x - ((y + 1) % 2)) / 2;
}
inline bool is_on_field(int x, int y) {
	return x >= 0 && x < 8 && y >= 0 && y < 8;
}


bool is_this_in_radius(int x, int y, int r) { // return is this sqare enmpty and in radius regarding selected
	return ((selected_x + r == x) && (selected_y + r == y || selected_y - r == y)) ||
		   ((selected_x - r == x) && (selected_y + r == y || selected_y - r == y));
}
bool is_double_jump(int x, int y,  mySqare sqare[][8]) {

	if (is_on_field(x + 1, y + 1) &&
		!sqare[x + 1][y + 1].is_empty() && sqare[x + 2][y + 2].is_empty() &&
		 sqare[x + 1][y + 1].get_color() != now_color ) {
			return true;	
	}
	if (is_on_field(x + 1, y - 1) &&
		!sqare[x + 1][y - 1].is_empty() && sqare[x + 2][y - 2].is_empty() &&
		 sqare[x + 1][y - 1].get_color() != now_color ) {
		return true;
	}
	if (is_on_field(x - 1, y + 1) &&
		!sqare[x - 1][y + 1].is_empty() && sqare[x - 2][y + 2].is_empty() &&
		 sqare[x - 1][y + 1].get_color() != now_color ) {
		return true;
	}
	if (is_on_field(x - 1, y - 1) &&
		!sqare[x - 1][y - 1].is_empty() && sqare[x - 2][y - 2].is_empty() &&
		 sqare[x - 1][y - 1].get_color() != now_color ) {
		return true;
	}

	return false;
}


void select(int x, int y, sf::RectangleShape* sq) {
	int num = to_sq_num(x, y);

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

// Events  =====================================================================================================================


// return: 0 - was move, 1 - was jump, 2 - selection, 3 - nothing
int sqare_type(int x, int y, sf::RectangleShape* sq, mySqare sqare[][8]) {
 	
	if ((x + y) % 2 == 0) { // is not black sqare
		return 3;
	}

	// selection
	if ((!sqare[x][y].is_empty())) {  
	
		
		if (now_color != sqare[x][y].get_color()) {
			return 3;
		}

		if (!is_selected_flag) { 
			select(x, y, sq);
		}
		else if (x == selected_x && y == selected_y && !is_double_jump_flag) { 
			remove_selection(sq);
		}
		else if (!is_double_jump_flag){
			remove_selection(sq);
			select(x, y, sq);
		}
		return 2;
	}

	// move
	else if (is_selected_flag) { 
		

		// only move
		if (is_this_in_radius(x, y, 1) && !is_double_jump_flag) {

			sqare[selected_x][selected_y].get_checker()->setPosition(calculate_position(x, y));
			mySqare::move_from_to(sqare[selected_x][selected_y], sqare[x][y]);

			remove_selection(sq);
			now_color = !now_color;
			return 0;
		}

		// jump
		else if (is_this_in_radius(x,y,2)){

			int tmp_x = (x + selected_x) / 2;
			int tmp_y = (y + selected_y) / 2;

			if (sqare[tmp_x][tmp_y].is_empty() ||
				sqare[tmp_x][tmp_y].get_color() == now_color) {
				return 1;
			}

			sqare[selected_x][selected_y].get_checker()->setPosition(calculate_position(x,y));
			mySqare::move_from_to(sqare[selected_x][selected_y], sqare[x][y]);
			sqare[tmp_x][tmp_y].delete_cheker();
			
			if (now_color == MY_COLOR) {
				++my_counter;
			}
			else {
				++other_counter;
			}

			
			if (is_double_jump(x, y, sqare)) {
				remove_selection(sq);
				select(x, y, sq);
				is_double_jump_flag = true;
				return 1;
			}
			

			remove_selection(sq);
			is_double_jump_flag = false;
			now_color = !now_color;
			
			
			return 0;
		}	
	}

	return 3;
}


// class my_sqare ===============================================================================================================
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

