
// a not null unique_ptr
template <typename T> class Box {
  public:
    Box(T *ptr) noexcept : m_ptr(ptr) { assert(m_ptr != nullptr); }
    Box(Box<T> &&b) noexcept {
        assert(b.m_ptr != nullptr);
        m_ptr = b.m_ptr;
        // �ⲻ�� unique_ptr, ���ʱ���㲻Ӧ������ʹ���Ѿ��� move �� box
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
