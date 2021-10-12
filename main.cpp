#include <iostream>
#include <vector>
#include <random>
#include <unordered_map>


class IDisjointSets{
protected:
    std::vector<ssize_t> parent;
    size_t num_elements{};
    size_t num_sets{};
public:

    [[nodiscard]] virtual size_t getNumElements() const {
        return num_elements;
    }

    [[nodiscard]] virtual size_t getNumSets() const {
        return num_sets;
    }

    virtual void makeSet(ssize_t element) = 0;
    virtual ssize_t findSet(ssize_t element) = 0;
    virtual void unionSets(ssize_t firstElement, ssize_t secondElement) = 0;
};

class DisjointSets: IDisjointSets {
protected:
    std::vector<ssize_t> parent;
    size_t num_elements;
    size_t num_sets;
public:
    explicit DisjointSets(size_t size): parent(size, -1), num_elements(0), num_sets(0){}

    void makeSet(ssize_t element) override {
        parent[element] = element;
        ++num_elements;
        ++num_sets;
    }

    ssize_t findSet(ssize_t element) override {
        if (element == parent[element]) {
            return element;
        }
        return findSet(parent[element]);
    }

    void unionSets(ssize_t firstElement, ssize_t secondElement) override {
        ssize_t firstRoot = findSet(firstElement);
        ssize_t secondRoot = findSet(secondElement);
        if (firstRoot != secondRoot) {
            parent[secondRoot] = firstRoot;
        }
        --num_sets;
    }
};


class DisjointSetsWithPCAndRank: DisjointSets {
    std::vector<size_t> set_sizes;
public:
    explicit DisjointSetsWithPCAndRank(size_t size): DisjointSets(size), set_sizes(size, 0) {}

    void makeSet(ssize_t element) override {
        parent[element] = element;
        set_sizes[element] = 1;
        ++num_elements;
        ++num_sets;
    }

    ssize_t findSet(ssize_t element) override {
        if (element == parent[element]) {
            return element;
        }
        parent[element] = findSet(parent[element]);
        return parent[element];
    }

    void unionSets(ssize_t firstElement, ssize_t secondElement) override {
        ssize_t firstRoot = findSet(firstElement);
        ssize_t secondRoot = findSet(secondElement);
        if (firstRoot != secondRoot) {
            if (set_sizes[firstRoot] < set_sizes[secondRoot]) {
                std::swap(parent[firstRoot], parent[secondRoot]);
            }
            parent[secondRoot] = firstRoot;
            set_sizes[firstRoot] += set_sizes[secondRoot];
        }
        --num_sets;
    }
};


class DisjointSetsWithRandomizedLinking: DisjointSets {
    std::vector<ssize_t> randomized_table;
    std::random_device random_dev;
public:
    explicit DisjointSetsWithRandomizedLinking(size_t size, const std::string& random_device_name):
    DisjointSets(size), randomized_table(size,-1), random_dev(random_device_name) {}

    void makeSet(ssize_t element) override {
        parent[element] = element;
        std::mt19937 gen(random_dev());
        std::uniform_int_distribution<> distrib(int(randomized_table.size()));
        randomized_table[element] = distrib(gen);
        ++num_sets;
        ++num_elements;
    }

    void unionSets(ssize_t firstElement, ssize_t secondElement) override {
        ssize_t firstRoot = findSet(firstElement);
        ssize_t secondRoot = findSet(secondElement);
        if (firstRoot != secondRoot) {
            if (randomized_table[firstRoot] < randomized_table[secondRoot]) {
                std::swap(parent[firstRoot], parent[secondRoot]);
            }
            parent[secondRoot] = firstRoot;
        }
        --num_sets;
    }

    ssize_t findSet(ssize_t element) override {
        if (element == parent[element]) {
            return element;
        }
        parent[element] = findSet(parent[element]);
        return parent[element];
    }

};

template <class T, class T_DS>
class DisjointSetsWithElements: DisjointSets {
    T_DS disjoint_set;
    std::unordered_map<size_t, T&> idx2element;
    std::unordered_map<T, size_t> element2idx;
public:
    DisjointSetsWithElements(size_t size, T_DS disjoint_set_):
    DisjointSets(size), disjoint_set(disjoint_set_) {}

    void makeSet(const T& element) override {
        size_t element_idx = disjoint_set.getNumElements();
        idx2element[element_idx] = element;
        element2idx[*element] = element_idx;
        disjoint_set.makeSet(element_idx);
    }

    T& findSet(const T& element) override {
        size_t element_idx = element2idx[*element];
        size_t parent_idx = disjoint_set.findSet(element_idx);
        return idx2element[parent_idx];
    }

    void unionSets(const T& firstElement, const T& secondElement) override {
        size_t first_idx = element2idx[*firstElement];
        size_t second_idx = element2idx[*secondElement];
        disjoint_set.unionSets(first_idx, second_idx);
    }
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
