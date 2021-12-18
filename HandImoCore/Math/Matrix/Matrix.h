#pragma once

template<typename T>
class Matrix4Base
{
    

private:
    //mdata - Values in Matrix[][] format, data Uniform array type data data[]
    union
    {
        T m[4][4];
        T data[16];
    };
    
};
