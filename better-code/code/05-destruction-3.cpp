class Field {
    int _property;
public:
    int property() const { return _property; }
};

vector<Field> _fields;
mutex _fields_mutex;

vector<Field> obsolete_fields;
{
    unique_lock<mutex> guard{_fields_mutex};
    auto it = remove_if(_fields.begin(), _fields.end(), 
        [items_to_remove](auto const& field) { return field.property() == items_to_remove; } );
        
    obsolete_fields.resize(distance(it, _fields.end()));
    std::move(it, _fields.end(), obsolete_fields.begin());
}
obsolete_fields.resize(0);