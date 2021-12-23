#pragma once
#include "Quaternion.h"
#include "../VectorBase.h"

namespace MathImpl
{
    template<typename T>
    class Matrix4Base
    {
        Matrix4Base()
        {
            x.X = 1; x.Y = 0; x.Z = 0; x.W = 0;
            y.X = 0; y.Y = 1; y.Z = 0; y.W = 0;
            z.X = 0; z.Y = 0; z.Z = 1; z.W = 0;
            w.X = 0; w.Y = 0; w.Z = 0; w.W = 1;
        }

        Matrix4Base(const T* m)
        {
            x.X = m[0];  x.Y = m[1];  x.Z = m[2];  x.W = m[3];
            y.X = m[4];  y.Y = m[5];  y.Z = m[6];  y.W = m[7];
            z.X = m[8];  z.Y = m[9];  z.Z = m[10]; z.W = m[11];
            w.X = m[12]; w.Y = m[13]; w.Z = m[14]; w.W = m[15];
        }

        void Invert() 
        {
            x.X = y.Z * z.W * w.Y - y.W * z.Z * w.Y + y.W * z.Y * w.Z - y.Y * z.W * w.Z - y.Z * z.Y * w.W + y.Y * z.Z * w.w; 
            x.Y = x.W * z.Z * w.Y - x.Z * z.W * w.Y - x.W * z.Y * w.Z + x.Y * z.W * w.Z + x.Z * z.Y * w.W - x.Y * z.Z * w.w; 
            x.Z = x.Z * y.W * w.Y - x.W * y.Z * w.Y + x.W * y.Y * w.Z - x.Y * y.W * w.Z - x.Z * y.Y * w.W + x.Y * y.Z * w.w; 
            x.W = x.W * y.Z * z.Y - x.Z * y.W * z.Y - x.W * y.Y * z.Z + x.Y * y.W * z.Z + x.Z * y.Y * z.W - x.Y * y.Z * z.w; 
            y.X = y.W * z.Z * w.X - y.Z * z.W * w.X - y.W * z.X * w.Z + y.X * z.W * w.Z + y.Z * z.X * w.W - y.X * z.Z * w.w; 
            y.Y = x.Z * z.W * w.X - x.W * z.Z * w.X + x.W * z.X * w.Z - x.X * z.W * w.Z - x.Z * z.X * w.W + x.X * z.Z * w.w; 
            y.Z = x.W * y.Z * w.X - x.Z * y.W * w.X - x.W * y.X * w.Z + x.X * y.W * w.Z + x.Z * y.X * w.W - x.X * y.Z * w.w; 
            y.W = x.Z * y.W * z.X - x.W * y.Z * z.X + x.W * y.X * z.Z - x.X * y.W * z.Z - x.Z * y.X * z.W + x.X * y.Z * z.w; 
            z.X = y.Y * z.W * w.X - y.W * z.Y * w.X + y.W * z.X * w.Y - y.X * z.W * w.Y - y.Y * z.X * w.W + y.X * z.Y * w.w; 
            z.Y = x.W * z.Y * w.X - x.Y * z.W * w.X - x.W * z.X * w.Y + x.X * z.W * w.Y + x.Y * z.X * w.W - x.X * z.Y * w.w; 
            z.Z = x.Y * y.W * w.X - x.W * y.Y * w.X + x.W * y.X * w.Y - x.X * y.W * w.Y - x.Y * y.X * w.W + x.X * y.Y * w.w; 
            z.W = x.W * y.Y * z.X - x.Y * y.W * z.X - x.W * y.X * z.Y + x.X * y.W * z.Y + x.Y * y.X * z.W - x.X * y.Y * z.w; 
            w.X = y.Z * z.Y * w.X - y.Y * z.Z * w.X - y.Z * z.X * w.Y + y.X * z.Z * w.Y + y.Y * z.X * w.Z - y.X * z.Y * w.z; 
            w.Y = x.Y * z.Z * w.X - x.Z * z.Y * w.X + x.Z * z.X * w.Y - x.X * z.Z * w.Y - x.Y * z.X * w.Z + x.X * z.Y * w.z; 
            w.Z = x.Z * y.Y * w.X - x.Y * y.Z * w.X - x.Z * y.X * w.Y + x.X * y.Z * w.Y + x.Y * y.X * w.Z - x.X * y.Y * w.z; 
            w.W = x.Y * y.Z * z.X - x.Z * y.Y * z.X + x.Z * y.X * z.Y - x.X * y.Z * z.Y - x.Y * y.X * z.Z + x.X * y.Y * z.z; 
		
            T d = Determinant();
		
            x = x / d;
            y = y / d;
            z = z / d;
            w = w / d;
        }
	
        T Determinant() const
        {
            T value;
            value =
            x.W * y.Z * z.Y * w.X - x.Z * y.W * z.Y * w.X - x.W * y.Y * z.Z * w.X + x.Y * y.W * z.Z * w.X +
            x.Z * y.Y * z.W * w.X - x.Y * y.Z * z.W * w.X - x.W * y.Z * z.X * w.Y + x.Z * y.W * z.X * w.Y +
            x.W * y.X * z.Z * w.Y - x.X * y.W * z.Z * w.Y - x.Z * y.X * z.W * w.Y + x.X * y.Z * z.W * w.Y +
            x.W * y.Y * z.X * w.Z - x.Y * y.W * z.X * w.Z - x.W * y.X * z.Y * w.Z + x.X * y.W * z.Y * w.Z +
            x.Y * y.X * z.W * w.Z - x.X * y.Y * z.W * w.Z - x.Z * y.Y * z.X * w.W + x.Y * y.Z * z.X * w.W +
            x.Z * y.X * z.Y * w.W - x.X * y.Z * z.Y * w.W - x.Y * y.X * z.Z * w.W + x.X * y.Y * z.Z * w.w; 
            return value;
        }

        static Matrix4Base<T> Identity()
        {
            return Matrix4Base<T>();
        }

        VectorBase<T, 3> Translation() const
        {
            VectorBase<T, 4> tx = *this * VectorBase<T, 4>( 0, 0, 0, 1 );
            return VectorBase<T, 3>(tx.X,tx.Y,tx.Z);
        }

        Quaternion Rotation() const
        {
            float W;
            float X;
            float Y;
            float Z;

           // W = sqrt(1 + X.X )
            return Quaternion();
        }

        void Translate(T tX, T tY, T tZ)
        {
            w.X = tX;
            w.Y = tY;
            w.Z = tZ;
        }

        void Translate(VectorBase<T, 3> Vector)
        {
            Translate(Vector.X, Vector.Y, Vector.Z);
        }

        void Scale( T x, T y, T z )
        {
            x.X = x;
            y.Y = y; 
            z.Z = z;
        }


    
        Matrix4Base<T> operator * (const Matrix4Base<T>& b) const
        {
            Matrix4Base<T> m;
            m.x.X = x.X * b.x.X + x.Y * b.y.X + x.Z * b.z.X + x.W * b.w.x;
            m.x.Y = x.X * b.x.Y + x.Y * b.y.Y + x.Z * b.z.Y + x.W * b.w.y;
            m.x.Z = x.X * b.x.Z + x.Y * b.y.Z + x.Z * b.z.Z + x.W * b.w.z;
            m.x.W = x.X * b.x.W + x.Y * b.y.W + x.Z * b.z.W + x.W * b.w.w;
            m.y.X = y.X * b.x.X + y.Y * b.y.X + y.Z * b.z.X + y.W * b.w.x;
            m.y.Y = y.X * b.x.Y + y.Y * b.y.Y + y.Z * b.z.Y + y.W * b.w.y;
            m.y.Z = y.X * b.x.Z + y.Y * b.y.Z + y.Z * b.z.Z + y.W * b.w.z;
            m.y.W = y.X * b.x.W + y.Y * b.y.W + y.Z * b.z.W + y.W * b.w.w;
            m.z.X = z.X * b.x.X + z.Y * b.y.X + z.Z * b.z.X + z.W * b.w.x;
            m.z.Y = z.X * b.x.Y + z.Y * b.y.Y + z.Z * b.z.Y + z.W * b.w.y;
            m.z.Z = z.X * b.x.Z + z.Y * b.y.Z + z.Z * b.z.Z + z.W * b.w.z;
            m.z.W = z.X * b.x.W + z.Y * b.y.W + z.Z * b.z.W + z.W * b.w.w;
            m.w.X = w.X * b.x.X + w.Y * b.y.X + w.Z * b.z.X + w.W * b.w.x;
            m.w.Y = w.X * b.x.Y + w.Y * b.y.Y + w.Z * b.z.Y + w.W * b.w.y;
            m.w.Z = w.X * b.x.Z + w.Y * b.y.Z + w.Z * b.z.Z + w.W * b.w.z;
            m.w.W = w.X * b.x.W + w.Y * b.y.W + w.Z * b.z.W + w.W * b.w.w;
            return m;
        }

        VectorBase<T, 4> operator * (const VectorBase<T, 4>& b) const
        {
            VectorBase<T, 4> v;
		
            v.X = x.X * b.X + y.X * b.Y + z.X * b.Z + w.X * b.w;
            v.Y = x.Y * b.X + y.Y * b.Y + z.Y * b.Z + w.Y * b.w;
            v.Z = x.Z * b.X + y.Z * b.Y + z.Z * b.Z + w.Z * b.w;
            v.W = x.W * b.X + y.W * b.Y + z.W * b.Z + w.W * b.w;
		
            return v;
        }
        
        VectorBase<T, 4> x;
        VectorBase<T, 4> y;
        VectorBase<T, 4> z;
        VectorBase<T, 4> w;
    
    };
}
