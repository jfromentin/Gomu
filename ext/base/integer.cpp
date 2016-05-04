/**
 * This file is part of Gomu.
 *
 *  Copyright 2016 by Jean Fromentin <jean.fromentin@math.cnrs.fr>
 *
 * Gomu is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Gomu is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Gomu. If not, see <http://www.gnu.org/licenses/>. 
 */

#include "integer.hpp"

//------------------------------
// Integer add(Integer,Integer)
//------------------------------

void* integer_add(void* a,void* b){
  fmpz* res=new fmpz;
  fmpz_init(res);
  fmpz_add(res,(fmpz*)a,(fmpz*)b);
  return (void*)res;
}

//------------------------------
// Integer mul(Integer,Integer)
//------------------------------

void* integer_mul(void* a,void* b){
  fmpz* res=new fmpz;
  fmpz_init(res);
  fmpz_mul(res,(fmpz*)a,(fmpz*)b);
  return (void*)res;
}

//-------------------------
// Integer negate(Integer)
//-------------------------

void* integer_negate(void* a){
  fmpz* res=new fmpz;
  fmpz_init(res);
  fmpz_neg(res,(fmpz*)a);
  return (void*)res;
}

//------------------------------
// Integer quo(Integer,Integer)
//------------------------------

void* integer_quo(void* a,void* b){
  fmpz* res=new fmpz;
  fmpz_init(res);
  fmpz_fdiv_q(res,(fmpz*)a,(fmpz*)b);
  return (void*)res;
}

//------------------------------
// Integer rem(Integer,Integer)
//------------------------------

void* integer_rem(void* a,void* b){
  fmpz* res=new fmpz;
  fmpz_init(res);
  fmpz_fdiv_r(res,(fmpz*)a,(fmpz*)b);
  return (void*)res;
}

//------------------------------
// Integer sub(Integer,Integer)
//------------------------------

void* integer_sub(void* a,void* b){
  fmpz* res=new fmpz;
  fmpz_init(res);
  fmpz_sub(res,(fmpz*)a,(fmpz*)b);
  return (void*)res;
}
