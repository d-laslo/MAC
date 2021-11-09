#pragma once 
#include <vector>
#include "defines.hpp"
#include "long_int.hpp"
#include "goppa.hpp"


class McEllice
{
private:
    // генеруюча матриця Гоппа кода
    std::vector<Long> G;

    // перевірочна матриця Гоппа кода
    std::vector<Long> H_T;

    // // транспонована перевірочна матриця Гоппа кода
    // std::vector<Long> G_T;

    // матриця перестановки
    std::vector<uint64_t> P;

    // inv_P = P^{-1}
    std::vector<uint64_t> inv_P;


    // випадкова невироджена матриця
    std::vector<Long> S;

    // inv_S = S^{-1}
    std::vector<Long> inv_S;

    // hG = SGP
    std::vector<Long> hG;

    // 
    uint64_t n;

    //
    uint64_t k;

    uint64_t t;

    __uint128_t g;
    __uint128_t p;
    __uint128_t primitive;
    std::vector<__uint128_t> L;

public:
    McEllice(type g, type p, uint64_t n);
    std::vector<Long>& encrypt(const Long& data);
    std::vector<Long>& decrypt(const Long& data);
    void savePrivateKey(std::string path);
    void savePublickKey(std::string path);
    void loadPrivateKey(std::string path);
    void loadPublickKey(std::string path);


    // TODO метод для тестування приватних методів 
    // потім видалити
    void test();

private:
    void generate_P();
    void calculate_inv_P();
    void generate_S();
    void calculate_inv_S();
    Long mod_pol(const Long& val, const Long& pol);
    Long& error_correction(const Long& msg);
    Long& syndrome(const Long& msg);
    // void calculate_transpose_G();
    // std::vector<Long>& transpose(const std::vector<Long> m);

    std::vector<Long>& mul_matrix(const std::vector<Long>& first, const std::vector<Long>& second);
    std::vector<Long>& mul_matrix(const std::vector<Long>& first, const std::vector<uint64_t>& second);

    // обрахувати выдкритий ключ
    void calculate_PK();


    __uint128_t mul(__uint128_t multiplier1, __uint128_t multiplier2);
    std::vector<__uint128_t> mul(const std::vector<__uint128_t>& multiplier1, const std::vector<__uint128_t>& multiplier2);
    __uint128_t invert(__uint128_t value, __uint128_t module);
    __uint128_t norm(__uint128_t val, __uint128_t module);
    __uint128_t mod(__uint128_t base, __uint128_t val);
    __uint128_t div(__uint128_t divided, __uint128_t divider);
    std::vector<std::vector<__uint128_t>> div(std::vector<__uint128_t> divided, const std::vector<__uint128_t> divider);
};