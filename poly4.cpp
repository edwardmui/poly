#include <bitset>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

using namespace std;

// number of bits in k
inline unsigned long bits(const unsigned long k) {
	unsigned long digits = 0;
	while (1) {
		digits += 1;
		if (1 << digits > k) {
			return digits;
		}
	}
}

//# qth bit of k, zero-indexed
inline bool get_bit(const unsigned long k, const unsigned long q) {
	return ((1 << q) & k) != 0;
}

//# qth bit, zero-indexed, of a * b (mod x^n-1)
inline bool multiply_bit(const unsigned long a, const unsigned long b, const unsigned long n, const unsigned long q) {
	bool bit = false;
	for (unsigned long i = 0; i < n; ++i) {
		bit ^= get_bit(a, i) & get_bit(b, (q >= i) ? (q - i) : (q - i + n));
	}

	return bit;
}

bool is_minimal(const unsigned long k, const unsigned long n) {
	unsigned long size = bits(k);
	for (unsigned long i = 0; i < (1 << (size - 1)); ++i) {
		unsigned long multiplier = i << (n - size + 1); 			// fills in first few digits
		for (long j = (n - size); j >= 0; --j) {
			if (multiply_bit(k, multiplier, n, j + size - 1)) {
				multiplier |= 1 << j; 								// fills in remaining digits;
			}
		}

		for (unsigned long j = 0; j < (size - 1); ++j) {
			if (multiply_bit(k, multiplier, n, j)) {
				return false;
			}
		}
	}

	return true;
}

int main(int argc, char *argv[]) {
	unsigned long starting_n = 2;
	unsigned long ending_n = 32;
	if (argc == 2) {
		starting_n = stoi(argv[1]);
		ending_n = starting_n + 1;
	}

	const auto time0 = std::chrono::system_clock::now();

	for (unsigned long n = starting_n; n < ending_n; ++n) {
		unsigned long counter = 0;

		chrono::system_clock::time_point start = chrono::system_clock::now();
		time_t start_time = chrono::system_clock::to_time_t(start);
		cout << "=========== n: " << setw(2) << n << " ===== start at: " << ctime(&start_time);

		for (unsigned long i = 0; i < (1 << n); ++i) {
			if (is_minimal(i, n)) {
				chrono::duration<double> elapsed_seconds = chrono::system_clock::now() - time0;
				bitset < 32 > bin(i);
				string bin_str = bin.to_string().substr(32 - n, n);
				cout << setw(4) << ++counter << setw(16) << i << setw(32) << bin_str << setw(16) << fixed << setprecision(6) << elapsed_seconds.count() << endl;
			}
		}

		chrono::system_clock::time_point stop = chrono::system_clock::now();
		time_t stop_time = chrono::system_clock::to_time_t(stop);
		cout << "=========== n: " << setw(2) << n << " =====  stop at: " << ctime(&stop_time);
	}
}
