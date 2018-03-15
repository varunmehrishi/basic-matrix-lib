/* Varun Mehrishi
 * Simple Matrix operations using Expression Templates
 * Learning resources used:
 * CppCon 2015: Joel Falcou “Expression Templates - Past, Present, Future”, 
 * Peter Gottschling's Discovering Modern C++
 * */

/* #include <iostream> */
#ifndef BASIC_MAT 
#define BASIC_MAT

#include <vector>
#include <cassert>
#include <iostream>
namespace basic {
    /* Base Expression that will capture the abstract syntax tree 
     * for lazy evalution
     *
     * */
    template <typename DerivedExpr>
    class BaseExpr {
    public:
        const DerivedExpr& get_derived() const {return static_cast<const DerivedExpr&>(*this); }
    };


    /* Binary Expression - apply operation op to operands op1, op2, ie op(op1, op2) */
    template<typename OpType, typename Op1, typename Op2>
    class BinaryExpr: public BaseExpr<BinaryExpr<OpType, Op1, Op2>> {
        const Op1& op1;
        const Op2& op2;
        OpType op;
    public: 
        BinaryExpr(const Op1& A, const Op2& B): op1(A), op2(B) {}
        
        /* need this here to get type of this expression */
        using type = std::common_type_t<typename Op1::type, typename Op2::type>;
        type operator()(size_t i, size_t j) const {return op(op1.get_derived(), op2.get_derived(), i, j); }
        inline size_t M() const {return op1.M();}
        inline size_t N() const {return op2.N();}
    };


    /* sum at position (i, j) */
    struct plus_ {
        template<typename Op1, typename Op2>
        std::common_type_t<typename Op1::type, typename Op2::type> 
        operator()(const Op1& op1, const Op2& op2, size_t i, size_t j) const {
            return op1(i, j) + op2(i, j);
        }
    };


    /* row vector i * column vector j */
    struct times_ {
        template<typename Op1, typename Op2>
        std::common_type_t<typename Op1::type, typename Op2::type> 
        operator()(const Op1& op1, const Op2& op2, size_t i, size_t j) const {
            std::common_type_t<typename Op1::type, typename Op2::type> val = 0;
            for(size_t k = 0; k < op1.N(); k++)
                val += op1(i, k) * op2(k, j);
            return val;
        }
    };

    /* overloading operator + on BinaryExpr<plus,..,..> */
    template<typename Op1, typename Op2>
    BinaryExpr<plus_, Op1, Op2> operator + (const BaseExpr<Op1>& op1, const BaseExpr<Op2>& op2) {
        assert(op1.get_derived().M() == op2.get_derived().M());
        assert(op1.get_derived().N() == op2.get_derived().N());
        return BinaryExpr<plus_, Op1, Op2>(op1.get_derived(), op2.get_derived());
    }

    /* overloading operator * on BinaryExpr<times,..,..> */
    template<typename Op1, typename Op2>
    BinaryExpr<times_, Op1, Op2> operator * (const BaseExpr<Op1>& op1, const BaseExpr<Op2>& op2) {
        assert(op1.get_derived().N() == op2.get_derived().M());
        return BinaryExpr<times_, Op1, Op2>(op1.get_derived(), op2.get_derived());
    }

    template<typename T>
    class matrix: public BaseExpr<matrix<T>> {
    protected:
        std::vector<T> mat;
        size_t m_;
        size_t n_;
    public:
        using type = T;
        
        /* empty constructor */ 
        matrix() {};

        /* basic constructor */ 
        matrix(const size_t m, const size_t n): m_(m), n_(n), mat(m*n) {}

        /* basic constructor with initialization */
        matrix(const size_t m, const size_t n, const T init): m_(m), n_(n), mat(m*n, init) {}
        matrix(const matrix& other): m_(other.M()), n_(other.N()) {
            for(size_t i = 0; i < m_; i++)
                for(size_t j = 0; j < n_; j++)
                    this->operator()(i, j) = other(i, j);
        }

        /* direct construction from expression */
        template<typename DerivedExpr>
        matrix(const BaseExpr<DerivedExpr>& other_exp) {
            DerivedExpr const& other = other_exp.get_derived();
            m_ = other.M();
            n_ = other.N();
            mat.resize(m_*n_);
            for(size_t i = 0; i < m_; i++)
                for(size_t j = 0; j < n_; j++)
                    this->operator()(i, j) = other(i, j);
        }


        T& operator()(size_t i, size_t j) {
            return mat[n_*i + j];
        }

        const T operator()(size_t i, size_t j) const {
            return mat[n_*i + j];
        }

        /* computation at time of assignment */
        template<typename DerivedExpr>
        matrix<T>& operator = (const BaseExpr<DerivedExpr>& other_exp) {
            DerivedExpr const& other = other_exp.get_derived();
            for(size_t i = 0; i < m_; i++)
                for(size_t j = 0; j < n_; j++)
                    this->operator()(i, j) = other(i, j);
            return *this;
        }
        
        /* operator += */ 
        template<typename DerivedExpr>
        matrix<T>& operator += (const BaseExpr<DerivedExpr>& other_exp) {
            /* *this = BinaryExpr<plus_,matrix<T>&, DerivedExpr&>(this->get_derived(), other_exp.get_derived()); */
            *this = *this + other_exp;
            return *this;   
        }

        /* operator *= */ 
        template<typename DerivedExpr>
        matrix<T>& operator *= (const BaseExpr<DerivedExpr>& other_exp) {
            matrix<T> result(m_, other_exp.get_derived().N());
            result = *this * other_exp;
            *this = result;
            return *this;
        }
       
        /* pretty print to ostream */
        friend std::ostream& operator << (std::ostream& out, const matrix& mat) {
            for(size_t i = 0; i < mat.M(); i++) {
                for(size_t j = 0; j < mat.N(); j++) {
                    out << mat(i, j) << " ";
                }
                out << "\n";
            }
            return out;
        }
    
        inline size_t M() const {return m_;}
        inline size_t N() const {return n_;}
    };
}
#endif
