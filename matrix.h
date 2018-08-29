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

template <size_t Rows, size_t Columns, typename T = int>
class matrix {

    private:

    T data[Rows][Columns];

    public:

    /**
        Default matrix constructor. All entries are set to their default.
    */

    inline matrix () {}

    /**
        Alternate matrix constructor. All entries are set to t.

        @param t the value to set all entries equal to.
    */

    inline matrix (T t) {
        for(size_t i = 0; i < Rows; ++ i) {
            for(size_t j = 0;j < Columns; ++ j) {
                data[i][j] = t;
            }
        }
    }

    /**
        Copy constructor.

        @param m the matrix to copy.
    */
        
    inline matrix (const matrix<Rows, Columns, T> &m) {
        for(size_t i = 0; i < Rows; ++ i) {
            for(size_t j = 0; j < Columns; ++ j) {
                data[i][j] = m[i][j];
            }
        }
    }

    /**
        Returns the identity matrix of size Rows x Columns.

        @returns a Rows x Columns identity matrix, if such a matrix is valid.
    */

    static inline matrix<Rows, Columns, T> identity() {
        static_assert(Rows == Columns, "The identiy matrix is a square matrix.");

        matrix<Rows, Columns, T>ret;

        for(size_t i = 0; i < Rows; ++ i)
            ret[i][i] = 1;

        return ret;
    }

    /**
        Retrives the number of rows in the matrix.

        @returns the number of rows in the matrix.
    */

    inline size_t rows() {
        return Rows;
    }


    /**
        Retrives the number of columns in the matrix.

        @returns the number of columns in the matrix.
    */

    inline size_t columns() {
        return Columns;
    }


    /**
        Adds two Rows x Columns matrices together and returns their result

        @param m the matrix to add.
        @returns the sum of the two matrices.
    */

    inline matrix<Rows, Columns, T> operator + (const matrix<Rows, Columns, T> &m) const {
        matrix<Rows, Columns, T> ret;

        for(size_t i = 0; i < Rows; ++ i) {
            for(size_t j = 0; j < Columns; ++ j) {
                ret[i][j] = data[i][j] + m[i][j];
            }
        }

        return ret;
    }

    /**
        Negates all entries in the matrix.

        @returns the matrix scaled by -1.
    */

    inline matrix<Rows, Columns, T> operator - () const {
        matrix<Rows, Columns, T> ret;

        for(size_t i = 0; i < Rows; ++ i) {
            for(size_t j = 0; j < Columns; ++ j) {
                ret[i][j] = -data[i][j];
            }
        }

        return ret;
    }

    /**
        Subtracts two Rows x Columns matrices and returns their result.

        @param m the matrix to subtract.
        @returns the difference of the two matricies.
    */

    inline matrix<Rows, Columns, T> operator - (const matrix<Rows, Columns, T> &m) const {
        return (*this) + -m;
    }

    /**
        Scales a matrix by a constant factor.

        @param t the constant to scale the matrix by.
        @returns the matrix scaled by t.
    */

    inline matrix<Rows, Columns, T> operator * (const T t) const {
        matrix<Rows, Columns, T> ret(*this);

        for(size_t i = 0; i < Rows; ++ i)
            for(size_t j = 0; j < Columns; ++ j)
                ret[i][j] = ret[i][j] * t;

        return ret;
    }

    /**
        Multiplies two matricies together and returns their result.

        @param NewColumns the number of columns in the resultant matrix.
        @param m the matrix to multiply by.
        @returns the result of multiplying the two matrices.
    */

    template<size_t NewColumns>
    inline matrix<Rows, NewColumns, T> operator * (matrix<Columns, NewColumns, T> & m) const {
        matrix<Rows, Columns, T> ret;

        // The unusual order of loops is an optimization

        for(size_t i = 0; i < Rows; ++ i)
            for(size_t k = 0; k < Columns; ++ k)
                for(size_t j = 0; j < NewColumns; ++ j)
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
    inline matrix<Rows, Columns, T1> inverse() const {
        static_assert(Rows == Columns, "You can't find the inverse of a non-square matrix!");

        matrix<Rows, Columns, T1> tmp = matrix(*this);
        matrix<Rows, Columns, T1> ret = matrix<Rows, Columns, T1>::identity();

        // This is where the fun starts...

        for(size_t i = 0, j; i < Rows; ++ i) {
            for(j = i; j < Rows && tmp[j][i] != T1(0); ++ j);
            if(j == Rows) {
                throw degenerate_matrix_error();
            }
            if(i != j) {
                for(size_t k = i; k < Columns; ++ k) {
                    std::swap(tmp[i][k], tmp[j][k]);
                    std::swap(ret[i][k], ret[j][k]);
                }
            }
            for(j = i + 1; j < Rows; ++ j) {
                T1 entry1 = tmp[j][i], entry2 = ret[j][i];
                for(size_t k = i; k < Columns; ++k) {
                    tmp[j][k] = tmp[j][k] - (tmp[i][k] / tmp[i][i]) * entry1;
                    ret[j][k] = ret[j][k] - (ret[i][k] / ret[i][i]) * entry2;
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
        static_assert(Rows == Columns, "You can't find the determinant of a non-square matrix!");

        matrix<Rows, Columns, T1> tmp = matrix(*this);

        T1 res = T1(1);

        bool b = 0;

        for(size_t i = 0, j; i < Rows - 1; ++ i) {
            for(j = i; j < Rows && tmp[j][i] != T1(0); ++ j);
            if(j == Rows) {
                return T1(0);
            }
            if(i != j) {
                for(size_t k = i; k < Columns; ++ k) {
                    std::swap(tmp[i][k], tmp[j][k]);
                }
                b ^= 1;
            }
            for(j = i + 1; j < Rows; ++ j) {
                T entry = tmp[j][i];
                for(size_t k = i; k < Columns; ++ k) {
                    tmp[j][k] = tmp[j][k] - (tmp[i][k] / tmp[i][i]) * entry;
                }
            }
        }

        for(size_t i = 0; i < Rows; ++ i)
            res = res * tmp[i][i];

        return b ? -res : res;
    }

    /**
        Allows access to the matrix entries.

        @param row the row of the matrix to return.
        @returns a pointer coressponding the the `row`-th row of the matrix.
    */

    inline T* operator [] (size_t row) {
        return data[row];
    }

    /**
        Checks if two matricies are not equal to each other.

        @param m the matrix to compare to.
        @returns true if the two are not equal, and false otherwise.
    */

    inline bool operator != (matrix<Rows, Columns, T> &m) {
        for(size_t i = 0; i < Rows; ++ i)
            for(size_t j = 0; j < Columns, ++ j)
                if(data[i][j] != m[i][j])
                    return true;
        return false;
    }

    /**
        Checks if two matricies are equal to each other.

        @param m the matrix to compare to.
        @returns true if the two are equal, and false otherwise.
    */

    inline bool operator == (matrix<Rows, Columns, T> &m) {
        return !((*this) != m);
    }
};

#endif
