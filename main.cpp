#include <iostream>
#include <vector>
#include <random>


class IDisjointSets{
protected:
    std::vector<ssize_t> parent;
    size_t num_elements;
    size_t num_sets;
public:


};

class DisjointSets {
protected:
    std::vector<ssize_t> parent;
    size_t num_elements;
    size_t num_sets;
public:
    explicit DisjointSets(size_t size): parent(size, -1), num_elements(0), num_sets(0){}

    virtual void makeSet(ssize_t element) {
        parent[element] = element;
        ++num_elements;
        ++num_sets;
    }

    virtual ssize_t findSet(ssize_t element) {
        if (element == parent[element]) {
            return element;
        }
        return findSet(parent[element]);
    }

    virtual void unionSets(ssize_t firstElement, ssize_t secondElement) {
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
    }

    ssize_t findSet(ssize_t element) override {
        if (element == parent[element]) {
            return element;
        }
        parent[element] = findSet(parent[element]);
        return parent[element];
    }

};

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
