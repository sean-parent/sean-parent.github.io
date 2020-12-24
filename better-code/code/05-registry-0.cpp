class registry 
{
    mutex _mutex;
    unordered_map<string, string> _map;
  public:
    void set(string key, string value) {
        unique_lock lock{mutex};
        _map.emplace(move(key), move(value));
    }
        
    auto get(const string& key) -> string {
        unique_lock lock{mutex};
        return _map.at(key);
    }
};