#include "assert/Assert.h"

inline intrusive::ListNode::ListNode()
  : m_previous(this)
  , m_next(this) {
}

inline intrusive::ListNode::ListNode(const ListNode&)
  : m_previous(this)
  , m_next(this) {
}

inline intrusive::ListNode::ListNode(ListNode&&)
  : m_previous(this)
  , m_next(this) {
}

inline intrusive::ListNode& intrusive::ListNode::operator=(const ListNode&) {
  return *this;
}

inline intrusive::ListNode& intrusive::ListNode::operator=(ListNode&&) {
  return *this;
}

inline intrusive::ListNode::~ListNode() {
  ASSERT(!onList());
}

inline bool intrusive::ListNode::onList() const {
  ASSERT_EQ(m_previous == this, m_next == this);
  return m_previous != this;
}

inline intrusive::ListNode* intrusive::ListNode::previous() const {
  return m_previous;
}

inline intrusive::ListNode* intrusive::ListNode::next() const {
  return m_next;
}

inline void intrusive::ListNode::insert(ListNode* pos) {
  ASSERT(!onList());
  m_previous = pos->m_previous;
  m_next = pos;
  m_previous->m_next = this;
  m_next->m_previous = this;
}

inline void intrusive::ListNode::erase() {
  ASSERT(onList());
  m_previous->m_next = m_next;
  m_next->m_previous = m_previous;
  m_previous = this;
  m_next = this;
}
