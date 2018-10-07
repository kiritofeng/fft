#ifndef VECTOR_H

#define VECTOR_H

template<typename T>
class vector {
    public:
        T x, y, z;

        vector<T>(const vector<T> &v): x(v.x), y(v.y), z(v.z) {}

        vector(T _x, T _y, T _z = T(0)): x(_x), y(_y), z(_z){}

        inline vector<T> operator + (const vector<T> &v) const {
            return vector<T>(x + v.x, y + v.y, z + v.z);
        }

        inline vector<T> operator - () const {
            return vector<T>(-x, -y, -z);
        }

        inline vector<T> operator - (const vector<T> &v) {
            return (*this) + (-v);
        }

        inline vector<T> operator * (const T &t) const {
            return vector<T>(t * x, t * y, t * z);
        }

        inline T operator * (const vector<T> &v) const {
            return x * v.x + y * v.y + z * v.z;
        }

        inline vector<T> operator ^ (const vector<T> &v) const {
            return vector<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
        }

        inline T magnitude() const {
            return (*this) * (*this);
        }
};

#endif
