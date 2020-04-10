template<typename T>
inline T* buffer::ByteBuffer::pushTail(size_type count) {
  return reinterpret_cast<T*>(pushTailImpl(sizeof(T) * count));
}

template<typename T>
inline T* buffer::ByteBuffer::popHead(size_type count) {
  return reinterpret_cast<T*>(popHeadImpl(sizeof(T) * count));
}
