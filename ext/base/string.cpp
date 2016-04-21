#include "string.hpp"

void* string_len(void* v){
  return Gomu::to_integer(((string*)v)->size());
}

