using lock_t = unique_lock<mutex>;

class background_worker 
{
    thread                   _thread;
    queue<function<void()>>  _queue;
    mutex                    _mutex;
    condition_variable       _cv;
    bool                     _done;

public:
    background_worker() {
        auto worker = [this] {
            while (true) {
                function<void()> task;
                {
                    lock_t lock{ _mutex };
                    while (!_done && _queue.empty()) _cv.wait(lock);
                    if (_done) return;
                    if (!_queue.empty()) {
                        task = move(_queue.front());
                        _queue.pop();
                    }
                }
                if (task) task();
            }
        };
        _thread = thread(worker);
    }

    ~background_worker() {
        {
            lock_t lock{_mutex};
            _done = true;
        }
        _cv.notify_one();    
        _thread.join();
    }
    
    void submit_task(function<void()> task) {
        {
            lock_t lock{_mutex};
            _queue.emplace(move(task));
        }
        _cv.notify_one();
    }
};
