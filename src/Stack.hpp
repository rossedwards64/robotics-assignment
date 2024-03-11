#ifndef STACK_HPP_
#define STACK_HPP_

#include <stddef.h>
#include <stdlib.h>

template<typename T, size_t N>
class Stack
{
public:
    Stack() = default;

    void push(T datum);
    T pop();
    T peek();
    bool empty();
    size_t size();

private:
    ptrdiff_t sp_ = -1;
    size_t size_ = 0;
    T data_[N];
};

#endif
