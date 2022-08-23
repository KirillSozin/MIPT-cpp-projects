#include <iostream>
#include <vector>
#include <string>

const long long base_st = 9;
const long long base = 1000000000;

class BigInteger {
 private:

  std::vector<long long> digit;

  void swap(BigInteger &num) {
    std::swap(digit, num.digit);
    std::swap(zn_neg, num.zn_neg);
  }
  bool abs_sr(const BigInteger &second) const {
    bool d = false;
    if (digit.size() < second.digit.size()) return !d;
    if (digit.size() > second.digit.size()) return d;
    size_t sz = digit.size() - 1;
    for (size_t i = sz; i > 0; --i) {
      if (digit[i] > second.digit[i]) return d;
      if (digit[i] < second.digit[i]) return !d;
    }
    if (digit[0] > second.digit[0]) return d;
    if (digit[0] < second.digit[0]) return !d;
    return d;
  }
  void delete_zero() {
    while (digit.size() > 1 && digit.back() == 0) {
      digit.pop_back();
    }
  }
  void sum(const BigInteger &second) {
    size_t tail = std::min(second.digit.size(), digit.size());
    long long mind = 0;
    long long current;
    for (size_t i = 0; i < tail; ++i) {
      current = mind + digit[i] + second.digit[i];
      if (current >= base) {
        current -= base;
        mind = 1;
      } else {
        mind = 0;
      }
      digit[i] = current;
    }
    if (tail == digit.size()) {
      for (size_t i = tail; i < second.digit.size(); ++i) {
        current = mind + second.digit[i];
        if (current >= base) {
          current -= base;
          mind = 1;
        }
        else {
          mind = 0;
        }
        digit.push_back(current);
      }
    }
    else {
      for (size_t i = tail; i < digit.size(); ++i) {
        current = mind + digit[i];
        if (current >= base) {
          current -= base;
          mind = 1;
        }
        else {
          mind = 0;
          digit[i] = current;
          break;
        }
        digit[i] = current;
      }
    }
    if (mind) digit.push_back(1);
  }
  void sub(const BigInteger &second) {
    size_t tail = std::min(second.digit.size(), digit.size());
    long long mind = 0;
    long long current;
    long long zn = 1;
    if (abs_sr(second)) zn = -1;
    for (size_t i = 0; i < tail; ++i) {
      current = zn * (digit[i] - second.digit[i]) - mind;
      if (current < 0) {
        current += base;
        mind = 1;
      } else {
        mind = 0;
      }
      digit[i] = current;
    }
    if (zn > 0) {
      for (size_t i = tail; i < digit.size(); ++i) {
        current = digit[i] - mind;
        if (current < 0) {
          current += base;
          mind = 1;
        } else {
          mind = 0;
          digit[i] = current;
          break;
        }
        digit[i] = current;
      }
    }
    if (zn < 0) {
      for (size_t i = tail; i < second.digit.size(); ++i) {
        current = second.digit[i] - mind;
        if (current < 0) {
          current += base;
          mind = 1;
        } else {
          mind = 0;
        }
        digit.push_back(current);
      }
    }
    this->delete_zero();
  }
 public:

  bool zn_neg;
  friend BigInteger operator/(const BigInteger &first, long long second);
  friend BigInteger operator<(const BigInteger &first, long long second);
  friend bool operator==(const BigInteger &first, const BigInteger &second);
  friend bool operator<(const BigInteger &first, const BigInteger &second);
  friend std::ostream &operator<<(std::ostream &out, const BigInteger &num);
  friend std::istream &operator>>(std::istream &in, BigInteger &num);
  friend BigInteger operator+(const BigInteger &first, const BigInteger &second);
  friend BigInteger operator*(const BigInteger &first, const BigInteger &second);
  friend BigInteger operator/(const BigInteger &first, const BigInteger &second);
  friend BigInteger operator%(const BigInteger &first, const BigInteger &second);
  friend BigInteger operator*(const BigInteger &first, const long long &second);
  friend BigInteger operator-(const BigInteger &num);
  BigInteger() {
    zn_neg = false;
    digit = {};
  }
  BigInteger(const std::string &str) {
    long long beg = 0;
    if (str[0] == '-') {
      zn_neg = true;
      beg = 1;
    } else zn_neg = 0;
    long long l = str.length();
    for (long long i = l; i > beg; i -= base_st) {
      if (i < base_st + beg) {
        digit.push_back(std::stoi(str.substr(beg, i - beg)));
      } else {
        digit.push_back(std::stoi(str.substr(i - base_st, base_st)));
      }
    }
  }
  BigInteger(long long num) {
    if (num < 0) {
      zn_neg = true;
      num = -num;
    } else {
      zn_neg = false;
    }
    while (num >= base) {
      digit.push_back(num % base);
      num /= base;
    }
    if (num > 0) digit.push_back(num);
    if (digit.empty()) digit.push_back(0);
  }
  BigInteger(const BigInteger &num) {
    this->zn_neg = num.zn_neg;
    this->digit = num.digit;
  }
  BigInteger &operator+=(const BigInteger &second) {
    if (zn_neg == second.zn_neg) {
      sum(second);
      return *this;
    }
    if (zn_neg != second.zn_neg) {
      if (abs_sr(second)) zn_neg = !zn_neg;
      sub(second);
      return *this;
    }
    return *this;
  }
  BigInteger &operator+=(const long long &second) {
    if (zn_neg == (second < 0)) {
      bool mind = false;
      long long sz = digit.size();
      long long zn_second = 1;
      if (zn_neg) zn_second = -1;
      long long current = mind + zn_second * second + digit[0];
      if (current >= base) {
        current -= base;
        mind = true;
      }
      digit[0] = current;
      for (long long i = 1; i < sz; ++i) {
        current = mind + digit[i];
        if (current >= base) {
          current -= base;
          mind = true;
        } else {
          digit[i] = current;
          break;
        }
        digit[i] = current;
      }
    }
    if (zn_neg != (second < 0)) {
      long long zn_second = 1;
      if (second < 0) {
        zn_second = -1;
      }
      long long mind = 0;
      long long current;
      long long zn = 1;
      if (digit.size() == 1 && digit[0] < zn_second * second) zn = -1;
      if (zn > 0) {
        for (size_t i = 0; i < digit.size(); ++i) {
          current = digit[i] - mind;
          if (current < 0) {
            current += base;
            mind = 1;
          } else {
            mind = 0;
            digit[i] = current;
            break;
          }
          digit[i] = current;
        }
      }
      if (zn == -1) {
        digit[0] = -second + digit[0];
        zn_neg = !zn_neg;
      }
      this->delete_zero();
    }
    return *this;
  }
  BigInteger &operator++() {
    return *this += 1;
  }
  BigInteger &operator--() {
    return *this -= 1;
  }
  BigInteger operator++(int) {
    BigInteger temp(*this);
    *this += 1;
    return temp;
  }
  BigInteger operator--(int) {
    BigInteger temp(*this);
    *this -= 1;
    return temp;
  }
  explicit operator bool() const {
    return !is_zero();
  }
  explicit operator double() const{
    return (double)(*this);
  }
  explicit operator float() const{
    return (float)(*this);
  }
  explicit operator long long() const {
    long long ans = 0;
    long long st = 1;
    for (long long i: digit) {
      ans += i * st;
      st *= base;
    }
    return ans;
  }

  BigInteger &operator-=(const BigInteger &second) {
    bool zn1 = zn_neg, zn2 = !second.zn_neg;
    if (zn1 == zn2) {
      sum(second);
      return *this;
    }
    if (zn1 != zn2) {
      if (abs_sr(second)) zn_neg = !zn_neg;
      sub(second);
      return *this;
    }
    return *this;
  }
  bool is_zero() const {
    if ((digit.size() == 1) && (digit[0] == 0)) return true;
    if (digit.size() == 0) return true;
    return false;
  }
  void move(long long pos) {
    for (long long i = 0; i < pos; ++i) digit.push_back(0);
    for (long long i = digit.size() - 1; i >= pos; --i) {
      digit[i] = digit[i - pos];
    }
    for (long long i = 0; i < pos; ++i) digit[i] = 0;
  }
  BigInteger &operator*=(const BigInteger &second) {
    *this = *this * second;
    return *this;
  }
  BigInteger &operator*=(long long a) {
    if (a == 0) return *this = 0;
    if (a == 1) return *this;
    if (a == -1) {
      zn_neg = !zn_neg;
      return *this;
    }
    digit.resize(digit.size() + 1);
    if (a < 0) {
      zn_neg = !zn_neg;
      a *= -1;
    }
    size_t sz = digit.size();
    for (size_t i = 0; i < sz; ++i) {
      digit[sz - i - 1] *= a;
      if (digit[sz - i - 1] >= base) {
        digit[sz - i] += digit[sz - i - 1] / base;
        digit[sz - i - 1] = digit[sz - i - 1] % base;
      }
    }
    for (size_t i = 0; i < sz; ++i) {
      if (digit[sz - i - 1] >= base) {
        digit[sz - i] += digit[sz - i - 1] / base;
        digit[sz - i - 1] = digit[sz - i - 1] % base;
      }
    }
    delete_zero();
    return *this;
  }
  BigInteger &operator/=(const BigInteger &second) {
    *this = *this / second;
    return *this;
  }
  BigInteger &operator/=(long long second) {
    *this = *this / second;
    return *this;
  }
  BigInteger &operator%=(const BigInteger &second) {
    *this = *this % second;
    return *this;
  }
  BigInteger &operator%=(long long second) {
    *this = *this % second;
    return *this;
  }
  std::string toString() const {
    std::string tmp = "";
    std::string ans = "";
    if (is_zero()) {
      return "0";
    }
    long long sz = digit.size() - 1;
    if (zn_neg) ans += '-';
    long long a = digit[sz];
    while (a > 0) {
      tmp = (char) ((a % 10) + '0') + tmp;
      a /= 10;
    }
    ans += tmp;
    for (long long i = sz - 1; i >= 0; --i) {
      tmp = "";
      long long a = digit[i];
      for (long long j = 1; j <= base_st; ++j) {
        tmp = (char) ((a % 10) + '0') + tmp;
        a /= 10;
      }
      ans += tmp;
    }
    return ans;
  }
  BigInteger &operator=(BigInteger num) {
    swap(num);
    return *this;
  }
};

BigInteger operator "" _bi(unsigned long long int num) {
  return BigInteger(num);
}
BigInteger operator "" _bi(const char *str, size_t size) {
  if (size == 0) return size;
  return BigInteger(str);
}
BigInteger operator "" _bi(const char *str) {
  return BigInteger(str);
}

BigInteger operator-(const BigInteger &num) {
  BigInteger copy = num;
  copy.zn_neg = !num.zn_neg;
  return copy;
}
std::ostream &operator<<(std::ostream &out, const BigInteger &num) {
  if (num.is_zero()) {
    out << "0";
    return out;
  }
  long long sz = num.digit.size() - 1;
  if (num.zn_neg) out << '-';
  out << num.digit[sz];
  for (long long i = sz - 1; i >= 0; --i) {
    long long base1 = base / 10;
    while (num.digit[i] < base1) {
      out << 0;
      base1 /= 10;
    }
    if (num.digit[i] > 0) out << num.digit[i];
  }
  return out;
}
std::istream &operator>>(std::istream &in, BigInteger &num) {
  std::string str;
  in >> str;
  num = str;
  return in;
}
bool operator==(const BigInteger &first, const BigInteger &second) {
  if (first.zn_neg != second.zn_neg) return 0;
  if (first.digit.size() != second.digit.size()) return 0;
  long long sz = first.digit.size() - 1;
  for (long long i = 0; i <= sz; ++i) {
    if (first.digit[i] != second.digit[i]) return 0;
  }
  return 1;
}
bool operator<(const BigInteger &first, const BigInteger &second) {
  bool d = 0;
  if (first.zn_neg != second.zn_neg) return first.zn_neg;
  else d = first.zn_neg;
  if (first.digit.size() < second.digit.size()) return !d;
  if (first.digit.size() > second.digit.size()) return d;
  long long sz = first.digit.size() - 1;
  for (long long i = sz; i >= 0; --i) {
    if (first.digit[i] > second.digit[i]) return d;
    if (first.digit[i] < second.digit[i]) return !d;
  }
  return d;
}
bool operator!=(const BigInteger &first, const BigInteger &second) {
  return !(first == second);
}
bool operator>(const BigInteger &first, const BigInteger &second) {
  return !(first < second) && !(first == second);
}
bool operator>=(const BigInteger &first, const BigInteger &second) {
  return !(first < second);
}
bool operator<=(const BigInteger &first, const BigInteger &second) {
  return (first < second) || (first == second);
}
BigInteger operator+(const BigInteger &first, const BigInteger &second) {
  BigInteger ans = first;
  ans += second;
  return ans;
}
BigInteger operator-(const BigInteger &first, const BigInteger &second) {
  BigInteger ans = first;
  ans -= second;
  return ans;
}
BigInteger operator*(const BigInteger &first, const BigInteger &second) {
  BigInteger ans = 0;
  long long sz1 = first.digit.size(), sz2 = second.digit.size();
  ans.move(sz1 + sz2 + 1);
  ans.zn_neg = second.zn_neg;
  long long current;
  for (long long i = 0; i < sz1; ++i) {
    current = 0;
    for (long long j = 0; j < sz2; ++j) {
      ans.digit[i + j] += current + first.digit[i] * second.digit[j];
      current = ans.digit[i + j] / base;
      ans.digit[i + j] %= base;
    }
    ans.digit[i + sz2] += current;
  }
  ans.delete_zero();
  if (first.zn_neg != second.zn_neg) ans.zn_neg = true;
  if (first.zn_neg == second.zn_neg) ans.zn_neg = false;
  return ans;
}
BigInteger operator*(const long long &first, const BigInteger &second) {
  return second * first;
}
BigInteger operator*(const BigInteger &first, const long long &second) {
  BigInteger ans = first;
  ans *= second;
  return ans;
}
BigInteger operator<(const BigInteger &first, long long second) {
  if (first.digit.size() == 0) {
    return 0 < second;
  }
  if (first.digit.size() == 1) {
    long long tmp = first.digit[0];
    if (first.zn_neg) {
      tmp = -tmp;
    }
    return tmp < second;
  }
  if (first.digit.size() == 2) {
    long long tmp = first.digit[0] + first.digit[1] * base;
    if (first.zn_neg) {
      tmp = -tmp;
    }
    return tmp < second;
  }
  return false;
}
BigInteger operator%(const BigInteger &first, long long second) {
  return first - (first / second) * second;
}
BigInteger operator/(const BigInteger &first, long long second) {
  BigInteger ans;
  long long zn2 = 1;
  if (second < 0) zn2 = -1;
  ans.digit.resize(first.digit.size() + 1, 0);
  BigInteger current;
  for (long long i = first.digit.size() - 1; i >= 0; --i) {
    current.move(1);
    current.delete_zero();
    current.digit[0] = first.digit[i];
    if (current < zn2 * second) continue;
    long long x  = 0;
    if (current.digit.size() == 1) {
      x = current.digit[0] / (zn2 * second);
    }
    if (current.digit.size() == 2) {
      x = (current.digit[0] + current.digit[1] * base) / (zn2 * second);
    }
    ans.digit[i] = x;
    current -= second * (x * zn2);
  }
  ans.delete_zero();
  if (first.zn_neg == (second < 0)) ans.zn_neg = false;
  if (first.zn_neg != (second < 0)) ans.zn_neg = true;
  return ans;
}
BigInteger operator/(const BigInteger &first, const BigInteger &second) {
  BigInteger ans;
  long long zn2 = 1;
  if (second.zn_neg) zn2 = -1;
  ans.digit.resize(first.digit.size() + 1, 0);
  BigInteger current;
  for (long long i = first.digit.size() - 1; i >= 0; --i) {
    current.move(1);
    current.delete_zero();
    current.digit[0] = first.digit[i];
    if (current.abs_sr(second)) continue;
    long long x = 0, l = 0, r = base;
    while (l <= r) {
      long long int med = (l + r) >> 1;
      if (current.abs_sr(second * med)) {
        r = med - 1;
      } else {
        l = med + 1;
        x = med;
      }
    }
    ans.digit[i] = x;
    current -= second * (x * zn2);
  }
  ans.delete_zero();
  if (first.zn_neg == second.zn_neg) ans.zn_neg = false;
  if (first.zn_neg != second.zn_neg) ans.zn_neg = true;
  return ans;
}
BigInteger operator%(const BigInteger &first, const BigInteger &second) {
  long long zn2 = 1;
  if (second.zn_neg) zn2 = -1;
  BigInteger current;
  for (long long i = first.digit.size() - 1; i >= 0; --i) {
    current.move(1);
    current.delete_zero();
    current.digit[0] = first.digit[i];
    if (current.abs_sr(second)) continue;
    long long x = 0, l = 0, r = base;
    while (l <= r) {
      long long med = (l + r) / 2;
      if (current.abs_sr(second * med)) {
        r = med - 1;
      } else {
        l = med + 1;
        x = med;
      }
    }
    current -= second * (x * zn2);
  }
  current.delete_zero();
  current.zn_neg = first.zn_neg;
  return current;
}
