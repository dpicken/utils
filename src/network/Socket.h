#ifndef network_Socket_h
#define network_Socket_h

#include "SocketAddress.h"

#include <system_error>
#include <utility>

namespace network {

class Socket {
public:
  Socket(Socket&& socket);

  /** Ensures the socket is closed. */
  ~Socket();

  /** Creates a new TCP socket. */
  static Socket tcp();

  /** Returns true if the socket is open. */
  operator bool() const;

  /** Binds the socket to an address. */
  std::error_code bind(const SocketAddress& address) const;

  /** Returns the socket's address. */
  SocketAddress getAddress() const;

  /** Makes the socket listen for incoming connections. */
  std::error_code listen(int backlog) const;

  /** Accepts the next incoming connection. */
  std::pair<Socket, std::error_code> accept() const;

  /** Connects to the listening socket at specified address. */
  std::error_code connect(const SocketAddress& peerAddress) const;

  /** Returns the address this socket is connected to. */
  SocketAddress getPeerAddress() const;

  /** Closes the socket. */
  std::error_code close();

  /** Returns the socket's file descriptor. */
  int getFd() const;

private:
  /** Takes ownership of the specified file descriptor. */
  Socket(int fd);

  int m_fd;
};

} // namespace network

#endif // ifndef network_Socket_h
