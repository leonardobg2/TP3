#ifndef HASH_FUNCTIONS_HPP
#define HASH_FUNCTIONS_HPP

#include <string>


unsigned int hash_str(const std::string &data) {
  unsigned int h(0);
  for (unsigned int i=0; i<data.length(); i++)
    h = (h << 6) ^ (h >> 26) ^ data[i];
  return h;
} // https://stackoverflow.com/questions/3413470/what-is-stdmove-and-when-should-it-be-used

unsigned int hash_int(const int& key){
    unsigned int x = static_cast<unsigned int>(key);
    x = ((x >> 16) ^ x) * 0x45d9f3bu;
    x = (x >> 16) ^ x;
    return x;
} // https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key

unsigned int hash_float(float key);

#endif // HASH_FUNCTIONS_HPP

//eu optei por utilizar funções hash do stackoverflow, pois elas serão melhores