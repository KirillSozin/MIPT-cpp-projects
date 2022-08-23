#include "biginteger.h"


void gcd(BigInteger &numerator, BigInteger &denominator) {
  if (numerator.is_zero()) {
    denominator = 1;
    return;
  }
  if (denominator.is_zero()) {
    denominator = 1;
    numerator = 0;
    return;
  }
  BigInteger a = numerator, b = denominator;
  while (!b.is_zero()) {
    BigInteger temp = a;
    a = b;
    b = temp % b;
  }
  denominator /= a;
  numerator /= a;
}
class Rational {
 private:
  BigInteger numerator;
  BigInteger denominator;
  bool zn_neg = false;

  void swap(Rational &A) {
    std::swap(numerator, A.numerator);
    std::swap(zn_neg, A.zn_neg);
    std::swap(denominator, A.denominator);
  }

 public:
  friend Rational operator-(const Rational &num);
  friend Rational operator*(const Rational &first, const Rational &second);
  friend Rational operator/(const Rational &first, const Rational &second);
  friend bool operator<(const Rational &first, const Rational &second);
  friend bool operator==(const Rational &first, const Rational &second);
  friend Rational operator+(const Rational &first, const Rational &second);
  friend Rational operator-(const Rational &first, const Rational &second);
  friend bool operator==(const Rational &first_const, const Rational &second_const);
  friend std::ostream &operator<<(std::ostream &out, const Rational &num);
  friend std::istream &operator>>(std::istream &in, Rational &num);
  Rational() {
    numerator = 0;
    denominator = 1;
    zn_neg = false;
  }
  explicit Rational(double num) {
    zn_neg = (num < 0);
    if (zn_neg) num = -num;
    long long tmp = (num*base);
    numerator = tmp;
    denominator = base;
  }
  Rational(const BigInteger &num, const BigInteger &den = 1) {
    numerator = num;
    zn_neg = (num.zn_neg != den.zn_neg);
    denominator = den;
    numerator.zn_neg = false;
    denominator.zn_neg = false;
  }
  Rational(const Rational &num) {
    zn_neg = num.zn_neg;
    numerator = num.numerator;
    denominator = num.denominator;
  }
  Rational(const long long &num, const long long &den = 1) {
    numerator = num;
    denominator = den;
    zn_neg = ((num < 0) != (den < 0));
    numerator.zn_neg = false;
    denominator.zn_neg = false;
  }
  Rational &operator=(Rational num) {
    swap(num);
    return *this;
  }
  Rational &operator+=(const Rational &num) {
    if (zn_neg == num.zn_neg) {
      numerator = numerator * num.denominator + num.numerator * denominator;
      denominator *= num.denominator;
    }
    if (zn_neg != num.zn_neg) {
      numerator = numerator * num.denominator - num.numerator * denominator;
      denominator *= num.denominator;
      if (numerator.zn_neg) {
        numerator.zn_neg = false;
        zn_neg = !zn_neg;
      }
    }
    return *this;
  }
  Rational &operator+=(long long num) {
    if (zn_neg == (num < 0)) {
      numerator += num * denominator;
    }
    if (zn_neg != (num < 0)) {
      numerator += num * denominator;
      if (numerator.zn_neg) {
        numerator.zn_neg = false;
        zn_neg = !zn_neg;
      }
    }
    return *this;
  }
  Rational &operator-=(long long num) {
    return *this += -num;
  }
  Rational &operator-=(const Rational &num) {
    if (zn_neg == !num.zn_neg) {
      numerator = numerator * num.denominator + num.numerator * denominator;
      denominator *= num.denominator;
    }
    if (zn_neg != !num.zn_neg) {
      numerator = numerator * num.denominator - num.numerator * denominator;
      denominator *= num.denominator;
      if (numerator.zn_neg) {
        numerator.zn_neg = false;
        zn_neg = !zn_neg;
      }
    }
    return *this;
  }
  Rational &operator*=(long long num) {
    zn_neg = (zn_neg != (num < 0));
    numerator = numerator * num;
    denominator.zn_neg = false;
    numerator.zn_neg = false;
    return *this;
  }
  Rational &operator*=(const Rational &num) {
    zn_neg = (zn_neg != num.zn_neg);
    numerator = numerator * num.numerator;
    denominator = denominator * num.denominator;
    denominator.zn_neg = false;
    numerator.zn_neg = false;
    return *this;
  }
  Rational &operator/=(long long num) {
    zn_neg = (zn_neg != (num < 0));
    denominator *= num;
    denominator.zn_neg = false;
    numerator.zn_neg = false;
    return *this;
  }
  Rational &operator/=(const Rational &num) {
    zn_neg = (zn_neg != num.zn_neg);
    numerator *= num.denominator;
    denominator *= num.numerator;
    denominator.zn_neg = false;
    numerator.zn_neg = false;
    return *this;
  }
  std::string toString() const {
    if (numerator.is_zero()) return "0";
    Rational copy = *this;
    gcd(copy.numerator, copy.denominator);
    std::string str = "";
    if (zn_neg) str += "-";
    str += copy.numerator.toString();
    std::string temp = copy.denominator.toString();
    if (temp.length() > 1 || temp[0] > '1') str += '/' + temp;
    return str;
  }
  std::string asDecimal(size_t precision = 0) const {
    if (numerator.is_zero()) return "0";
    std::string str = "";
    if (zn_neg) str += '-';
    Rational copy = *this;
    gcd(copy.numerator, copy.denominator);
    BigInteger st = 1;
    precision++;
    st.move(precision / base);
    long long ost_st = precision % base;
    for (long long i = 0; i < ost_st; ++i) st *= 10;
    precision--;
    copy.numerator *= st;
    copy.numerator /= copy.denominator;
    copy.denominator = copy.numerator % (st);
    copy.numerator /= st;
    std::string str_tmp = copy.denominator.toString();
    while (str_tmp.length() <= precision) str_tmp = '0' + str_tmp;
    str += copy.numerator.toString() + '.' + str_tmp;
    if (str.back() >= '5') {
      long long i = str.length() - 2;
      while (i >= 0 && (str[i] == '9' || str[i] == '.')) {
        if (str[i] != '.') str[i] = '0';
        i--;
      }
      if (i < 0) {
        str = '1' + str;
      }
      if (i >= 0) {
        str[i]++;
      }
    }
    str.pop_back();
    if (precision == 0) {
      str.pop_back();
    }
    if (str.back() == '.') str.pop_back();
    return str;
  }
  explicit operator double() const {
    if (numerator.is_zero()) return (double) 0;
    size_t precision = 10;
    Rational copy = *this;
    gcd(copy.numerator, copy.denominator);
    BigInteger st = 1;
    long long ost_st = precision % base;
    for (long long i = 0; i < ost_st; ++i) st *= 10;
    copy.numerator *= st;
    copy.numerator /= copy.denominator;
    copy.denominator = copy.numerator % st;
    copy.numerator /= st;
    long long zn = 1;
    if (zn_neg) zn = -1;
    return (zn) * (double) ((double) (long long) copy.numerator
        + (double) ((double) (long long) copy.denominator / (double) (long long) st));
  }
};

Rational operator-(const Rational &num) {
  Rational copy = num;
  copy.zn_neg = !copy.zn_neg;
  return copy;
}
Rational operator+(const Rational &first, const Rational &second) {
  Rational ans = first;
  ans += second;
  return ans;
}
Rational operator-(const Rational &first, const Rational &second) {
  Rational ans = first;
  ans -= second;
  return ans;
}
Rational operator*(const Rational &first, const Rational &second) {
  Rational ans = first;
  ans *= second;
  return ans;
}
Rational operator/(const Rational &first, const Rational &second) {
  Rational ans = first;
  ans /= second;
  return ans;
}
bool operator==(const Rational &first_const, const Rational &second_const) {
  Rational first = first_const;
  Rational second = second_const;
  gcd(first.numerator, first.denominator);
  gcd(second.numerator, second.denominator);
  if (first.numerator.is_zero() && second.numerator.is_zero()) return 1;
  bool ans = (first.zn_neg == second.zn_neg) && (first.numerator == second.numerator)
      && (first.denominator == second.denominator);
  return ans;
}
bool operator<(const Rational &first, const Rational &second) {
  bool d = 0;
  bool ans;
  if (first.zn_neg != second.zn_neg) {
    ans = first.zn_neg;
    return ans;
  } else d = first.zn_neg;
  if (first.numerator * second.denominator < second.numerator * first.denominator) ans = !d;
  else ans = d;
  return ans;
}
bool operator>(const Rational &first, const Rational &second) {
  return !(first < second) && !(first == second);
}
bool operator<=(const Rational &first, const Rational &second) {
  return (first < second) || (first == second);
}
bool operator>=(const Rational &first, const Rational &second) {
  return !(first < second);
}
bool operator!=(const Rational &first, const Rational &second) {
  return !(first == second);
}
std::ostream &operator<<(std::ostream &out, const Rational &num) {
  if (num.numerator.is_zero()) {
    out << "0";
    return out;
  }
  out << num.toString();
  return out;
}
std::istream &operator>>(std::istream &in, Rational &num) {
  in >> num.numerator;
  return in;
}
