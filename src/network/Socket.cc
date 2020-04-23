#include "Socket.h"

#include "assert/Assert.h"
#include "error/makeErrnoErrorCode.h"
#include "error/throwErrnoSystemError.h"

#include <sys/socket.h>
#include <unistd.h>

network::Socket::Socket(int fd)
  : m_fd(fd) {
}

network::Socket::Socket(Socket&& socket)
  : m_fd(-1) {
  std::swap(m_fd, socket.m_fd);
}

network::Socket::~Socket() {
  if (*this) {
    close();
  }
}

network::Socket network::Socket::tcp() {
  int status = socket(AF_INET6, SOCK_STREAM, 0);
  error::throwErrnoSystemErrorIfEq(status, -1);
  return Socket(status);
}

network::Socket::operator bool() const {
  return m_fd != -1;
}

std::error_code network::Socket::bind(const SocketAddress& address) const {
  int status = ::bind(m_fd, address.addr(), address.addrSize());
  return error::makeErrnoErrorCodeIfEq(status, -1);
}

network::SocketAddress network::Socket::getAddress() const {
  sockaddr_in6 addr;
  socklen_t addrSize = sizeof(addr);
  int status = getsockname(m_fd, reinterpret_cast<sockaddr*>(&addr), &addrSize);
  error::throwErrnoSystemErrorIfEq(status, -1);
  ASSERT_EQ(addrSize, sizeof(addr));
  return SocketAddress(addr);
}

std::error_code network::Socket::listen(int backlog) const {
  int status = ::listen(m_fd, backlog);
  return error::makeErrnoErrorCodeIfEq(status, -1);
}

std::pair<network::Socket, std::error_code> network::Socket::accept() const {
  int status;
  do {
    status = ::accept(m_fd, nullptr, nullptr);
  } while (status == -1 && errno == EINTR);

  return std::make_pair(Socket(status), error::makeErrnoErrorCodeIfEq(status, -1));
}

std::error_code network::Socket::connect(const SocketAddress& peerAddress) const {
  int status;
  do {
    status = ::connect(m_fd, peerAddress.addr(), peerAddress.addrSize());
  } while (status == -1 && errno == EINTR);

  return error::makeErrnoErrorCodeIfEq(status, -1);
}

network::SocketAddress network::Socket::getPeerAddress() const {
  sockaddr_in6 addr;
  socklen_t addrSize = sizeof(addr);
  int status = getpeername(m_fd, reinterpret_cast<sockaddr*>(&addr), &addrSize);
  error::throwErrnoSystemErrorIfEq(status, -1);
  ASSERT_EQ(addrSize, sizeof(addr));
  return SocketAddress(addr);
}

std::error_code network::Socket::close() {
  int fd = -1;
  std::swap(fd, m_fd);
  int status = ::close(fd);
  return error::makeErrnoErrorCodeIfEq(status, -1);
}

int network::Socket::getFd() const {
  return m_fd;
}
