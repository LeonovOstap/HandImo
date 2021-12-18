#pragma once
#include <cmath>
#include <cstring>
#include <type_traits>


namespace MathImpl
{
    template<typename T, size_t N>
    struct VectorBase
    {
    public:
        static_assert(std::is_arithmetic<T>(), "Invalid Vector type, only numeric values are supported");
        static_assert(N > 1, "Invalid Vector size, minimum is 2");

        
        VectorBase()
            : ContainerSize(N)
        {}
    
        template<typename ... Args>
        VectorBase(Args ... args) : VectorBase()
        {
            const T Buffer[N] = {args...};
            memcpy(mdata, Buffer, sizeof(Buffer));
        }

        //Construct vector from C-Style array
        VectorBase(T* DataPtr) : VectorBase()
        {
            *this = DataPtr;
        }
    

        //Vector Ops
        T Dot(const VectorBase<T, N>& Other) const
        {
            T DotProduct = 0;
            for(int i = 0; i < ContainerSize; ++i)
            {
                DotProduct+=mdata[i]*Other.mdata[i];
            }
            return DotProduct;
        }

        T Length() const
        {
            return sqrt(LengthSquared());
        }

        T LengthSquared() const
        {
            T len = 0;
            for (int i = 0; i < ContainerSize; ++i) {
                len += mdata[i] * mdata[i];
            }
            return len;
        }
        
        VectorBase<T, N> GetNormal() const
        {
            return operator/(Length());
        }


        bool Equals(const VectorBase<T, N>& Other) const
        {
            return memcmp(mdata,Other.mdata,sizeof(mdata)) == 0;
        }
        

        //Operators

        VectorBase<T, N>& operator-()
        {
            for(int i = 0; i < ContainerSize; ++i)
            {
                mdata[i] = -abs(mdata[i]);
            }
            return *this;
        }
        
        VectorBase<T, N>& operator=(const VectorBase<T, N>& Other)
        {
           *this = Other.mdata;
            return *this;
        }

        VectorBase<T, N>& operator=(T* Other)
        {
            for(int i = 0; i < sizeof(Other)/sizeof(*Other) ; ++i)
            {
                mdata[i] = Other[i];
            }
            return *this;
        }
    

        VectorBase<T, N>& operator+=(const VectorBase<T,N>& Other)
        {
            for(int i = 0; i < Other.ContainerSize; ++i)
            {
                mdata[i] += Other.mdata[i];
            }
            return *this;
        }

        VectorBase<T, N>& operator-=(const VectorBase<T,N>& Other)
        {
            for(int i = 0; i < Other.ContainerSize; ++i)
            {
                mdata[i] -= Other.mdata[i];
            }
            return *this;
        }

        VectorBase<T, N>& operator*=(const VectorBase<T,N>& Other)
        {
            for(int i = 0; i < Other.ContainerSize; ++i)
            {
                mdata[i] *= Other.mdata[i];
            }
            return *this;
        }

        VectorBase<T, N>& operator/=(const VectorBase<T,N>& Other)
        {
            for(int i = 0; i < Other.ContainerSize; ++i)
            {
                mdata[i] /= Other.mdata[i];
            }
            return *this;
        }

        VectorBase<T, N> operator+(const VectorBase<T,N>& Other)
        {
            auto Copy = *this;
            for(int i = 0; i < Other.ContainerSize; ++i)
            {
                Copy.mdata[i] += Other.mdata[i];
            }
            return Copy;
        }

        VectorBase<T, N> operator-(const VectorBase<T,N>& Other)
        {
            auto Copy = *this;
            for(int i = 0; i < Other.ContainerSize; ++i)
            {
                Copy.mdata[i] -= Other.mdata[i];
            }
            return Copy;
        }

        VectorBase<T, N> operator*(const VectorBase<T,N>& Other)
        {
            auto Copy = *this;
            for(int i = 0; i < Other.ContainerSize; ++i)
            {
                Copy.mdata[i] *= Other.mdata[i];
            }
            return Copy;
        }

        VectorBase<T, N> operator/(const VectorBase<T,N>& Other)
        {
            auto Copy = *this;
            for(int i = 0; i < Other.ContainerSize; ++i)
            {
                Copy.mdata[i] /= Other.mdata[i];
            }
            return Copy;
        }

        VectorBase<T, N>& operator+=(T Bias)
        {
            for(int i = 0; i < ContainerSize; ++i)
            {
                mdata[i] += Bias;
            }
            return *this;
        }

        VectorBase<T, N>& operator-=(T Bias)
        {
            for(int i = 0; i < ContainerSize; ++i)
            {
                mdata[i] -= Bias;
            }
            return *this;
        }

        VectorBase<T, N>& operator*=(T Bias)
        {
            for(int i = 0; i < ContainerSize; ++i)
            {
                mdata[i] *= Bias;
            }
            return *this;
        }

        VectorBase<T, N>& operator/=(T Bias)
        {
            for(int i = 0; i < ContainerSize; ++i)
            {
                mdata[i] /= Bias;
            }
            return *this;
        }

        VectorBase<T, N> operator+(T Bias)
        {
            auto Copy = *this;
            for(int i = 0; i < ContainerSize; ++i)
            {
                Copy.mdata[i] += Bias;
            }
            return Copy;
        }

        VectorBase<T, N> operator-(T Bias)
        {
            auto Copy = *this;
            for(int i = 0; i < ContainerSize; ++i)
            {
                Copy.mdata[i] -= Bias;
            }
            return Copy;
        }

        VectorBase<T, N> operator*(T Bias)
        {
            auto Copy = *this;
            for(int i = 0; i < ContainerSize; ++i)
            {
                Copy.mdata[i] *= Bias;
            }
            return Copy;
        }

        VectorBase<T, N> operator/(T Bias)
        {
            VectorBase<T, N> Copy = *this;
            for(int i = 0; i < ContainerSize; ++i)
            {
                Copy.mdata[i] /= Bias;
            }
            return Copy;
        }

        bool operator==(const VectorBase<T, N>& Other) const
        {
            return Equals(Other);
        }

        bool operator!=(const VectorBase<T, N>& Other) const
        {
            return !Equals(Other);
        }

        T& operator[](const int& Index)
        {
            //static_assert(Index > ContainerSize, "Out Of bounds");
            return mdata[Index];
        }

    public:
        union
        {
            T mdata[N];

            //todo:: this is bad, reimplement properly
            struct 
            {
                T X;
                T Y;
                T Z;
                T W;
            };
        };
    
    private:
        size_t ContainerSize;
    };
}


