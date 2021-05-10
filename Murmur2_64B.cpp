#include <bits/stdc++.h>
using namespace std;
using ui = unsigned int;
using ul = unsigned long;
using uc = unsigned char;
using ull = unsigned long long;

#define SEED 0xEE6B27EB

ull MurmurHash64B(const void * key, int len, ui seed){
	const ui m = 0x5bd1e995;
	const int r = 24;
 
	ui h1 = seed ^ len;
	ui h2 = 0;
 
	const ui * data = (const ui *)key;
 
	while(len >= 8){
		ui k1 = *data++;
		k1 *= m; k1 ^= k1 >> r; k1 *= m;
		h1 *= m; h1 ^= k1;
		len -= 4;
 
		ui k2 = *data++;
		k2 *= m; k2 ^= k2 >> r; k2 *= m;
		h2 *= m; h2 ^= k2;
		len -= 4;
	}
 
	if(len >= 4){
		ui k1 = *data++;
		k1 *= m; k1 ^= k1 >> r; k1 *= m;
		h1 *= m; h1 ^= k1;
		len -= 4;
	}
 
	switch(len){
	  case 3: h2 ^= ((uc*)data)[2] << 16;
	  case 2: h2 ^= ((uc*)data)[1] << 8;
	  case 1: h2 ^= ((uc*)data)[0];
		h2 *= m;
	};
 
	h1 ^= h2 >> 18; h1 *= m;
	h2 ^= h1 >> 22; h2 *= m;
	h1 ^= h2 >> 17; h1 *= m;
	h2 ^= h1 >> 19; h2 *= m;
 
	ull h = h1;
 
	h = (h << 32) | h2;
 
	return h;
} 

int main(int argc, char**argv){
  string str = "最強|Hash #5";
  ull x = MurmurHash64B(str.c_str(), str.size(), SEED);
  cout<< x <<endl;
  return 0;
}
