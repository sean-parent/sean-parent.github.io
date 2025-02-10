class notification_queue {
  deque<task<void()>> _q;
  bool _done{false};
  mutex _mutex;
  condition_variable _ready;

 public:
  bool try_pop(task<void()>& x) {
    lock_t lock{_mutex, try_to_lock};
    if (!lock || _q.empty()) return false;
    x = move(_q.front());
    _q.pop_front();
    return true;
  }

  template <typename F>
  bool try_push(F&& f) {
    {
      lock_t lock{_mutex, try_to_lock};
      if (!lock) return false;
      _q.emplace_back(forward<F>(f));
    }
    _ready.notify_one();
    return true;
  }
};