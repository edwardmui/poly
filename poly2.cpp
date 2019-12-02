#include <bitset>
#include <chrono>
#include <ctime>
//#include <climits>
#include <iomanip>
#include <iostream>
#include <set>

using namespace std;

// The following functions, most arguments are 'pass by value' with the "const" modifier
// The constness on pass by value is not a common practice, e.g. std library.
// Reason is that callers of these function are not interested in what the function implementation do to the copy of the value.
// In the implementation later on decides to change the copied value, it would not be able to without changing the function API.

//'g++ -O3 -o poly2 poly2.cpp' has the best performance.
// placing 'register' on each parameter and local variable does not make a difference when 'time ./poly2' it run. Take 1'13" for n=22



inline unsigned long add(const unsigned long a, const unsigned long b) {
	return a ^ b;
}

inline unsigned long remainder(unsigned long a, const unsigned long n) {
	unsigned long number { 0 };
	unsigned long exponent = { 0 };
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

inline unsigned long multiply(unsigned long a, const unsigned long b) {
	unsigned long number { 0 };
	unsigned long power { 1 };
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

inline unsigned long ideal(const unsigned long k, const unsigned long n) {
	for (unsigned long p = 0; p < (1 << n); ++p) {
		unsigned long remainder_val = remainder(multiply(p, k), n);
		if (remainder_val != 0 && remainder_val < k) {
			return 0;
		}
	}

	return k; // == UINT_MAX ? 0 : k;
}

inline void all_ideals(const unsigned long n, set<unsigned long> &unique_ideals) {
	for (unsigned long p = 0; p < (1 << n); ++p) {
		unique_ideals.insert(ideal(p, n));
	}
}

int main() {

	for (unsigned long n = 2; n < 22; ++n) {
		cout << "======================= " << setw(2) << n << " =======================" << endl;

		int count = 0;
		set<unsigned long> unique_ideals;
		all_ideals(n, unique_ideals);

		auto start = std::chrono::system_clock::now();
		auto end = std::chrono::system_clock::now();

		std::chrono::duration<double> elapsed_seconds = end - start;
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);

		std::cout << "finished computation at " << std::ctime(&end_time) << "elapsed time: " << elapsed_seconds.count() << "s\n";

		for (unsigned long i : unique_ideals) {
			bitset < 32 > bin(i);
			string bin_str = "0b" + bin.to_string().substr(32 - n, n);
			cout << setw(4) << ++count << setw(16) << i << setw(32) << bin_str << endl;
		}
	}
}
