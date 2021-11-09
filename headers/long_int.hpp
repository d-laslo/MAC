#pragma once
#include "defines.hpp"
#include <vector>
#include <random>

// using ln_t = __uint128_t;
using ln_t = uint8_t;

// спосіб представлення Litle Endian
class Long
{
private:
    // число яке зберігаємо
    std::vector<ln_t> number;

    // максимальна довжина числа в бітах
    uint64_t max_number_length;

    // кількість кластерів у числі
    uint64_t num_clusters;

    // // наявна довжина числа в бітах
    // __uint128_t length;

    // кількість зайнятих біт в останьому кластері
    uint64_t length_last_cluster;

    // максимальна кількість бітів яка поміститься в кластері
    uint64_t max_cluster_length;

    // вказує на кількість використовуваних кластерів
    uint64_t number_of_clusters_used;
    
public:
    Long(){};
    Long(__uint128_t max_len);
    Long(__uint128_t num, __uint128_t max_len);
    Long(const Long& other);


    Long& operator^(const Long& other) const;
    bool operator==(const Long& other) const;
    Long& operator=(const Long& number);
    bool operator>(const Long& other) const;
    Long& operator<<(uint64_t shift) const;
    Long& operator>>(uint64_t shift) const;
    // __uint128_t& operator[](uint64_t index);

    // добавляє біт в кінець послідовності
    void push_back(uint64_t val);

    // повертає номер найстаршого біта
    // відрахунок починається із сторони молодших бітів 
    // індексація починається з 1
    // повертає 0, якщо послідовність заповнена 0
    uint64_t hight_bit() const;

    // повертає індекси всіх не нульових бітів
    // відрахунок починається із сторони молодших бітів 
    // індексація починається з 0
    // повертає порожній масив, якщо послідовність заповнена 0 
    std::vector<uint64_t> get_indexes() const;

    // повертає true якщо усі біти 0
    // повертає false в іншому випадку
    bool is_null() const;

    // генерує випадковий бітовий вектор вказаної довжини
    // якщо довжина не вказана генерує вектор випадкової довжини не більшої ніж max_number_length
    // якщо параметер len більший ніж max_number_length, то len присвоюється значення max_number_length
    void set_random_num(uint64_t len = 0);

    uint64_t get_len() const {return max_number_length;}

    // встановлює index біт в одиницю
    // наймолодший біт знаходиться за індексом 1
    void raise_bit(uint64_t index);

    Long& mod_pol(const Long&& pol) const;

    __uint128_t get_last_byte() const;

    // Long div(const Long& divider);
    // Long invert_pol(const Long& pol);
    // Long& mul(const Long& multiplier2);
    // Long& norm(const Long& pol);
    // Long& mod(const Long& pol);


    void test();

private:
    // void check(const Long& other) const;

    // генерує випадковий вектор довжини не більшої ніж max_number_length
    void set_random();

    // зануляє n старших біт числа
    void null_the_bits(uint64_t n);
};

#include "source.hpp"