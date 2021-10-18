#include "../headers/source.hpp"

std::vector<Long>& transpose(const std::vector<Long>& m) 
{
    auto num_rows = m.size();
    auto num_columns = m[0].get_len();
    std::vector<std::vector<uint64_t>> indexes;
    for (const auto& row : m) {
        indexes.push_back(row.get_indexes());
    }

    std::vector<std::vector<uint64_t>> transpose_indexes(num_columns);
    auto tmp = num_rows - 1;
    auto tt = num_columns - 1;
    for (uint64_t i = 0; i < num_rows; i++){
        for (auto el : indexes[i]) {
            transpose_indexes[tt - el].push_back(tmp - i);
        }
    }

    // auto row_indexes = transpose_indexes.begin();
    auto* m_T = new std::vector<Long>(num_columns, Long(num_rows));
    auto g_t = m_T->begin();
    for (const auto& row_indexes : transpose_indexes) {
        for (auto index : row_indexes) {
            g_t->raise_bit(index + 1);
        }
        ++g_t;
    }

    return *m_T;
}


std::vector<Long>& concatenation(const std::vector<Long>& first, const std::vector<Long>& second)
{
    auto shift = second[0].get_len();
    auto n = first[0].get_len() + shift;

    auto* ret = new std::vector<Long>(first.size(), Long(n));

    uint64_t iter = 0;
    for (const auto& i : first) {
        auto indexes = i.get_indexes();
        for(auto ind : indexes) {
            ret[0][iter].raise_bit(ind + shift + 1);
        }
        iter++;
    }

    iter = 0;
    for (const auto& i : second) {
        auto indexes = i.get_indexes();
        for(auto ind : indexes) {
            ret[0][iter].raise_bit(ind  + 1);
        }
        iter++;
    }

    return *ret;
}