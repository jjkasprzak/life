#pragma once

#include <vector>
#include <cstdint>
#include <cinttypes>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

class BigInt
{
public:
	BigInt(intmax_t number = INTMAX_C(0))
		: sign_(number < 0)
		, buffer_()
	{
		number = std::abs(number);
		while (number > 0)
		{
			buffer_.emplace_back(number % BigInt::base_);
			number /= BigInt::base_;
		}
		if (buffer_.empty())
			buffer_.emplace_back(UINTMAX_C(0));
	}
	explicit BigInt(std::string number)
		: sign_(false)
		, buffer_()
	{
		std::stringstream ss;
		ss << number;
		ss >> *this;
	}
	BigInt& operator+=(const BigInt& r) 
	{
		if (sign_ == r.sign_)
			add(buffer_, r.buffer_);
		else
		{
			int comp = sub(buffer_, r.buffer_);
			sign_ = (sign_ && comp > 0) || (!sign_ && comp < 0);
		}
		return *this;
	}
	BigInt& operator-=(const BigInt& r)
	{
		if (sign_ != r.sign_)
			add(buffer_, r.buffer_);
		else
		{
			int comp = sub(buffer_, r.buffer_);
			sign_ = (sign_ && comp > 0) || (!sign_ && comp < 0);
		}
		return *this;
	}
	BigInt& operator*=(const BigInt& r) 
	{
		sign_ = (sign_ != r.sign_);
		mul(buffer_, r.buffer_);
		return *this;
	}
	BigInt& operator/=(const BigInt& r)
	{
		sign_ = (sign_ != r.sign_);
		div(buffer_, r.buffer_, nullptr);
		return *this;
	}
	BigInt& operator%=(const BigInt& r)
	{
		sign_ = (sign_ != r.sign_);
		std::vector<uintmax_t> rem;
		div(buffer_, r.buffer_, &rem);
		return *this;
	}

	void neg()
	{
		sign_ = !sign_;
	}
	BigInt operator-() const
	{
		BigInt result(*this);
		result.neg();
		return result;
	}
	BigInt operator+(const BigInt& r) const
	{
		BigInt result(*this);
		return result+=r;
	}
	BigInt operator-(const BigInt& r) const
	{
		BigInt result(*this);
		return result -= r;
	}
	BigInt operator*(const BigInt& r) const
	{
		BigInt result(*this);
		return result *= r;
	}
	BigInt operator/(const BigInt& r) const
	{
		BigInt result(*this);
		return result /= r;
	}
	BigInt operator%(const BigInt& r) const
	{
		BigInt result(*this);
		return result %= r;
	}

	bool operator<(const BigInt& r) const
	{
		if (sign_ != r.sign_)
			return sign_;
		int tmp = cmp(buffer_, r.buffer_);
		return (sign_ && tmp > 0) || (!sign_ && tmp < 0);
	}
	bool operator<=(const BigInt& r) const
	{
		if (sign_ != r.sign_)
			return sign_;
		int tmp = cmp(buffer_, r.buffer_);
		return (sign_ && tmp >= 0) || (!sign_ && tmp <= 0);
	}
	bool operator>(const BigInt& r) const
	{
		return !operator<=(r);
	}
	bool operator>=(const BigInt& r) const
	{
		return !operator<(r);
	}
	bool operator==(const BigInt& r) const
	{
		return sign_ == r.sign_ && cmp(buffer_, r.buffer_) == 0;
	}
	bool operator!=(const BigInt& r) const
	{
		return sign_ != r.sign_ || cmp(buffer_, r.buffer_) != 0;
	}
	explicit operator std::string() const
	{
		std::stringstream ss;
		ss << *this;
		return ss.str();
	}

	friend std::ostream& operator<< (std::ostream& stream, const BigInt& number);
	friend std::istream& operator>> (std::istream& stream, BigInt& number);
protected:
	static int cmp(const std::vector<uintmax_t>& l, const std::vector<uintmax_t>& r, size_t rshift = 0);
	static void add(std::vector<uintmax_t>& l, const std::vector<uintmax_t>& r, size_t rshift = 0);
	static int sub(std::vector<uintmax_t>& l, const std::vector<uintmax_t>& r);
	static void mul(std::vector<uintmax_t>& l, const std::vector<uintmax_t>& r);
	static void div(std::vector<uintmax_t>& l, const std::vector<uintmax_t>& r, std::vector<uintmax_t>* rem);
private:
	static const uintmax_t exponent_, base_;

	bool sign_;
	std::vector<uintmax_t> buffer_;
};