#include <netinet/in.h>
#include <netinet/ip.h>

struct FactorialArgs {
  uint64_t begin;
  uint64_t end;
  uint64_t mod;
};

uint64_t MultModulo(uint64_t, uint64_t, uint64_t);
