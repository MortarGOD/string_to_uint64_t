//Google FarmHash

#include <bits/stdc++.h>

typedef std::pair<uint64_t, uint64_t> uint128_t;
inline uint64_t Uint128Low64(const uint128_t x) { return x.first; }
inline uint64_t Uint128High64(const uint128_t x) { return x.second; }
inline uint128_t Uint128(uint64_t lo, uint64_t hi) { return uint128_t(lo, hi); }
#define STATIC_INLINE static inline

using namespace std;

using ui = unsigned int;
using ul = unsigned long;
using uc = unsigned char;
using ull = unsigned long long;

// Some primes between 2^63 and 2^64 for various uses.
static const uint64_t k0 = 0xc3a5c85c97cb3127ULL;
static const uint64_t k1 = 0xb492b66fbe98f273ULL;
static const uint64_t k2 = 0x9ae16a3b2f90404fULL;

#define uint32_in_expected_order(x) (x)
#define uint64_in_expected_order(x) (x)

STATIC_INLINE uint64_t Fetch64(const char *p) {
  uint64_t result;
  memcpy(&result, p, sizeof(result));
  return uint64_in_expected_order(result);
}

STATIC_INLINE uint32_t Fetch32(const char *p) {
  uint32_t result;
  memcpy(&result, p, sizeof(result));
  return uint32_in_expected_order(result);
}

STATIC_INLINE uint32_t bswap_32(const uint32_t x) {
  uint32_t y = x;
  for (size_t i = 0; i<sizeof(uint32_t)>> 1; i++) {
    uint32_t d = sizeof(uint32_t) - i - 1;
    uint32_t mh = ((uint32_t)0xff) << (d << 3);
    uint32_t ml = ((uint32_t)0xff) << (i << 3);
    uint32_t h = x & mh;
    uint32_t l = x & ml;
    uint64_t t = (l << ((d - i) << 3)) | (h >> ((d - i) << 3));
    y = t | (y & ~(mh | ml));
  }
  return y;
}

STATIC_INLINE uint64_t bswap_64(const uint64_t x) {
  uint64_t y = x;
  for (size_t i = 0; i<sizeof(uint64_t)>> 1; i++) {
    uint64_t d = sizeof(uint64_t) - i - 1;
    uint64_t mh = ((uint64_t)0xff) << (d << 3);
    uint64_t ml = ((uint64_t)0xff) << (i << 3);
    uint64_t h = x & mh;
    uint64_t l = x & ml;
    uint64_t t = (l << ((d - i) << 3)) | (h >> ((d - i) << 3));
    y = t | (y & ~(mh | ml));
  }
  return y;
}

STATIC_INLINE uint32_t Bswap32(uint32_t val) { return bswap_32(val); }
STATIC_INLINE uint64_t Bswap64(uint64_t val) { return bswap_64(val); }

// FARMHASH PORTABILITY LAYER: bitwise rot

STATIC_INLINE uint32_t BasicRotate32(uint32_t val, int shift) {
  // Avoid shifting by 32: doing so yields an undefined result.
  return shift == 0 ? val : ((val >> shift) | (val << (32 - shift)));
}

STATIC_INLINE uint64_t BasicRotate64(uint64_t val, int shift) {
  // Avoid shifting by 64: doing so yields an undefined result.
  return shift == 0 ? val : ((val >> shift) | (val << (64 - shift)));
}

STATIC_INLINE uint32_t Rotate32(uint32_t val, int shift) {
  return BasicRotate32(val, shift);
}
STATIC_INLINE uint64_t Rotate64(uint64_t val, int shift) {
  return BasicRotate64(val, shift);
}

// Hash 128 input bits down to 64 bits of output.
// This is intended to be a reasonably good hash function.
// May change from time to time, may differ on different platforms, may differ
// depending on NDEBUG.
STATIC_INLINE uint64_t Hash128to64(uint128_t x) {
  // Murmur-inspired hashing.
  const uint64_t kMul = 0x9ddfea08eb382d69ULL;
  uint64_t a = (Uint128Low64(x) ^ Uint128High64(x)) * kMul;
  a ^= (a >> 47);
  uint64_t b = (Uint128High64(x) ^ a) * kMul;
  b ^= (b >> 47);
  b *= kMul;
  return b;
}

STATIC_INLINE uint64_t ShiftMix(uint64_t val) {
  return val ^ (val >> 47);
}

STATIC_INLINE uint64_t HashLen16(uint64_t u, uint64_t v) {
  return Hash128to64(Uint128(u, v));
}

STATIC_INLINE uint64_t HashLen16(uint64_t u, uint64_t v, uint64_t mul) {
  // Murmur-inspired hashing.
  uint64_t a = (u ^ v) * mul;
  a ^= (a >> 47);
  uint64_t b = (v ^ a) * mul;
  b ^= (b >> 47);
  b *= mul;
  return b;
}

STATIC_INLINE uint64_t HashLen0to16(const char *s, size_t len) {
  if (len >= 8) {
    uint64_t mul = k2 + len * 2;
    uint64_t a = Fetch64(s) + k2;
    uint64_t b = Fetch64(s + len - 8);
    uint64_t c = Rotate64(b, 37) * mul + a;
    uint64_t d = (Rotate64(a, 25) + b) * mul;
    return HashLen16(c, d, mul);
  }
  if (len >= 4) {
    uint64_t mul = k2 + len * 2;
    uint64_t a = Fetch32(s);
    return HashLen16(len + (a << 3), Fetch32(s + len - 4), mul);
  }
  if (len > 0) {
    uint8_t a = s[0];
    uint8_t b = s[len >> 1];
    uint8_t c = s[len - 1];
    uint32_t y = static_cast<uint32_t>(a) + (static_cast<uint32_t>(b) << 8);
    uint32_t z = len + (static_cast<uint32_t>(c) << 2);
    return ShiftMix(y * k2 ^ z * k0) * k2;
  }
  return k2;
}

// This probably works well for 16-byte strings as well, but it may be overkill
// in that case.
STATIC_INLINE uint64_t HashLen17to32(const char *s, size_t len) {
  uint64_t mul = k2 + len * 2;
  uint64_t a = Fetch64(s) * k1;
  uint64_t b = Fetch64(s + 8);
  uint64_t c = Fetch64(s + len - 8) * mul;
  uint64_t d = Fetch64(s + len - 16) * k2;
  return HashLen16(Rotate64(a + b, 43) + Rotate64(c, 30) + d,
                   a + Rotate64(b + k2, 18) + c, mul);
}

// Return a 16-byte hash for 48 bytes.  Quick and dirty.
// Callers do best to use "random-looking" values for a and b.
STATIC_INLINE pair<uint64_t, uint64_t> WeakHashLen32WithSeeds(
    uint64_t w, uint64_t x, uint64_t y, uint64_t z, uint64_t a, uint64_t b) {
  a += w;
  b = Rotate64(b + a + z, 21);
  uint64_t c = a;
  a += x;
  a += y;
  b += Rotate64(a, 44);
  return make_pair(a + z, b + c);
}

// Return a 16-byte hash for s[0] ... s[31], a, and b.  Quick and dirty.
STATIC_INLINE pair<uint64_t, uint64_t> WeakHashLen32WithSeeds(
    const char* s, uint64_t a, uint64_t b) {
  return WeakHashLen32WithSeeds(Fetch64(s),
                                Fetch64(s + 8),
                                Fetch64(s + 16),
                                Fetch64(s + 24),
                                a,
                                b);
}

// Return an 8-byte hash for 33 to 64 bytes.
STATIC_INLINE uint64_t HashLen33to64(const char *s, size_t len) {
  uint64_t mul = k2 + len * 2;
  uint64_t a = Fetch64(s) * k2;
  uint64_t b = Fetch64(s + 8);
  uint64_t c = Fetch64(s + len - 8) * mul;
  uint64_t d = Fetch64(s + len - 16) * k2;
  uint64_t y = Rotate64(a + b, 43) + Rotate64(c, 30) + d;
  uint64_t z = HashLen16(y, a + Rotate64(b + k2, 18) + c, mul);
  uint64_t e = Fetch64(s + 16) * mul;
  uint64_t f = Fetch64(s + 24);
  uint64_t g = (y + Fetch64(s + len - 32)) * mul;
  uint64_t h = (z + Fetch64(s + len - 24)) * mul;
  return HashLen16(Rotate64(e + f, 43) + Rotate64(g, 30) + h,
                   e + Rotate64(f + a, 18) + g, mul);
}


uint64_t FarmHash64(const char *s, size_t len) {
  const uint64_t seed = 81;
  if (len <= 32) {
    if (len <= 16) {
      return HashLen0to16(s, len);
    } else {
      return HashLen17to32(s, len);
    }
  } else if (len <= 64) {
    return HashLen33to64(s, len);
  }

  // For strings over 64 bytes we loop.  Internal state consists of
  // 56 bytes: v, w, x, y, and z.
  uint64_t x = seed;
  uint64_t y = seed * k1 + 113;
  uint64_t z = ShiftMix(y * k2 + 113) * k2;
  pair<uint64_t, uint64_t> v = make_pair(0, 0);
  pair<uint64_t, uint64_t> w = make_pair(0, 0);
  x = x * k2 + Fetch64(s);

  // Set end so that after the loop we have 1 to 64 bytes left to process.
  const char* end = s + ((len - 1) / 64) * 64;
  const char* last64 = end + ((len - 1) & 63) - 63;
  assert(s + len - 64 == last64);
  do {
    x = Rotate64(x + y + v.first + Fetch64(s + 8), 37) * k1;
    y = Rotate64(y + v.second + Fetch64(s + 48), 42) * k1;
    x ^= w.second;
    y += v.first + Fetch64(s + 40);
    z = Rotate64(z + w.first, 33) * k1;
    v = WeakHashLen32WithSeeds(s, v.second * k1, x + w.first);
    w = WeakHashLen32WithSeeds(s + 32, z + w.second, y + Fetch64(s + 16));
    std::swap(z, x);
    s += 64;
  } while (s != end);
  uint64_t mul = k1 + ((z & 0xff) << 1);
  // Make s point to the last 64 bytes of input.
  s = last64;
  w.first += ((len - 1) & 63);
  v.first += w.first;
  w.first += v.first;
  x = Rotate64(x + y + v.first + Fetch64(s + 8), 37) * mul;
  y = Rotate64(y + v.second + Fetch64(s + 48), 42) * mul;
  x ^= w.second * 9;
  y += v.first * 9 + Fetch64(s + 40);
  z = Rotate64(z + w.first, 33) * mul;
  v = WeakHashLen32WithSeeds(s, v.second * mul, x + w.first);
  w = WeakHashLen32WithSeeds(s + 32, z + w.second, y + Fetch64(s + 16));
  std::swap(z, x);
  return HashLen16(HashLen16(v.first, w.first, mul) + ShiftMix(y) * k0 + z,
                   HashLen16(v.second, w.second, mul) + x,
                   mul);
}

int main(int argc, char** argv){
  string str = "最強|Hash #2";
  uint64_t x = FarmHash64(str.c_str(), str.size());
  cout<< x <<endl;
  return 0;
}
