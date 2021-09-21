#include "../headers/goppa.hpp"

GoppaCode::GoppaCode(type g, type p, uint64_t n): p(p), g(g), n(n) 
{
    m = high_bit(p) - 1;
    t = high_bit(g) - 1;
    k = n - m * t;
    d = 2 * t + 1;

    H = new Matrix(t, n);

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
    return product;
}


type GoppaCode::pow(type base, type exp)
{
    type res = 1;
    while (exp > 0)
    {
        if (exp & 0x1) {
            res = mod(mul(res, base), p);
        } 
        base = mod(mul(base, base), p);

        exp >>= 1;
    }
        
    return res;
}


type GoppaCode::mod(type base, type val)
{
    auto val_l = high_bit(val);
    auto base_l = high_bit(base);
    while (base_l >= val_l) {
        base ^= val << (base_l - val_l);
        base_l = high_bit(base);
    }

    return base;
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
        //rem = a ^ mul(tmp, value);
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
            return a ^ this->pow(x, b); // a ^ (x << b);
        }
    );
    sum ^= primitive;

    return mod(sum, p);
}

void GoppaCode::test()
{
    auto tmp = calc_g(12);
}