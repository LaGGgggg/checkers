#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>


// Константы ==============================================================================================
const int WINDOW_X = 1280;
const int WINDOW_Y = 720;

const float FIELD_SIZE = 672;
const float FIELD_OFFSET_X = 308.f;
const float FIELD_OFFSET_Y = 24.f;
const float SQ_SIZE = 84.f;

const float CHECKER_RADIUS = 32.f;
const float CHECKER_OFFSET = 10.f;

const float CHEKER_STORAGE_X = 20.f;////////////
const float CHEKER_STORAGE_Y = 100.f;


// Цвета 
const sf::Color back_color(120, 116, 81);
const sf::Color boulders(34, 29, 19); // для контура
const sf::Color black_sq(95, 61, 33);
const sf::Color white_sq(179, 139, 89);
const sf::Color wite_checker(255, 255, 255);
const sf::Color black_checker(0, 0, 0);
const sf::Color selected_sq(0, 150, 40);


// from main() ============================================================================================

sf::Vector2f calculate_checker_position(int i, int j);

inline int to_sq_num(int x, int y);
bool is_empty_in_radius(int x, int y, int r);
void remove_selection(sf::RectangleShape* sq);


// class my_sqare =========================================================================================

class mySqare {
private:
	bool isEmpty;
	bool color;
	bool isQueen;

	sf::CircleShape* figure;

public:

	mySqare(bool is_empty, bool check_color, bool is_queen, sf::CircleShape* shape) :
		isEmpty(is_empty), color(check_color), isQueen(is_queen), figure(shape) {}
	mySqare() :
		isEmpty(true), color(false), isQueen(false), figure(nullptr) {}

	// getters
	bool is_empty() const {
		return (isEmpty);
	}
	bool get_color() const {
		return (color);
	}
	bool get_is_queen() const {
		return (isQueen);
	}
	sf::CircleShape* get_checker() const{
		return figure;
	}


	// setters
	void set_empty() {
		isEmpty = true;
	}
	void set_checker(bool check_color, bool is_queen, sf::CircleShape* shape) {

		isEmpty = false;
		color = check_color; 
		isQueen = is_queen;
		figure = shape;
	}
	void delete_cheker() {
		isEmpty = true;
		isQueen = false;
		figure->setPosition({ CHEKER_STORAGE_X, CHEKER_STORAGE_Y });
		figure = nullptr;
	}

	static void move_from_to(mySqare& a, mySqare& b);



	// only for debug
	void print() {  
		if (isEmpty) std::cout << "is Empty  ";
		else std::cout << "not Empty  ";

		if (color) std::cout << "White  ";
		else std::cout << "Black  ";

		if (isQueen) std::cout << "isQueen  ";
		else std::cout << "not Queen  ";

		if (figure) std::cout << "check " << std::endl;
		else std::cout << std::endl;

	}
};


// Обработка событий ======================================================================================

void type(int click_x, int click_y, sf::RectangleShape* sq, mySqare sqare[][8]);
