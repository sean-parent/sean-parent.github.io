class registry {
  serial_queue _q;

  using map_t = unordered_map<string, string>;

  shared_ptr<map_t> _map = make_shared<map_t>();

 public:
  void set(string key, string value) {
    _q.async(
        [_map = _map](string key, string value) {
          _map->emplace(move(key), move(value));
        },
        move(key), move(value));
  }

  void set(vector<pair<string, string>> sequence) {
    _q.async(
        [_map = _map](vector<pair<string, string>> sequence) {
          _map->insert(make_move_iterator(begin(sequence)),
                       make_move_iterator(end(sequence)));
        },
        move(sequence));
  }

  auto get(string key) -> future<string> {
    return _q.async([_map = _map](string key) { return _map->at(key); },
                    move(key));
  }
};