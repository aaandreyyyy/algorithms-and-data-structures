/*!
 * Декартово дерево поддерживает операции:
 * - merge(t1, t2) -- сливает два дерева в одно
 * - split(t, x) -- разбивает дерево на два по ключу
 * - size(v) -- возвращает размер поддерева данной вершины
 * - less(x) -- возвращает количество ключей, меньших данного значения
 * - get(k) -- возвращает к-ую порядковую статистику
 * Все операции работают за O(log n)
 * */

#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>


struct Node {
    int key, prior;
    int sz;
    Node *left = nullptr, *right = nullptr;

    explicit Node(int _key) : key(_key), sz(1) {
        prior = rand();
    }

    Node(int _key, int _prior) : key(_key), prior(_prior), sz(1) {
    }
};

int size(Node *v) {
    return v ? v->sz : 0;
}

void update(Node *v) {
    if (v) {
        v->sz = size(v->left) + size(v->right) + 1;
    }
}

std::pair<Node *, Node *> split(Node *v, int x) {
    if (!v) {
        return {nullptr, nullptr};
    }
    if (size(v->left) >= x) {
        std::pair<Node *, Node *> p = split(v->left, x);
        v->left = p.second;
        update(v);
        return {p.first, v};
    }
    std::pair<Node *, Node *> p = split(v->right, x - size(v->left) - 1);
    v->right = p.first;
    update(v);
    return {v, p.second};
}

Node *merge(Node *a, Node *b) {
    if (!a) {
        return b;
    }
    if (!b) {
        return a;
    }
    if (a->prior > b->prior) {
        a->right = merge(a->right, b);
        update(a);
        return a;
    }
    b->left = merge(a, b->left);
    update(b);
    return b;
}

int less(Node *v, int x) {
    if (!v) {
        return 0;
    }
    if (v->key < x) {
        return size(v->left) + 1 + less(v->right, x);
    }
    return less(v->left, x);
}

void insert(Node *&treap, int x) {
    std::pair<Node *, Node *> p = split(treap, less(treap, x));
    treap = merge(merge(p.first, new Node(x)), p.second);
}

void erase(Node *&treap, int x) {
    if (!treap) {
        return;
    }
    if (treap->key == x) {
        treap = merge(treap->left, treap->right);
    } else if (treap->key < x) {
        erase(treap->right, x);
    } else {
        erase(treap->left, x);
    }
    update(treap);
}

bool contains(Node *treap, int x) {
    if (!treap) {
        return false;
    }
    if (treap->key == x) {
        return true;
    } else if (treap->key < x) {
        return contains(treap->right, x);
    }
    return contains(treap->left, x);
}

Node* get(Node* v, int k) {
    if (!v) {
        return v;
    }
    int sz = size(v->left);
    if (sz == k) {
        return v;
    } else if (sz < k) {
        return get(v->right, k - sz - 1);
    }
    return get(v->left, k);
}


int main() {
    Node *treap = nullptr;
    int n;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::string s;
        std::cin >> s;
        if (s == "INSERT") {
            int key;
            std::cin >> key;
            insert(treap, key);
        } else if (s == "ERASE") {
            int key;
            std::cin >> key;
            if (contains(treap, key)) {
                erase(treap, key);
            }
        } else if (s == "GET") {
            int key;
            std::cin >> key;
            Node* x = get(treap, key);
            if (x) {
                std::cout << x->key << '\n';
            } else {
                std::cout << "-1\n";
            }
        }
    }
    return 0;
}
