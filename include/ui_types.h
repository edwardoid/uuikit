#pragma once

#include <stdint.h>
#include <string>

typedef int16_t pos_t;

// Single dimentions, can't be negative
typedef int16_t dim_t;

// Represents 1 byte(8 bits)
typedef unsigned char byte_t;

typedef uint8_t** font_t;

typedef font_t* font_ref_t;

enum text_size_t: uint8_t
{
    small = 0,
    medium,
    huge
};

enum class user_input_t: uint8_t
{
           num_0 = 0,
    num_1, num_2, num_3,
    num_4, num_5, num_6,
    num_7, num_8, num_9,

    key_a, key_b, key_c,
    key_d, key_e, key_f,
    key_g, key_h, key_i,
    key_j, key_k, key_l,
    key_m, key_n, key_o,
    key_p, key_q, key_r,
    key_s, key_t, key_u,
    key_v, key_w, key_x,
    key_y, key_z,

    key_dot /* . */, key_at /* @ */,

    key_released,
    key_pressed,
    key_long_pressed,
    key_increment,
    key_decrement
};

template<typename T, uint8_t SizeValue>
class FixedSizeArray
{
public:
    FixedSizeArray() = default;
    inline const T& operator[](uint8_t i) const { return m_data[i]; }
    inline T& operator[](uint8_t i) { return m_data[i]; }
    int8_t indexOf(const T& e) const
    {
        for(uint8_t i = 0; i < SizeValue; ++i)
        { if (m_data[i] == e) return i; }
        return -1;
    }
private:
    T m_data[SizeValue];
};

template<typename T, uint8_t SizeValue>
class Queue
{
public:
    Queue() = default;

    void push(const T& element)
    {
        // We are full
        if (m_back == SizeValue - 1) return;
        if (m_front == -1) m_front = 0;
        m_data[++m_back] = element;
        ++m_size;
    }

    void pop()
    {
        if (m_front == -1 || m_front < m_back) return;
        m_front++;
    }

    inline const T& top() const { return m_data[m_front]; }
    T& top() { return m_data[m_front]; }
    inline uint8_t size() const { return m_size; }
    inline bool empty() const { return size() == 0; }
    inline bool full() const { return m_back = -1; }
private:
    uint8_t m_front = -1;
    uint8_t m_back = -1;
    uint8_t m_size = 0;
    FixedSizeArray<T, SizeValue> m_data;
};