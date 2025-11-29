#include <SFML/Window/WindowEnums.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <SFML/Window.hpp>
#include <optional>
#include <SFML/Audio.hpp>
#include <cstdlib>


#include "storage.h"
#include "socket_manager.h"


bool is_load = true;
bool is_waiting_for = false;
bool is_end = false;
bool is_error = false;
bool flag = false;

int last_x, last_y;
std::string error_message = "error";
mySqare sqare[8][8]; // creating fied [y][x]


int main(int argc, char* argv[]) {


	// window ========================================================================================================================
	sf::RenderWindow window(sf::VideoMode({ WINDOW_X, WINDOW_Y }), "Checks", sf::Style::Titlebar | sf::Style::Close);
	sf::Font font;
	if (!font.openFromFile("text.ttf")) {
		std::cout << "ERROR: can't open font " << std::endl;
		return 0;
	}

	sf::RectangleShape background({ WINDOW_X, WINDOW_Y });
	background.setFillColor(back_color);


	// Home screen ------------------------------------------------------------------------------------
	sf::Text text(font);
	setting_home_screen(text);

	window.draw(background);
	window.draw(text);
	window.display();



	// Interface --------------------------------------------------------------------------------------
	sf::CircleShape example;
	example.setFillColor(white_sq);
	example.setPosition({ 100, 150 });
	example.setOutlineThickness(-1);
	example.setOutlineColor(white_sq);
	example.setRadius(CHECKER_RADIUS);


	// Field screen -----------------------------------------------------------------------------------
	sf::RectangleShape field({ FIELD_SIZE, FIELD_SIZE }); // (White sqares)
	sf::RectangleShape sq[32];		// Black sqares
	sf::CircleShape w_checker[12];	// White checkers
	sf::CircleShape b_checker[12];	// Black checkers
	setting_field(field, sq);



	// Creating server / client -----------------------------------------------------------------------
	const bool is_client = (argc > 1 && std::string(argv[1]) == "client");

	SocketManager socket_manager_server;
	SocketManager socket_manager_client;

	socket_manager_client.start(is_client ? port_1 : port_2, IP);
	socket_manager_server.start(is_client ? port_2 : port_1);


	// set color 
	bool MY_COLOR;
	if (!is_client) {

		MY_COLOR = rand() % 2;
		Message message{ .x_from = !MY_COLOR, .y_from = 0, .x_to = 0, .y_to = 0, .state = 5 };
		socket_manager_client.send_message(message);  // send other color
	}



	//   Window loop  ================================================================================================================
	while (window.isOpen()) {


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


				// setting and binding checkers and sqares ------------------------------------------
				int pos = MY_COLOR ? 5 : 0;
				set_my_color(MY_COLOR); // set to storage.cpp

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

		if (get_status_of_play() == 1 && !flag) { // my win -----------------------------------------

			Message message{ .x_from = 0, .y_from = 0, .x_to = 0, .y_to = 0, .state = 4 };
			socket_manager_client.send_message(message);
			error_message = "Win";
			text.setPosition({ 500, 200 });
			is_end = true;
			flag = true;
		}
		if (get_status_of_play() == 2 && !flag) { // other win --------------------------------------

			error_message = "Lose";
			text.setPosition({ 500, 200 });
			is_end = true;
			flag = true;
		}

		if (is_waiting_for) { // other move 
			setting_example(example);
			if (socket_manager_server.is_message_received()) {

				Message received_message = socket_manager_server.get_received_message();
				std::cout << "[INFO] Receive message: " << received_message.x_from << " " << received_message.y_from << " -> "
					<< received_message.x_to << " " << received_message.y_to << ", " << "state: " << received_message.state << std::endl;


				if (received_message.state == 0) { // error connection
					error_message = "Disconnected";
					text.setPosition({ 250, 200 });
					is_error = true;
				}
				if (received_message.state == 3) { // error move
					error_message = "Wrong move";
					is_error = true;
				}


				sqare_type(received_message.x_from, received_message.y_from, sq, sqare);
				if (sqare_type(received_message.x_to, received_message.y_to, sq, sqare) == 2 ||
					sqare_type(received_message.x_to, received_message.y_to, sq, sqare) == 3) { // incorrect move

					is_error = true;
					error_message = "Wrong move";
					Message message{ .x_from = 0, .y_from = 0, .x_to = 0, .y_to = 0, .state = 3 };
					socket_manager_client.send_message(message);

				}


				if (received_message.state != 2) { // if not double jump
					is_waiting_for = false;
				}
			}
		}
		else { // recive message --------------------------------------------------------------------
			setting_example(example);
			if (socket_manager_server.is_message_received()) {
				Message received_message = socket_manager_server.get_received_message();

				std::cout << "[INFO] Received message: " << received_message.x_from << " " << received_message.y_from << " -> "
					<< received_message.x_to << " " << received_message.y_to << ", " << "state: " << received_message.state << std::endl;

				if (received_message.state == 0) { // error connection
					text.setPosition({ 250, 200 });
					error_message = "Disconnected";
					is_error = true;
				}
			}
		}

		//   Events   ============================================================================================================================
		while (const std::optional event = window.pollEvent()) {

			if (event->is<sf::Event::Closed>()) {
				Message message{ .x_from = 0, .y_from = 0, .x_to = 0, .y_to = 0, .state = 0 };
				socket_manager_client.send_message(message);  // send message about closing
				window.close();
			}

			// type ---------------------------------------------------------------------------------
			if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {

				int click_x = mouseButton->position.x;
				int click_y = mouseButton->position.y;

				// calculate point if click
				sf::Vector2f point{ static_cast<float>(click_x), static_cast<float>(click_y) };

				if (field.getGlobalBounds().contains(point) && !is_waiting_for) {

					int x = (click_x - FIELD_OFFSET_X) / SQ_SIZE;
					int y = (click_y - FIELD_OFFSET_Y) / SQ_SIZE;

					int tmp = sqare_type(x, y, sq, sqare);

					if (tmp == 0) { // single move

						Message message{ .x_from = 7 - last_x, .y_from = 7 - last_y, .x_to = 7 - x, .y_to = 7 - y, .state = 1 };
						//Message message{ .x_from = 1, .y_from = 1, .x_to = 6, .y_to = 6, .state = 1 };
						socket_manager_client.send_message(message);
						std::cout << "Sent message: " << last_x << " " << last_y << " -> " << x << ", " << y << ", state: 1" << std::endl;

						is_waiting_for = true;

					}
					else if (tmp == 1) { // double move

						Message message{ .x_from = 7 - last_x, .y_from = 7 - last_y, .x_to = 7 - x,  .y_to = 7 - y, .state = 2 };
						socket_manager_client.send_message(message);
						std::cout << "Sent message: " << last_x << " " << last_y << " -> " << x << ", " << y << ", state: 2" << std::endl;

						last_x = x;
						last_y = y;

					}
					else { // selection

						last_x = x;
						last_y = y;
					}
				}
			}
		}





		//   Drawing   ===========================================================================================================================
		window.clear();
		window.draw(background);

		if (is_load) {
			window.draw(text);
		}
		else if (is_end) {

			text.setString(error_message);
			window.draw(text);
		}
		else if (is_error) {

			text.setString(error_message);
			window.draw(text);
		}
		else {

			window.draw(field);
			window.draw(example);
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

/*
message state:
0 - exit
1 - move
2 - double move
3 - error
4 - end
5 - connection ok
*/

