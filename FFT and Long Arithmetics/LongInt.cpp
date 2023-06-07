#include "LongInt.h"

#include <utility>

LongInt::LongInt(std::string &num) {
    if (num.empty()) {
        number.push_back(0);
    } else {
        int first_digit = 0;
        if (num[0] == '-') {
            negative = true;
            first_digit = 1;
        }
        int i = static_cast<int>(num.size()) - 1;
        while (i >= first_digit) {
            std::string cur_chunk;
            for (int j = i; j > i - base_log && j >= 0 && j >= first_digit; --j) {
                cur_chunk += num[j];
            }
            std::reverse(cur_chunk.begin(), cur_chunk.end());
            number.push_back(std::stoi(cur_chunk));
            i -= base_log;
        }
    }
}


[[maybe_unused]] LongInt::LongInt(int num) {
    if (num < 0) {
        negative = true;
        num *= -1;
    }
    number.push_back(num % base);
    if (num >= base) {
        number.push_back((num / base) % base);
    }
}

LongInt::LongInt(const LongInt &other) : number(other.number), negative(other.negative) {
}


LongInt::LongInt() {
    number.push_back(0);
}

LongInt &LongInt::operator=(const LongInt &other) {
    if (this == &other) {
        return *this;
    }
    number = other.number;
    negative = other.negative;
    return *this;
}

LongInt LongInt::difference(const LongInt &l_value, const LongInt &r_value) {
    bool sign = false;
    LongInt a = l_value;
    LongInt b = r_value;
    if (a < b) {
        std::swap(a, b);
        sign = true;
    }
    std::vector<int> a_num = a.number;
    std::vector<int> b_num = b.number;
    while (b_num.size() < a_num.size()) {
        b_num.push_back(0);
    }
    std::vector<int> result;
    int carry = 0;
    for (int i = 0; i < a_num.size(); ++i) {
        int diff = a_num[i] - carry - ((i < b_num.size()) ? b_num[i] : 0);
        if (diff < 0) {
            diff += 10;
            carry = 1;
        } else {
            carry = 0;
        }
        result.push_back(diff);
    }
    while (result.back() == 0 && result.size() > 1) {
        result.pop_back();
    }
    std::reverse(result.begin(), result.end());
    return {result, sign};
}

LongInt LongInt::operator+(const LongInt &other) const {
    bool sign;
    if (!negative && !other.negative) {
        sign = true;
    } else if (negative && other.negative) {
        sign = false;
    } else {
        return difference(*this, other);
    }
    std::vector<int32_t> new_number;
    int add_value = 0;
    for (size_t i = 0; i < std::min(number.size(), other.number.size()); ++i) {
        int cur_value = number[i] + other.number[i] + add_value;
        add_value = 0;
        if (cur_value >= base) {
            add_value = 1;
            cur_value %= base;
        }
        new_number.push_back(cur_value);
    }
    if (number.size() > other.number.size()) {
        for (size_t i = other.number.size(); i < number.size(); ++i) {
            int cur_value = number[i] + add_value;
            add_value = 0;
            if (cur_value >= base) {
                add_value = 1;
                cur_value %= base;
            }
            new_number.push_back(cur_value);
        }
    } else if (number.size() < other.number.size()) {
        for (size_t i = number.size(); i < other.number.size(); ++i) {
            int cur_value = other.number[i] + add_value;
            add_value = 0;
            if (cur_value >= base) {
                add_value = 1;
                cur_value %= base;
            }
            new_number.push_back(cur_value);
        }
    }

    if (add_value) {
        new_number.push_back(1);
    }
    return {new_number, sign};
}

LongInt LongInt::operator-() const {
    LongInt res(*this);
    res.negative ^= true;
    return res;
}

LongInt LongInt::operator-(const LongInt &other) const {
    return *this + (-other);
}

LongInt LongInt::operator+=(const LongInt &other) {
    *this = *this + other;
    return *this;
}

LongInt LongInt::operator-=(const LongInt &other) {
    *this = *this - other;
    return *this;
}

LongInt LongInt::operator*(const LongInt &other) const { // FFT algorithm
    std::vector<int> l_value = convert_to_base10(number);
    std::vector<int> r_value = convert_to_base10(other.number);
    bool sign = negative ^ other.negative;
    std::vector<int> multiplied = multiplication(l_value, r_value);
    return {convert_to_base(multiplied), sign};
}

void FFT(std::vector<std::complex<double>> &a, bool invert) {
    size_t n = a.size();
    if (n == 1) {
        return;
    }
    std::vector<std::complex<double>> a0(n / 2), a1(n / 2);
    for (size_t i = 0, j = 0; i < n; i += 2, ++j) {
        a0[j] = a[i];
        a1[j] = a[i + 1];
    }
    FFT(a0, invert);
    FFT(a1, invert);

    const double PI = std::atan(1.) * 4;
    double alpha = 2 * PI / static_cast<int>(n) * (invert ? -1 : 1);
    std::complex<double> w(1), wn(std::cos(alpha), std::sin(alpha));
    for (size_t i = 0; i < n / 2; ++i) {
        a[i] = a0[i] + w * a1[i];
        a[i + n / 2] = a0[i] - w * a1[i];
        if (invert) {
            a[i] /= 2;
            a[i + n / 2] /= 2;
        }
        w *= wn;
    }
}

std::vector<int> LongInt::multiplication(const std::vector<int> &l_value, const std::vector<int> &r_value) {
    if (l_value.size() + r_value.size() <= 64) {
        // optimization in case of short numbers (because of recursion constant its better)
        return slow_multiplication(l_value, r_value);
    }
    std::vector<int> result;
    std::vector<std::complex<double>> fa(l_value.begin(), l_value.end());
    std::vector<std::complex<double>> fb(r_value.begin(), r_value.end());
    size_t n = 1;
    size_t mx_size = std::max(l_value.size(), r_value.size());
    while (n < mx_size) {
        n *= 2;
    }
    n *= 2;
    fa.resize(n);
    fb.resize(n);
    FFT(fa, false);
    FFT(fb, false);
    for (size_t i = 0; i < n; ++i) {
        fa[i] *= fb[i];
    }
    FFT(fa, true);
    result.resize(n);
    for (size_t i = 0; i < n; ++i) {
        result[i] = static_cast<int>(fa[i].real() + .5);
    }
    int carry = 0;
    for (size_t i = 0; i < n; ++i) {
        result[i] += carry;
        carry = result[i] / 10;
        result[i] %= 10;
    }
    return result;
}

std::vector<int> LongInt::slow_multiplication(const std::vector<int> &l_value, const std::vector<int> &r_value) {
    std::vector<int> result(l_value.size() + r_value.size());
    for (size_t i = 0; i < l_value.size(); i++) {
        for (size_t j = 0; j < r_value.size(); j++) {
            result[i + j] += l_value[i] * r_value[j];
        }
    }
    for (size_t i = 0; i + 1 < result.size(); i++) {
        result[i + 1] += result[i] / 10;
        result[i] %= 10;
    }
    while (result.back() == 0 && result.size() > 1) {
        result.pop_back();
    }
    return result;
}

LongInt LongInt::operator*=(const LongInt &other) {
    *this = *this * other;
    return *this;
}

std::istream &operator>>(std::istream &is, LongInt &num) {
    std::string num_;
    is >> num_;
    num = LongInt(num_);
    return is;
}

std::ostream &operator<<(std::ostream &os, const LongInt &num) {
    if (num.negative) {
        os << '-';
    }
    for (int i = static_cast<int>(num.number.size()) - 1; i >= 0; --i) {
        std::cout << num.number[i];
    }
    return os;
}

[[maybe_unused]] LongInt::LongInt(std::vector<int32_t> number_, bool negative_) : number(std::move(number_)), negative(negative_) {
}

std::vector<int> LongInt::convert_to_base10(const std::vector<int> &num) {
    std::vector<int> res;
    for (auto val: num) {
        while (val) {
            res.push_back(val % 10);
            val /= 10;
        }
    }
    return res;
}


std::vector<int> LongInt::convert_to_base(const std::vector<int> &num_) const {
    std::vector<int> num = num_;
    while (num.size() % 6 != 0) {
        num.push_back(0);
    }
    std::vector<int> res;
    for (int i = 0; i < num.size(); i += base_log) {
        int val = 0;
        int pow_ = 1;
        for (int j = i; j < i + base_log; ++j) {
            val += pow_ * num[j];
            pow_ *= 10;
        }
        res.push_back(val);
    }
    while (res.back() == 0 && res.size() > 1) {
        res.pop_back();
    }
    return res;
}

bool LongInt::operator<(const LongInt &other) const {
    if (negative && !other.negative) {
        return true;
    } else if (!negative && other.negative) {
        return false;
    }
    bool change = false;
    if (negative && other.negative) {
        change = true;
    }
    if (number.size() < other.number.size()) {
        return !change;
    } else if (number.size() > other.number.size()) {
        return change;
    }
    for (int i = static_cast<int>(number.size()) - 1; i >= 0; --i) {
        if (number[i] != other.number[i]) {
            return (number[i] < other.number[i]) ^ change;
        }
    }
    return change;
}
