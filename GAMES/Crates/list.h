// minimal class to replace std::vector
template<typename T, uint8_t Capacity>
struct List {
    uint8_t d_size = 0; // stores the number of actually stored objects
    T d_data[Capacity]; // stores the actual data

    // adds a new value and allocates more space if needed (and allowed)
    void add(T const &x) {
        if (d_size < Capacity) d_data[d_size++] = x;
    }

    // wipe the array
    void clear() {
        d_size = 0;
    }

    // size getter
    uint8_t size() const {
        return d_size;
    };

    uint8_t capacity() const {
        return Capacity;
    };

    bool full() const {
        return d_size == Capacity;
    };

    // delete an index
    void erase(uint8_t const &index) {
        for (uint8_t i = index; i < d_size; i++) {
            d_data[i] = d_data[i + 1]; // copy next element left
        }

        d_size--;
    }

    // getter
    T &operator[](uint8_t const &index) {
        return d_data[index];
    }
};
