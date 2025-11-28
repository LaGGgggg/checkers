#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <SFML/Window.hpp>
#include <optional>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>


#include "storage.h"
#include "socket_manager.h"


bool is_load = true;
bool is_waiting_for = false;
bool is_end = false;
bool is_error = false;

int last_x, last_y;
int my_counter, other_counter;



mySqare sqare[8][8]; // creating fied [y][x]

void seting_home_screen(sf::CircleShape& white_example, sf::CircleShape& black_example, sf::Text& load_text);
void seting_field(sf::RectangleShape& field, sf::RectangleShape sq[32]);
void setting_chekers(int pos, sf::CircleShape w_checker[12], sf::CircleShape b_checker[12]);
void setting_error_screen(sf::RectangleShape& button);

int main(int argc, char* argv[]) {
	

	// window ==================================================================================================================================================================
	sf::RenderWindow window(sf::VideoMode({ WINDOW_X, WINDOW_Y }), "Checks");
	sf::Font font ;
	if (!font.openFromFile("text.ttf")){
		std::cout << "ERROR: can't open font " << std::endl;
		return 0;
	}

	sf::RectangleShape background({ WINDOW_X, WINDOW_Y });
	background.setFillColor(back_color);


	// Home screen ====================================================================================
	sf::CircleShape white_example;
	sf::CircleShape black_example;
	sf::Text load_text(font);
	seting_home_screen(white_example, black_example, load_text);

	
	window.draw(background);
	//window.draw(white_example);
	//window.draw(black_example);
	window.draw(load_text);
	window.display();

	// =====================================================
	sf::RectangleShape button({ BUTTON_X, BUTTON_Y});


	// Field screen ===================================================================================
	sf::RectangleShape field({ FIELD_SIZE, FIELD_SIZE }); // (White sqares)
	sf::RectangleShape sq[32];		// Black sqares
	sf::CircleShape w_checker[12];	// White checkers
	sf::CircleShape b_checker[12];	// Black checkers
	seting_field(field, sq);


	// Creating server / client =======================================================================
	const bool is_client = (argc > 1 && std::string(argv[1]) == "client");

	SocketManager socket_manager_server;
	SocketManager socket_manager_client;

	socket_manager_client.start(is_client ? 8012 : 9001, sf::IpAddress({ 127, 0, 0, 1 }));  
	socket_manager_server.start(is_client ? 9001 : 8012);  


	// set color 
	bool MY_COLOR;
	if (!is_client) {
		MY_COLOR = rand() % 2;

		Message message{.x_from = !MY_COLOR, .y_from = 0, .x_to = 0, .y_to = 0, .state = 5};
		socket_manager_client.send_message(message);  // send other color

	}
	

	while (window.isOpen()) { //   Window   ====================================================================================================================================
		
		if (is_load) {
			if (socket_manager_server.is_message_received()) {

				Message received_message = socket_manager_server.get_received_message();
				if (received_message.state == 5) {
					is_load = false;
				}
				if (is_client) {
					
					MY_COLOR = received_message.x_from;

					Message message{ .x_from = 0, .y_from = 0, .x_to = 0, .y_to = 0, .state = 5 };
					socket_manager_client.send_message(message); // send about connection
				}


				int pos = MY_COLOR ? 5 : 0;
				set_my_color(MY_COLOR); // set to storage.cpp



				// setting and binding checkers and sqares ========================================================
				setting_chekers(pos, w_checker, b_checker);
				int k = 0;
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

				is_waiting_for = !MY_COLOR;

			}
		}

		while (const std::optional event = window.pollEvent()) {  

			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
			if (is_error) {


				
				if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
					sf::Vector2f point{ static_cast<float>(mouseButton->position.x), static_cast<float>(mouseButton->position.y) };
					if (button.getGlobalBounds().contains(point)) {
						return 0;
					}
				}

			}
			

			// type ==============================================================================================================================
			if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {

				int click_x = mouseButton->position.x;
				int click_y = mouseButton->position.y;

				// calculate point if click
				sf::Vector2f point{ static_cast<float>(click_x), static_cast<float>(click_y) };

				if (field.getGlobalBounds().contains(point) && !is_waiting_for) {

					int x = (click_x - FIELD_OFFSET_X) / SQ_SIZE;
					int y = (click_y - FIELD_OFFSET_Y) / SQ_SIZE;

					int tmp = sqare_type(x, y, sq, sqare);
					if (!tmp) {
						is_waiting_for = true;

						Message message{
					  .x_from = 7 - last_x,
					  .y_from = 7 - last_y,
					  .x_to = 7 - x,
					  .y_to = 7 - y,
					  .state = 1 };
					

						// send message
						socket_manager_client.send_message(message);
						std::cout << "Sent message: " << last_x << " " << last_y << " -> " << x << ", " << y << ", state: 1" << std::endl;

					}
					else if (tmp == 1) {

						Message message{
					  .x_from = 7 - last_x,
					  .y_from = 7 - last_y,
					  .x_to = 7 - x,
					  .y_to = 7 - y,
					  .state = 2 };


						// send message
						socket_manager_client.send_message(message);
						std::cout << "Sent message: " << last_x << " " << last_y << " -> " << x << ", " << y << ", state: 1" << std::endl;

						last_x = x;
						last_y = y;
					}
					else {

						last_x = x;
						last_y = y;
					}
				}
			}
		}

		if (is_waiting_for) {

			if (socket_manager_server.is_message_received()) {
				Message received_message = socket_manager_server.get_received_message();

				if (received_message.state == 0) {
					std::cout << "Error connection" << std::endl;
					is_error = true;
				}
				if (received_message.state == 3) {
					std::cout << "Error 3" << std::endl;
					is_error = true;
				}

				std::cout << "[INFO] Received message: from (" << received_message.x_from << ", " << received_message.y_from << ") "
					<< "to (" << received_message.x_to << ", " << received_message.y_to << ") "
					<< "state: " << received_message.state << std::endl;

				// move
				sqare_type(received_message.x_from, received_message.y_from, sq, sqare);
				if (sqare_type(received_message.x_to, received_message.y_to, sq, sqare) == 2) { // incorrect move

					std::cout << "ERROR" << std::endl;
					is_error = true;
					Message message{ .x_from = 0, .y_from = 0, .x_to = 0, .y_to = 0, .state = 3 };
					socket_manager_client.send_message(message);

				}
				if (received_message.state != 2){
					is_waiting_for = false;
				}

			}


		}



		window.clear();
		window.draw(background);
		if (is_load) {
			window.draw(background);
			//window.draw(white_example);
			//window.draw(black_example);
			window.draw(load_text);
		}
		else if (is_end) {
			
			

		}
		else if (is_error) {
			
			
			setting_error_screen(button);
			window.draw(button);

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



void seting_home_screen(sf::CircleShape& white_example, sf::CircleShape& black_example, sf::Text& load_text) {

	white_example.setRadius(CHECKER_RADIUS * 3);
	white_example.setFillColor(wite_checker);
	white_example.setOutlineThickness(-1);
	white_example.setOutlineColor(white_sq);
	white_example.setPosition({ WINDOW_X / 2 - 100 - CHECKER_RADIUS * 6, WINDOW_Y / 2 - 100 });

	black_example.setRadius(CHECKER_RADIUS * 3);
	black_example.setFillColor(black_checker);
	black_example.setOutlineThickness(-1);
	black_example.setOutlineColor(outline);
	black_example.setPosition({ WINDOW_X / 2 + 300 - CHECKER_RADIUS * 6, WINDOW_Y / 2 - 100 });

	load_text.setString("Loading...");
	load_text.setCharacterSize(100);
	load_text.setPosition({400, 100});
	load_text.setFillColor(interface);
}
void seting_field(sf::RectangleShape& field, sf::RectangleShape sq[32]) {

	field.setFillColor(white_sq);
	field.setPosition({ FIELD_OFFSET_X, FIELD_OFFSET_Y });
	field.setOutlineThickness(3);
	field.setOutlineColor(outline);

	// Black sqares
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



}
void setting_chekers(int pos, sf::CircleShape w_checker[12], sf::CircleShape b_checker[12]) {

	// White checkers
	int k = 0;
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
	k = 0;
	for (int i = 0; i < 12; ++i) {

		b_checker[i].setRadius(CHECKER_RADIUS);
		b_checker[i].setFillColor(black_checker);
		b_checker[i].setOutlineThickness(-1);
		b_checker[i].setOutlineColor(outline);

	}
	for (int i = 5 - pos; i < 8 - pos; ++i) {
		for (int j = 0; j < 4; ++j) {

			b_checker[k].setPosition(calculate_checker_position(i, j) + sf::Vector2f{ CHECKER_OFFSET, CHECKER_OFFSET });
			++k;

		}
	}

}
void setting_error_screen(sf::RectangleShape& button) {

	button.setFillColor(interface);
	button.setOutlineThickness(-1);
	button.setOutlineColor(black_checker);
	button.setPosition({ BUTTON_OFFSET_X, BUTTON_OFFSET_Y });

}

/*
message state:
0 - exit
1 - move
2 - double move
3 - error
4 - end
5 - connection
*/
