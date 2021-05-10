#include <bits/stdc++.h>
using namespace std;
using ui = unsigned int;
using ul = unsigned long;
using uc = unsigned char;
using ull = unsigned long long;

#define SEED 16

ull MurmurHash64A( const void * key, int len, ui seed){
  const ull m = 0xc6a4a7935bd1e995;
  const int r = 47;
 
  ull h = seed ^ (len * m);
 
  const ull * data = (const ull *)key;
  const ull * end = data + (len/8);
 
  while(data != end){
    ull k = *data++;
 
    k *= m; 
    k ^= k >> r; 
    k *= m; 
    h ^= k;
    h *= m; 
  }
 
  const uc * data2 = (const uc*)data;
 
  switch(len & 7){
    case 7: h ^= ull(data2[6]) << 48;
    case 6: h ^= ull(data2[5]) << 40;
    case 5: h ^= ull(data2[4]) << 32;
    case 4: h ^= ull(data2[3]) << 24;
    case 3: h ^= ull(data2[2]) << 16;
    case 2: h ^= ull(data2[1]) << 8;
    case 1: h ^= ull(data2[0]);
    h *= m;
  };
 
  h ^= h >> r;
  h *= m;
  h ^= h >> r;

  return h;
} 

int main(int argc, char**argv){
  string str = "最強|Hash #4";
  ull x = MurmurHash64A(str.c_str(), str.size(), SEED);
  cout<< x <<endl;
  return 0;
}
