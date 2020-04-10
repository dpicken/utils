#ifndef buffer_ByteBuffer_h
#define buffer_ByteBuffer_h

#include <cstddef>
#include <memory>

namespace buffer {

class ByteBuffer {
public:
  using value_type = std::byte;
  using size_type = std::size_t;

  explicit ByteBuffer(size_type capacity);

  value_type* begin();
  value_type* end();

  const value_type* begin() const;
  const value_type* end() const;

  bool empty() const;
  size_type size() const;
  size_type remaining() const;

  /** Returns end(), then moves end() "sizeof(T) * count" bytes towards begin() + capacity. */
  template<typename T = value_type>
  T* pushTail(size_type count = 1);

  /** Returns begin(), then moves begin() "sizeof(T) * count" bytes towards end(). */
  template<typename T = value_type>
  T* popHead(size_type count = 1);

  /** Resets begin() and end(). */
  void reset();

private:
  value_type* pushTailImpl(size_type count);

  value_type* popHeadImpl(size_type count);

  const size_type m_capacity;

  std::unique_ptr<value_type[]> m_buffer;

  value_type* m_begin;
  value_type* m_end;
};

} // namespace buffer

#include "ByteBuffer.inl.h"

#endif // ifndef buffer_ByteBuffer_h
