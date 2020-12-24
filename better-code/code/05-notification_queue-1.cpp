using lock_t = unique_lock<mutex>;

class notification_queue 
{
    deque<function<void()>> _q;
    mutex                   _mutex;
    condition_variable      _ready;

public:
    void pop(function<void()>& x) {
        lock_t lock{_mutex};

        while (_q.empty()) _ready.wait(lock);
        
        x = move(_q.front());
        _q.pop_front();
    }

    template<typename F>
    void push(F&& f) {
        {
            lock_t lock{_mutex};
            _q.emplace_back(forward<F>(f));
        }
        _ready.notify_one();
    }
};