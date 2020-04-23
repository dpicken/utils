#include "IpAddress.h"

#include "error/throwErrnoSystemError.h"

#include <arpa/inet.h>

#include <cstring>
#include <iostream>

network::IpAddress::IpAddress(const in6_addr& ipAddress)
  : m_address(ipAddress) {
}

const network::IpAddress& network::IpAddress::any() {
  static IpAddress any(in6addr_any);
  return any;
}

const network::IpAddress& network::IpAddress::loopback() {
  static IpAddress loopback(in6addr_loopback);
  return loopback;
}

network::IpAddress network::IpAddress::ipv6(const std::string& ipAddress) {
  in6_addr ipv6Address;
  int status = inet_pton(AF_INET6, ipAddress.c_str(), &ipv6Address);
  if (status != 1) {
    error::throwErrnoSystemErrorIfEq(status, -1);
    throw std::invalid_argument("invalid ip address");
  }

  return IpAddress(ipv6Address);
}

network::IpAddress network::IpAddress::ipv4(const std::string& ipAddress) {
  return ipv6("::FFFF:" + ipAddress);
}

network::IpAddress network::IpAddress::ip(const std::string& ipAddress) {
  try {
    return ipv6(ipAddress);
  } catch (const std::invalid_argument&) {
    return ipv4(ipAddress);
  }
}

const in6_addr* network::IpAddress::addr() const {
  return &m_address;
}

namespace network {
  std::ostream& operator<<(std::ostream& os, const IpAddress& address) {
    char ipAddressStr[INET6_ADDRSTRLEN];
    const char* status = inet_ntop(AF_INET6, address.addr(), ipAddressStr, sizeof(ipAddressStr));
    error::throwErrnoSystemErrorIfNull(status);
    return os << status;
  }

  bool operator==(const IpAddress& lhs, const IpAddress& rhs) {
    return std::memcmp(&lhs, &rhs, sizeof(lhs)) == 0;
  }

  bool operator!=(const IpAddress& lhs, const IpAddress& rhs) {
    return !(lhs == rhs);
  }
}
