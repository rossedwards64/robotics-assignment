#ifndef STACK_HPP_
#define STACK_HPP_

#include <stddef.h>
#include <stdlib.h>


template<typename T, size_t N>
class Stack
{
public:
    Stack() = default;

    void push(const T &datum)
    {
        if (!full()) {
            // initialise the stack pointer if it hasn't been already
            // if it has, move the pointer to the top
            !stack_ptr_ ? stack_ptr_ = data_ : stack_ptr_++;
            *stack_ptr_ = datum;
            size_++;
        }
    }

    T pop()
    {
        T datum = *stack_ptr_;
        *stack_ptr_ = T{};// clear the top value before moving the stack pointer
        stack_ptr_--;
        size_--;
        return datum;
    }

    const T &peek() const { return *stack_ptr_; }

    bool empty() const { return size_ == 0; }

    size_t size() const { return size_; }

private:
    bool full() const { return size_ > (N - 1); }

    T data_[N]{};
    T *stack_ptr_{ nullptr };
    size_t size_{ 0 };
};

#endif
