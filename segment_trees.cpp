#include <iostream>
#include <vector>

template<typename T>
auto test_addition_operator(T a, T b) -> decltype(a + b);


template<typename T>
struct SumSegmentTree {
    std::vector<T> tree;
    int size;

    explicit SumSegmentTree(int n) {
        if constexpr (!std::is_same_v<decltype(test_addition_operator(T{}, T{})), T>) {
            std::cerr << "type which segment tree stores should have '+' operator\n";
            throw std::exception();
        }

        size = 1;
        while (size < n) {
            size *= 2;
        }
        tree.assign(2 * size - 1, 0);
    }


    void set(int i, T v, int x, int lx, int rx) {
        if (rx - lx == 1) {
            tree[x] = v;
            return;
        }
        
        int m = (lx + rx) / 2;
        if (i < m) {
            set(i, v, 2 * x + 1, lx, m);
        } else {
            set(i, v, 2 * x + 2, m, rx);
        }
        tree[x] = tree[2 * x + 1] + tree[2 * x + 2];
    }

    void set(int i, T v) {
        return set(i, v, 0, 0, size);
    }

    T sum(int l, int r, int x, int lx, int rx) {
        if (l >= rx || lx >= r) {
            return 0;
        }
        if (lx >= l && rx <= r) {
            return tree[x];
        }

        int m = (lx + rx) / 2;
        T s1 = sum(l, r, 2 * x + 1, lx, m);
        T s2 = sum(l, r, 2 * x + 2, m, rx);
        return s1 + s2;
    }

    T sum(int l, int r) {
        return sum(l, r, 0, 0, size);
    }
};


struct MinSegmentTree {
    const int INF = 1e9;
    std::vector<int> tree;
    int size;

    MinSegmentTree(int n) {
        size = 1;
        while (size < n) {
            size *= 2;
        }
        tree.assign(2 * size - 1, INF);
    }


    void set(int i, int v, int x, int lx, int rx) {
        if (rx - lx == 1) {
            tree[x] = v;
            return;
        }
        
        int m = (lx + rx) / 2;
        if (i < m) {
            set(i, v, 2 * x + 1, lx, m);
        } else {
            set(i, v, 2 * x + 2, m, rx);
        }
        tree[x] = std::min(tree[2 * x + 1], tree[2 * x + 2]);
    }

    void set(int i, int v) {
        return set(i, v, 0, 0, size);
    }

    int min(int l, int r, int x, int lx, int rx) {
        if (l >= rx || lx >= r) {
            return INF;
        }
        if (lx >= l && rx <= r) {
            return tree[x];
        }

        int m = (lx + rx) / 2;
        int s1 = min(l, r, 2 * x + 1, lx, m);
        int s2 = min(l, r, 2 * x + 2, m, rx);
        return std::min(s1, s2);
    }

    int min(int l, int r) {
        return min(l, r, 0, 0, size);
    }
};
