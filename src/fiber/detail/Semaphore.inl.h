template<typename Traits>
inline fiber::detail::Semaphore<Traits>::Semaphore(size_type count)
  : m_count(count) {
}

template<typename Traits>
inline void fiber::detail::Semaphore<Traits>::wait() {
  UniqueLock lock(m_mutex);
  while (m_count == 0) {
    m_countIncremented.wait(lock);
  }

  --m_count;
}

template<typename Traits>
inline bool fiber::detail::Semaphore<Traits>::tryWait() {
  UniqueLock lock(m_mutex);
  if (m_count == 0) {
    return false;
  }

  --m_count;
  return true;
}

template<typename Traits>
inline void fiber::detail::Semaphore<Traits>::notifyOne() {
  UniqueLock lock(m_mutex);
  ++m_count;
  lock.unlock();

  m_countIncremented.notifyOne();
}
