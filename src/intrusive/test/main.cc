#include "assert/Assert.h"
#include "intrusive/List.h"

#include <utility>

namespace {

struct ListEntry {
  explicit ListEntry(char id)
    : id(id) {
  }

  char id;
  intrusive::ListNode listNode;
};

typedef INTRUSIVE_LIST(ListEntry, listNode) List;

void testEmptyList() {
  List list;
  VERIFY(list.empty());
  VERIFY_EQ(list.size(), 0U);
  VERIFY(list.begin() == list.end());
}

template<typename Iterator>
void testIteratorEq(const Iterator& it, const ListEntry& entry) {
  VERIFY_EQ(&*it, &entry);
  VERIFY_EQ(it->id, entry.id);
}

void testPushBack() {
  List list;

  ListEntry a('a');
  list.push_back(a);
  VERIFY(!list.empty());
  VERIFY_EQ(list.size(), 1U);
  auto it = list.begin();
  testIteratorEq(it, a);
  VERIFY(++it == list.end());

  ListEntry b('b');
  list.push_back(b);
  VERIFY(!list.empty());
  VERIFY_EQ(list.size(), 2U);
  it = list.begin();
  testIteratorEq(it, a);
  testIteratorEq(++it, b);
  VERIFY(++it == list.end());

  ListEntry c('c');
  list.push_back(c);
  VERIFY(!list.empty());
  VERIFY_EQ(list.size(), 3U);
  it = list.begin();
  testIteratorEq(it, a);
  testIteratorEq(++it, b);
  testIteratorEq(++it, c);
  VERIFY(++it == list.end());

  list.clear();
  VERIFY(list.empty());
  VERIFY_EQ(list.size(), 0U);
  VERIFY(list.begin() == list.end());
}

void testPopBack() {
  ListEntry a('a');
  ListEntry b('b');
  ListEntry c('c');

  List list;
  list.push_back(a);
  list.push_back(b);
  list.push_back(c);

  VERIFY(!list.empty());
  VERIFY_EQ(list.size(), 3U);
  auto it = list.begin();
  testIteratorEq(it, a);
  testIteratorEq(++it, b);
  testIteratorEq(++it, c);
  VERIFY(++it == list.end());

  list.pop_back();
  VERIFY(!list.empty());
  VERIFY_EQ(list.size(), 2U);
  it = list.begin();
  testIteratorEq(it, a);
  testIteratorEq(++it, b);
  VERIFY(++it == list.end());

  list.pop_back();
  VERIFY(!list.empty());
  VERIFY_EQ(list.size(), 1U);
  it = list.begin();
  testIteratorEq(it, a);
  VERIFY(++it == list.end());

  list.pop_back();
  VERIFY(list.empty());
  VERIFY_EQ(list.size(), 0U);
  it = list.begin();
  VERIFY(it == list.end());
}

void testPushFront() {
  List list;

  ListEntry a('a');
  list.push_front(a);
  VERIFY(!list.empty());
  VERIFY_EQ(list.size(), 1U);
  auto it = list.begin();
  testIteratorEq(it, a);
  VERIFY(++it == list.end());

  ListEntry b('b');
  list.push_front(b);
  VERIFY(!list.empty());
  VERIFY_EQ(list.size(), 2U);
  it = list.begin();
  testIteratorEq(it, b);
  testIteratorEq(++it, a);
  VERIFY(++it == list.end());

  ListEntry c('c');
  list.push_front(c);
  VERIFY(!list.empty());
  VERIFY_EQ(list.size(), 3U);
  it = list.begin();
  testIteratorEq(it, c);
  testIteratorEq(++it, b);
  testIteratorEq(++it, a);
  VERIFY(++it == list.end());

  list.clear();
  VERIFY(list.empty());
  VERIFY_EQ(list.size(), 0U);
  VERIFY(list.begin() == list.end());
}

void testPopFront() {
  ListEntry a('a');
  ListEntry b('b');
  ListEntry c('c');

  List list;
  list.push_back(a);
  list.push_back(b);
  list.push_back(c);

  VERIFY(!list.empty());
  VERIFY_EQ(list.size(), 3U);
  auto it = list.begin();
  testIteratorEq(it, a);
  testIteratorEq(++it, b);
  testIteratorEq(++it, c);
  VERIFY(++it == list.end());

  list.pop_front();
  VERIFY(!list.empty());
  VERIFY_EQ(list.size(), 2U);
  it = list.begin();
  testIteratorEq(it, b);
  testIteratorEq(++it, c);
  VERIFY(++it == list.end());

  list.pop_front();
  VERIFY(!list.empty());
  VERIFY_EQ(list.size(), 1U);
  it = list.begin();
  testIteratorEq(it, c);
  VERIFY(++it == list.end());

  list.pop_front();
  VERIFY(list.empty());
  VERIFY_EQ(list.size(), 0U);
  it = list.begin();
  VERIFY(it == list.end());
}

void testInsert() {
  List list;

  ListEntry a('a');
  ListEntry c('c');

  list.push_back(a);
  list.push_back(c);

  VERIFY(!list.empty());
  VERIFY_EQ(list.size(), 2U);
  auto it = list.begin();
  testIteratorEq(it, a);
  testIteratorEq(++it, c);
  VERIFY(++it == list.end());

  // Insert into middle
  ListEntry b('b');
  list.insert(++list.begin(), b);
  VERIFY(!list.empty());
  VERIFY_EQ(list.size(), 3U);
  it = list.begin();
  testIteratorEq(it, a);
  testIteratorEq(++it, b);
  testIteratorEq(++it, c);
  VERIFY(++it == list.end());

  list.clear();
  VERIFY(list.empty());
  VERIFY_EQ(list.size(), 0U);
  VERIFY(list.begin() == list.end());
}

void testErase() {
  ListEntry a('a');
  ListEntry b('b');
  ListEntry c('c');

  List list;
  list.push_back(a);
  list.push_back(b);
  list.push_back(c);

  VERIFY(!list.empty());
  VERIFY_EQ(list.size(), 3U);
  auto it = list.begin();
  testIteratorEq(it, a);
  testIteratorEq(++it, b);
  testIteratorEq(++it, c);
  VERIFY(++it == list.end());

  // Erase from middle
  list.erase(++list.begin());
  VERIFY(!list.empty());
  VERIFY_EQ(list.size(), 2U);
  it = list.begin();
  testIteratorEq(it, a);
  testIteratorEq(++it, c);
  VERIFY(++it == list.end());

  list.clear();
  VERIFY(list.empty());
  VERIFY_EQ(list.size(), 0U);
  VERIFY(list.begin() == list.end());
}

void testNodeErase() {
  List list;

  ListEntry a('a');
  ListEntry b('b');
  ListEntry c('c');

  list.push_back(a);
  list.push_back(b);
  list.push_back(c);

  VERIFY(!list.empty());
  VERIFY_EQ(list.size(), 3U);
  auto it = list.begin();
  testIteratorEq(it, a);
  testIteratorEq(++it, b);
  testIteratorEq(++it, c);
  VERIFY(++it == list.end());

  // Erase from middle
  b.listNode.erase();
  VERIFY(!list.empty());
  VERIFY_EQ(list.size(), 2U);
  it = list.begin();
  testIteratorEq(it, a);
  testIteratorEq(++it, c);
  VERIFY(++it == list.end());

  // Erase from end
  c.listNode.erase();
  VERIFY(!list.empty());
  VERIFY_EQ(list.size(), 1U);
  it = list.begin();
  testIteratorEq(it, a);
  VERIFY(++it == list.end());

  list.push_back(b);
  VERIFY(!list.empty());
  VERIFY_EQ(list.size(), 2U);
  it = list.begin();
  testIteratorEq(it, a);
  testIteratorEq(++it, b);
  VERIFY(++it == list.end());

  // Erase from begin, next node is not-end
  a.listNode.erase();
  VERIFY(!list.empty());
  VERIFY_EQ(list.size(), 1U);
  it = list.begin();
  testIteratorEq(it, b);
  VERIFY(++it == list.end());

  // Erase from begin, next node is end
  b.listNode.erase();
  VERIFY(list.empty());
  VERIFY_EQ(list.size(), 0U);
  VERIFY(list.begin() == list.end());
}

void testIteration() {
  ListEntry a('a');
  ListEntry b('b');
  ListEntry c('c');

  List list;
  list.push_back(a);
  list.push_back(b);
  list.push_back(c);

  // Pre increment/decrement
  VERIFY(!list.empty());
  VERIFY_EQ(list.size(), 3U);
  auto it = list.begin();
  testIteratorEq(it, a);
  testIteratorEq(++it, b);
  testIteratorEq(++it, c);
  VERIFY(++it == list.end());
  testIteratorEq(--it, c);
  testIteratorEq(--it, b);
  testIteratorEq(--it, a);
  VERIFY(it == list.begin());

  // Const pre increment/decrement
  auto cit = std::as_const(list).begin();
  testIteratorEq(cit, a);
  testIteratorEq(++cit, b);
  testIteratorEq(++cit, c);
  VERIFY(++cit == std::as_const(list).end());
  testIteratorEq(--cit, c);
  testIteratorEq(--cit, b);
  testIteratorEq(--cit, a);

  // Post increment/decrement
  testIteratorEq(it++, a);
  testIteratorEq(it, b);
  testIteratorEq(it++, b);
  testIteratorEq(it, c);
  testIteratorEq(it++, c);
  VERIFY(it == list.end());
  VERIFY(it-- == list.end());
  testIteratorEq(it, c);
  testIteratorEq(it--, c);
  testIteratorEq(it, b);
  testIteratorEq(it--, b);
  testIteratorEq(it, a);
  VERIFY(it == list.begin());

  // Const post increment/decrement
  testIteratorEq(cit++, a);
  testIteratorEq(cit, b);
  testIteratorEq(cit++, b);
  testIteratorEq(cit, c);
  testIteratorEq(cit++, c);
  VERIFY(cit == std::as_const(list).end());
  VERIFY(cit-- == std::as_const(list).end());
  testIteratorEq(cit, c);
  testIteratorEq(cit--, c);
  testIteratorEq(cit, b);
  testIteratorEq(cit--, b);
  testIteratorEq(cit, a);
  VERIFY(cit == std::as_const(list).begin());

  list.clear();
  VERIFY(list.empty());
  VERIFY_EQ(list.size(), 0U);
  VERIFY(list.begin() == list.end());
}

void testReverseIteration() {
  ListEntry c('c');
  ListEntry b('b');
  ListEntry a('a');

  List list;
  list.push_back(c);
  list.push_back(b);
  list.push_back(a);

  // Pre increment/decrement
  VERIFY(!list.empty());
  VERIFY_EQ(list.size(), 3U);
  auto it = list.rbegin();
  testIteratorEq(it, a);
  testIteratorEq(++it, b);
  testIteratorEq(++it, c);
  VERIFY(++it == list.rend());
  testIteratorEq(--it, c);
  testIteratorEq(--it, b);
  testIteratorEq(--it, a);
  VERIFY(it == list.rbegin());

  // Const pre increment/decrement
  auto cit = std::as_const(list).rbegin();
  testIteratorEq(cit, a);
  testIteratorEq(++cit, b);
  testIteratorEq(++cit, c);
  VERIFY(++cit == std::as_const(list).rend());
  testIteratorEq(--cit, c);
  testIteratorEq(--cit, b);
  testIteratorEq(--cit, a);

  // Post increment/decrement
  testIteratorEq(it++, a);
  testIteratorEq(it, b);
  testIteratorEq(it++, b);
  testIteratorEq(it, c);
  testIteratorEq(it++, c);
  VERIFY(it == list.rend());
  VERIFY(it-- == list.rend());
  testIteratorEq(it, c);
  testIteratorEq(it--, c);
  testIteratorEq(it, b);
  testIteratorEq(it--, b);
  testIteratorEq(it, a);
  VERIFY(it == list.rbegin());

  // Const post increment/decrement
  testIteratorEq(cit++, a);
  testIteratorEq(cit, b);
  testIteratorEq(cit++, b);
  testIteratorEq(cit, c);
  testIteratorEq(cit++, c);
  VERIFY(cit == std::as_const(list).rend());
  VERIFY(cit-- == std::as_const(list).rend());
  testIteratorEq(cit, c);
  testIteratorEq(cit--, c);
  testIteratorEq(cit, b);
  testIteratorEq(cit--, b);
  testIteratorEq(cit, a);
  VERIFY(cit == std::as_const(list).rbegin());

  list.clear();
  VERIFY(list.empty());
  VERIFY_EQ(list.size(), 0U);
  VERIFY(list.begin() == list.end());
}

void testCopyConstruct() {
  {
    ListEntry a('a');
    ListEntry a2(a);

    VERIFY_EQ(a2.id, a.id);
    VERIFY(!a.listNode.onList());
    VERIFY(!a2.listNode.onList());
  }

  {
    ListEntry a('a');
    List list;
    list.push_back(a);

    ListEntry a2(a);

    VERIFY_EQ(a.id, a2.id);
    VERIFY(a.listNode.onList());
    VERIFY(!a2.listNode.onList());
  }
}

void testAssign() {
  {
    ListEntry a('a');
    ListEntry a2('2');
    a2 = a;

    VERIFY_EQ(a2.id, a.id);
    VERIFY(!a.listNode.onList());
    VERIFY(!a2.listNode.onList());
  }

  {
    ListEntry a('a');
    List list;
    list.push_back(a);

    ListEntry a2('2');
    a2 = a;

    VERIFY_EQ(a2.id, a.id);
    VERIFY(a.listNode.onList());
    VERIFY(!a2.listNode.onList());
  }
}

void testMoveConstruct() {
  {
    ListEntry a('a');
    ListEntry a2(std::move(a));

    VERIFY_EQ(a2.id, a.id);
    VERIFY(!a.listNode.onList());
    VERIFY(!a2.listNode.onList());
  }

  {
    ListEntry a('a');
    List list;
    list.push_back(a);

    ListEntry a2(std::move(a));

    VERIFY_EQ(a2.id, a.id);
    VERIFY(a.listNode.onList());
    VERIFY(!a2.listNode.onList());
  }
}

void testMoveAssign() {
  {
    ListEntry a('a');
    ListEntry a2('2');
    a2 = std::move(a);

    VERIFY_EQ(a2.id, a.id);
    VERIFY(!a.listNode.onList());
    VERIFY(!a2.listNode.onList());
  }

  {
    ListEntry a('a');
    List list;
    list.push_back(a);

    ListEntry a2('2');
    a2 = std::move(a);

    VERIFY_EQ(a2.id, a.id);
    VERIFY(a.listNode.onList());
    VERIFY(!a2.listNode.onList());
  }
}

} // namespace

int main(int, char**)
{
  testEmptyList();
  testPushBack();
  testPopBack();
  testPushFront();
  testPopFront();
  testInsert();
  testErase();
  testNodeErase();
  testIteration();
  testReverseIteration();
  testCopyConstruct();
  testAssign();
  testMoveConstruct();
  testMoveAssign();
  return 0;
}
