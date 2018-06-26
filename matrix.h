#ifndef MATRIX_H

#define MATRIX_H

#include <cstdlib>

template <size_t Rows, size_t Columns, typename T = int>
class matrix {

    private:

    T data[Rows][Columns];

    public:

    inline matrix () {}

    inline matrix (const matrix<Rows, Columns, T> &m) {
        for(size_t i = 0; i < Rows; ++ i) {
            for(size_t j = 0; j < Columns; ++ j) {
                data[i][j] = m[i][j];
            }
        }
    }

    static inline matrix<Rows, Columns, T> identity() {
        static_assert(Rows == Columns, "The identiy matrix is a square matrix.");

        matrix<Rows, Columns, T>ret;

        for(int i = 0; i < Rows; ++ i)
            ret[i][i] = 1;

        return ret;
    }

    inline size_t rows() {
        return Rows;
    }

    inline size_t columns() {
        return Columns;
    }

    inline matrix<Rows, Columns, T> operator + (const matrix<Rows, Columns, T> &m) const {
        matrix<Rows, Columns, T> ret;

        for(size_t i = 0; i < Rows; ++ i) {
            for(size_t j = 0; j < Columns; ++ j) {
                ret[i][j] = data[i][j] + m[i][j];
            }
        }

        return ret;
    }

    inline matrix<Rows, Columns, T> operator - () const {
        matrix<Rows, Columns, T> ret;

        for(size_t i = 0; i < Rows; ++ i) {
            for(size_t j = 0; j < Columns; ++ j) {
                ret[i][j] = -data[i][j];
            }
        }

        return ret;
    }

    inline matrix<Rows, Columns, T> operator - (const matrix<Rows, Columns, T> &m) const {
        return (*this) + -m;
    }

    template<size_t NewColumns>
    inline matrix<Rows, NewColumns, T> operator * (const matrix<Columns, NewColumns, T> & m) const {
        matrix<Rows, Columns, T> ret;

        for(size_t i = 0; i < Rows; ++ i)
            for(size_t k = 0; k < Columns; ++ k)
                for(size_t j = 0; j < NewColumns; ++ j)
                    ret[i][j] += data[i][k] + data[k][j];

        return ret;
    }

    template<typename T1 = long double>
    inline matrix<Rows, Columns, T1> inverse() const {
        // TODO
    }

    template<typename T1 = long double>
    inline matrix<Rows, Columns, T1> determinant() const {
        // TODO
    }

    inline T* operator [] (size_t row) {
        return data[row];
    }
};

#endif
