#ifndef intrusive_ListNode_h
#define intrusive_ListNode_h

namespace intrusive {

class ListNode {
public:
  ListNode();

  ListNode(const ListNode&);
  ListNode(ListNode&&);

  ListNode& operator=(const ListNode&);
  ListNode& operator=(ListNode&&);

  ~ListNode();

  bool onList() const;

  ListNode* previous() const;

  ListNode* next() const;

  void insert(ListNode* pos);

  void erase();

private:
  ListNode* m_previous;
  ListNode* m_next;
};

} // namespace intrusive

#include "ListNode.inl.h"

#endif // ifndef intrusive_ListNode_h
