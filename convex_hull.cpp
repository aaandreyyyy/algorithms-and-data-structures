#include <iostream>
#include <vector>

// тут используется алгоритм Грэхэма

struct Point {
    double x = 0;
    double y = 0;

    Point(double x_, double y_) : x(x_), y(y_) {
    }

    Point() = default;
};

bool compare(Point a, Point b) {
    return a.x < b.x || a.x == b.x && a.y < b.y;
}

bool right_turn(Point a, Point b, Point c) {
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y) < 0;
}

bool left_turn(Point a, Point b, Point c) {
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y) > 0;
}

void convex_hull(std::vector<Point> &a) {
    if (a.size() == 1) {
        return;
    }
    sort(a.begin(), a.end(), &compare);
    Point p1 = a[0], p2 = a.back();
    std::vector<Point> up;
    std::vector<Point> down;
    up.push_back(p1);
    down.push_back(p1);
    for (size_t i = 1; i < a.size(); ++i) {
        if (i == a.size() - 1 || right_turn(p1, a[i], p2)) {
            while (up.size() >= 2 && !right_turn(up[up.size() - 2], up[up.size() - 1], a[i])) {
                up.pop_back();
            }
            up.push_back(a[i]);
        }
        if (i == a.size() - 1 || left_turn(p1, a[i], p2)) {
            while (down.size() >= 2 && !left_turn(down[down.size() - 2], down[down.size() - 1], a[i])) {
                down.pop_back();
            }
            down.push_back(a[i]);
        }
    }
    a.clear();
    for (auto i: up) {
        a.push_back(i);
    }
    for (size_t i = down.size() - 2; i > 0; --i) {
        a.push_back(down[i]);
    }
}

int main() {
    int n;
    std::cin >> n;
    std::vector<Point> points(n);
    for (size_t i = 0; i < n; ++i) {
        double x, y;
        std::cin >> x >> y;
        points[i] = Point(x, y);
    }
    convex_hull(points);
    for (auto &point: points) {
        std::cout << point.x << ' ' << point.y << '\n';
    }
    return 0;
}
