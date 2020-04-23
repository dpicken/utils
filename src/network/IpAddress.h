#ifndef network_IpAddress_h
#define network_IpAddress_h

#include <netinet/in.h>

#include <iosfwd>
#include <string>

namespace network {

class IpAddress {
public:
  IpAddress(const in6_addr& address);

  static const IpAddress& any();

  static const IpAddress& loopback();

  static IpAddress ipv6(const std::string& address);

  static IpAddress ipv4(const std::string& address);

  static IpAddress ip(const std::string& address);

  const in6_addr* addr() const;

private:
  const in6_addr m_address;
};

std::ostream& operator<<(std::ostream& os, const IpAddress& address);

bool operator==(const IpAddress& lhs, const IpAddress& address);

bool operator!=(const IpAddress& lhs, const IpAddress& address);

} // namespace network

#endif // ifndef network_IpAddress_h
