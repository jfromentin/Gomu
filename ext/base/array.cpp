#include "array.hpp"

void* array_len(void* v){
  return Gomu::to_integer(((Gomu::ArrayValue*)v)->size);
}
