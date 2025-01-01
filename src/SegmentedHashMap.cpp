#include "SegmentedHashMap.h"

template <typename Key, typename Value>
Segment<Key, Value>& SegmentedHashMap<Key, Value>::get_segment(const Key& key) {
    size_t hash = std::hash<Key>{}(key);
    return segments[hash % segment_count];
}

template <typename Key, typename Value>
void SegmentedHashMap<Key, Value>::insert_or_update(const Key& key, const Value& value) {
    auto& segment = get_segment(key);
    std::unique_lock lock(segment.rw_lock);
    segment.map[key] = value;
}

template <typename Key, typename Value>
bool SegmentedHashMap<Key, Value>::find(const Key& key, Value& out_value) {
    const auto& segment = get_segment(key);
    std::shared_lock lock(segment.rw_lock);
    auto it = segment.map.find(key);
    if (it != segment.map.end()) {
        out_value = it->second;
        return true;
    }
    return false;
}

template <typename Key, typename Value>
void SegmentedHashMap<Key, Value>::apply_to_all(std::function<void(const Key&, const Value&)> func) const {
    for (const auto& segment : segments) {
        std::shared_lock lock(segment.rw_lock);
        for (const auto& [key, value] : segment.map) {
            func(key, value);
        }
    }
}

template <typename Key, typename Value>
void SegmentedHashMap<Key, Value>::insert_to_set(const Key& key, const typename Value::value_type& element) {
    auto& segment = get_segment(key);
    std::unique_lock lock(segment.rw_lock);
    segment.map[key].insert(element);
}

template class SegmentedHashMap<std::string, std::set<int>>;
