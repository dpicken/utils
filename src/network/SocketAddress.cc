#include "SocketAddress.h"

#include <cstring>
#include <iostream>
#include <sstream>

network::SocketAddress::SocketAddress(const sockaddr_in6& address)
  : m_address(address) {
}

network::SocketAddress network::SocketAddress::ip(const IpAddress& ipAddress, uint16_t port) {
  return SocketAddress({AF_INET6, htons(port), 0, *ipAddress.addr(), 0});
}

const sockaddr* network::SocketAddress::addr() const {
  return reinterpret_cast<const sockaddr*>(&m_address);
}

std::size_t network::SocketAddress::addrSize() const {
  return sizeof(m_address);
}

std::string network::SocketAddress::str() const {
  std::ostringstream os;
  os << '[' << IpAddress(m_address.sin6_addr) << ']' << ':' << ntohs(m_address.sin6_port);
  return os.str();
}

namespace network {
  std::ostream& operator<<(std::ostream& os, const SocketAddress& address) {
    return os << address.str();
  }

  bool operator==(const SocketAddress& lhs, const SocketAddress& rhs) {
    return std::memcmp(&lhs, &rhs, sizeof(lhs)) == 0;
  }

  bool operator!=(const SocketAddress& lhs, const SocketAddress& rhs) {
    return !(lhs == rhs);
  }
}
