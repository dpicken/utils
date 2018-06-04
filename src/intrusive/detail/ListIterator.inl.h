template<typename T, typename ListNodeT, typename Traits>
inline intrusive::detail::ListIterator<T, ListNodeT, Traits>::ListIterator()
  : m_pos(0) {
}

template<typename T, typename ListNodeT, typename Traits>
inline intrusive::detail::ListIterator<T, ListNodeT, Traits>::ListIterator(ListNodeT* pos)
  : m_pos(pos) {
}

template<typename T, typename ListNodeT, typename Traits>
inline intrusive::detail::ListIterator<T, ListNodeT, Traits>& intrusive::detail::ListIterator<T, ListNodeT, Traits>::operator++() {
  m_pos = m_pos->next();
  return *this;
}

template<typename T, typename ListNodeT, typename Traits>
inline intrusive::detail::ListIterator<T, ListNodeT, Traits> intrusive::detail::ListIterator<T, ListNodeT, Traits>::operator++(int) {
  ListIterator it(m_pos);
  ++(*this);
  return it;
}

template<typename T, typename ListNodeT, typename Traits>
inline intrusive::detail::ListIterator<T, ListNodeT, Traits>& intrusive::detail::ListIterator<T, ListNodeT, Traits>::operator--() {
  m_pos = m_pos->previous();
  return *this;
}

template<typename T, typename ListNodeT, typename Traits>
inline intrusive::detail::ListIterator<T, ListNodeT, Traits> intrusive::detail::ListIterator<T, ListNodeT, Traits>::operator--(int) {
  ListIterator it(m_pos);
  --(*this);
  return it;
}

template<typename T, typename ListNodeT, typename Traits>
inline T& intrusive::detail::ListIterator<T, ListNodeT, Traits>::operator*() const {
  return *operator->();
}

template<typename T, typename ListNodeT, typename Traits>
inline T* intrusive::detail::ListIterator<T, ListNodeT, Traits>::operator->() const {
  return Traits::toObject(m_pos);
}

template<typename T, typename ListNodeT, typename Traits>
inline bool intrusive::detail::ListIterator<T, ListNodeT, Traits>::operator==(const ListIterator& rhs) const {
  return m_pos == rhs.m_pos;
}

template<typename T, typename ListNodeT, typename Traits>
inline bool intrusive::detail::ListIterator<T, ListNodeT, Traits>::operator!=(const ListIterator& rhs) const {
  return !(*this == rhs);
}

template<typename T, typename ListNodeT, typename Traits>
inline ListNodeT* intrusive::detail::ListIterator<T, ListNodeT, Traits>::getListNode() const {
  return m_pos;
}
