#include "../headers/goppa.hpp"

GoppaCode::GoppaCode(type g, type p, uint64_t n): p(p), g(g), n(n) 
{
    m = high_bit(p) - 1;
    t = high_bit(g) - 1;
    k = n - m * t;
    d = 2 * t + 1;

    // H = Matrix(t, n);
    H = std::vector<std::vector<uint64_t>> (t, std::vector<uint64_t>(n));

    // G = std::vector<Long>(n, Long(k));
    G = std::vector<Long>(k, Long(n));

    L = std::vector<type> {4, 8, 3, 6, 12, 11, 5, 10, 7, 14, 15, 13};

    primitive = 8;

    for (type i = 1; i <= t; i++) {
        if ((g >> i) & 0x1) {
            g_degs.push_back(i);
        }
    }

    generate_H();
    calculate_G();
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


// type GoppaCode::get_random_alpha()
// {
//     return dist(rd);
// }


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

    return norm(sum); 
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

void GoppaCode::calculate_G() 
{
    auto num_equations = m * t;
    std::vector<Long> equations(num_equations, Long(n));
    
    uint64_t index = -1;
    uint64_t row_index = -1;
    for(auto& equation : equations) {
        index++;
        if (!(index % m)) {
            row_index++;
        }
        for (uint64_t element = 0; element < n; element++) {
            equation.push_back(H[row_index][element] >> (m - index % m - 1));
        }
    } 

    // приводимо систему рівнянь до трапецеїдального вигляду
    for (uint64_t i = 0; i < num_equations - 1; i++) {
        auto tmp = std::max_element(equations.begin() + i, equations.end(), [i, this](const Long& a, const Long& b) 
            {
                return a.hight_bit() < b.hight_bit();
            }
        );

        auto hight_bit = tmp->hight_bit();
        iter_swap(equations.begin() + i, tmp);

        for (auto itr = equations.begin() + i + 1; itr < equations.end(); itr++) {
            if (itr->hight_bit() == hight_bit) {
                *itr = *itr ^ *(equations.begin() + i);
            }
        }
    }

    std::vector<Long> G_T;
    std::vector<std::vector<uint64_t>> transpose_indexes;
    bool flag = false;
    auto tt = equations[0].get_len() - 1;
    do {
        G_T = std::vector<Long>(n, Long(k));
        // заповнення матриці G_T, яка є транспонованою мвтрицею G 
        for (auto itr = equations.end() - 1; itr >= equations.begin(); itr--) {
            auto indexes = itr->get_indexes();
            for (auto& ind : indexes) {
                ind = tt - ind;
            }
            
            // знаходимо індекси всіх незаповнених елементів матриці G
            std::vector<uint64_t> null_indexes;
            for (auto index : indexes) {
                if (G_T[index].is_null()) {
                    null_indexes.push_back(index);
                }
            }

            for (auto index : null_indexes) {
                Long tmp(k);
                do {
                    tmp.set_random_num();
                }while(tmp.is_null());

                G_T[index] = tmp;
            }

            // auto t = 0;
            // перший незаповнений елемент є сумою всіх інших елементів у строкі
            G_T[null_indexes[0]] = std::accumulate(ALL(indexes), Long(k), [this, &null_indexes, &G_T](auto a, auto b) 
                {   
                    if (null_indexes[0] == b) {
                        return a;
                    }
                    return a ^ G_T[b];
                }
            );                                                 

            // якщо сума всіх елементів, окрім першого незаповненого рівняється 0,
            // тоді має існувати, як мінімум два незаповнених елементи, у такому разі
            // достатньо перегенерувати другий незаповнений елемент, і перерахувати перший незаповнений елемент
            if (G_T[null_indexes[0]].is_null()) {
                Long tmp(k);
                do{
                    tmp.set_random_num();
                }
                while (G_T[null_indexes[1]] == tmp);
                G_T[null_indexes[0]] = G_T[null_indexes[1]] ^ tmp;
                G_T[null_indexes[1]] = tmp;
            }
        }


        ////
        ////
        ////
        auto num_rows = n;
        auto num_columns = k;
        std::vector<std::vector<uint64_t>> indexes;
        for (const auto& row : G_T) {
            indexes.push_back(row.get_indexes());
        }

        transpose_indexes = std::vector<std::vector<uint64_t>>(num_columns);
        auto tmp = num_rows - 1;
        auto tt = num_columns - 1;
        for (uint64_t i = 0; i < num_rows; i++){
            for (auto el : indexes[i]) {
                transpose_indexes[tt - el].push_back(tmp - i);
            }
        }

        flag = false;
        for (const auto& r : transpose_indexes) {
            if (r.size() == 0) {
                flag = true;
                break;
            }
        }
    }while(flag);

    // // auto row_indexes = transpose_indexes.begin();
    // auto g = G.begin();
    // for (const auto& row_indexes : transpose_indexes) {
    //     for (auto index : row_indexes) {
    //         g->raise_bit(index + 1);
    //     }
    //     ++g;
    // }
    G = transpose(G_T);
}


std::vector<Long>& GoppaCode::get_H()
{
    auto num_rows = m * t;
    std::vector<Long> *H_ = new std::vector<Long>(num_rows, Long(n));
    
    uint64_t index = -1;
    uint64_t row_index = -1;
    for(auto& row : *H_) {
        index++;
        if (!(index % m)) {
            row_index++;
        }
        for (uint64_t element = 0; element < n; element++) {
            row.push_back(H[row_index][element] >> (m - index % m - 1));
        }
    }
    return *H_;
}


void GoppaCode::test()
{
    // generate_H();
    // calculate_G();
    // auto t = H[0];
}