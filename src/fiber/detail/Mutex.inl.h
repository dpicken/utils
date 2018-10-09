template<typename Traits>
inline fiber::detail::Mutex<Traits>::Mutex()
  : m_semaphore(1) {
}

template<typename Traits>
inline fiber::detail::Mutex<Traits>::~Mutex() {
}

template<typename Traits>
inline void fiber::detail::Mutex<Traits>::lock() {
  m_semaphore.wait();
}

template<typename Traits>
inline bool fiber::detail::Mutex<Traits>::tryLock() {
  return m_semaphore.tryWait();
}

template<typename Traits>
inline void fiber::detail::Mutex<Traits>::unlock() {
  m_semaphore.notifyOne();
}
