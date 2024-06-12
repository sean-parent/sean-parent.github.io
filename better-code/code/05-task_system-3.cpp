class task_system {
  const unsigned _count{thread::hardware_concurrency()};
  vector<thread> _threads;
  vector<notification_queue> _q{_count};
  atomic<unsigned> _index{0};

  void run(unsigned i) {
    while (true) {
      task<void()> f;
      if (!_q[i].pop(f)) break;
      f();
    }
  }

 public:
  ~task_system() {
    for (auto& e : _q) e.done();
    for (auto& e : _threads) e.join();
  }

  template <typename F>
  void async(F&& f) {
    auto i = _index++;
    _q[i % _count].push(forward<F>(f));
  }
};