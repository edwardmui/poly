#include <bitset>
#include <climits>
#include <iostream>
#include <iomanip>
#include <set>

using namespace std;

inline unsigned int add(unsigned int a, unsigned int b) {
	return a ^ b;
}

inline unsigned int remainder(unsigned int a, unsigned int n) {
	unsigned int number { 0 };
	unsigned int exponent = { 0 };
	while (1) {
		if (a & 1) {
			number = add(number, 1 << (exponent % n));
		}
		exponent += 1;
		a >>= 1;
		if (a == 0) {
			return number;
		}
	}
}

inline unsigned int multiply(unsigned int a, unsigned int b) {
	unsigned int number {0};
	unsigned int power {1};
	while (1) {
		if (a & 1) {
			number = add(number, power * b);
		}
		power <<= 1;
		a >>= 1;
		if (a == 0) {
			return number;
		}
	}
}

inline unsigned int ideal(unsigned int k, unsigned int n) {
	unsigned int smallest_positive {UINT_MAX};
	for (int p=0; p < (1 << n); ++p) {
		unsigned int remainder_val = remainder(multiply(p, k), n);
		if (remainder_val != 0 && remainder_val < smallest_positive) {
			smallest_positive = remainder_val;
		}
	}
	return smallest_positive == UINT_MAX ? 0 : smallest_positive;
}

void all_ideals(unsigned int n, set < int > & unique_ideals) {
	for (int p=0; p < (1 << n); ++p) {
		unique_ideals.insert(ideal(p, n));
	}
}

int main() {
	for (unsigned int n = 2; n < 32; ++n) { //25
		set < int > unique_ideals;
		all_ideals(n, unique_ideals);
		cout << "============== " << setw(2) << n << " ==============" << endl;
		int count = 0;
		for (unsigned int i : unique_ideals) {
			bitset<32> b(i);
			string bstr = "0b" + b.to_string().substr(32-n,n);
			cout << setw(2) << ++count << setw(10) << i << setw(20) << bstr << endl;
		}
	}
}
