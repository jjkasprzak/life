#include "BigInt.h"
const uintmax_t BigInt::exponent_ = (uintmax_t)(std::log10(2) * 4 * sizeof(uintmax_t));
const uintmax_t BigInt::base_ = (uintmax_t)std::pow(10, BigInt::exponent_);

int BigInt::cmp(const std::vector<uintmax_t>& l, const std::vector<uintmax_t>& r, size_t rshift)
{
	size_t pos = std::max(l.size(), r.size() + rshift);
	while (pos-- > rshift)
	{
		uintmax_t lv = pos < l.size() ? l[pos] : 0;
		uintmax_t rv = (pos - rshift) < r.size() ? r[pos - rshift] : 0;
		if (lv < rv)
			return -1;
		else if (lv > rv)
			return 1;
	}
	return 0;
}

void BigInt::add(std::vector<uintmax_t>& l, const std::vector<uintmax_t>& r, size_t rshift)
{
	const size_t minLength = std::min(l.size(), r.size() + rshift);
	uintmax_t carry = 0;
	uintmax_t tmp = 0;
	size_t i = rshift;
	for (; i < minLength; ++i)
	{
		tmp = carry + l[i] + r[i-rshift];
		carry = tmp / BigInt::base_;
		l[i] = tmp % BigInt::base_;
	}
	for (; i < l.size(); ++i)
	{
		if (carry == 0)
			break;
		tmp = carry + l[i];
		carry = tmp / BigInt::base_;
		l[i] = tmp % BigInt::base_;
	}
	for (; i < r.size() + rshift; ++i)
	{
		tmp = carry + r[i-rshift];
		carry = tmp / BigInt::base_;
		l.emplace_back(tmp % BigInt::base_);
	}
	if (carry != 0)
		l.emplace_back(carry);
}

int BigInt::sub(std::vector<uintmax_t>& l, const std::vector<uintmax_t>& r)
{
	int comp = cmp(l, r);
	const std::vector<uintmax_t>* big;
	const std::vector<uintmax_t>* small;
	if (comp == 0)
	{
		l.clear();
		l.emplace_back(0);
		return comp;
	}
	else if (comp < 0)
	{
		big = &r;
		small = &l;
		l.reserve(r.size());
		while (l.size() < r.size())
			l.emplace_back(0);
	}
	else
	{
		big = &l;
		small = &r;
	}
	size_t i = 0;
	intmax_t carry = 0;
	intmax_t diff;
	for (; i < small->size(); ++i)
	{
		diff = intmax_t(big->at(i)) - (small->at(i) + carry);
		if (diff < 0)
		{
			diff += BigInt::base_;
			carry = 1;
		}
		else
			carry = 0;
		l[i] = diff;
	}
	for (; i < big->size(); ++i)
	{
		diff = intmax_t(big->at(i)) - carry;
		if (diff < 0)
		{
			diff += BigInt::base_;
			carry = 1;
		}
		else
			carry = 0;
		l[i] = diff;
	}
	while (l.size() > 1 && l.back() == 0)
		l.pop_back();
	return comp;
}

void BigInt::mul(std::vector<uintmax_t>& l, const std::vector<uintmax_t>& r)
{
	//standard long multiplication algorithm
	std::vector<uintmax_t> left = std::move(l);
	l = { 0 };
	l.reserve(l.size() + r.size());

	const std::vector<uintmax_t>* big;
	const std::vector<uintmax_t>* small;
	if (left.size() < r.size())
	{
		small = &left;
		big = &r;
	}
	else 
	{
		small = &r;
		big = &left;
	}
	size_t i = 0;
	for (; i < small->size(); ++i)
	{
		std::vector<uintmax_t> tmp;
		uintmax_t carry = 0;
		uintmax_t value = 0;
		for (size_t j = 0; j < big->size(); ++j)
		{
			value = small->at(i) * big->at(j) + carry;
			carry = value / BigInt::base_;
			tmp.emplace_back(value % BigInt::base_);
		}
		if (carry != 0)
			tmp.emplace_back(carry);
		add(l, tmp, i);
	}
}

void BigInt::div(std::vector<uintmax_t>& l, const std::vector<uintmax_t>& r, std::vector<uintmax_t>* rem)
{

}



std::ostream& operator<<(std::ostream& stream, const BigInt& number)
{
	if (number.sign_)
		stream << '-';
	if (!number.buffer_.empty())
	{
		auto ri = number.buffer_.crbegin();
		stream << *ri;
		while (++ri != number.buffer_.crend())
			stream << std::setfill('0') << std::setw(BigInt::exponent_) << *ri;
	}
	else 
		stream << '0';
	return stream;
}

std::istream& operator>>(std::istream& stream, BigInt& number)
{
	stream >> std::ws;
	int next = stream.peek();
	if (next != '+' && next != '-' && next < '0' && next > '9')
	{
		stream.setstate(std::ios_base::failbit);
		return stream;
	}

	number.buffer_.clear();
	number.sign_ = (next == '-');
	if (next == '+' || next == '-')
	{
		stream.get();
		next = stream.peek();
	}
	std::string acc;
	while (next >= '0' && next <= '9')
	{
		stream.get();
		if(next != '0' || acc.size() != 0)
			acc += (char)next;
		next = stream.peek();
	}
	if (acc.size() == 0)
		number.buffer_.emplace_back(UINTMAX_C(0));
	else
		for (intmax_t pos = acc.size() - BigInt::exponent_; true ; pos -= BigInt::exponent_)
		{
			if (pos <= 0)
			{
				number.buffer_.emplace_back(std::strtoumax(acc.substr(0, BigInt::exponent_ + pos).c_str(), nullptr, 10));
				break;
			}
			else
				number.buffer_.emplace_back(std::strtoumax(acc.substr(pos, BigInt::exponent_).c_str(), nullptr, 10));
		}
	return stream;
}
