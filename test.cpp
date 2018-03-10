#include <iostream>
#include <complex>
#include <chrono>
#include "basic_matrix.hpp"

int main(void) {
	auto duration = [](auto begin, auto end) {
        return std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	};


	basic::matrix<int> m(1000,500,1);
	basic::matrix<int> n(1000,500,2);
	
	auto begin = std::chrono::steady_clock::now();
	basic::matrix<int> o = m + n + 10;
	o += o;
	auto end = std::chrono::steady_clock::now();
	std::cout << "Addition "<<duration(begin, end) << " microseconds\n\n";
	
	
	
	basic::matrix<double> a(100, 500, 2);
	basic::matrix<double> b(500, 100, 5);

	begin = std::chrono::steady_clock::now();
	basic::matrix<double> c = a * b * (1.0/3);
	c *= c + c;
	end = std::chrono::steady_clock::now();
	std::cout << "Multiplication " << duration(begin, end) << " microseconds\n\n";


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
