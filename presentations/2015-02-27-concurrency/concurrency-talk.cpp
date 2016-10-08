/*
    Copyright 2015 Adobe Systems Incorporated
    Distributed under the MIT License (see license at http://stlab.adobe.com/licenses.html)
    
    This file is intended as example code and is not production quality.
*/

/**************************************************************************************************/

#include <deque>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include <boost/multiprecision/cpp_int.hpp>

/**************************************************************************************************/

using namespace std;
using namespace boost::multiprecision;

/**************************************************************************************************/

template <typename T, typename N, typename O>
T power(T x, N n, O op)
{
    if (n == 0) return identity_element(op);
    
    while ((n & 1) == 0) {
        n >>= 1;
        x = op(x, x);
    }
    
    T result = x;
    n >>= 1;
    while (n != 0) {
        x = op(x, x);
        if ((n & 1) != 0) result = op(result, x);
        n >>= 1;
    }
    return result;
}

/**************************************************************************************************/

template <typename N>
struct multiply_2x2 {
    array<N, 4> operator()(const array<N, 4>& x, const array<N, 4>& y)
    {
        return { x[0] * y[0] + x[1] * y[2], x[0] * y[1] + x[1] * y[3],
            x[2] * y[0] + x[3] * y[2], x[2] * y[1] + x[3] * y[3] };
    }
};

template <typename N>
array<N, 4> identity_element(const multiply_2x2<N>&) { return { N(1), N(0), N(0), N(1) }; }

template <typename R, typename N>
R fibonacci(N n) {
    if (n == 0) return R(0);
    return power(array<R, 4>{ 1, 1, 1, 0 }, N(n - 1), multiply_2x2<R>())[0];
}

/**************************************************************************************************/

using lock_t = unique_lock<mutex>;

class notification_queue {
    deque<function<void()>> _q;
    bool                    _done{false};
    mutex                   _mutex;
    condition_variable      _ready;
    
public:
    bool try_pop(function<void()>& x) {
        lock_t lock{_mutex, try_to_lock};
        if (!lock || _q.empty()) return false;
        x = move(_q.front());
        _q.pop_front();
        return true;
    }
    
    template<typename F>
    bool try_push(F&& f) {
        {
            lock_t lock{_mutex, try_to_lock};
            if (!lock) return false;
            _q.emplace_back(forward<F>(f));
        }
        _ready.notify_one();
        return true;
    }
    
    void done() {
        {
            unique_lock<mutex> lock{_mutex};
            _done = true;
        }
        _ready.notify_all();
    }
    
    bool pop(function<void()>& x) {
        lock_t lock{_mutex};
        while (_q.empty() && !_done) _ready.wait(lock);
        if (_q.empty()) return false;
        x = move(_q.front());
        _q.pop_front();
        return true;
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

/**************************************************************************************************/

class task_system {
    const unsigned              _count{thread::hardware_concurrency()};
    vector<thread>              _threads;
    vector<notification_queue>  _q{_count};
    atomic<unsigned>            _index{0};
    
    void run(unsigned i) {
        while (true) {
            function<void()> f;
            
            for (unsigned n = 0; n != _count * 32; ++n) {
                if (_q[(i + n) % _count].try_pop(f)) break;
            }
            if (!f && !_q[i].pop(f)) break;
            
            f();
        }
    }
    
public:
    task_system() {
        for (unsigned n = 0; n != _count; ++n) {
            _threads.emplace_back([&, n]{ run(n); });
        }
    }
    
    ~task_system() {
        for (auto& e : _q) e.done();
        for (auto& e : _threads) e.join();
    }
    
    template <typename F>
    void async_(F&& f) {
        auto i = _index++;
        
        for (unsigned n = 0; n != _count; ++n) {
            if (_q[(i + n) % _count].try_push(forward<F>(f))) return;
        }
        
        _q[i % _count].push(forward<F>(f));
    }
};

/**************************************************************************************************/

task_system _system;

/**************************************************************************************************/

template <typename>
struct result_of_;

template <typename R, typename... Args>
struct result_of_<R(Args...)> { using type = R; };

template <typename F>
using result_of_t_ = typename result_of_<F>::type;

/**************************************************************************************************/

template <typename R>
struct shared_base {
    vector<R> _r; // optional
    mutex _mutex;
    condition_variable _ready;
    vector<function<void()>> _then;
    
    virtual ~shared_base() { }
    
    void set(R&& r) {
        vector<function<void()>> then;
        {
            lock_t lock{_mutex};
            _r.push_back(move(r));
            swap(_then, then);
        }
        _ready.notify_all();
        for (const auto& f : then) _system.async_(move(f));
    }
    
    template <typename F>
    void then(F&& f) {
        bool resolved{false};
        {
            lock_t lock{_mutex};
            if (_r.empty()) _then.push_back(forward<F>(f));
            else resolved = true;
        }
        if (resolved) _system.async_(move(f));
    }
    
    const R& get() {
        lock_t lock{_mutex};
        while (_r.empty()) _ready.wait(lock);
        return _r.back();
    }
};

template <typename> struct shared; // not defined

template <typename R, typename... Args>
struct shared<R(Args...)> : shared_base<R> {
    function<R(Args...)> _f;
    
    template<typename F>
    shared(F&& f) : _f(forward<F>(f)) { }
    
    template <typename... A>
    void operator()(A&&... args) {
        this->set(_f(forward<A>(args)...));
        _f = nullptr;
    }
};

template <typename> class packaged_task; //not defined
template <typename> class future;

template <typename S, typename F>
auto package(F&& f) -> pair<packaged_task<S>, future<result_of_t_<S>>>;

template <typename R>
class future {
    shared_ptr<shared_base<R>> _p;
    
    template <typename S, typename F>
    friend auto package(F&& f) -> pair<packaged_task<S>, future<result_of_t_<S>>>;
    
    explicit future(shared_ptr<shared_base<R>> p) : _p(move(p)) { }
 public:
    future() = default;
    
    template <typename F>
    auto then(F&& f) {
        auto pack = package<result_of_t<F(R)>()>([p = _p, f = forward<F>(f)](){
            return f(p->_r.back());
        });
        _p->then(move(pack.first));
        return pack.second;
    }
    
    const R& get() const { return _p->get(); }
};

template<typename R, typename ...Args >
class packaged_task<R (Args...)> {
    weak_ptr<shared<R(Args...)>> _p;
    
    template <typename S, typename F>
    friend auto package(F&& f) -> pair<packaged_task<S>, future<result_of_t_<S>>>;
    
    explicit packaged_task(weak_ptr<shared<R(Args...)>> p) : _p(move(p)) { }
    
 public:
    packaged_task() = default;
    
    template <typename... A>
    void operator()(A&&... args) const {
        auto p = _p.lock();
        if (p) (*p)(forward<A>(args)...);
    }
};

template <typename S, typename F>
auto package(F&& f) -> pair<packaged_task<S>, future<result_of_t_<S>>> {
    auto p = make_shared<shared<S>>(forward<F>(f));
    return make_pair(packaged_task<S>(p), future<result_of_t_<S>>(p));
}

/**************************************************************************************************/

template <typename F, typename ...Args>
auto async(F&& f, Args&&... args)
{
    using result_type = result_of_t<F (Args...)>;
    using packaged_type = packaged_task<result_type()>;
    
    auto pack = package<result_type()>(bind(forward<F>(f), forward<Args>(args)...));
    
    _system.async_(move(get<0>(pack)));
    return get<1>(pack);
}

/**************************************************************************************************/

int main() {
    future<cpp_int> x = async([]{ return fibonacci<cpp_int>(100); });
    
    future<cpp_int> y = x.then([](const cpp_int& x){ return cpp_int(x * 2); });
    future<cpp_int> z = x.then([](const cpp_int& x){ return cpp_int(x / 15); });
                                                            
    cout << y.get() << endl;
    cout << z.get() << endl;
}
