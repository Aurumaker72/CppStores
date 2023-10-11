#ifndef CPPSTORES_CPPSTORES_H
#define CPPSTORES_CPPSTORES_H

#include <vector>
#include <functional>
#include <cstdint>

/**
 * \brief A writable store for reactive data storage and manipulation
 * \tparam T The type of the held type
 */
template<typename T>
class Writable {
    T value;
    std::vector<std::pair<int32_t, std::function<void(T)>>> subscribers;
    int32_t current_subscriber_index = 0;
    void on_value_changed() {
        for (auto pair : subscribers) {
            pair.second(value);
        }
    }
public:
    /**
     * \brief Creates a writable store with a default value
     * \param value The default value
     */
    explicit Writable(T value) {
        this->value = value;
    }

    /**
     * \brief Updates the store's value and notifies subscribers
     * \param callback A callback which receives the store's value and returns a mutated value
     */
    void update(std::function<T(T)> callback) {
        this->value = callback(this->value);
        on_value_changed();
    }

    /**
     * \brief Sets the store's value and notifies all subscribers
     * \param value The store's value
     */
    void set(T value) {
        this->value = value;
        on_value_changed();
    }

    /**
     * \return The store's internal value
     * \remark By using this, no reactivity is gained 
     */
    T immediate_value() {
        return this->value;
    }

    /**
     * \brief Subscribes to value changes
     * \param callback The callback to be invoked upon a value change
     * \return An action to be invoked by the caller upon unsubscribing
     */
    std::function<void()> subscribe(std::function<void(T)> callback) {
        this->subscribers.push_back(std::make_pair(this->current_subscriber_index, callback));
        ++this->current_subscriber_index;
        on_value_changed();
        return [&] {
            std::erase_if(this->subscribers, [&](std::pair<int32_t, std::function<void(T)>> pair) {
                return pair.first == this->current_subscriber_index - 1;
            });
        };
    }
};

#endif //CPPSTORES_CPPSTORES_H
