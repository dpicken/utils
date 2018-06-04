#include "assert/Assert.h"

template<typename T, typename Traits>
inline intrusive::List<T, Traits>::List() {
}

template<typename T, typename Traits>
inline intrusive::List<T, Traits>::~List() {
  clear();
}

template<typename T, typename Traits>
inline bool intrusive::List<T, Traits>::empty() const {
  return begin() == end();
}

template<typename T, typename Traits>
inline typename intrusive::List<T, Traits>::size_type intrusive::List<T, Traits>::size() const {
  size_type s = 0;
  for (const_iterator i = begin(), e = end(); i != e; ++i) {
    ++s;
  }
  return s;
}

template<typename T, typename Traits>
inline typename intrusive::List<T, Traits>::iterator intrusive::List<T, Traits>::begin() {
  return iterator(m_head.next());
}

template<typename T, typename Traits>
inline typename intrusive::List<T, Traits>::iterator intrusive::List<T, Traits>::end() {
  return iterator(&m_head);
}

template<typename T, typename Traits>
inline typename intrusive::List<T, Traits>::const_iterator intrusive::List<T, Traits>::begin() const {
  return const_iterator(m_head.next());
}

template<typename T, typename Traits>
inline typename intrusive::List<T, Traits>::const_iterator intrusive::List<T, Traits>::end() const {
  return const_iterator(&m_head);
}

template<typename T, typename Traits>
inline typename intrusive::List<T, Traits>::reverse_iterator intrusive::List<T, Traits>::rbegin() {
  return reverse_iterator(end());
}

template<typename T, typename Traits>
inline typename intrusive::List<T, Traits>::reverse_iterator intrusive::List<T, Traits>::rend() {
  return reverse_iterator(begin());
}

template<typename T, typename Traits>
inline typename intrusive::List<T, Traits>::const_reverse_iterator intrusive::List<T, Traits>::rbegin() const {
  return const_reverse_iterator(end());
}

template<typename T, typename Traits>
inline typename intrusive::List<T, Traits>::const_reverse_iterator intrusive::List<T, Traits>::rend() const {
  return const_reverse_iterator(begin());
}

template<typename T, typename Traits>
inline T& intrusive::List<T, Traits>::front() {
  ASSERT(!empty());
  return *begin();
}

template<typename T, typename Traits>
inline T& intrusive::List<T, Traits>::back() {
  ASSERT(!empty());
  return *rbegin();
}

template<typename T, typename Traits>
inline const T& intrusive::List<T, Traits>::front() const {
  ASSERT(!empty());
  return *begin();
}

template<typename T, typename Traits>
inline const T& intrusive::List<T, Traits>::back() const {
  ASSERT(!empty());
  return *rbegin();
}

template<typename T, typename Traits>
inline void intrusive::List<T, Traits>::push_front(T& x) {
  insert(begin(), x);
}

template<typename T, typename Traits>
inline void intrusive::List<T, Traits>::push_back(T& x) {
  insert(end(), x);
}

template<typename T, typename Traits>
inline void intrusive::List<T, Traits>::pop_front() {
  erase(begin());
}

template<typename T, typename Traits>
inline void intrusive::List<T, Traits>::pop_back() {
  erase(--end());
}

template<typename T, typename Traits>
inline void intrusive::List<T, Traits>::clear() {
  while (!empty()) {
    erase(begin());
  }
}

template<typename T, typename Traits>
void intrusive::List<T, Traits>::insert(iterator pos, T& object) {
  Traits::toListNode(&object)->insert(pos.getListNode());
}

template<typename T, typename Traits>
void intrusive::List<T, Traits>::erase(iterator pos) {
  pos.getListNode()->erase();
}
