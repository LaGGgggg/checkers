#pragma once

#include <string>
#include <thread>
#include <atomic>
#include <optional>

#include <SFML/Network/IpAddress.hpp>


class SocketManager {
public:

  void start(std::optional<sf::IpAddress> client_ip_address = std::nullopt);
  void stop();

  bool send_message(const std::string& message);

  bool is_message_received();
  std::string get_received_message();

  SocketManager();
  ~SocketManager();

  SocketManager(const SocketManager&) = delete;
  SocketManager& operator=(const SocketManager&) = delete;

private:

  const unsigned int PORT_ = 8012;

  std::thread thread_;
  std::atomic<bool> is_running_ = false;

  std::string message_to_send_;
  std::string received_message_;

  void run_server_();
  void run_client_(std::optional<sf::IpAddress> ip_address);
};
