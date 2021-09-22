#include "../headers/goppa.hpp"

GoppaCode::GoppaCode(type g, type p,uint64_t n): p(p), g(g), n(n) 
{
    m = high_bit(p) - 1;
    t = high_bit(g) - 1;
    k = n - m * t;
    d = 2 * t + 1;

    // TODO добавити присвоєння через переміщення
    H = Matrix(t, n);

    L = std::vector<type> {4, 8, 3, 6, 12, 11, 5, 10, 7, 14, 15, 13};

    primitive = 8;

    for (type i = 1; i <= t; i++) {
        if ((g >> i) & 0x1) {
            g_degs.push_back(i);
        }
    }
}


type GoppaCode::div(type divided, type divider)
{
    if (divider >= divided) {
        throw -1;
    }
    uint64_t quotient = 0;
    while (divided > divider) {
        uint64_t divided_l = high_bit(divided);
        uint64_t divider_l = high_bit(divider);

        quotient ^= (1 << (divided_l - divider_l));
        divided ^= (divider << (divided_l - divider_l));
    }

    return quotient;
}


type GoppaCode::mul(type multiplier1, type multiplier2)
{
    uint64_t end = high_bit(multiplier2);
    type product = 0;
    for (uint64_t i = 0; i < end; i++) {
        product ^= ((multiplier2 >> i) & 0x1) ? (multiplier1 << i) : 0; 
    }
    return norm(product);
}


type GoppaCode::pow(type base, type exp)
{
    type res = 1;
    while (exp > 0)
    {
        if (exp & 0x1) {
            res = mul(res, base);
        } 
        base = mul(base, base);

        exp >>= 1;
    }
        
    return res;
}


type GoppaCode::mod(type base, type val)
{
    if (val == 0) {
        throw -1;
    }
    auto val_l = high_bit(val);
    while( base >= val ){
        base ^= val << (high_bit(base) - val_l);
    }

    return base;
}


type GoppaCode::norm(type val) {
    auto p_l = high_bit(p);
    auto val_l = high_bit(val);
    while( val_l >= p_l ){
        val ^= p << (val_l - p_l);
        val_l = high_bit(val);
    }

    return val;
}


type GoppaCode::invert(type value)
{
    std::array<uint64_t, 2> v{0, 1};
    type rem = 0;
    type a = p;
    do {
        auto tmp = div(a, value);
        v[0] ^= mul(v[1], tmp);
        std::swap(v[0], v[1]);
        rem = mod(a, value);
        a = value;
        value = rem;
    }
    while(rem);

    return v[0];
}


type GoppaCode::calc_g(type x)
{
    type sum = std::accumulate(ALL(g_degs), 0, 
        [x, this](auto a, auto b) 
        {
            return a ^ this->pow(x, b);
        }
    );
    sum ^= primitive;

    return norm(sum); ///////////////////////////////////////////
}

void GoppaCode::generate_H() 
{
    std::vector<type> invert_g;
    for (auto alpha: L) {
        invert_g.push_back(invert(calc_g(alpha)));
    }

    std::vector<type> alpha(n, 1);

    for (auto& row : H) {
        for (uint64_t i = 0; i < n; i++) {
            row[i] = mul(alpha[i], invert_g[i]);
            alpha[i] = mul(alpha[i], L[i]);
        }
    }
}

void GoppaCode::test()
{
    generate_H();
    auto tmp2 = pow(2, 1);
    auto tmp4 = pow(2, 2);
    auto tmp8 = pow(2, 3);
    auto tmp3 = pow(2, 4);
    auto tmp6 = pow(2, 5);
    auto tmp12 = pow(2, 6);
    auto tmp11 = pow(2, 7);
}