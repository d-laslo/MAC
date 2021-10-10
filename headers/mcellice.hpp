#pragma once 
#include <vector>
#include "defines.hpp"
#include "long_int.hpp"
#include "goppa.hpp"


class McEllice
{
private:
    // перевірочна матриця Гоппа кода
    std::vector<Long> G;

    // транспонована перевірочна матриця Гоппа кода
    std::vector<Long> G_T;

    // матриця перестановки
    std::vector<uint64_t> P;

    // inv_P = P^{-1}
    std::vector<uint64_t> inv_P;


    // випадкова невироджена матриця
    std::vector<Long> S;

    // inv_S = S^{-1}
    std::vector<Long> inv_S;

    std::vector<Long> publicKey;

    // 
    uint64_t n;

    //
    uint64_t k;

public:
    McEllice(type g, type p, uint64_t n);

    // TODO метод для тестування приватних методів 
    // потім видалити
    void test();

private:
    void generate_P();
    void calculate_inv_P();
    void generate_S();
    void calculate_inv_S();
    void calculate_transpose_G();

    // обрахувати выдкритий ключ
    void calculate_PK();
};