/**
    vector.h
    Purpose: general 2D/3D vector operations

    @author Kirito Feng
    @version 1.0
**/

#ifndef VECTOR_H

#define VECTOR_H

/**
    vector class, for representation and manipulation of vectors

    @param T the data type being stored in the vector.
*/
template<typename T>
class vector {
    public:

        /**
            The x, y, and z coordinates
        */
        T x, y, z;

        /**
            Copy constructor

            @param v the vector to copy
        */
        vector<T>(const vector<T> &v): x(v.x), y(v.y), z(v.z) {}

        /**
            Constructor for vector. Passing two dimensions results in Z being set to T(0).

            @param _x the x value.
            @param _y the y value.
            @param _z the z value (defaults to 0).
        */
        vector(T _x, T _y, T _z = T(0)): x(_x), y(_y), z(_z){}

        /**
            Adds two vectors and returns their sum.

            @param v the vector to add.
            @returns the vector sum of the two vectors.
        */
        inline vector<T> operator + (const vector<T> &v) const {
            return vector<T>(x + v.x, y + v.y, z + v.z);
        }

        /**
            Returns the negative of the vector

            @returns the negative of the vector.
        */
        inline vector<T> operator - () const {
            return vector<T>(-x, -y, -z);
        }

        /**
            Subtracts two vectors and returns their difference.

            @param v the vector to subtract.
            @returns the vector difference of the two vectors.
        */
        inline vector<T> operator - (const vector<T> &v) {
            return (*this) + (-v);
        }

        /**
            Scales a vector by a constant.

            @param t the constant to scale by.
            @returns the scaled vector.
        */
        inline vector<T> operator * (const T &t) const {
            return vector<T>(t * x, t * y, t * z);
        }

        /**
            Takes two vectors and returns their dot product.

            @param v the vector to take the dot product with.
            @returns the dot product of the two vectors.
        */
        inline T operator * (const vector<T> &v) const {
            return x * v.x + y * v.y + z * v.z;
        }

        /**
            Takes two vectors and returns their dot product.

            @param v the vector to take the dot product with.
            @returns the dot product of the two vectors.
        */
        inline vector<T> operator ^ (const vector<T> &v) const {
            return vector<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
        }

        /**
            Returns the magnitude squared of the vector.

            The magnitude is squared to avoid having to convert data types (int -> double).

            @returns the magnitude of the vector, squared.
        */
        inline T magnitude() const {
            return (*this) * (*this);
        }

        /**
            Returns the unit vector of the vector.
            Please don't use this unless sqrt accepts the data type T...

            @returns the vector as a unit vector
        */
        inline vector<T> normalize() const {
            return (*this) * sqrt(1/magnitude());
        }
};

#endif
