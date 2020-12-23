template <typename T>
class bad_cow 
{
    struct object_t 
    {
        explicit object_t(const T& x) : _data(x) {}
        atomic<int> _count{1};
        T           _data; 
    };
    object_t* _object;

 public:
    explicit bad_cow(const T& x) : _object(new object_t(x)) { }
    ~bad_cow() { if (0 == --_object->_count) delete _object; }
    bad_cow(const bad_cow& x) : _object(x._object) { ++_object->_count; }
    
    bad_cow& operator=(const T& x) {
        if (_object->_count == 1) _object->_data = x;
        else {
            object_t* tmp = new object_t(x);
            --_object->_count;
            _object = tmp;
        }
        return *this;
    }
};
