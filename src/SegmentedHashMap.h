#ifndef COURSE_WORK_SEGMENTEDHASHMAP_H
#define COURSE_WORK_SEGMENTEDHASHMAP_H

#include <vector>
#include <unordered_map>
#include <shared_mutex>
#include <set>
#include <string>

using read_write_lock = std::shared_mutex;
using read_lock = std::shared_lock<read_write_lock>;
using write_lock = std::unique_lock<read_write_lock>;

template <typename Key, typename Value>
struct Segment {
    mutable std::shared_mutex rw_lock;
    std::unordered_map<Key, Value> map;
};

template <typename Key, typename Value>
class SegmentedHashMap {
private:
    std::vector<Segment<Key, Value>> segments;
    size_t segment_count;

    Segment<Key, Value>& get_segment(const Key& key);

public:
    explicit SegmentedHashMap(size_t num_segments) : segment_count(num_segments), segments(num_segments) {}

    void insert_or_update(const Key& key, const Value& value);
    bool find(const Key& key, Value& out_value);
    void apply_to_all(std::function<void(const Key&, const Value&)> func) const;
    void insert_to_set(const Key& key, const typename Value::value_type& element);
};

#endif //COURSE_WORK_SEGMENTEDHASHMAP_H
