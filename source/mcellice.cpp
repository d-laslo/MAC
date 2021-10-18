#include "../headers/mcellice.hpp"
#include <iostream>

McEllice::McEllice(type g, type p, uint64_t _n)
{
    GoppaCode code(g, p , _n);

    k = code.get_k();

    n = code.get_n();
    
    t = code.get_t();

    G = code.get_G();

    H_T = transpose(code.get_H());

    // auto tt = mul_matrix(G, H_T);
    
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
    S = std::vector<Long>(4, Long(4));
    S[0].raise_bit(4);
    S[0].raise_bit(1);

    S[1].raise_bit(3);
    S[1].raise_bit(1);

    S[2].raise_bit(3);

    S[3].raise_bit(2);
    S[3].raise_bit(1);


    G = std::vector<Long>(4, Long(n));
    G[0].raise_bit(11);
    G[0].raise_bit(10);
    G[0].raise_bit(8);
    G[0].raise_bit(6);
    G[0].raise_bit(3);

    G[1].raise_bit(11);
    G[1].raise_bit(10);
    G[1].raise_bit(9);
    G[1].raise_bit(8);
    G[1].raise_bit(5);
    G[1].raise_bit(4);

    G[2].raise_bit(12);
    G[2].raise_bit(11);
    G[2].raise_bit(9);
    G[2].raise_bit(8);
    G[2].raise_bit(1);

    G[3].raise_bit(12);
    G[3].raise_bit(11);
    G[3].raise_bit(10);
    G[3].raise_bit(8);
    G[3].raise_bit(7);
    G[3].raise_bit(5);
    G[3].raise_bit(2);








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

    P = std::vector<uint64_t>(n);
    P[0] = 0;
    P[1] = 2;
    P[2] = 8;
    P[3] = 5;
    P[4] = 4;
    P[5] = 1;
    P[6] = 3;
    P[7] = 11;
    P[8] = 7;
    P[9] = 9;
    P[10] = 10;
    P[11] = 6;


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
    for (uint64_t i = 0; i < data.get_len(); i++) {
        sequence.push_back(i + 1);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(sequence.begin(), sequence.end(), g);


    //! 
    sequence[0] = 12;
    sequence[1] = 11;

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

    auto err = mul_matrix(c, H_T);

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
    
    return err;
}


void McEllice::test() 
{
    generate_S();
    generate_P();
    // calculate_inv_S();
    // calculate_inv_P();
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