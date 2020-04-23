#ifndef network_Connection_h
#define network_Connection_h

#include "Socket.h"

#include "buffer/ByteBuffer.h"

#include <system_error>

namespace network {

class Connection {
public:
  explicit Connection(Socket&& socket);

  Connection(Connection&& connection);

  /** Read from the socket (appends to the back of the buffer). */
  std::error_code read(buffer::ByteBuffer& buffer) const;

  /** Read up to "count" bytes from the socket (appends to the back of the buffer). */
  std::error_code read(buffer::ByteBuffer& buffer, buffer::ByteBuffer::size_type count) const;

  /** Write to the socket (consumes data from the front of the buffer). */
  std::error_code write(buffer::ByteBuffer& buffer) const;

  /** Write up to "count" bytes to the socket (consumes from the front of the buffer). */
  std::error_code write(buffer::ByteBuffer& buffer, buffer::ByteBuffer::size_type count) const;

private:
  Socket m_socket;
};

} // namespace network

#endif // ifndef network_Connection_h
