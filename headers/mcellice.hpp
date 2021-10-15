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

public:
    McEllice(type g, type p, uint64_t n);

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
    // void calculate_transpose_G();
    std::vector<Long>& transpose(const std::vector<Long> m);

    std::vector<Long>& mul_matrix(const std::vector<Long>& first, const std::vector<Long>& second);
    std::vector<Long>& mul_matrix(const std::vector<Long>& first, const std::vector<uint64_t>& second);

    // обрахувати выдкритий ключ
    void calculate_PK();
};