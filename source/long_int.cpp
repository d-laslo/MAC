#include "../headers/long_int.hpp"

Long::Long(__uint128_t max_len) : max_number_length(max_len)
{
    max_cluster_length = sizeof(__uint128_t) * 8;
    number = std::vector<__uint128_t>(max_number_length / max_cluster_length + 1);

    length = 0;
    length_last_cluster = max_cluster_length;
    number_of_clusters_used = 0;
}


Long::Long(const Long& other)
{
    this->max_number_length = other.max_number_length;
    this->max_cluster_length = other.max_cluster_length;
    this->length = other.length;
    this->length_last_cluster = other.length_last_cluster;
    this->number_of_clusters_used = other.number_of_clusters_used;
    this->number = other.number;
}


Long& Long::operator^(const Long& other) const
{
    check(other);
    Long* result = new Long(*this);

    auto& res = result->number;

    for (uint64_t index = 0; index < number_of_clusters_used; index++) {
        res[index] ^= other.number[index];
    }
    return *result;
}


// __uint128_t& Long::operator[](uint64_t index)
// {
//     return number[index];
// }


void Long::push_back(uint64_t val)
{
    if (length_last_cluster == max_cluster_length)
    {
        number_of_clusters_used++;
        length_last_cluster = 0;
    }

    // number[number_of_clusters_used - 1] ^= (val & 1) << length_last_cluster;
    number[number_of_clusters_used - 1] <<= 1;//
    number[number_of_clusters_used - 1] ^= (val & 1); 
    length++;
    length_last_cluster++;
}


uint64_t Long::hight_bit() const
{
    uint64_t cluster_index = -1;
    for (const auto& cluster: number) {
        cluster_index++;
        for (uint64_t i = max_cluster_length - 1; (i >= 0) && (i < -1); i--) {
            if ( (cluster >> i) & 1 ) {
                return i + (length / max_cluster_length - cluster_index) * max_cluster_length + 1;
            }
        }
    }
    return 0;
}


std::vector<uint64_t> Long::get_indexes() const
{
    std::vector<uint64_t>* indexes = new std::vector<uint64_t>;

    for (uint64_t i = 0; i < number_of_clusters_used; i++) {
        if (number[i] == 0) {
            continue;
        }

        auto num = number[i];
        for (uint64_t j = max_cluster_length - 1; j >= 0 && j < -1; j--) {
            if ( (num >> j) & 0x1 ) {
                indexes->push_back(i * max_cluster_length + j);
            }
        }
    }

    return *indexes;
}


void Long::check(const Long& other) const 
{
    if (this->max_number_length != other.max_number_length) {
        throw -1;
    }
    if (this->max_cluster_length != other.max_cluster_length) {
        throw -2;
    }
    if (this->length != other.length) {
        throw -3;
    }
    if (this->number_of_clusters_used != other.number_of_clusters_used) {
        throw -4;
    }
}