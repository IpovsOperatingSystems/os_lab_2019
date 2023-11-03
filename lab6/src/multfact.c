#include <sys/types.h>

u_int64_t MultModulo(u_int64_t a, u_int64_t b, u_int64_t mod) {
  u_int64_t result = 0;
  a = a % mod;
  while (b > 0) {
    if (b % 2 == 1)
      result = (result + a) % mod;
    a = (a * 2) % mod;
    b /= 2;
  }
  return result % mod;
}