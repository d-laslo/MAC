#pragma once
#include "goppa_code_parameters.hpp"
#include "matrix.hpp"
#include "source.hpp"
#include "long_int.hpp"
#include <array>
#include <cmath>


using type = uint64_t;
// using type = Long;

class GoppaCode
{
private:
    // незвідний поліном
    type p;

    // примітивний елемент
    type primitive;

    // поліном Гоппа
    type g;

    // не нульові степені полінома Гоппа
    std::vector<type> g_degs;

    //множина елементів із поля GF(q)
    std::vector<type> L;

    // Перевірочна матриця
    Matrix H;

    // Породжуюча матриця
    std::vector<Long> G;

    // параметри кода [n,k,d] = [n, n - mt, 2t + 1]
    uint64_t n;
    uint64_t k;
    uint64_t d;
    uint64_t m; // q = 2^m
    uint64_t t; // deg(g)

    
public: 
    GoppaCode();
    GoppaCode(type g, type p, uint64_t n);

    std::vector<Long>& get_G() {return G;}
    uint64_t get_n() {return n;}
    uint64_t get_k() {return k;}
    uint64_t get_d() {return d;}
    uint64_t get_m() {return m;}
    uint64_t get_t() {return t;}

    // TODO метод для тестування приватних методів 
    // потім видалити
    void test();

private:
    type invert(type value);
    type mul(type multiplier1, type multiplier2);
    type mod(type base, type val);
    type div(type divided, type divider);
    type pow(type base, type exp);

    type get_random_alpha();

    // нормує елемент в полі GF(q)
    type norm(type val);

    // обчислює g(x)
    type calc_g(type x);

    void generate_H();
    void calculate_G();
};