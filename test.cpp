#include <iostream>
#include <complex>
#include <chrono>
#include "basic_matrix.hpp"

int main(void) {
	auto duration = [](auto begin, auto end) {
		return std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	};


	basic::matrix<int> m(10,5,1);
	basic::matrix<int> n(10,5,2);
	
	auto begin = std::chrono::steady_clock::now();
	basic::matrix<int> o = m + n + 10;
	o += o;
	auto end = std::chrono::steady_clock::now();
	std::cout << o << duration(begin, end) << " microseconds\n\n";
	
	
	
	basic::matrix<double> a(10, 5, 2);
	basic::matrix<double> b(5, 10, 5);

	begin = std::chrono::steady_clock::now();
	basic::matrix<double> c = a * b * (1.0/3);
	c *= c + c;
	end = std::chrono::steady_clock::now();
	std::cout << c << duration(begin, end) << " microseconds\n\n";


	std::complex<double> z1{5,2}; 
	basic::matrix<std::complex<double>> p(2, 2);
	p = {z1, z1*z1, -z1, -z1*z1};
	begin = std::chrono::steady_clock::now();
	p *= p;
	p += p;
	end = std::chrono::steady_clock::now();
	std::cout << p << duration(begin, end) << " microseconds\n\n";
	std::endl(std::cout);
}
