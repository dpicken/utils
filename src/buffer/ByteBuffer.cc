#include "ByteBuffer.h"

#include "assert/Assert.h"

#include <stdexcept>

buffer::ByteBuffer::ByteBuffer(size_type capacity)
  : m_capacity(capacity)
  , m_buffer(new value_type[m_capacity]) {
  reset();
}

buffer::ByteBuffer::value_type* buffer::ByteBuffer::begin() {
  return m_begin;
}

buffer::ByteBuffer::value_type* buffer::ByteBuffer::end() {
  return m_end;
}

const buffer::ByteBuffer::value_type* buffer::ByteBuffer::begin() const {
  return m_begin;
}

const buffer::ByteBuffer::value_type* buffer::ByteBuffer::end() const {
  return m_end;
}

bool buffer::ByteBuffer::empty() const {
  return begin() == end();
}

buffer::ByteBuffer::size_type buffer::ByteBuffer::size() const {
  return size_type(end() - begin());
}

buffer::ByteBuffer::size_type buffer::ByteBuffer::remaining() const {
  return size_type((m_buffer.get() + m_capacity) - m_end);
}

buffer::ByteBuffer::value_type* buffer::ByteBuffer::pushTailImpl(size_type count) {
  if (remaining() < count) {
    throw std::out_of_range("remaining() insufficient");
  }

  value_type* buffer = m_end;
  m_end += count;
  return buffer;
}

buffer::ByteBuffer::value_type* buffer::ByteBuffer::popHeadImpl(size_type count) {
  if (size() < count) {
    throw std::out_of_range("size() insufficient");
  }

  value_type* buffer = m_begin;
  m_begin += count;
  return buffer;
}

void buffer::ByteBuffer::reset() {
  m_begin = m_buffer.get();
  m_end = m_begin;
}
