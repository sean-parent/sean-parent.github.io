class task_system {
  const unsigned _count{thread::hardware_concurrency()};
  const unsigned _spin{_count < 64 ? 64 : _count};

  vector<thread> _threads;
  vector<notification_queue> _q{_count};
  atomic<unsigned> _index{0};

  void run(unsigned i) {
    while (true) {
      task<void()> f;

      // TODO Take _spin / _count or something different?
      for (unsigned n = 0; n != _spin / _count; ++n) {
        if (_q[(i + n) % _count].try_pop(f)) break;
      }
      if (!f && !_q[i].pop(f)) break;

      f();
    }
  }

 public:
  template <typename F>
  void async(F&& f) {
    auto i = _index++;

    for (unsigned n = 0; n != _spin / _count; ++n) {
      if (_q[(i + n) % _count].try_push(forward<F>(f))) return;
    }

    _q[i % _count].push(forward<F>(f));
  }
};