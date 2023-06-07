#ifndef LONG_ARITHMETIC_LONGINT_H
#define LONG_ARITHMETIC_LONGINT_H

#include <vector>
#include <string>
#include <iostream>
#include <complex>

class LongInt {
private:
    std::vector<int32_t> number; // stores number in vector in {base}-notation
    bool negative = false;
    const int32_t base = 1e6;
    const int base_log = 6;

    [[maybe_unused]] LongInt(std::vector<int32_t> , bool);

    static std::vector<int> convert_to_base10(const std::vector<int> &);

    [[nodiscard]] std::vector<int> convert_to_base(const std::vector<int> &) const;

    [[nodiscard]] static std::vector<int> slow_multiplication(const std::vector<int> &, const std::vector<int> &);

    [[nodiscard]] static std::vector<int> multiplication(const std::vector<int> &, const std::vector<int> &);

    [[nodiscard]] static LongInt difference(const LongInt &, const LongInt &) ;

public:
    explicit LongInt(std::string &);

    [[maybe_unused]] explicit LongInt(int);

    LongInt(const LongInt &);

    LongInt();

    LongInt &operator=(const LongInt &);

    LongInt operator+(const LongInt &other) const;

    LongInt operator-() const;

    LongInt operator-(const LongInt &other) const;

    LongInt operator+=(const LongInt &other);

    LongInt operator-=(const LongInt &other);

    LongInt operator*(const LongInt &other) const;

    LongInt operator*=(const LongInt &other);

    bool operator<(const LongInt &other) const;

    friend std::istream &operator>>(std::istream &is, LongInt &num);

    friend std::ostream &operator<<(std::ostream &os, const LongInt &num);
};


#endif //LONG_ARITHMETIC_LONGINT_H
