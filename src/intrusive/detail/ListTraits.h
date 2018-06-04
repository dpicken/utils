#ifndef intrusive_detail_ListTraits_h
#define intrusive_detail_ListTraits_h

#include <cstddef>

#define INTRUSIVE_LIST_TRAITS(T, listNodeMember) intrusive::detail::ListTraits<T, intrusive::ListNode, &T::listNodeMember, offsetof(T, listNodeMember)>

namespace intrusive { namespace detail {

template<typename T, typename ListNodeT, ListNodeT T::*listNodeMember, size_t listNodeMemberOffset>
struct ListTraits {
  typedef ListNodeT list_node_type;

  static ListNodeT* toListNode(T* object) {
    return &(object->*listNodeMember);
  }

  static T* toObject(ListNodeT* listNode) {
    return reinterpret_cast<T*>(reinterpret_cast<char*>(listNode) - listNodeMemberOffset);
  }

  static const T* toObject(const ListNodeT* listNode) {
    return reinterpret_cast<const T*>(reinterpret_cast<const char*>(listNode) - listNodeMemberOffset);
  }
};

} } // namespace intrusive::detail

#endif // ifndef intrusive_detail_ListTraits_h
