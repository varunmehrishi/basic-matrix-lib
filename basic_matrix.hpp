#ifndef BASIC_MAT
#define BASIC_MAT

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <cassert>

namespace basic {
	template<typename T>
	class matrix{
		protected:
		std::vector<T> mat;
		size_t m;
		size_t n;
		public:
		size_t M() const { return m; }
		size_t N() const { return n; }

		/* empty constructor */
		matrix() {};

		/* basic constructor */
		matrix(size_t M, size_t N) {
			m = M;
			n = N;
			mat.resize(m*n);
		}


		/* basic constructor with initial value */
		matrix(size_t M, size_t N, T init) {
			m = M;
			n = N;
			mat.resize(m*n);
			mat.assign(mat.size(), init);
		}

		/* copy constructor */	
		matrix(const matrix& other) {
			mat = other.mat;
			m = other.m;
			n = other.n;
		}

		/* move constructor, need to read more and check again 
		 * compiler seems to take care of move semantics on its own*/ 
		matrix(matrix&& other) {
			mat = std::move(other.mat);
			m = other.m;
			n = other.n;
		}

		/* return value by refrence to mutate */
		T& operator()(size_t i, size_t j) {
			return mat[n*i + j];
		}

		/* const return by value */
		T operator()(size_t i, size_t j) const {
			return mat[n*i + j];
		}

		/* copying from initializer list*/
		void operator = (std::initializer_list<T> l) {
			std::copy(l.begin(), l.begin() + std::min(m*n, l.size()), mat.begin());
			for(size_t i = l.size(); i < m*n; i++)
				mat[i] = 0;
		}

		/* matrix + matrix operator, asserts same shape */	
		matrix operator + (const matrix& other) {
			assert(m == other.m);
			assert(n == other.n);
			matrix result(m, n);
			std::transform(mat.begin(), mat.end(),
							other.mat.begin(), result.mat.begin(), std::plus<T>());
			return result;
		}

		/* matrix + val operator, returns new matrix with val added to each element in matrix */	
		matrix operator + (T val) {
			matrix result(m, n);
			std::transform(mat.begin(), mat.end(), result.mat.begin(),
							[&val](auto x){return x + val;});
			return result;
		}

		/* matrix += val operator, adds val to each element in matrix */	
		void operator += (T val) {
			std::transform(mat.begin(), mat.end(), mat.begin(),
							[&val](auto x){return x + val;});
		}


		/* matrix += matrix operator, adds values of other to matrix */	
		void operator += (const matrix& other) {
			assert(m == other.m);
			assert(n == other.n);
			std::transform(mat.begin(), mat.end(),
							other.mat.begin(), mat.begin(), std::plus<T>());
		}

		/* matrix multiplication * by simplest algorithm */	
		matrix operator * (const matrix& other) {
			assert(n == other.m);
			matrix result(m, other.n);
			for(size_t i = 0; i < m; i++) {
				for(size_t j = 0; j < other.n; j++) {
					T total = 0;
					for(size_t k = 0; k < n; k++)
						total += mat[n*i+k] * other(k, j);
					result(i, j) = total;
				}
			}
			return result;
		}

		/* matrix * val operator, returns new matrix with val multiplied to each element in matrix */	
		matrix operator * (T val) {
			matrix result(m, n);
			std::transform(mat.begin(), mat.end(), result.mat.begin(),
							[&val](auto x){return x * val;});
			return result;
		}

		/* matrix multiplication *= by simplest algorithm */	
		void operator *= (const matrix& other) {
			assert(n == other.m);
			std::vector<T> new_mat(m*other.n);
			for(size_t i = 0; i < m; i++) {
				for(size_t j = 0; j < other.n; j++) {
					T total = 0;
					for(size_t k = 0; k < n; k++)
						total += mat[n*i+k] * other(k, j);
					new_mat[m*i+j] = total;
				}
			}
			n = other.n;
			mat = new_mat;
		}
	
		/* matrix *= val operator, multiplies val to each element in matrix */	
		void operator *= (T val) {
			std::transform(mat.begin(), mat.end(), mat.begin(),
							[&val](auto x){return x * val;});
		}

		/* pretty print to ostream */
		friend std::ostream& operator << (std::ostream& out, const matrix& mat) {
			for(size_t i = 0; i < mat.m; i++) {
				for(size_t j = 0; j < mat.n; j++) {
					out << mat(i, j) << " ";
				}
				out << "\n";
			}
			return out;
		}

	};
}

#endif
