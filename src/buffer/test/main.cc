#include "buffer/ByteBuffer.h"

#include "assert/Assert.h"

int main(int, char**) {
  buffer::ByteBuffer::size_type capacity = 8;
  buffer::ByteBuffer buffer(capacity);
  const buffer::ByteBuffer& cbuffer = buffer;

  VERIFY_EQ(buffer.begin(), buffer.end());
  VERIFY_EQ(cbuffer.begin(), cbuffer.end());
  VERIFY(buffer.empty());
  VERIFY_EQ(buffer.size(), 0U);
  VERIFY_EQ(buffer.remaining(), capacity);

  buffer::ByteBuffer::size_type size = capacity / 2;
  buffer.pushTail(size);

  VERIFY_EQ(buffer.end() - buffer.begin(), int(size));
  VERIFY_EQ(cbuffer.end() - cbuffer.begin(), int(size));
  VERIFY(!buffer.empty());
  VERIFY_EQ(buffer.size(), size);
  VERIFY_EQ(buffer.remaining(), capacity - size);

  buffer::ByteBuffer::size_type pop = size / 2;
  size = size - pop;
  buffer.popHead(pop);

  VERIFY_EQ(buffer.end() - buffer.begin(), int(size));
  VERIFY_EQ(cbuffer.end() - cbuffer.begin(), int(size));
  VERIFY(!buffer.empty());
  VERIFY_EQ(buffer.size(), size);
  VERIFY_EQ(buffer.remaining(), capacity - (pop + size));

  buffer.reset();

  VERIFY_EQ(buffer.begin(), buffer.end());
  VERIFY_EQ(cbuffer.begin(), cbuffer.end());
  VERIFY(buffer.empty());
  VERIFY_EQ(buffer.size(), 0U);
  VERIFY_EQ(buffer.remaining(), capacity);

  VERIFY_NE(buffer.pushTail(capacity), nullptr);
  try {
    buffer.pushTail();
    VERIFY(false);
  } catch (const std::out_of_range&) {
    VERIFY(true);
  }
  VERIFY_NE(buffer.popHead(capacity), nullptr);
  try {
    buffer.popHead();
    VERIFY(false);
  } catch (const std::out_of_range&) {
    VERIFY(true);
  }

  return 0;
}
