#include "basic_matrix.hpp"
#include <iostream>
#include <chrono>
using namespace basic;
int main(void) {
	auto duration = [](auto begin, auto end) {
        return std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	};
	using clk = std::chrono::high_resolution_clock;
	matrix<double> a(200, 300, 2);
	matrix<double> b(200, 300, 5);
	matrix<double> c(200, 300, 7);
	auto begin = clk::now(); 
	std::cout << (a+b+c)(100, 100) << "\n";
	std::cout << "Time taken : "<<  duration(begin, clk::now()) << " ms\n\n";

	begin = clk::now();	
	std::cout << (a+a+b+b+c+c+a+b+c)(100, 100) << "\n";
	std::cout << "Time taken : "<<  duration(begin, clk::now()) << " ms\n\n";
	
	
	begin = clk::now();
	matrix<int> m(100, 200, 5);
	matrix<int> n(200, 100, 2);
	matrix<int> o(100, 100, 100);
	/* faster than normal as only two vector product and one addition */
	std::cout << (m*n+o*o)(50, 50) << "\n";
	std::cout << "Time taken : "<<  duration(begin, clk::now()) << " ms\n\n";

	matrix<float> p(5, 6, 0.1);
	matrix<float> q(6, 5, 10);
	begin = clk::now();
	matrix<float> r = p * q;
	r *= r;
	r += r;
	std::cout << "Time taken : "<<  duration(begin, clk::now()) << " ms\n\n";
	std::cout << r << "\n";
}
