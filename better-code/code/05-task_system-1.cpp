class task_system {
  const unsigned _count{thread::hardware_concurrency()};
  vector<thread> _threads;
  notification_queue _q;

  void run(unsigned i) {
    while (true) {
      function<void()> f;
      _q.pop(f);
      f();
    }
  }

 public:
  task_system() {
    for (unsigned n = 0; n != _count; ++n) {
      _threads.emplace_back([&, n] { run(n); });
    }
  }

  ~task_system() {
    for (auto& e : _threads) e.join();
  }

  template <typename F>
  void async(F&& f) {
    _q.push(forward<F>(f));
  }
};