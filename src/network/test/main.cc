#include "network/Connection.h"
#include "network/IpAddress.h"
#include "network/Socket.h"
#include "network/SocketAddress.h"

#include "assert/Assert.h"
#include "buffer/ByteBuffer.h"
#include "error/throwErrnoSystemError.h"

#include <arpa/inet.h>
#include <sys/socket.h>

#include <cstring>
#include <iostream>
#include <utility>

bool operator==(const in6_addr& lhs, const in6_addr& rhs) {
  return std::memcmp(&lhs, &rhs, sizeof(in6_addr)) == 0;
}

std::ostream& operator<<(std::ostream& os, const in6_addr& rhs) {
  char ipAddressStr[INET6_ADDRSTRLEN];
  const char* status = inet_ntop(AF_INET6, &rhs, ipAddressStr, sizeof(ipAddressStr));
  error::throwErrnoSystemErrorIfNull(status);
  return os << status;
}

void testIpAddress() {
  VERIFY_EQ(*network::IpAddress::any().addr(), in6addr_any);
  VERIFY_EQ(*network::IpAddress::loopback().addr(), in6addr_loopback);

  auto a1 = network::IpAddress::ipv4("0.0.0.0");
  VERIFY_EQ(*a1.addr(), (in6_addr{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0, 0, 0}));

  auto a2 = network::IpAddress::ipv4("127.0.0.1");
  VERIFY_EQ(*a2.addr(), (in6_addr{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF, 127, 0, 0, 1}));

  try {
    network::IpAddress::ipv4("");
    VERIFY(false);
  } catch (const std::invalid_argument&) {
    VERIFY(true);
  }
  try {
    network::IpAddress::ipv4("::");
    VERIFY(false);
  } catch (const std::invalid_argument&) {
    VERIFY(true);
  }

  auto a3 = network::IpAddress::ipv6("::");
  VERIFY_EQ(*a3.addr(), in6addr_any);

  auto a4 = network::IpAddress::ipv6("::1");
  VERIFY_EQ(*a4.addr(), in6addr_loopback);

  try {
    network::IpAddress::ipv6("");
    VERIFY(false);
  } catch (const std::invalid_argument&) {
    VERIFY(true);
  }
  try {
    network::IpAddress::ipv6("0.0.0.0");
    VERIFY(false);
  } catch (const std::invalid_argument&) {
    VERIFY(true);
  }

  auto a5 = network::IpAddress::ip("127.0.0.1");
  VERIFY_EQ(*a5.addr(), (in6_addr{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF, 127, 0, 0, 1}));

  auto a6= network::IpAddress::ip("::1");
  VERIFY_EQ(*a6.addr(), in6addr_loopback);

  try {
    network::IpAddress::ip("");
    VERIFY(false);
  } catch (const std::invalid_argument&) {
    VERIFY(true);
  }
}

void testSocketAddress() {
  auto a1 = network::SocketAddress::ip(network::IpAddress::any());
  VERIFY_EQ(a1.str(), "[::]:0");

  auto a2 = network::SocketAddress::ip(network::IpAddress::any(), 100);
  VERIFY_EQ(a2.str(), "[::]:100");
}

void testSocket() {
  auto s1 = network::Socket::tcp();
  VERIFY(s1);
  VERIFY(!s1.close());
  VERIFY(!s1);
  VERIFY_EQ(s1.close().value(), EBADF);

  auto s2 = network::Socket::tcp();
  VERIFY(s2);

  network::Socket s3(std::move(s2));
  VERIFY(!s2);
  VERIFY(s3);
  VERIFY(!s3.close());
  VERIFY(!s3);

  VERIFY_EQ(s2.close().value(), EBADF);
  VERIFY_EQ(s3.close().value(), EBADF);

  auto s4 = network::Socket::tcp();
  VERIFY(s4);
  VERIFY(!s4.bind(network::SocketAddress::ip(network::IpAddress::any())));
  VERIFY(!s4.close());
  VERIFY(!s4);

  auto s5 = network::Socket::tcp();
  VERIFY(s5);
  VERIFY(!s5.bind(network::SocketAddress::ip(network::IpAddress::ip("::"))));
  VERIFY(!s5.close());
  VERIFY(!s5);

  auto s6 = network::Socket::tcp();
  VERIFY(s6);
  VERIFY(!s6.bind(network::SocketAddress::ip(network::IpAddress::ip("127.0.0.1"))));
  VERIFY(!s6.close());
  VERIFY(!s6);

  auto s7 = network::Socket::tcp();
  VERIFY(s7);
  VERIFY(!s7.bind(network::SocketAddress::ip(network::IpAddress::any(), 50000)));
  VERIFY_EQ(s7.getAddress(), network::SocketAddress::ip(network::IpAddress::any(), 50000));

  {
    auto s8 = network::Socket::tcp();
    VERIFY(s8);
    VERIFY_EQ(s8.bind(network::SocketAddress::ip(network::IpAddress::any(), 50000)).value(), EADDRINUSE);
    VERIFY(!s8.bind(network::SocketAddress::ip(network::IpAddress::any(), 50001)));
    VERIFY_EQ(s8.getAddress(), network::SocketAddress::ip(network::IpAddress::any(), 50001));
    VERIFY(!s8.close());
    VERIFY(!s8);
  }

  auto s9 = network::Socket::tcp();
  VERIFY(s9);
  VERIFY(!s9.bind(network::SocketAddress::ip(network::IpAddress::any(), 50001)));
  VERIFY_EQ(s9.getAddress(), network::SocketAddress::ip(network::IpAddress::any(), 50001));
  VERIFY(!s9.close());
  VERIFY(!s9);

  auto s10 = network::Socket::tcp();
  VERIFY(s10);
  VERIFY(!s10.bind(network::SocketAddress::ip(network::IpAddress::any(), 50001)));
  VERIFY_EQ(s10.getAddress(), network::SocketAddress::ip(network::IpAddress::any(), 50001));
  VERIFY(!s10.listen(1));

  auto s11 = network::Socket::tcp();
  VERIFY(s11);
  VERIFY(!s11.connect(s10.getAddress()));

  auto es12 = s10.accept();
  VERIFY(!es12.second);
  auto s12 = std::move(es12.first);
  VERIFY(s12);

  VERIFY(!s10.close());
  VERIFY(!s10);

  network::Connection c11(std::move(s11));
  VERIFY(!s11);
  VERIFY_EQ(s11.close().value(), EBADF);

  network::Connection c12(std::move(s12));
  VERIFY(!s12);
  VERIFY_EQ(s12.close().value(), EBADF);

  const std::string testData = "testData";
  auto xferSize = testData.size();

  buffer::ByteBuffer sendBuffer(1024);
  buffer::ByteBuffer receiveBuffer(1024);

  testData.copy(sendBuffer.pushTail<char>(xferSize), xferSize);
  while (!sendBuffer.empty()) {
    VERIFY(!c11.write(sendBuffer));
  }

  while (receiveBuffer.size() < xferSize) {
    VERIFY(!c12.read(receiveBuffer));
  }
  VERIFY_EQ(receiveBuffer.size(), xferSize);
  VERIFY_EQ(std::string(receiveBuffer.popHead<char>(xferSize), xferSize), testData);
}

int main(int, char**) {
  testIpAddress();
  testSocketAddress();
  testSocket();
  return 0;
}
