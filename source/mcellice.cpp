#include "../headers/mcellice.hpp"

McEllice::McEllice(type _g, type _p, uint64_t _n)
{
    GoppaCode code(_g, _p , _n);

    k = code.get_k();

    n = code.get_n();
    
    t = code.get_t();

    g = code.get_g();

    p = code.get_p();

    primitive = code.get_primitive();

    L = code.get_L();

    G = code.get_G();

    H_T = transpose(code.get_H());

    
    P = std::vector<uint64_t>(n);

    inv_P = std::vector<uint64_t>(n);

    S = std::vector<Long>(k, Long(k)); 

    inv_S = std::vector<Long>(k, Long(k));

    hG = std::vector<Long>(G.size(), Long(G[0].get_len()));
}


void McEllice::generate_S()
{ 
    for (uint i = 0; i < k; i++) {
        S[i].set_random_num(k - i);
    } 

    // TODO перемішати вектори матриці S

    //** TODO зробити випадкове перемішування 
    // а не оце
    Long sum = std::accumulate(S.begin() + 1, S.end(), Long(S[0].get_len()), [](auto a, auto b) 
        {
            return a ^ b;
        }
    );

    for (uint64_t i = 1; i < S.size(); i++) {
        S[i] = S[0] ^ S[i];
    }
}

void McEllice::generate_P()
{ 
    uint64_t i = 0;
    for (auto& el : P) {
        el = i++;
    }
 
    std::random_device rd;
    std::mt19937 g(rd());
 
    std::shuffle(P.begin(), P.end(), g);
}


// void McEllice::calculate_transpose_G()
// {
//     auto num_rows = G.size();
//     auto num_columns = G[0].get_len();
//     std::vector<std::vector<uint64_t>> indexes;
//     for (const auto& row : G) {
//         indexes.push_back(row.get_indexes());
//     }

//     std::vector<std::vector<uint64_t>> transpose_indexes(num_columns);
//     auto tmp = num_rows - 1;
//     for (uint64_t i = 0; i < num_rows; i++){
//         for (auto el : indexes[i]) {
//             transpose_indexes[el].push_back(tmp - i);
//         }
//     }

//     // auto row_indexes = transpose_indexes.begin();
//     auto g_t = G_T.begin();
//     for (const auto& row_indexes : transpose_indexes) {
//         for (auto index : row_indexes) {
//             g_t->raise_bit(index + 1);
//         }
//         ++g_t;
//     }
// }


void McEllice::calculate_PK()
{
    // S = std::vector<Long>(4, Long(4));
    // S[0].raise_bit(4);
    // S[0].raise_bit(1);

    // S[1].raise_bit(3);
    // S[1].raise_bit(1);

    // S[2].raise_bit(3);

    // S[3].raise_bit(2);
    // S[3].raise_bit(1);


    // G = std::vector<Long>(4, Long(n));
    // G[0].raise_bit(11);
    // G[0].raise_bit(10);
    // G[0].raise_bit(8);
    // G[0].raise_bit(6);
    // G[0].raise_bit(3);

    // G[1].raise_bit(11);
    // G[1].raise_bit(10);
    // G[1].raise_bit(9);
    // G[1].raise_bit(8);
    // G[1].raise_bit(5);
    // G[1].raise_bit(4);

    // G[2].raise_bit(12);
    // G[2].raise_bit(11);
    // G[2].raise_bit(9);
    // G[2].raise_bit(8);
    // G[2].raise_bit(1);

    // G[3].raise_bit(12);
    // G[3].raise_bit(11);
    // G[3].raise_bit(10);
    // G[3].raise_bit(8);
    // G[3].raise_bit(7);
    // G[3].raise_bit(5);
    // G[3].raise_bit(2);








    //** множення матриці S на матрицю G
    hG = mul_matrix(S, G);
    // auto num_rows = publicKey.size();
    // auto num_columns = publicKey[0].get_len();

    // auto tmp = publicKey;
    // for (uint64_t row = 0; row < num_rows; row++) {
    //     for (const auto& r : G_T) {
    //         tmp[row].push_back( (S[row] ^ r).get_indexes().size() % 2 );
    //     }
    // } 

    // auto tmp = publicKey;
    // auto len = S.size() - 1;
    // uint64_t index = 0;

    // for (const auto& r : S) {
    //     auto indexes = r.get_indexes();
    //     auto& p = tmp[index];
    //     for (auto i : indexes) {
    //         p = p ^ G[len - i];
    //     }
    //     index++;
    // }

    // P = std::vector<uint64_t>(n);
    // P[0] = 0;
    // P[1] = 2;
    // P[2] = 8;
    // P[3] = 5;
    // P[4] = 4;
    // P[5] = 1;
    // P[6] = 3;
    // P[7] = 11;
    // P[8] = 7;
    // P[9] = 9;
    // P[10] = 10;
    // P[11] = 6;


    //** множення матриці SG на матрицю P
    hG = mul_matrix(hG, P);
    // for (uint64_t i = 0; i < P.size(); i++) {
    //     publicKey[P[i]] = tmp[i];
    // }
}


void McEllice::calculate_inv_P()
{
    for (uint64_t i = 0; i < n; i++) {
        inv_P[P[i]] = i;
    }
}


void McEllice::calculate_inv_S()
{
    /*
    * Обернена матриця шукається за допомогою метода Жордана-Гауса
    */

    std::vector<Long> tmp_inv = std::vector<Long> (S.size(), Long(S[0].get_len() * 2));
    auto len = S.size();
    for (uint64_t i = 0; i < len; i++) {
        tmp_inv[i] = S[i];
        tmp_inv[i].raise_bit(len-i);
    }

    // приводимо систему рівнянь до вигляду де перша частина матриці має трикутний вигляд
    for (uint64_t i = 0; i < len - 1; i++) {
        auto tmp = std::max_element(tmp_inv.begin() + i, tmp_inv.end(), [i, this](const Long& a, const Long& b) 
            {
                return a.hight_bit() < b.hight_bit();
            }
        );

        auto hight_bit = tmp->hight_bit();
        iter_swap(tmp_inv.begin() + i, tmp);

        for (auto itr = tmp_inv.begin() + i + 1; itr < tmp_inv.end(); itr++) {
            if (itr->hight_bit() == hight_bit) {
                *itr = *itr ^ *(tmp_inv.begin() + i);
            }
        }
    }

    // приводимо систему рівнянь до вигляду де перша частина матриці має вигляд одиничної матриці
    auto index = len - 2;
    for (uint64_t i = 1; i < len; i++) {
        auto indexes = tmp_inv[index].get_indexes();
        for (uint64_t j = 1; j < indexes.size(); j++) {
            if (indexes[j] < len) {
                break;
            }
            tmp_inv[index] = tmp_inv[index] ^ tmp_inv[2 * len - indexes[j] - 1];
        }
        index--;
    }

    // будуємо обернену матрицю по отриманій системі рівнянь
    for (uint64_t i = 0; i < len; i++) {
        auto indexes = tmp_inv[i].get_indexes();
        for (uint64_t j = 1; j < indexes.size(); j++) {
            inv_S[i].raise_bit(indexes[j] + 1);
        }
    }
}


std::vector<Long>& McEllice::mul_matrix(const std::vector<Long>& first, const std::vector<Long>& second)
{
    std::vector<Long> *tmp = new std::vector<Long>(first.size(), Long(second[0].get_len()));
    auto len = second.size() - 1;
    uint64_t index = 0;

    for (const auto& r : first) {
        auto indexes = r.get_indexes();
        auto& p = (*tmp)[index];
        for (auto i : indexes) {
            p = p ^ second[len - i];
        }
        index++;
    }
    return *tmp;
}


std::vector<Long>& McEllice::mul_matrix(const std::vector<Long>& first, const std::vector<uint64_t>& second)
{
    auto tr_matrix = transpose(first);
    std::vector<Long> *new_matrix = new std::vector<Long> (tr_matrix.size(), tr_matrix[0].get_len());
    for (uint64_t i = 0; i < second.size(); i++) {
        (*new_matrix)[second[i]] = tr_matrix[i];
    }
    return transpose(*new_matrix);
}


// std::vector<Long>& McEllice::transpose(const std::vector<Long> m) 
// {
//     auto num_rows = m.size();
//     auto num_columns = m[0].get_len();
//     std::vector<std::vector<uint64_t>> indexes;
//     for (const auto& row : m) {
//         indexes.push_back(row.get_indexes());
//     }

//     std::vector<std::vector<uint64_t>> transpose_indexes(num_columns);
//     auto tmp = num_rows - 1;
//     auto tt = num_columns - 1;
//     for (uint64_t i = 0; i < num_rows; i++){
//         for (auto el : indexes[i]) {
//             transpose_indexes[tt - el].push_back(tmp - i);
//         }
//     }

//     // auto row_indexes = transpose_indexes.begin();
//     auto* G_T = new std::vector<Long>(num_columns, Long(num_rows));
//     auto g_t = (*G_T).begin();
//     for (const auto& row_indexes : transpose_indexes) {
//         for (auto index : row_indexes) {
//             g_t->raise_bit(index + 1);
//         }
//         ++g_t;
//     }

//     return *G_T;
// }


std::vector<Long>& McEllice::encrypt(const Long& data)
{
    std::vector<uint64_t> sequence;
    for (uint64_t i = 0; i < n; i++) {
        sequence.push_back(i + 1);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(sequence.begin(), sequence.end(), g);


    //! 
    // sequence[0] = 12;
    // sequence[1] = 11;

    auto tmp_data = std::vector<Long>(1, data);
    Long tmp_rd_vec (n);
    for (uint64_t i = 0; i < t; i++) {
        tmp_rd_vec.raise_bit(sequence[i]);
    }

    // auto tmp = new std::vector<Long> (1, Long(data.get_len()));

    auto tmp = mul_matrix(tmp_data, hG);
    auto *ret =  new std::vector<Long> (1, n);
    ret[0][0] = tmp[0] ^ tmp_rd_vec;
    return *ret;
}

std::vector<Long>& McEllice::decrypt(const Long& data)
{
    auto tmp_data = std::vector<Long>(1, data);
    calculate_inv_P();
    auto c = mul_matrix(tmp_data, inv_P);

    // auto l = Long(15, 3);

    auto e = error_correction(c[0]);

    // auto err = mul_matrix(c, H_T);



    auto conc = concatenation(transpose(G), transpose(c));
    // ромбовидна матриця
    for (uint64_t i = 0; i < conc.size() - 1; i++) {
        auto tmp = std::max_element(conc.begin() + i, conc.end(), [i, this](const Long& a, const Long& b) 
            {
                return a.hight_bit() < b.hight_bit();
            }
        );

        auto hight_bit = tmp->hight_bit();
        iter_swap(conc.begin() + i, tmp);

        for (auto itr = conc.begin() + i + 1; itr < conc.end(); itr++) {
            if (itr->hight_bit() == hight_bit) {
                *itr = *itr ^ *(conc.begin() + i);
            }
        }
    }

    // calculate_inv_S();
    // auto err = mul_matrix(c, inv_S);
    // auto tt = err[0].get_indexes();
    
    return c;
}


Long& McEllice::error_correction(const Long& msg)
{
    auto vmsg = std::vector<Long> (1, msg);
    syndrome(msg);
    // auto St = mul_matrix(vmsg, H_T)[0];
    // auto Stt = mul_matrix(vmsg, H_T)[0].mod_pol(Long(g, k));
    // auto S = mul_matrix(vmsg, H_T)[0].mod_pol(Long(g, k)).get_last_byte();
    // auto h = invert(S);
    auto tt = 0;
}


Long& McEllice::syndrome(const Long& msg) 
{
    auto indx = msg.get_indexes();
    std::reverse(ALL(indx));

    auto pol = std::vector<__uint128_t> (high_bit(g));
    pol[0] = primitive;
    for (uint64_t i = 1; i < pol.size(); i++) {
        if ((g >> i) & 1) {
            pol[i] = 1;
        }
    }
    std::reverse(ALL(pol));

    // auto gg = std::vector<__uint128_t> {3, 5, 0, 1, 3};
    // auto a = std::vector<__uint128_t> {5, 1};
    // auto t = div(gg, a);


    // auto tg = mul(t[0], a);

    auto inverse = [this](const std::vector<__uint128_t>& element, const std::vector<__uint128_t>& pol) 
    {
        std::vector<std::vector<__uint128_t>> v{std::vector<__uint128_t>(1, 0), std::vector<__uint128_t>(1, 1)};
        std::vector<__uint128_t> rem;
        std::vector<__uint128_t> value  = element;
        auto a = pol;

        do {
            auto t = div(a, value);
            auto tmp = t[0];
            rem = t[1];

            auto tmp_mul = mul(v[1], tmp);
            v[0].resize(tmp_mul.size());
            for (auto i = 0; i < v[0].size(); i++) {
                v[0][i] ^= tmp_mul[i];
            }
            std::swap(v[0], v[1]);
            a = value;
            value = rem;
        } while(rem.size() > 1);

        auto inv = invert(rem[0], p);
        for (auto& i :v[1]) {
            i = norm(mul(inv, i), p);
        }
        return v[1];
    };



    // auto R = std::vector<__uint128_t> {1, 2, 4, 8, 10, 7, 13, 9};
    auto indx = msg.get_indexes();
    std::vector<__uint128_t> R;
    for (auto i : indx) {
        R.push_back(L[i]);
    }

    auto S_error = std::vector<__uint128_t> (high_bit(g) - 1);
    for (auto r: R) {
        auto inv = inverse(std::vector<__uint128_t> {1, r}, pol);
        std::reverse(ALL(inv));
        for (auto i = 0; i < inv.size(); i++){
            S_error[i] ^= inv[i];
        }
    }
    std::reverse(ALL(S_error));

    auto T_error = inverse(S_error, pol);

    /*
    * Зробити пошук квадратного корення
    */



   auto mini_euclid = [&]() 
   {
       std::vector<std::vector<__uint128_t>> v{std::vector<__uint128_t>(1, 0), std::vector<__uint128_t>(1, 1)};
       
   };
}


__uint128_t McEllice::div(__uint128_t divided, __uint128_t divider)
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


std::vector<std::vector<__uint128_t>> McEllice::div(std::vector<__uint128_t> divided, std::vector<__uint128_t> divider)
{
    if (divider.size() > divided.size()) {
        throw -1;
    }
    std::vector<__uint128_t> quotient(divided.size());

    uint64_t divider_l = divider.size();

    auto inv = invert(divider[0], p);
    for (auto& i : divider) {
        i = norm(mul(i, inv), p);
    }
    while (divided.size() >= divider_l) {
        uint64_t divided_l = divided.size();

        auto mult = divided[0];
        quotient[divided_l - divider_l] = norm(mul(mult, inv), p);

        auto tmp = divider;
        for (auto& i : tmp ) {
            i = norm(mul(i, mult), p);
        }
        tmp.resize(tmp.size() + divided_l - divider_l);
        for (uint64_t i = 0; i < divided_l; i++) {
            divided[i] ^= tmp[i];
        };

        auto const first_non_zero = std::find_if(ALL(divided), [](__uint128_t i){ return i != 0; });
        divided.erase(divided.begin(), first_non_zero);
    }

    if (divided.size() == 0) {
        divided = std::vector<__uint128_t> (1, 0); 
    }

    std::reverse(ALL(quotient));
    auto const first_non_zero = std::find_if(ALL(quotient), [](__uint128_t i){ return i != 0; });
    quotient.erase(quotient.begin(), first_non_zero);
    return std::vector<std::vector<__uint128_t>> {quotient, divided};
}


__uint128_t McEllice::mod(__uint128_t base, __uint128_t val)
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


__uint128_t McEllice::norm(__uint128_t val, __uint128_t module) {
    auto p_l = high_bit(module);
    auto val_l = high_bit(val);
    while( val_l >= p_l ){
        val ^= module << (val_l - p_l);
        val_l = high_bit(val);
    }

    return val;
}


__uint128_t McEllice::invert(__uint128_t value, __uint128_t module)
{
    std::array<uint64_t, 2> v{0, 1};
    __uint128_t rem = 0;
    __uint128_t a = module;
    do {
        auto tmp = div(a, value);
        v[0] ^= norm(mul(v[1], tmp), module);
        std::swap(v[0], v[1]);
        rem = mod(a, value);
        a = value;
        value = rem;
    }
    while(rem);

    return v[0];
}


__uint128_t McEllice::mul(__uint128_t multiplier1, __uint128_t multiplier2)
{
    uint64_t end = high_bit(multiplier2);
    __uint128_t product = 0;
    for (uint64_t i = 0; i < end; i++) {
        product ^= ((multiplier2 >> i) & 0x1) ? (multiplier1 << i) : 0; 
    }
    return product ; //norm(product);
}


std::vector<__uint128_t> McEllice::mul(const std::vector<__uint128_t>& multiplier1_, const std::vector<__uint128_t>& multiplier2_)
{

    auto multiplier1 = multiplier1_;
    auto multiplier2 = multiplier2_;

    if (multiplier1.size() > multiplier2.size()) {
        std::swap(multiplier1, multiplier2);
    }

    auto mult1_s = multiplier1.size();
    auto mult2_s = multiplier2.size();
    auto res = std::vector<__uint128_t> (mult1_s + mult2_s - 1);
    for (auto i = 0; i < mult2_s; i++) {
        auto tmp = multiplier1;
        for (auto& j : tmp) {
            j = norm(mul(j, multiplier2[i]), p);
        }

        tmp.resize(mult1_s + mult2_s - i -1);
        
        std::reverse(ALL(tmp));

        for (auto j = 0; j < tmp.size(); j++) {
            res[j] ^= tmp[j];
        }
        
    }

    // auto const first_non_zero = std::find_if(ALL(res), [](__uint128_t i){ return i != 0; });
    // res.erase(res.begin(), first_non_zero);
    std::reverse(ALL(res));
    return res;
};


void McEllice::test() 
{

    generate_S();
    generate_P();
    calculate_inv_S();
    calculate_inv_P();
    calculate_PK();

    Long data(k);
    data.raise_bit(4);
    // data.raise_bit(3);
    data.raise_bit(2);
    // data.raise_bit(1);
    auto e_data = encrypt(data);
    auto d_data = decrypt(e_data[0]);
    auto t = 0;
}