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

#ifndef BRAIDS_HPP
#define BRAIDS_HPP
#include "monoid.hpp"

//******************
//* Global objects *
//******************

extern MonoidFamily ArtinA_mf;
extern MonoidFamily DualA_mf;

//***********************************
//* Auxiliary functions declaration *
//***********************************

//! Initilise all braids structure
void braids_init();
Generator generator(uint8_t i,uint8_t j);
uint8_t get_i(const Generator& x);
uint8_t get_j(const Generator& y);

//-----------------
// Artin of type A
//-----------------

//! Display function for generators of type A braids
string ArtinA_disp(const Generator& x);

//! Return the number of generators of type A braid of rank n
size_t ArtinA_gnum(size_t n);

//! Set left complement for Artin monoid of type A
int ArtinA_left_sc(const Generator& x,const Generator &y,Generator* comp);

//! Return the rank of an Artin generator of type A
size_t ArtinA_rank(const Generator& x);

//! Set right complement for Artin monoid of type A
int ArtinA_right_sc(const Generator& x,const Generator &y,Generator* comp);

//! Ranked phi germ for Artin monoid of type A
// \param r the rank
// \param x the generator to map
// \param p power to apply
Generator ArtinA_rpg(size_t r,const Generator& x,int p);

//! Ranked Garside word factory
Word ArtinA_rgwf(size_t r);

//-----------------
// Dual of type A
//-----------------

//! Display function for generators of type A dual braids
string DualA_disp(const Generator& x);

//! Return the number of generators of type A dual braids of rank n
size_t DualA_gnum(size_t n);

//! Set left complement for dual monoid of type A
int DualA_left_sc(const Generator& x,const Generator &y,Generator* comp);

//! Return the rank of an dual generator of type A
size_t DualA_rank(const Generator& x);

//! Set right complement for dual monoid of type A
int DualA_right_sc(const Generator& x,const Generator &y,Generator* comp);

//! Ranked phi germ for Artin monoid of type A
// \param r the rank
// \param x the generator to map
// \param p power to apply
Generator DualA_rpg(size_t r,const Generator& x,int p);

//! Ranked Garside word factory
Word DualA_rgwf(size_t r);

//**********************
//* Inline definitions *
//**********************

//---------------------
// Auxiliary functions
//---------------------

inline string
ArtinA_disp(const Generator& x){
  if(x==0) return "e";
  if(x>0) return "a"+to_string(x);
  return "A"+to_string(-x);
}

inline size_t
ArtinA_gnum(size_t n){
  return n-1;
}

inline size_t
ArtinA_rank(const Generator& x){
  return abs(x);
}

inline string
DualA_disp(const Generator& x){
  if(x==0) return "e";
  if(x>0) return "a"+to_string(get_i(x))+to_string(get_j(x));
  return "A"+to_string(get_i(-x))+to_string(get_j(-x));
}

inline size_t
DualA_rank(const Generator& x){
  return get_j(abs(x))-1;
}

inline Generator
generator(uint8_t i,uint8_t j){
  Generator res=j;
  res<<=8;
  return res+i;
}

inline uint8_t
get_i(const Generator& x){
  uint8_t* t=(uint8_t*)&x;
  return t[0];
}

inline uint8_t
get_j(const Generator& x){
  uint8_t* t=(uint8_t*)&x;
  return t[1];
}



#endif
