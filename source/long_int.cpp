#include "../headers/long_int.hpp"

Long::Long(__uint128_t max_len) : max_number_length(max_len)
{
    max_cluster_length = sizeof(ln_t) * 8;
    num_clusters = max_number_length / max_cluster_length + 1;
    if (!(max_number_length % max_cluster_length)) {
        num_clusters--;
    }
    number = std::vector<ln_t>(num_clusters, 0);

    // length = 0;
    length_last_cluster = max_cluster_length;
    number_of_clusters_used = 0;
}


Long::Long(const Long& other)
{
    this->max_number_length = other.max_number_length;
    this->max_cluster_length = other.max_cluster_length;
    this->num_clusters = other.num_clusters;
    // this->length = other.length;
    this->length_last_cluster = other.length_last_cluster;
    this->number_of_clusters_used = other.number_of_clusters_used;
    this->number = other.number;
}


Long& Long::operator^(const Long& other) const
{
    check(other);
    Long* result = new Long(*this);

    auto& res = result->number;

    for (uint64_t index = 0; index < num_clusters; index++) {
        res[index] ^= other.number[index];
    }
    return *result;
}


bool Long::operator==(const Long& other) const
{
    check(other);
    auto len = number.size();
    for (uint64_t i = 0; i < len; i++) {
        if (this->number[i] != other.number[i]) {
            return false;
        }
    }
    return true;
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
    // length++;
    length_last_cluster++;
}


uint64_t Long::hight_bit() const
{
    uint64_t cluster_index = -1;
    for (const auto& cluster: number) {
        cluster_index++;
        for (uint64_t i = max_cluster_length - 1; (i >= 0) && (i < -1); i--) {
            if ( (cluster >> i) & 1 ) {
                return i + (num_clusters - cluster_index) * max_cluster_length + 1;
            }
        }
    }
    return 0;
}


std::vector<uint64_t> Long::get_indexes() const
{
    std::vector<uint64_t>* indexes = new std::vector<uint64_t>;

    for (uint64_t i = 0; i < num_clusters - 1; i++) {
        if (number[i] == 0) {
            continue;
        }

        auto num = number[i];
        for (uint64_t j = max_cluster_length - 1; j >= 0 && j < -1; j--) {
            if ( (num >> j) & 0x1 ) {
                indexes->push_back( max_number_length - (i + 1) * max_cluster_length + j);
            }
        }
    }

    auto num = *std::prev(number.end());
    for (uint64_t j = max_cluster_length - 1; j >= 0 && j < -1; j--) {
        if ( (num >> j) & 0x1 ) {
            indexes->push_back(j);
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
    if (this->num_clusters != other.num_clusters) {
        throw -2;
    }
    // if (this->length != other.length) {
    //     throw -3;
    // }
    // if (this->number_of_clusters_used != other.number_of_clusters_used) {
    //     throw -4;
    // }
}

bool Long::is_null() const
{
    for (auto i : number) {
        if (i) {
            return false;
        }
    }
    return true;
}


void Long::set_random_num()
{
    std::random_device rd;
    std::uniform_int_distribution<uint64_t> dist = std::uniform_int_distribution<uint64_t> (1, 50);
    for (auto& i : number) {
        i = dist(rd);
        i ^= ((__uint128_t)dist(rd)) << (sizeof(uint64_t) * 8);
    }
    auto it = std::prev(number.end());
    auto shift = max_cluster_length - (max_number_length % max_cluster_length);
    if (shift == max_cluster_length) {
        shift = 0;
    }
    // зануляємо shift старших біт у останьому кластері
    *it = *it << shift;
    *it = *it >> shift;
}