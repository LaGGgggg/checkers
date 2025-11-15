#pragma once

#include <thread>
#include <atomic>
#include <optional>

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>


struct Message {
  int x_from;
  int y_from;
  int x_to;
  int y_to;
  int state;
};

sf::Packet& operator<<(sf::Packet& packet, const Message message);
sf::Packet& operator>>(sf::Packet& packet, Message& message);


class SocketManager {
public:

  void start(unsigned int port, std::optional<sf::IpAddress> client_ip_address = std::nullopt);
  void stop();

  bool send_message(Message& message);

  bool is_message_received();
  Message get_received_message();

  SocketManager();
  ~SocketManager();

  SocketManager(const SocketManager&) = delete;
  SocketManager& operator=(const SocketManager&) = delete;

private:

  unsigned int port_;

  std::thread thread_;
  std::atomic<bool> is_running_ = false;

  std::optional<Message> message_to_send_;
  std::optional<Message> received_message_;

  void run_server_();
  void run_client_(std::optional<sf::IpAddress> ip_address);
};
