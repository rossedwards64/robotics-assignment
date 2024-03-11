#include "Stack.hpp"

template<typename T, size_t N>
void Stack<T, N>::push(T datum)
{
    if (!(sp_ >= N - 1)) {
        data_[sp_] = datum;
        sp_++;
    }
}

template<typename T, size_t N>
T Stack<T, N>::pop()
{
    T datum = data_[sp_];
    sp_--;
    return datum;
}

template<typename T, size_t N>
T Stack<T, N>::peek()
{
    return data_[sp_];
}

template<typename T, size_t N>
bool Stack<T, N>::empty()
{
    return sp_ == -1;
}

template<typename T, size_t N>
size_t Stack<T, N>::size()
{
    return size_;
}
