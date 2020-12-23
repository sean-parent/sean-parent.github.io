template <typename T>
class correct_cow 
{
    struct object_t 
    {
        explicit object_t(const T& x) : _data(x) {}
        atomic<int> _count{1};
        T           _data; 
    };
    object_t* _object;

 public:
    explicit correct_cow(const T& x) : _object(new object_t(x)) { }
    ~correct_cow() { if (0 == --_object->_count) delete _object; }
    correct_cow(const correct_cow& x) : _object(x._object) { ++_object->_count; }
    
    correct_cow& operator=(const T& x) {
        if (_object->_count == 1) _object->_data = x;
        else {
            object_t* tmp = new object_t(x);
            if (0 == --_object->_count) delete _object;
            _object = tmp;
        }
        return *this;
    }
};
