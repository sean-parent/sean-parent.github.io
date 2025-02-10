class notification_queue {
  deque<task<void()>> _q;
  bool _done{false};
  mutex _mutex;
  condition_variable _ready;

 public:
  void done() {
    {
      lock_t lock{_mutex};
      _done = true;
    }
    _ready.notify_all();
  }

  bool pop(task<void()>& x) {
    lock_t lock{_mutex};

    while (_q.empty() && !_done) _ready.wait(lock);

    if (_q.empty()) return false;

    x = move(_q.front());
    _q.pop_front();
    return true;
  }

  ...
};