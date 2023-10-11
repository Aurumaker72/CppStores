#ifndef CPPSTORES_CPPSTORES_H
#define CPPSTORES_CPPSTORES_H

#include <vector>
#include <functional>
#include <cstdint>

template<typename T>
class Writable {
private:
    T value;
    std::vector<std::pair<int32_t, std::function<void(T)>>> subscribers;
    int32_t current_subscriber_index;
    void on_value_changed() {
        for (auto pair : subscribers) {
            pair.second(value);
        }
    }
public:
    Writable(T value) {
        this->value = value;
    }

    void update(std::function<T(T)> callback) {
        this->value = callback(this->value);
        on_value_changed();
    }

    void set(T value) {
        this->value = value;
        on_value_changed();
    }

    T immediate_value() {
        return this->value;
    }

    std::function<void()> subscribe(std::function<void(T)> callback) {
        this->subscribers.push_back(std::make_pair(this->current_subscriber_index, callback));
        this->current_subscriber_index++;
        on_value_changed();
        return [&] {
            std::erase_if(this->subscribers, [&](std::pair<int32_t, std::function<void(T)>> pair) {
                return pair.first == this->current_subscriber_index - 1;
            });
        };
    }
};

#endif //CPPSTORES_CPPSTORES_H
