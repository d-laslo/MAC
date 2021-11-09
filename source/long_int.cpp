#include "../headers/long_int.hpp"
#include <array>

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


Long::Long(__uint128_t num, __uint128_t max_len) : Long(max_len)
{
    number[num_clusters - 1] = num;
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
    // check(other);
    Long* result = new Long(*this);

    auto& res = result->number;

    for (uint64_t index = 0; index < num_clusters; index++) {
        res[index] ^= other.number[index];
    }
    return *result;
}


bool Long::operator==(const Long& other) const
{
    // check(other);
    auto len = number.size();
    for (uint64_t i = 0; i < len; i++) {
        if (this->number[i] != other.number[i]) {
            return false;
        }
    }
    return true;
}


bool Long::operator>(const Long& other) const
{
    auto th_indx = this->get_indexes();
    auto oth_indx = other.get_indexes();
    uint64_t i = 0;
    while (th_indx[i] == oth_indx[i]) {
        i++;
    }

    if (th_indx[i] > oth_indx[i]) {
        return true;
    }
    return false;
}


Long& Long::operator=(const Long& other) 
{
    if (this->max_number_length < other.max_number_length) {
        throw -1;
    } 
    if (this->max_cluster_length != other.max_cluster_length) {
        throw - 2;
    }
    
    // this->length_last_cluster = other.length_last_cluster;
    this->number_of_clusters_used = other.number_of_clusters_used;
    for (uint64_t i = 0; i < other.num_clusters - 1; i++) {
        this->number[i] = other.number[i];   
    }
    if ( (this->max_number_length - other.max_number_length) >=  (other.max_cluster_length - (other.max_number_length % other.max_cluster_length)))
    {
        this->number[other.num_clusters - 1] = other.number[other.num_clusters - 1] << (other.max_cluster_length - (other.max_number_length % other.max_cluster_length));
    }
    else {
        this->number[other.num_clusters - 1] = other.number[other.num_clusters - 1] << (this->max_number_length - other.max_number_length);
    }

    return *this;
}


Long& Long::mod_pol(const Long&& pol) const 
{
    auto th_indx = this->get_indexes();
    auto pol_indx = pol.get_indexes();
    auto* res = new Long((__uint128_t)pol.get_len());
    if (th_indx[0] == pol_indx[0]) {
        *res = *this ^ pol;
    }
    if (th_indx[0] < pol_indx[0]) {
        *res = *this;
    }

    
    delete res;
    res = new Long(*this);

    auto new_pol_indx = pol_indx;
    auto len_ind = new_pol_indx.size();
    while (th_indx[0] >= pol_indx[0]) {
        auto shift = res->hight_bit() - pol_indx[0] - 1;
        for(uint64_t i = 0; i < len_ind; i++) {
            new_pol_indx[i] = pol_indx[i] + shift;
        }

        
        auto new_pol = Long(this->max_number_length);
        for (auto i : new_pol_indx) { 
            new_pol.raise_bit(i + 1);
        }

        *res = *res ^ new_pol;
        th_indx[0] = res->hight_bit() - 1;
    }

    return *res;
}


// Long Long::div(const Long& divider)
// {
//     auto res = *this;
//     if (divider > res || divider == res) {
//         throw -1;
//     }
//     Long quotient = Long(0, res.get_len());
//     auto one = Long(1, 1);
//     while (res > divider) {
//         auto divided_l = res.hight_bit();
//         auto divider_l = divider.hight_bit();
        
//         quotient = quotient ^ (1 << (divided_l - divider_l));
//         res = res ^ (divider << (divided_l - divider_l));
//     }

//     return quotient;
// }


// Long& Long::mul(const Long& multiplier2)
// {
//     auto multiplier1 = *this;
//     auto end = multiplier2.hight_bit();
//     auto zero = Long(this->max_number_length + multiplier2.max_number_length);
//     Long product(this->max_number_length + multiplier2.max_number_length);
//     for (uint64_t i = 0; i < end; i++) {
//         auto t = (multiplier2 >> i).get_indexes();
//         product = product ^ ((t[t.size() - 1] == 0) ? (multiplier1 << i) : zero); 
//     }
//     return product;
// }


// Long& Long::norm(const Long& pol) {
//     auto* val = new  Long(*this);
//     auto p_l = pol.hight_bit();
//     auto val_l = val->hight_bit();
//     while( val_l >= p_l ){
//         *val = *val ^ (pol << (val_l - p_l));
//         val_l = val->hight_bit();
//     }

//     return *val;
// }


// Long& Long::mod(const Long& pol)
// {
//     auto base = new Long(*this);
//     if (pol.is_null()) {
//         throw -1;
//     }
//     auto pol_l = pol.hight_bit();
//     while( *base > pol  || *base == pol){
//        *base = *base ^ (pol << (base->hight_bit() - pol_l));
//     }

//     return *base;
// }


// Long Long::invert_pol(const Long& pol)
// {
//     auto value = *this;
//     std::vector<Long> v{Long(value.get_len()), Long(1, value.get_len())};
//     Long rem = 0;
//     Long a = pol;
//     do {
//         // auto tmp = div(a, value);
//         auto tmp = a.div(value);
//         v[0] = v[0] ^ v[1].mul(tmp).norm(pol);
//         std::swap(v[0], v[1]);
//         rem = a.mod(value);
//         a = value;
//         value = rem;
//     }
//     while(!rem.is_null());

//     return v[0];
// }


// __uint128_t& Long::operator[](uint64_t index)
// {
//     return number[index];
// }


Long& Long::operator<<(uint64_t shift) const
{
    auto* res = new Long(this->max_number_length + shift);
    auto indxs = this->get_indexes();

    for (auto i : indxs) {
        res->raise_bit(i + shift);
    }
    return *res;
}


Long& Long::operator>>(uint64_t shift) const
{
    auto* res = new Long(this->max_number_length);
    auto indxs = this->get_indexes();

    for (auto i : indxs) {
        if ((i - shift) < 0) {
            break;
        }
        res->raise_bit(i - shift);
    }
    return *res;
}


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
    uint64_t cluster_index = 0;
    for (; cluster_index < num_clusters; cluster_index++) {
        if (number[cluster_index] != 0) {
            break;
        }
    }

    // if (cluster_index == num_clusters && number[cluster_index] == 0) {
    //     return 0;
    // }

    auto cluster = number[cluster_index];
    for (uint64_t i = max_cluster_length - 1; (i >= 0) && (i < -1); i--) {
        if ( (cluster >> i) & 1 ) {
            return  i 
                    + (num_clusters - cluster_index - 1) 
                    * max_cluster_length 
                    + 1
                    - (++cluster_index == num_clusters ? 0 :(max_number_length % max_cluster_length));            }
    }
    return 0;
}


// uint64_t Long::hight_bit() const
// {
//     uint64_t cluster_index = -1;
//     for (const auto& cluster: number) {
//         cluster_index++;
//         for (uint64_t i = max_cluster_length - 1; (i >= 0) && (i < -1); i--) {
//             if ( (cluster >> i) & 1 ) {
//                 return  i 
//                         + (num_clusters - cluster_index - 1) 
//                         * max_cluster_length 
//                         + 1;
//                         // - (num_clusters == 1 ? max_cluster_length : 0);
//             }
//         }
//     }
//     return 0;
// }


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


// void Long::check(const Long& other) const 
// {
//     if (this->max_number_length != other.max_number_length) {
//         throw -1;
//     }
//     if (this->max_cluster_length != other.max_cluster_length) {
//         throw -2;
//     }
//     if (this->num_clusters != other.num_clusters) {
//         throw -2;
//     }
//     // if (this->length != other.length) {
//     //     throw -3;
//     // }
//     // if (this->number_of_clusters_used != other.number_of_clusters_used) {
//     //     throw -4;
//     // }
// }

bool Long::is_null() const
{
    for (auto i : number) {
        if (i) {
            return false;
        }
    }
    return true;
}


void Long::set_random()
{
    std::random_device rd;
    std::uniform_int_distribution<uint64_t> dist = std::uniform_int_distribution<uint64_t> (1, 50);
    // srand (42);
    
    for (auto& i : number) {
        i = dist(rd);
        i ^= ((__uint128_t)dist(rd)) << (sizeof(uint64_t) * 8);
        // i ^= ((__uint128_t)rand()) << (sizeof(uint64_t) * 8);
    }
    auto it = std::prev(number.end());
    auto shift = max_cluster_length - (max_number_length % max_cluster_length);
    if (shift == max_cluster_length) {
        shift = 0;
    }
    // зануляємо shift старших біт у останьому кластері
    *it <<= shift;
    *it >>= shift;
}


void Long::set_random_num(uint64_t len)
{
    if (len > max_number_length) {
        len = max_number_length;
    }

    set_random();
    if (len) {
        null_the_bits(max_number_length - len);
        raise_bit(len);
    }
}


void Long::raise_bit(uint64_t _index)
{
    uint64_t remainder = max_number_length % max_cluster_length;
    if (_index <= remainder) {
        number[num_clusters - 1] |= ((ln_t)1 << --_index);
        return;
    }
    // _index -= remainder;
    // uint64_t cluster_index = _index / max_cluster_length;
    // uint64_t index = --_index % max_cluster_length; 

    
    // cluster_index = num_clusters - cluster_index - 1
    //                 - ((max_cluster_length - 1) == index ? 0 : 1);

    // number[cluster_index] |= ((ln_t)1 << index);

    _index = _index - remainder - 1;
    uint64_t cluster_index = num_clusters - _index / max_cluster_length - (bool)remainder;
    --cluster_index;
    uint64_t index = _index % max_cluster_length; 
    number[cluster_index] |= ((ln_t)1 << index);
}


void Long::null_the_bits(uint64_t n)
{
    if (n == 0) {
        return;
    }

    uint64_t cluster = n / max_cluster_length;
    uint64_t shift = --n % max_cluster_length;

    for (uint64_t i = 0; i < cluster; i++) {
        number[i] = 0;
    }

    // uint64_t shift = max_cluster_length - index;
    if (cluster == (num_clusters - 1) ) {
        shift = max_cluster_length - (max_number_length % max_cluster_length - shift);
    } 
    shift++;
    number[cluster] <<= shift;
    number[cluster] >>= shift;
}


__uint128_t Long::get_last_byte() const
{
    auto indx = this->get_indexes();

    __uint128_t res = 0;
    for (auto i : indx) {
        if (i > 127) {
            continue;
        }
        res ^= 1 << i;
    }
    return res;
}


void Long::test()
{
    raise_bit(1);
    raise_bit(128);
    raise_bit(129);
}