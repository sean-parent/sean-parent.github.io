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

  auto get(string key) -> future<string> {
    return _q.async([_map = _map](string key) { return _map->at(key); },
                    move(key));
  }
};