class Field {
    int _property;
public:
    int property() const { return _property; }
};

unordered_map<Field> _fields;
mutex _fields_mutex;

{
    unordered_map<int, Field>::note_type obsolete_node;
    {
        unique_lock<mutex> guard{_fields_mutex};
        obsolete_node = _fields.extract(_fields.find(key_to_remove));
    }
}