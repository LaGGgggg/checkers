#include <SFML/System/Time.hpp>
#include <iostream>
#include <optional>

#include <SFML/Network.hpp>
#include <SFML/Network/IpAddress.hpp>

#include "socket_manager.h"


/**
 * Starts a socket as a server or client.
 *
 * If `client_ip_address` is not provided, the socket starts as a server.
 *
 * @param client_ip_address the ip address of the server to connect to as a client (optional).
 */
void SocketManager::start(const std::optional<sf::IpAddress> client_ip_address) {

  bool expected_is_running = false;

  if (!is_running_.compare_exchange_strong(expected_is_running, true)) {
    return;
  }

  if (client_ip_address == std::nullopt) {
    thread_ = std::thread(&SocketManager::run_server_, this);
  } else {
    thread_ = std::thread(&SocketManager::run_client_, this, client_ip_address);
  }
}

void SocketManager::stop() {

  if (!is_running_.exchange(false)) {
    return;
  }

  if (std::this_thread::get_id() == thread_.get_id()) {
    std::cerr <<"[WARNING] SocketManager::stop() called from inner thread. Ignore this call to avoid deadlock"
      << std::endl;
    return;
  }

  if (thread_.joinable()) {
    thread_.join();
  }
}

/**
* Accepts a message for sending.
*
* If a previous message is still pending, the new message is rejected.
*
* @param message the message to send.
* @return `true` if the message was accepted for sending, `false` if a previous message is still pending.
*/
bool SocketManager::send_message(const std::string& message) {

  if (message_to_send_.empty()) {
    message_to_send_ = message;
    return true;
  }

  return false;
}

bool SocketManager::is_message_received() {
  return !received_message_.empty();
}

std::string SocketManager::get_received_message() {

  std::string message = received_message_;
  received_message_.clear();

  return message;
}

SocketManager::SocketManager() = default;
SocketManager::~SocketManager() {
  stop();
};

void SocketManager::run_server_() {

  sf::TcpListener listener;

  if (listener.listen(PORT_) != sf::Socket::Status::Done) {
    std::cerr << "[ERROR] Failed to receive a connection with the socket at port: " << PORT_ << std::endl;
    return;
  }

  sf::TcpSocket client;
  if (listener.accept(client) != sf::Socket::Status::Done) {
    std::cerr << "[ERROR] Failed to assign socket to the connection at port: " << PORT_ << std::endl;
    return;
  }

  listener.close();
  client.setBlocking(false);

  sf::Packet received_packet;

  while (is_running_.load()) {

    switch (client.receive(received_packet)) {

      case sf::Socket::Status::NotReady:
        break;

      case sf::Socket::Status::Done:

        if (!received_message_.empty()) {
          std::cerr << "[WARNING] Overwriting unprocessed received message" << std::endl;
          received_message_.clear();
        }

        received_packet >> received_message_;
        received_packet.clear();

        break;

      default:

        std::cerr << "[ERROR] Failed to receive data from client";
        break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  client.disconnect();
}

void SocketManager::run_client_(std::optional<sf::IpAddress> ip_address) {

  sf::TcpSocket socket;

  sf::Socket::Status status = socket.connect(*ip_address, PORT_, sf::seconds(5));

  if (status != sf::Socket::Status::Done) {
    std::cerr << "[ERROR] Failed to start a socket at " + ip_address->toString() << std::endl;
    return;
  }

  while (is_running_.load()) {

    if (!message_to_send_.empty()) {

      sf::Packet packet;

      packet << message_to_send_;

      if (socket.send(packet) != sf::Socket::Status::Done) {
        std::cerr << "[ERROR] Failed to send data to " + ip_address->toString();
      } else {
        message_to_send_.clear();
      }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  socket.disconnect();
}
