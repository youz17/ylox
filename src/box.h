
// a not null unique_ptr
template <typename T> class Box {
  public:
    Box(T *ptr) noexcept : m_ptr(ptr) { assert(m_ptr != nullptr); }
    Box(Box<T> &&b) noexcept {
        assert(b.m_ptr != nullptr);
        m_ptr = b.m_ptr;
        // 这不是 unique_ptr, 这个时候你不应该重新使用已经被 move 的 box
        b.m_ptr = nullptr;
    }

    ~Box() { delete m_ptr; }

    Box<T> &operator=(Box<T> &&b) noexcept {}

    T &operator*() {
        assert(m_ptr);
        return *m_ptr;
    }

    T *operator->() {
        assert(m_ptr);
        return m_ptr;
    }

    T *get() {
        assert(m_ptr);
        return m_ptr;
    }

  private:
    T *m_ptr;
};
