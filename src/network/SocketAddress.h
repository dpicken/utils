#ifndef network_SocketAddress_h
#define network_SocketAddress_h

#include "IpAddress.h"

#include <netinet/in.h>

#include <cstddef>
#include <cstdint>
#include <iosfwd>

namespace network {

class SocketAddress {
public:
  SocketAddress(const sockaddr_in6& m_address);

  static SocketAddress ip(const IpAddress& ipAddress, uint16_t port = 0);

  const sockaddr* addr() const;

  std::size_t addrSize() const;

  std::string str() const;

private:
  const sockaddr_in6 m_address;
};

std::ostream& operator<<(std::ostream& os, const SocketAddress& address);

bool operator==(const SocketAddress& lhs, const SocketAddress& address);

bool operator!=(const SocketAddress& lhs, const SocketAddress& address);

} // namespace network

#endif // ifndef network_SocketAddress_h
