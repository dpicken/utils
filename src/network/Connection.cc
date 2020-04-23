#include "Connection.h"

#include "assert/Assert.h"
#include "error/makeErrnoErrorCode.h"

#include <unistd.h>

#include <utility>

network::Connection::Connection(Socket&& socket)
  : m_socket(std::move(socket)) {
}

network::Connection::Connection(Connection&& connection)
  : m_socket(std::move(connection.m_socket)) {
}

std::error_code network::Connection::read(buffer::ByteBuffer& buffer) const {
  return read(buffer, buffer.remaining());
}

std::error_code network::Connection::read(buffer::ByteBuffer& buffer, buffer::ByteBuffer::size_type count) const {
  VERIFY_GE(buffer.remaining(), count);

  ssize_t status;
  do {
    status = ::read(m_socket.getFd(), buffer.end(), count);
  } while (status == -1 && errno == EINTR);

  if (status == -1) {
    return error::makeErrnoErrorCode();
  }

  buffer.pushTail(buffer::ByteBuffer::size_type(status));
  return std::error_code();
}

std::error_code network::Connection::write(buffer::ByteBuffer& buffer) const {
  return write(buffer, buffer.size());
}

std::error_code network::Connection::write(buffer::ByteBuffer& buffer, buffer::ByteBuffer::size_type count) const {
  VERIFY_LE(buffer.size(), count);

  ssize_t status;
  do {
    status = ::write(m_socket.getFd(), buffer.begin(), count);
  } while (status == -1 && errno == EINTR);

  if (status == -1) {
    return error::makeErrnoErrorCode();
  }

  buffer.popHead(buffer::ByteBuffer::size_type(status));
  return std::error_code();
}
