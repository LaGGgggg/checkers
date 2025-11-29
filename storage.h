#pragma once
#include <SFML/Graphics.hpp>


// Сonstants ==============================================================================================
const int WINDOW_X = 1280;
const int WINDOW_Y = 720;

const float FIELD_SIZE = 672;
const float FIELD_OFFSET_X = 308.f;
const float FIELD_OFFSET_Y = 24.f;
const float SQ_SIZE = 84.f;

const float CHECKER_RADIUS = 32.f;
const float CHECKER_OFFSET = 10.f;

const float CHECKER_STORAGE_X = 2000.f;
const float CHECKER_STORAGE_Y = 100.f;


// Colors 
const sf::Color back_color(120, 116, 81);
const sf::Color outline(34, 29, 19); 
const sf::Color black_sq(95, 61, 33);
const sf::Color white_sq(179, 139, 89);
const sf::Color white_cheker(255, 255, 255);
const sf::Color black_checker(53, 46, 30);
const sf::Color selected_sq(0, 150, 40);
const sf::Color interface(54, 64, 37);


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
	sf::CircleShape* get_checker() const {
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
		figure->setPosition({ CHECKER_STORAGE_X, CHECKER_STORAGE_Y });
		figure = nullptr;
	}

	static void move_from_to(mySqare& a, mySqare& b);
};


// from main() ============================================================================================

sf::Vector2f calculate_checker_position(int i, int j);
sf::Vector2f calculate_position(int x, int y);

inline int to_sq_num(int x, int y);
inline bool is_on_field(int x, int y);

void set_my_color(bool color);
int get_status_of_play();



// Settings
void setting_home_screen(sf::Text& load_text);
void setting_example(sf::CircleShape& example);
void setting_field(sf::RectangleShape& field, sf::RectangleShape sq[32]);
void setting_chekers(int pos, sf::CircleShape w_checker[12], sf::CircleShape b_checker[12]);

// Events
int sqare_type(int click_x, int click_y, sf::RectangleShape* sq, mySqare sqare[][8]);


// from storage.cpp() ============================================================================================

bool is_this_in_radius(int x, int y, int r);
bool is_double_jump(int x, int y, mySqare sqare[][8]);

void select(int x, int y, sf::RectangleShape* sq);
void remove_selection(sf::RectangleShape* sq);

