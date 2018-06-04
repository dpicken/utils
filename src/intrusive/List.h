#ifndef intrusive_List_h
#define intrusive_List_h

#include "ListNode.h"

#include "detail/ListIterator.h"
#include "detail/ListTraits.h"

#include <cstddef>
#include <iterator>

#define INTRUSIVE_LIST(T, listNodeMember) intrusive::List<T, INTRUSIVE_LIST_TRAITS(T, listNodeMember) >

namespace intrusive {

/**
 * An STL-like intrusive list.
 *
 * Example usage:
 *
 * struct ExampleObject {
 *   int member1, member2, etc;
 *   intrusive::ListNode list1Node;
 * };
 *
 * typedef INTRUSIVE_LIST(ExampleObject, list1Node) List1;
 *
 * void example() {
 *   ExampleObject exampleObject;
 *
 *   List1 list;
 *   list.push_back(exampleObject);
 *   assert(&list.front() == &exampleObject);
 *  }
 */
template<typename T, typename Traits>
class List {
public:
  typedef T value_type;
  typedef std::size_t size_type;

  typedef detail::ListIterator<T, typename Traits::list_node_type, Traits> iterator;
  typedef detail::ListIterator<const T, const typename Traits::list_node_type, Traits> const_iterator;

  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  List();

  List(const List&) = delete;
  List& operator=(const List&) = delete;

  ~List();

  bool empty() const;

  size_type size() const;

  iterator begin();
  iterator end();

  const_iterator begin() const;
  const_iterator end() const;

  reverse_iterator rbegin();
  reverse_iterator rend();

  const_reverse_iterator rbegin() const;
  const_reverse_iterator rend() const;

  T& front();
  T& back();

  const T& front() const;
  const T& back() const;

  void push_front(T& x);
  void push_back(T& x);

  void pop_front();
  void pop_back();

  void clear();

  void insert(iterator pos, T& object);
  void erase(iterator pos);

private:
  typename Traits::list_node_type m_head;
};

} // namespace intrusive

#include "List.inl.h"

#endif // ifndef intrusive_List_h
