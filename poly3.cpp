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

// global variable naming convention: https://google.github.io/styleguide/cppguide.html#Static_and_Global_Variables
const auto kStart = std::chrono::system_clock::now();

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

inline void print(const unsigned int count, const unsigned long n, const unsigned long ideal_val) {
	bitset < 32 > bin(ideal_val);
	string bin_str = bin.to_string().substr(32 - n, n);
	chrono::duration<double> elapsed_seconds = chrono::system_clock::now() - kStart;
	cout << setw(4) << count << setw(16) << ideal_val << setw(32) << bin_str << setw(16) << fixed << setprecision(6) << elapsed_seconds.count() << endl;
}

inline void all_ideals(const unsigned long n) {
	set<unsigned long> unique_ideals {0};
	print(unique_ideals.size(), n, 0);

	for (unsigned long p = 1; p < (1 << (n-1)); p+=2) {
		unsigned long ideal_val = ideal(p, n);
		if (unique_ideals.find(ideal_val) == unique_ideals.end()) {
			unique_ideals.insert(ideal_val);
			print(unique_ideals.size(), n,ideal_val);
		}
	}
	unsigned long all_ones = (1<<n) - 1;
	unique_ideals.insert(all_ones);
	print(unique_ideals.size(), n, all_ones);
}

inline void prolog(chrono::system_clock::time_point& start, unsigned long n) {
	time_t start_time = chrono::system_clock::to_time_t(start);
	cout << "=========== n: " << setw(2) << n << " start at: " << ctime(&start_time) << endl;
}

inline void epilog(chrono::system_clock::time_point& end, unsigned long n) {
	time_t end_time = chrono::system_clock::to_time_t(end);
	cout << "=========== n: " << setw(2) << n << " end at " << ctime(&end_time) << endl;
}

int main(int argc, char *argv[]) {
	unsigned long n;
	if (argc == 2) {
		unsigned long n = stoi(argv[1]);
		chrono::system_clock::time_point start = chrono::system_clock::now();
		prolog(start, n);
		all_ideals(n);

		chrono::system_clock::time_point end = chrono::system_clock::now();
		//time_t end_time = chrono::system_clock::to_time_t(end);
		//chrono::duration<double> elapsed_seconds = end - start;
		epilog(end, n);
	} else {
		for (n = 2; n < 32; ++n) {
			chrono::system_clock::time_point start = chrono::system_clock::now();
			prolog(start, n);

			all_ideals(n);

			chrono::system_clock::time_point end = chrono::system_clock::now();
			//time_t end_time = chrono::system_clock::to_time_t(end);
			//chrono::duration<double> elapsed_seconds = end - start;
			epilog(end, n);
		}
	}
}
