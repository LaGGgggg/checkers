#include <TGUI/Widgets/EditBox.hpp>
#include <iostream>
#include <optional>
#include <string>
#include <memory>

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Socket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <SFML/Network.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include "socket_manager.h"


class App {
public:

  App() {

    window_ = sf::RenderWindow(sf::VideoMode({600, 400}), "Checkers", sf::Style::Default, sf::State::Windowed);

    window_.setVerticalSyncEnabled(true);

    gui_ = std::make_unique<tgui::Gui>(window_);

    ip_input_ = tgui::EditBox::create();

    ip_input_->setSize(400, 40);
    ip_input_->setPosition(100, 100);

    gui_->add(ip_input_);

    status_label_ = tgui::Label::create("Enter IP Address:");

    status_label_->setPosition(100, 60);
    status_label_->setTextSize(24);

    gui_->add(status_label_);

    connect_button_ = tgui::Button::create("Connect");

    connect_button_->setSize(200, 40);
    connect_button_->setPosition(100, 160);

    connect_button_->onPress(&App::on_connect_button_pressed, this);

    gui_->add(connect_button_);

    send_input_ = tgui::EditBox::create();

    send_input_->setSize(400, 40);
    send_input_->setPosition(100, 220);

    gui_->add(send_input_);

    send_button_ = tgui::Button::create("Send");

    send_button_->setSize(200, 40);
    send_button_->setPosition(100, 280);

    send_button_->onPress(&App::on_send_button_pressed, this);

    gui_->add(send_button_);
  }

  void set_status_label(const std::string& text) {

    status_label_->setText(text);

    std::cout << text << std::endl;
  }

  void on_connect_button_pressed() {

    const std::string ip_address_raw = ip_input_->getText().toStdString();

    auto ip_address = sf::IpAddress::resolve(ip_address_raw);

    if (ip_address == std::nullopt) {
      set_status_label("Invalid IP Address: " + ip_address_raw);



      set_status_label("Try to connect as a server");

      socket_manager_.start();



      return;
    }

    set_status_label("Try to connect to " + ip_address_raw);

    socket_manager_.start(sf::IpAddress::resolve(ip_address_raw));
  }

  void on_send_button_pressed() {
    socket_manager_.send_message(send_input_->getText().toStdString());
  }

  void handle_socket_message_receiving() {
    if (socket_manager_.is_message_received()) {
      set_status_label("Received message: " + socket_manager_.get_received_message());
    }
  }

  void run() {
    while (window_.isOpen()) {

      while (const std::optional event = window_.pollEvent()) {

        if (event->is<sf::Event::Closed>()) {
          window_.close();
          socket_manager_.stop();
        }

        gui_->handleEvent(*event);
      }

      handle_socket_message_receiving();

      window_.clear();

      gui_->draw();

      window_.display();
    }
  }

private:

  sf::RenderWindow window_;
  std::unique_ptr<tgui::Gui> gui_;

  tgui::Label::Ptr status_label_;
  tgui::EditBox::Ptr ip_input_;
  tgui::Button::Ptr connect_button_;
  tgui::EditBox::Ptr send_input_;
  tgui::Button::Ptr send_button_;

  SocketManager socket_manager_;
};


int main() {
  App().run();
}
