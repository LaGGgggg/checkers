#pragma once

#include <thread>
#include <atomic>
#include <optional>

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>


struct Message {
  unsigned int x_from;
  unsigned int y_from;
  unsigned int x_to;
  unsigned int y_to;
  unsigned int state;
};

sf::Packet& operator<<(sf::Packet& packet, const Message message);
sf::Packet& operator>>(sf::Packet& packet, Message& message);


class SocketManager {
public:

  void start(std::optional<sf::IpAddress> client_ip_address = std::nullopt);
  void stop();

  bool send_message(Message& message);

  bool is_message_received();
  Message get_received_message();

  SocketManager();
  ~SocketManager();

  SocketManager(const SocketManager&) = delete;
  SocketManager& operator=(const SocketManager&) = delete;

private:

  const unsigned int PORT_ = 8012;

  std::thread thread_;
  std::atomic<bool> is_running_ = false;

  std::optional<Message> message_to_send_;
  std::optional<Message> received_message_;

  void run_server_();
  void run_client_(std::optional<sf::IpAddress> ip_address);
};
