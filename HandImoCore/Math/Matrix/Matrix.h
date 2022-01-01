#pragma once
#include "Quaternion.h"
#include "../VectorBase.h"

namespace MathImpl
{
    template<typename T>
    struct Matrix4Base
    {

        typedef VectorBase<T, 3> Vector3;
        typedef Matrix4Base<T> Mat4x4;
    public:
        //Identity Matrix
        Matrix4Base()
        {
            T Ident[4][4] =
                {{1, 0, 0, 0},
                {0, 1, 0, 0},
                {0, 0, 1, 0},
                {0, 0, 0, 1}};
            //static_assert(sizeof(Ident) < 16 * sizeof(T), "Size");
            memcpy(m4, Ident, 16 * sizeof(T));
        }

        Matrix4Base(T** Mat)
        {
            *this = Mat;
        }

        Matrix4Base(Quaternion Quat)
        {
            *this = Quat;
        }

        Mat4x4 Inverse(const Mat4x4& b) 
        {
            Mat4x4 a = *this;
            
            T s = 1 / b.Determinant();
            a.m4[0][0] = (b.m4[1][1]*b.m4[2][2] - b.m4[1][2]*b.m4[2][1]) * s;
            a.m4[0][1] = (b.m4[2][1]*b.m4[0][2] - b.m4[2][2]*b.m4[0][1]) * s;
            a.m4[0][2] = (b.m4[0][1]*b.m4[1][2] - b.m4[0][2]*b.m4[1][1]) * s;
            a.m4[0][3] =  b.m4[0][3];
            a.m4[1][0] = (b.m4[1][2]*b.m4[2][0] - b.m4[1][0]*b.m4[2][2]) * s;
            a.m4[1][1] = (b.m4[2][2]*b.m4[0][0] - b.m4[2][0]*b.m4[0][2]) * s;
            a.m4[1][2] = (b.m4[0][2]*b.m4[1][0] - b.m4[0][0]*b.m4[1][2]) * s;
            a.m4[1][3] =  b.m4[1][3];
            a.m4[2][0] = (b.m4[1][0]*b.m4[2][1] - b.m4[1][1]*b.m4[2][0]) * s;
            a.m4[2][1] = (b.m4[2][0]*b.m4[0][1] - b.m4[2][1]*b.m4[0][0]) * s;
            a.m4[2][2] = (b.m4[0][0]*b.m4[1][1] - b.m4[0][1]*b.m4[1][0]) * s;
            a.m4[2][3] =  b.m4[2][3];
            a.m4[3][0] =  -(m4[0][0]*b.m4[3][0] + m4[1][0]*b.m4[3][1] + m4[2][0]*b.m4[3][2]);
            a.m4[3][1] =  -(m4[0][1]*b.m4[3][0] + m4[1][1]*b.m4[3][1] + m4[2][1]*b.m4[3][2]);
            a.m4[3][2] =  -(m4[0][2]*b.m4[3][0] + m4[1][2]*b.m4[3][1] + m4[2][2]*b.m4[3][2]);
            a.m4[3][3] =  b.m4[3][3];
            return a;
        }
	
        T Determinant() const
        {
            return
                m4[0][0] * m4[1][1] * m4[2][2] +
                m4[0][1] * m4[1][2] * m4[2][0] +
                m4[0][2] * m4[1][0] * m4[2][1] -
                m4[0][2] * m4[1][1] * m4[2][0] -
                m4[0][1] * m4[1][0] * m4[2][2] -
                m4[0][0] * m4[1][2] * m4[2][1];
        }

        static Mat4x4 Identity()
        {
            return Mat4x4();
        }

        Vector3 Translation() const
        {
            
        }

        Quaternion Rotation() const
        {
            Quaternion Quat;
            
            T tr = m4[0][0] + m4[1][1] + m4[2][2];
            if ( tr > 0 )
            {
                T s = static_cast<T>(sqrt(tr + 1));
                Quat.W = s * static_cast<T>(0.5);
                s = static_cast<T>(0.5) / s;
                Quat.X = (m4[1][2] - m4[2][1]) * s;
                Quat.Y = (m4[2][0] - m4[0][2]) * s;
                Quat.Z = (m4[0][1] - m4[1][0]) * s;
            }
            else
            {
                int i = 0;
                if ( m4[1][1] > m4[0][0] ) i = 1;
                if ( m4[2][2] > m4[i][i] ) i = 2;

                int j = i + 1; if ( j > 2 ) j = 0;
                int k = j + 1; if ( k > 2 ) k = 0;

                T s = static_cast<T>(sqrt(m4[i][i] - m4[j][j] - m4[k][k] + 1));
                if ( s < std::numeric_limits<T>::epsilon() )
                {
                    Quat.X = Quat.Y = Quat.Z = 0;
                    Quat.W = 1;
                }
                else
                {
                   T pq[16];
                    memcpy(pq, m16, sizeof(m16));
                    pq[i] = s * static_cast<T>(0.5);
                    s = static_cast<T>(0.5) / s;
                    pq[3] = (m4[j][k] - m4[k][j]) * s;
                    pq[j] = (m4[i][j] + m4[j][i]) * s;
                    pq[k] = (m4[i][k] + m4[k][i]) * s;
                }
            }
            return Quat;
        }
        

        void Translate(T tX, T tY, T tZ)
        {
            m4[3][0] = tX;
            m4[3][1] = tY;
            m4[3][2] = tZ;
        }

        void Translate(const Vector3 Vector)
        {
            Translate(Vector.X, Vector.Y, Vector.Z);
        }
        Mat4x4 Transpose()
        {
            Mat4x4 Copy = Transpose(*this);
            return Copy;
        }

        Mat4x4 Transpose(const Mat4x4& u)
        {
            Mat4x4 a = *this;
            
            a.m4[0][0] = u.m4[0][0];
            a.m4[0][1] = u.m4[1][0];
            a.m4[0][2] = u.m4[2][0];
            a.m4[0][3] = u.m4[3][0];
            a.m4[1][0] = u.m4[0][1];
            a.m4[1][1] = u.m4[1][1];
            a.m4[1][2] = u.m4[2][1];
            a.m4[1][3] = u.m4[3][1];
            a.m4[2][0] = u.m4[0][2];
            a.m4[2][1] = u.m4[1][2];
            a.m4[2][2] = u.m4[2][2];
            a.m4[2][3] = u.m4[3][2];
            a.m4[3][0] = u.m4[0][3];
            a.m4[3][1] = u.m4[1][3];
            a.m4[3][2] = u.m4[2][3];
            a.m4[3][3] = u.m4[3][3];
            return a;
        }

        void RotateAngleAxis(const T& angle, const Vector3& axis)
        {
            T length2 = axis.LengthSquared();
            if ( length2 == 0 )
            {
                Identity();
                return;
            }
	
            Vector3 n = axis / static_cast<T>(sqrt(length2));
            T s = static_cast<T>(sin(angle));
            T c = static_cast<T>(cos(angle));
            T k = 1 - c;

            T xx = n.x * n.x * k + c;
            T yy = n.y * n.y * k + c;
            T zz = n.z * n.z * k + c;
            T xy = n.x * n.y * k;
            T yz = n.y * n.z * k;
            T zx = n.z * n.x * k;
            T xs = n.x * s;
            T ys = n.y * s;
            T zs = n.z * s;
	
            m4[0][0] = xx;
            m4[0][1] = xy + zs;
            m4[0][2] = zx - ys;
            m4[0][3] = 0;
            m4[1][0] = xy - zs;
            m4[1][1] = yy;
            m4[1][2] = yz + xs;
            m4[1][3] = 0;
            m4[2][0] = zx + ys;
            m4[2][1] = yz - xs;
            m4[2][2] = zz;
            m4[2][3] = 0;
            m4[3][0] = 0;
            m4[3][1] = 0;
            m4[3][2] = 0;
            m4[3][3] = 1;
        }

        void Scale( T x, T y, T z )
        {
            m4[0][0] = x;
            m4[1][1] = y;
            m4[2][2] = z;
        }
    
        Mat4x4 operator+ (const Mat4x4& u) const
        {
            Mat4x4 r;
            for ( int i=0; i<4; ++i )
            {
                r.m4[i][0] = m4[i][0] + u.m4[i][0];
                r.m4[i][1] = m4[i][1] + u.m4[i][1];
                r.m4[i][2] = m4[i][2] + u.m4[i][2];
                r.m4[i][3] = m4[i][3] + u.m4[i][3];
            }
            return r;
        }

        Mat4x4 operator- (const Mat4x4& u) const
        {
            Mat4x4 r;
            for ( int i=0; i<4; ++i )
            {
                r.m4[i][0] = m4[i][0] - u.m4[i][0];
                r.m4[i][1] = m4[i][1] - u.m4[i][1];
                r.m4[i][2] = m4[i][2] - u.m4[i][2];
                r.m4[i][3] = m4[i][3] - u.m4[i][3];
            }
            return r;
        }

        Mat4x4 operator* (const Mat4x4& u) const
        {
            Mat4x4 r;
            for ( int i=0; i<4; ++i )
            {
                const T* v = m16 + i * 4;
                r.m4[i][0] = v[0]*u.m4[0][0] + v[1]*u.m4[1][0] + v[2]*u.m4[2][0] + v[3]*u.m4[3][0];
                r.m4[i][1] = v[0]*u.m4[0][1] + v[1]*u.m4[1][1] + v[2]*u.m4[2][1] + v[3]*u.m4[3][1];
                r.m4[i][2] = v[0]*u.m4[0][2] + v[1]*u.m4[1][2] + v[2]*u.m4[2][2] + v[3]*u.m4[3][2];
                r.m4[i][3] = v[0]*u.m4[0][3] + v[1]*u.m4[1][3] + v[2]*u.m4[2][3] + v[3]*u.m4[3][3];
            }
            return r;
        }

        Mat4x4& operator+= (const Mat4x4& u)
        {
            for ( int i=0; i<4; ++i )
            {
                m4[i][0] += u.m4[i][0];
                m4[i][1] += u.m4[i][1];
                m4[i][2] += u.m4[i][2];
                m4[i][3] += u.m4[i][3];
            }
            return *this;
        }

        Mat4x4& operator-= (const Mat4x4& u)
        {
            for ( int i=0; i<4; ++i )
            {
                m4[i][0] -= u.m4[i][0];
                m4[i][1] -= u.m4[i][1];
                m4[i][2] -= u.m4[i][2];
                m4[i][3] -= u.m4[i][3];
            }
            return *this;
        }

        Mat4x4& operator*= (const Mat4x4& u)
        {
            for ( int i=0; i<4; ++i )
            {
                T v[4];
                v[0] = m4[i][0];
                v[1] = m4[i][1];
                v[2] = m4[i][2];
                v[3] = m4[i][3];
                m4[i][0] = v[0]*u.m4[0][0] + v[1]*u.m4[1][0] + v[2]*u.m4[2][0] + v[3]*u.m4[3][0];
                m4[i][1] = v[0]*u.m4[0][1] + v[1]*u.m4[1][1] + v[2]*u.m4[2][1] + v[3]*u.m4[3][1];
                m4[i][2] = v[0]*u.m4[0][2] + v[1]*u.m4[1][2] + v[2]*u.m4[2][2] + v[3]*u.m4[3][2];
                m4[i][3] = v[0]*u.m4[0][3] + v[1]*u.m4[1][3] + v[2]*u.m4[2][3] + v[3]*u.m4[3][3];
            }
            return *this;
        }

        Mat4x4& operator*= (const T& s)
        {
            for ( int i=0; i<16; ++i )
            {
                m16[i] *= s;
            }
            return *this;
        }

        void operator= (const T& s)
        {
            m4[0][0] = s; m4[0][1] = 0; m4[0][2] = 0; m4[0][3] = 0;
            m4[1][0] = 0; m4[1][1] = s; m4[1][2] = 0; m4[1][3] = 0;
            m4[2][0] = 0; m4[2][1] = 0; m4[2][2] = s; m4[2][3] = 0;
            m4[3][0] = 0; m4[3][1] = 0; m4[3][2] = 0; m4[3][3] = 1;
        }

        void operator=(const Mat4x4& u)
        {
            for ( int i=0; i<4; ++i )
            {
                m4[i][0] = u.m4[i][0];
                m4[i][1] = u.m4[i][1];
                m4[i][2] = u.m4[i][2];
                m4[i][3] = u.m4[i][3];
            }
        }
        void operator=(const T **Mat)
        {
            static_assert(sizeof(Mat) < 16 * sizeof(T));
            memcpy(m4, Mat, 16 * sizeof(T));
        }

        void operator= (const Quaternion& q)
        {
            T sx = q.X * 2;
            T sy = q.Y * 2;
            T sz = q.Z * 2;
            T wx = q.W * sx;
            T wy = q.W * sy;
            T wz = q.W * sz;
            T xx = q.X * sx;
            T xy = q.X * sy;
            T xz = q.X * sz;
            T yy = q.Y * sy;
            T yz = q.Y * sz;
            T zz = q.Z * sz;
	
            m4[0][0] = 1 - yy - zz;
            m4[0][1] = xy + wz;
            m4[0][2] = xz - wy;
            m4[0][3] = 0;
            m4[1][0] = xy - wz;
            m4[1][1] = 1 - xx - zz;
            m4[1][2] = yz + wx;
            m4[1][3] = 0;
            m4[2][0] = xz + wy;
            m4[2][1] = yz - wx;
            m4[2][2] = 1 - xx - yy;
            m4[2][3] = 0;
            m4[3][0] = 0;
            m4[3][1] = 0;
            m4[3][2] = 0;
            m4[3][3] = 1;
        }
        

        union
        {
            T m4[4][4];
            T m16[16];
        };
    
    };
}
