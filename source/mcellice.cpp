#include "../headers/mcellice.hpp"
#include <iostream>

McEllice::McEllice(type g, type p, uint64_t n)
{
    GoppaCode code(g, p ,n);

    k = code.get_k();

    n = code.get_n();

    G = code.get_G();

    G_T = std::vector<Long>(G[0].get_len(), Long(G.size()));
    
    P = std::vector<uint64_t>(n);

    inv_P = std::vector<uint64_t>(n);

    S = std::vector<Long>(k, Long(k)); 

    inv_S = std::vector<Long>(k, Long(k));

    publicKey = std::vector<Long>(G.size(), Long(G[0].get_len()));
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
    

    // auto tt = 0;



    // bool is_prim;
    // Long tmp_long(k);
    // do{
    //     is_prim = true;

    //     for (auto& i : S){
    //         do {
    //             tmp_long.set_random_num();
    //         } while(tmp_long.is_null());
    //         i = tmp_long;
    //     }

    //     // перевірка матриці на невиродженість

    //     // приводимо матрицю до трикутного вигляду
    //     for (uint64_t i = 0; i < k - 1; i++) {
    //         auto tmp = std::max_element(S.begin() + i, S.end(), [i, this](const Long& a, const Long& b) 
    //             {
    //                 return a.hight_bit() < b.hight_bit();
    //             }
    //         );

    //         auto hight_bit = tmp->hight_bit();
    //         iter_swap(S.begin() + i, tmp);

    //         for (auto itr = S.begin() + i + 1; itr < S.end(); itr++) {
    //             if (itr->hight_bit() == hight_bit) {
    //                 *itr = *itr ^ *(S.begin() + i);
    //             }
    //         }
    //     }

    //     // перевіряємо діагональ матриці
    //     uint64_t index = k;
    //     for (const auto& vec : S) {
    //         if (vec.hight_bit() != index) {
    //             is_prim = false;
    //             break;
    //         }
    //         index--;
    //     }
    //     std::cout << "hi\n";
    // } while (!is_prim);
}

void McEllice::generate_P()
{ 
    uint64_t i = 0;
    for (auto& el : P) {
        el = ++i;
    }
 
    std::random_device rd;
    std::mt19937 g(rd());
 
    std::shuffle(P.begin(), P.end(), g);
}


void McEllice::calculate_transpose_G()
{
    auto num_rows = G.size();
    auto num_columns = G[0].get_len();
    std::vector<std::vector<uint64_t>> indexes;
    for (const auto& row : G) {
        indexes.push_back(row.get_indexes());
    }

    std::vector<std::vector<uint64_t>> transpose_indexes(num_columns);
    auto tmp = num_rows - 1;
    for (uint64_t i = 0; i < num_rows; i++){
        for (auto el : indexes[i]) {
            transpose_indexes[el].push_back(tmp - i);
        }
    }

    // auto row_indexes = transpose_indexes.begin();
    auto g_t = G_T.begin();
    for (const auto& row_indexes : transpose_indexes) {
        for (auto index : row_indexes) {
            g_t->raise_bit(index + 1);
        }
        ++g_t;
    }
}


void McEllice::calculate_PK()
{
    //** множення матриці S на матрицю G
    auto num_rows = publicKey.size();
    auto num_columns = publicKey[0].get_len();

    auto tmp = publicKey;
    for (uint64_t row = 0; row < num_rows; row++) {
        for (const auto& r : G_T) {
            tmp[row].push_back( (S[row] ^ r).get_indexes().size() % 2 );
        }
    } 

    //** множення матриці SG на матрицю P
    for (uint64_t i = 0; i < P.size(); i++) {
        publicKey[P[i]] = tmp[i];
    }
}


void McEllice::calculate_inv_P()
{
    for (uint64_t i = 0; i < n; i++) {
        inv_P[P[i]] = i;
    }
}

void McEllice::calculate_inv_S()
{
    std::vector<Long> tmp_inv = std::vector<Long> (S.size(), Long(S[0].get_len() * 2));
    auto len = S.size();
    for (uint64_t i = 0; i < len; i++) {
        tmp_inv[i] = S[i];
        tmp_inv[i].raise_bit(len-i);
    }

    // приводимо систему рівнянь де перша частина матриці має трикутний вигляд
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


    auto index = len - 2;
    for (uint64_t i = 1; i < len; i++) {
        auto indexes = tmp_inv[index].get_indexes();
        for (uint64_t j = 1; j < indexes.size(); j++) {
            if (indexes[j] < len) {
                break;
            }
            tmp_inv[index] = tmp_inv[index] ^ tmp_inv[2 * len - indexes[j] - 1];
        }
        index--;++
    }

    for (uint64_t i = 0; i < len; i++) {
        auto indexes = tmp_inv[i].get_indexes();
        for (uint64_t j = 1; j < indexes.size(); j++) {
            inv_S[i].raise_bit(indexes[j] + 1);
        }
    }
}

void McEllice::test() 
{
    generate_S();
    calculate_inv_S();
}