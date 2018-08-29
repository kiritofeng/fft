/**
    matrix.h
    Purpose: general matrix operations

    @author Kirito Feng
    @version 1.0
*/

#ifndef MATRIX_H

#define MATRIX_H

// I feel bad for including a whole header just for std::swap
// But it's far more efficent in most use cases than what I'd write myself...

#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <vector>

/**
    degenerate_matrix_error class, thrown whenever one tries to invert a degenerate matrix
*/

class degenerate_matrix_error : public std::exception {

    public:

    virtual const char* what() const throw() {
      return "Attempted to take the inverse of degenerate matrix!";
    }
};

/**
    matrix class, for representation and manipulation of matrices

    @param Rows the number of rows in the matrix.
    @param Colums the number of columns in the matrix.
    @param T the data type being stored in the matrix.
*/

template <typename T = int>
class matrix {

    private:

    std::vector<std::vector<T>> data;

    public:

    /**
        Default matrix constructor. All entries are set to their default.
    */

    inline matrix (size_t Rows, size_t Columns) {
        data.reserve(Rows);
        for(size_t i = 0; i < Rows; ++ i) {
            data[i].reserve(Columns);
        }
    }

    /**
        Alternate matrix constructor. All entries are set to t.

        @param t the value to set all entries equal to.
    */

    inline matrix (size_t Rows, size_t Columns, T t) {
        data.reserve(Rows);
        for(size_t i = 0; i < Rows; ++ i) {
            data[i] = std::vector<T>(Columns, t);
        }
    }

    /**
        Copy constructor.

        @param m the matrix to copy.
    */
        
    inline matrix (const matrix<T> &m) {
        data.reserve(m.Rows());
        for(size_t i = 0; i < m.rows(); ++ i) {
            for(size_t j = 0; j < m.columns(); ++ j) {
                data[i].push_back(m(i,j));
            }
        }
    }

    /**
        Returns the identity matrix of size N x N.

        @returns a N x N identity matrix, if such a matrix is valid.
    */

    static inline matrix<T> identity(size_t N) {
        matrix<T>ret = matrix(N,N);

        for(size_t i = 0; i < N; ++ i)
            ret[i][i] = 1;

        return ret;
    }

    /**
        Retrives the number of rows in the matrix.

        @returns the number of rows in the matrix.
    */

    inline size_t rows() {
        if(data.empty()) {
            return 0;
        }
        return data.size();
    }


    /**
        Retrives the number of columns in the matrix.

        @returns the number of columns in the matrix.
    */

    inline size_t columns() {
        if(data.empty()) {
            return 0;
        }
        return data[0].size();
    }


    /**
        Adds two matrices together and returns their result

        @param m the matrix to add.
        @returns the sum of the two matrices.
    */

    inline matrix<T> operator + (const matrix<T> &m) const {
        static_assert(rows() == m.rows() && columns() == m.columns(), "Cannot add matrices of different sizes!");

        matrix<T> ret;

        for(size_t i = 0; i < rows(); ++ i) {
            for(size_t j = 0; j < columns(); ++ j) {
                ret[i][j] = data[i][j] + m[i][j];
            }
        }

        return ret;
    }

    /**
        Negates all entries in the matrix.

        @returns the matrix scaled by -1.
    */

    inline matrix<T> operator - () const {
        matrix<T> ret;

        for(size_t i = 0; i < rows(); ++ i) {
            for(size_t j = 0; j < columns(); ++ j) {
                ret(i,j) = -data[i][j];
            }
        }

        return ret;
    }

    /**
        Subtracts two matrices and returns their result.

        @param m the matrix to subtract.
        @returns the difference of the two matricies.
    */

    inline matrix<T> operator - (const matrix<T> &m) const {
        return (*this) + -m;
    }

    /**
        Scales a matrix by a constant factor.

        @param t the constant to scale the matrix by.
        @returns the matrix scaled by t.
    */

    inline matrix<T> operator * (const T t) const {
        matrix<T> ret(*this);

        for(size_t i = 0; i < rows(); ++ i)
            for(size_t j = 0; j < columns(); ++ j)
                ret(i,j) = ret(i,j) * t;

        return ret;
    }

    /**
        Multiplies two matricies together and returns their result.

        @param NewColumns the number of columns in the resultant matrix.
        @param m the matrix to multiply by.
        @returns the result of multiplying the two matrices.
    */

    inline matrix<T> operator * (matrix<T> & m) const {
        static_assert(columns() == m.rows(), "Cannot mulitply matricies!");

        matrix<T> ret = matrix(rows(), m.columns());

        // The unusual order of loops is an optimization

        for(size_t i = 0; i < rows(); ++ i)
            for(size_t k = 0; k < columns(); ++ k)
                for(size_t j = 0; j < m.columns(); ++ j)
                    ret[i][j] += data[i][k] * data[k][j];

        return ret;
    }

    /**
        Computes the inverse of the matrix.

        @param T1 the data type of the inverted matrix.
        @returns the inverse matrix.
        @throws degernate_matrix_error if the matrix is degenerate
    */

    template<typename T1 = long double>
    inline matrix<T1> inverse() const {
        static_assert(rows() == columns(), "You can't find the inverse of a non-square matrix!");

        matrix<T1> tmp = matrix(*this);
        matrix<T1> ret = matrix<T1>::identity(rows());

        // This is where the fun starts...

        for(size_t i = 0, j; i < rows(); ++ i) {
            for(j = i; j < rows() && tmp(j,i) != T1(0); ++ j);
            if(j == rows()) {
                throw degenerate_matrix_error();
            }
            if(i != j) {
                for(size_t k = i; k < columns(); ++ k) {
                    std::swap(tmp(i,k), tmp(j,k));
                    std::swap(ret(i,k), ret(j,k));
                }
            }
            for(j = i + 1; j < rows(); ++ j) {
                T1 entry1 = tmp(j,i), entry2 = ret(j,i);
                for(size_t k = i; k < columns(); ++k) {
                    tmp(j,k) = tmp[j][k] - (tmp(i,k) / tmp(i,i)) * entry1;
                    ret(j,k) = ret[j][k] - (ret(i,k) / ret(i,i)) * entry2;
                }
            }
        }

        return ret;
    }

    /**
        Computes the matrix determinant.

        @param T1 the data type of the determinant.
        @returns the determinant.
    */

    template<typename T1 = long double>
    inline T1 determinant() const {
        static_assert(rows() == columns(), "You can't find the determinant of a non-square matrix!");

        matrix<T1> tmp = matrix(*this);

        T1 res = T1(1);

        bool b = 0;

        for(size_t i = 0, j; i < rows() - 1; ++ i) {
            for(j = i; j < rows() && tmp(j,i) != T1(0); ++ j);
            if(j == rows()) {
                return T1(0);
            }
            if(i != j) {
                for(size_t k = i; k < columns(); ++ k) {
                    std::swap(tmp(i,k), tmp(j,k));
                }
                b ^= 1;
            }
            for(j = i + 1; j < rows(); ++ j) {
                T entry = tmp(j,i);
                for(size_t k = i; k < columns(); ++ k) {
                    tmp(j,k) = tmp(j,k) - (tmp(i,k) / tmp(i,i)) * entry;
                }
            }
        }

        for(size_t i = 0; i < rows(); ++ i)
            res = res * tmp(i,i);

        return b ? -res : res;
    }

    /**
        Allows access to the matrix entries.

        @param row the row the entry is in.
        @param column the column the entry is in.
        @returns a reference coressponding the the [row][column]-th element of the matrix.
    */

    inline T &operator () (size_t row, size_t column) {
        return data[row][column];
    }

    /**
        Allows access to the matrix entries.

        @param row the row the entry is in.
        @param column the column the entry is in.
        @returns a cosnt_reference coressponding the the [row][column]-th element of the matrix.
    */

    inline const T &operator () (size_t row, size_t column) const {
        return data[row][column];
    }

    /**
        Checks if two matricies are not equal to each other.

        @param m the matrix to compare to.
        @returns true if the two are not equal, and false otherwise.
    */

    inline bool operator != (matrix<T> &m) {
        if(rows() != m.rows() || columns() != m.columns()) return true;
        for(size_t i = 0; i < rows(); ++ i)
            for(size_t j = 0; j < columns(); ++ j)
                if(data[i][j] != m[i][j])
                    return true;
        return false;
    }

    /**
        Checks if two matricies are equal to each other.

        @param m the matrix to compare to.
        @returns true if the two are equal, and false otherwise.
    */

    inline bool operator == (matrix<T> &m) {
        return !((*this) != m);
    }
};

/**
    Override to print a matrix with an std::ostream.

    @param out the ostream to print on.
    @param m the matrix to print.
    @returns out.
*/

template <typename T>
std::ostream& operator <<(std::ostream &out, matrix<T> &m){
    for(size_t i = 0; i < m.rows(); ++ i){
        for(size_t j = 1; j < m.columns(); ++ j) {
            out << m[i][j];
            if(j == m.columns() - 1){
                out << " ";
            }
        }
        out << "\n";
    }
    return out;
}

#endif
