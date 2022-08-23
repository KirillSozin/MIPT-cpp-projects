template<size_t N>
class StackStorage {
 private:
  char *arr;
  size_t beg;

 public:
  StackStorage();
  ~StackStorage() { free(arr); }

  StackStorage(const StackStorage &other) = delete;

  void *allocate(size_t n, size_t align);
  void deallocate(const void *, size_t) {}
};
//----------------------------------------------------------------------------------------------------------------------

template<size_t N>
void *StackStorage<N>::allocate(size_t n, size_t align) {
  if (beg % align) beg += align - beg % align;
  beg += n;

  return arr + (beg - n);
}

template<size_t N>
StackStorage<N>::StackStorage() : beg() {
  arr = (char *) malloc(N * sizeof(char));
}

//----------------------------------------------------------------------------------------------------------------------
template<typename T, size_t N>
class StackAllocator {
 private:
  StackStorage<N> *stack_storage_;

 public:
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef T *pointer;
  typedef const T *const_pointer;
  typedef T &reference;
  typedef const T &const_reference;
  typedef T value_type;

  template<class A>
  struct rebind {
    typedef A value_type;
    typedef StackAllocator<A, N> other;
  };

  StackAllocator() : stack_storage_() {}

  StackAllocator(StackStorage<N> &arr) : stack_storage_(&arr) {}

  StackStorage<N> *address() const { return stack_storage_; }

  template<typename A>
  StackAllocator(const StackAllocator<A, N> &other) : stack_storage_(other.address()) {}

  template<typename A>
  StackAllocator<T, N> &operator=(const StackAllocator<A, N> &other);

  T *allocate(size_t n);
  void deallocate(const T *, size_t) {}
};


//----------------------------------------------------------------------------------------------------------------------


template<typename T, typename A, size_t N>
bool operator==(const StackAllocator<T, N> &first, const StackAllocator<A, N> &second) {
  return first.address() == second.address();
}

template<typename T, typename A, size_t N>
bool operator!=(const StackAllocator<T, N> &first, const StackAllocator<A, N> &second) {
  return first.address() != second.address();
}

template<typename T, size_t N>
template<typename A>
StackAllocator<T, N> &StackAllocator<T, N>::operator=(const StackAllocator<A, N> &other) {
  stack_storage_ = other.address();
  return *this;
}

template<typename T, size_t N>
T *StackAllocator<T, N>::allocate(size_t n) {
  size_t sz = sizeof(T);
  return (T *) stack_storage_->allocate(n * sz, sz);
}

