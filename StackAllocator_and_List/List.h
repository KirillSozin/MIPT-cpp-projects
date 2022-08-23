#include <iterator>
#include <memory>

template<class T, class Alloc = std::allocator<T>>
class List {

 private:
  struct Node {
    Node *prev;
    Node *next;
    T data;

    explicit Node() : prev(nullptr), next(nullptr) {}
    explicit Node(Node *prevOther, Node *nextOther) : prev(prevOther), next(nextOther) {}
    Node(const T &dataOther, Node *prevOther, Node *nextOther) : prev(prevOther), next(nextOther), data(dataOther) {}
  };

  template<bool IsConst>
  struct BaseIterator : public std::iterator<std::bidirectional_iterator_tag, std::conditional_t<IsConst, const T, T>> {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using ptr = typename std::conditional<IsConst, const T *, T *>::type;
    using ref = typename std::conditional<IsConst, const T &, T &>::type;
    using iterator_type = BaseIterator<IsConst>;

    BaseIterator() : node() {}
    BaseIterator(const BaseIterator<false> &other) : node(other.GetNode()) {}
    explicit BaseIterator(Node *node) : node(node) {}

    BaseIterator<IsConst> &operator++() {
      node = node->next;
      return *this;
    }

    BaseIterator<IsConst> operator++(int) {
      BaseIterator<IsConst> copy = *this;
      node = node->next;
      return copy;
    }

    BaseIterator<IsConst> &operator--() {
      node = node->prev;
      return *this;
    }

    BaseIterator<IsConst> operator--(int) {
      BaseIterator<IsConst> copy = *this;
      node = node->prev;
      return copy;
    }

    BaseIterator<IsConst> &operator=(const BaseIterator &other) {
      node = other.node;
      return *this;
    }

    ref operator*() const { return node->data; }

    ptr operator->() const { return &(node->data); }

    bool operator==(const BaseIterator<IsConst> &other) const { return node == other.node; }

    bool operator!=(const BaseIterator<IsConst> &other) const { return node != other.node; }

    Node *GetNode() const { return node; }

   private:
    Node *node;
  };

 public:
  using value_type = T;
  using iterator = BaseIterator<false>;
  using const_iterator = BaseIterator<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using AllocTraits = typename std::allocator_traits<typename std::allocator_traits<Alloc>::template rebind_alloc<Node>>;
  using UselessAllocTraits = typename std::allocator_traits<Alloc>;
  typename Alloc::template rebind<Node>::other alloc;

  explicit List(const Alloc &alloc = Alloc());
  List(size_t n, const Alloc &alloc = Alloc());
  List(size_t n, const T &, const Alloc &alloc = Alloc());

  List(const List &other);

  ~List();

  Alloc get_allocator() const { return alloc; }

  List &operator=(const List &other);

  size_t size() const { return ListSize; }

  void push_back(const T &);
  void push_front(const T &);

  void pop_back();
  void pop_front();

  iterator begin() { return iterator(head->next); }
  iterator end() { return iterator(head); }

  const_iterator begin() const { return const_iterator(head->next); }
  const_iterator end() const { return const_iterator(head); }

  const_iterator cbegin() const { return const_iterator(head->next); }
  const_iterator cend() const { return const_iterator(head); }

  reverse_iterator rbegin() { return reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }

  const_reverse_iterator rbegin() const { return const_reverse_iterator(cend()); }
  const_reverse_iterator rend() const { return const_reverse_iterator(cbegin()); }

  const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); }
  const_reverse_iterator crend() const { return const_reverse_iterator(cbegin()); }

  void insert(const_iterator, const T &);
  void erase(const_iterator);

 private:
  Node *head;
  size_t ListSize;

};
//----------------------------------------------------------------------------------------------------------------------

template<class T, class Alloc>
List<T, Alloc>::List(size_t n, const Alloc &alloc) : List(alloc) {
  Node *cur = head;
  for (size_t i = 0; i < n; ++i) {
    Node *node = AllocTraits::allocate(this->alloc, 1);
    AllocTraits::construct(this->alloc, node, node, node);
    cur->next = node;
    node->prev = cur;
    cur = node;
    ++ListSize;
  }
  cur->next = head;
  head->prev = cur;
}

template<class T, class Alloc>
List<T, Alloc>::List(size_t n, const T &value, const Alloc &alloc) : List(alloc) {
  Node *cur = head;
  for (size_t i = 0; i < n; ++i) {
    Node *node = AllocTraits::allocate(this->alloc, 1);
    AllocTraits::construct(this->alloc, node, value, node, node);
    cur->next = node;
    node->prev = cur;
    cur = node;
    ++ListSize;
  }
  cur->next = head;
  head->prev = cur;
}

template<class T, class Alloc>
void List<T, Alloc>::insert(List::const_iterator pos, const T &value) {
  Node *node = AllocTraits::allocate(this->alloc, 1);
  AllocTraits::construct(this->alloc, node, value, node, node);

  Node *ptr = pos.GetNode();

  node->prev = ptr->prev;
  node->next = ptr;

  ptr->prev->next = node;
  ptr->prev = node;

  ++ListSize;
}

template<class T, class Alloc>
void List<T, Alloc>::erase(List::const_iterator pos) {
  Node *ptr = pos.GetNode();
  ptr->prev->next = ptr->next;
  ptr->next->prev = ptr->prev;
  pos->~T();
  AllocTraits::deallocate(this->alloc, ptr, 1);
  --ListSize;
}

template<class T, class Alloc>
List<T, Alloc>::~List() {
  while (ListSize) erase(cbegin());

  AllocTraits::deallocate(this->alloc, head, 1);
}

template<class T, class Alloc>
List<T, Alloc>::List(const List &other) : List(UselessAllocTraits::select_on_container_copy_construction(other.alloc)) {
  for (auto it = other.begin(); it != other.end(); ++it) {
    insert(cend(), *it);
  }
}

template<class T, class Alloc>
void List<T, Alloc>::push_back(const T &value) {
  insert(cend(), value);
}

template<class T, class Alloc>
void List<T, Alloc>::push_front(const T &value) {
  insert(cbegin(), value);
}

template<class T, class Alloc>
void List<T, Alloc>::pop_back() {
  erase(--cend());
}

template<class T, class Alloc>
void List<T, Alloc>::pop_front() {
  erase(cbegin());
}

template<class T, class Alloc>
List<T, Alloc> &List<T, Alloc>::operator=(const List &other) {
while (ListSize) erase(cbegin());

  if (AllocTraits::propagate_on_container_copy_assignment::value) {
    this->alloc = other.alloc;
  }

  try {
    for (auto it = other.begin(); it != other.end(); ++it) {
      insert(cbegin(), *it);
    }
  } catch(...) {}

  return *this;
}

template<class T, class Alloc>
List<T, Alloc>::List(const Alloc &alloc): alloc(alloc), ListSize(0) {
  head = AllocTraits::allocate(this->alloc, 1);
  head->prev = head;
  head->next = head;
}
//----------------------------------------------------------------------------------------------------------------------
