#ifndef intrusive_detail_ListIterator_h
#define intrusive_detail_ListIterator_h

#include <iterator>

namespace intrusive { namespace detail {

template<typename T, typename ListNodeT, typename Traits>
class ListIterator {
public:
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef T value_type;
  typedef std::ptrdiff_t difference_type;
  typedef T* pointer;
  typedef T& reference;

  ListIterator();

  explicit ListIterator(ListNodeT* pos);

  ListIterator& operator++();
  ListIterator operator++(int);

  ListIterator& operator--();
  ListIterator operator--(int);

  reference operator*() const;
  pointer operator->() const;

  bool operator==(const ListIterator& rhs) const;
  bool operator!=(const ListIterator& rhs) const;

  ListNodeT* getListNode() const;

private:
  ListNodeT* m_pos;
};

} } // namespace intrusive::detail

#include "ListIterator.inl.h"

#endif // ifndef intrusive_detail_ListIterator_h
