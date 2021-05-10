#include <bits/stdc++.h>
using namespace std;
using ui = unsigned int;
using ul = unsigned long;
using uc = unsigned char;

#define Type 0
ul MagicTable[0x500];

void init(){
  ul seed =0x00100001, index1 =0, index2 =0, i;   
  for(index1 = 0; index1 < 0x100; ++index1){    
    for(index2 = index1, i = 0; i < 5; ++i, index2 += 0x100){    
      ul temp1, temp2;   
      seed = (seed * 125 + 3) % 0x2AAAAB;   
      temp1 = (seed & 0xFFFF) << 0x10;   
      seed = (seed * 125 + 3) % 0x2AAAAB;   
      temp2 = (seed & 0xFFFF);   
      MagicTable[index2] = ( temp1 | temp2 );    
    }    
  }
  return ;
}

// Blizzard  
ul MPQHash(const char* str, ul HashType){    
  uc *key = (uc*)str;   
  ul seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;   
  int ch;   
  while(*key != 0){    
    ch = toupper(*key++);   
    seed1 = MagicTable[(HashType <<8) + ch] ^ (seed1 + seed2);   
    seed2 = ch + seed1 + seed2 + (seed2 << 5) +3;    
  }   
  return seed1;    
}

int main(int argc, char**argv){
  init();
  string str = "最強|Hash #6";
  ul x = MPQHash(str.c_str(), Type);
  cout<< x <<endl;
  return 0;
}
