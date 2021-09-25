#pragma once
#include "defines.hpp"
#include "source.hpp"
#include <vector>

// спосіб представлення Litle Endian
class Long
{
private:
    // число яке зберігаємо
    std::vector<__uint128_t> number;

    // максимальна довжина числа в бітах
    __uint128_t max_number_length;

    // наявна довжина числа в бітах
    __uint128_t length;

    // кількість зайнятих біт в останьому кластері
    uint64_t length_last_cluster;

    // максимальна кількість бітів яка поміститься в кластері
    uint64_t max_cluster_length;

    // вказує на кількість використовуваних кластерів
    uint64_t number_of_clusters_used;
    
public:
    Long();
    Long(__uint128_t max_len);
    Long(const Long& other);


    Long& operator^(const Long& other) const;
    // __uint128_t& operator[](uint64_t index);

    // добавляє біт в кінець послідовності
    void push_back(uint64_t val);

    // повертає індек найстаршого біта
    // відрахунок починається із сторони молодших бітів 
    // індексація починається з 1
    // повертає 0, якщо послідовність заповнена 0
    uint64_t hight_bit() const;


    // Long& operator<<(__uint128_t shift) const;
    // Long& operator>>(__uint128_t shift) const;

private:
    void check(const Long& other) const;
};