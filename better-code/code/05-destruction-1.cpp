class Field {
    int _property;
public:
    int property() const { return _property; }
};

list<Field> _fields;
mutex _fields_mutex;

list<Field> obsolete_field;
{
    unique_lock<mutex> guard{_fields_mutex};
    auto it = find_if(_fields.begin(), _fields.end(), 
        [item_to_remove](auto const& field) { return field.property() == item_to_remove; } );

    if (it != _fields.end()) {
        obsolete_field.splice(obsolete_field.end(), _fields, it);
    }
}

obsolete_field.clear();