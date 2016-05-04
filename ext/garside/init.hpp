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

#include <sstream>
#include "../../module.hpp"
#include "monoid.hpp"
#include "braids.hpp"

//*****************
//* Global object *
//*****************

extern Gomu::Type* type_ArtinWordA;
extern Gomu::Type* type_DualWordA;

//****************
//* MonoidFamily *
//****************

//! Display a MonoidFamily
string mf_display(void* m);

//! Delete a MonoidFamily
void mf_delete(void* m);

//! Return generators number fror rank n
void* mf_generators_number(void* m,void* n);

//***************
//* MonoidTrait *
//***************

//! Return garside element of a given rank
void* mt_garside_element(void* m,void* r);

//! Test is a left divides b
void* mt_is_left_divisible(void* m,void* a,void* b);

//! Test is a left divides b and return extar informations
void* mt_is_left_divisible_x(void* m,void* a,void* b);

//! Test is a right divides b
void* mt_is_right_divisible(void* m,void* a,void* b);

//! Test is a right divides b and return extar informations
void* mt_is_right_divisible_x(void* m,void* a,void* b);

//! Return left complememnt
void* mt_left_complement(void* m,void* a,void*b);

//! Return left denominator
void* mt_left_denominator(void* m);

//! Return left gcd of a and b
void* mt_left_gcd(void* m,void* a,void* b);

//! Return left gcd with extra informations of a and b
void* mt_left_gcd_x(void* m,void* a,void *b);

//! Return left lcm of a and b
void* mt_left_lcm(void* m,void* a,void* b);

//! Return left lcm complement of a and b
void* mt_left_lcm_complement(void* m,void* a,void* b);

//! Return left numerator
void* mt_left_numerator(void* m);

//! Return the word under ranked Garside automorphism
void* mt_phi(void* m,void* r,void* w);

//! Return the word under power of ranked Garside automorphism
void* mt_phi_power(void* m,void* r,void* w,void* p);

//! Left reverse a word
void* mt_left_reverse(void* m,void* w);

//! Left reverse num*den^-1
void* mt_left_reverse2(void* m,void* num,void* den);

//! Return right complememnt
void* mt_right_complement(void* m,void* a,void*b);

//! Return right denominator
void* mt_right_denominator(void* m);

//! Return right gcd of a and b
void* mt_right_gcd(void* m,void* a,void* b);

//! Return right gcd with extra informations of a and b
void* mt_right_gcd_x(void* m,void* a,void *b);

//! Return right lcm of a and b
void* mt_right_lcm(void* m,void* a,void* b);

//! Return right lcm complement of a and b
void* mt_right_lcm_complement(void* m,void* a,void* b);

//! Right reverse a word
void* mt_right_reverse(void* m,void* w);

//! Right reverse den^-1*num
void* mt_right_reverse2(void* m,void* den,void* num);

//! Return right numerator
void* mt_right_numerator(void* m);


//********
//* Word *
//********

//! Display a Word monoid
string word_display(void* w);

//! Delete a Word monoid
void word_delete(void* w);

//! Copy a word monoid
void* word_copy(void* w);

//! Compare to Word monoid
int word_compare(void* w1,void* w2);

//! Create a Word monoid from an array of integer
void* word_from_array(void* arr);

//! Return the length of a Word
void* word_length(void*);

//! Inverse a Word
void* word_invserse(void*);

//! Concatenate two words
void* word_concatenate(void*,void*);

//**************
//* ArtinWordA *
//**************

//! Display a ArtinWordA
string ArtinWordA_display(void* w);

//*************
//* DualWordA *
//*************

//! Display a DualWordA
string DualWordA_display(void* w);

//**********************
//* Inline definitions *
//**********************

//--------------
// MonoidFamily
//--------------

inline string
mf_display(void* m){
  return ((MonoidFamily*)m)->display();
}

inline void
mf_delete(void* m){
  delete (MonoidFamily*)m;
}

inline void*
mf_generators_number(void* m,void* n){
  return Gomu::to_integer(((MonoidFamily*)m)->generators_number(Gomu::get_slong(n)));
}

//------
// Word
//------

inline string
word_display(void* w){
  return to_string(*((Word*)w));
}

inline void
word_delete(void* w){
  delete (Word*)w;
}

inline void*
word_copy(void* w){
  return (void*)new Word(*(Word*)w);
}

inline int
word_compare(void* u,void* v){
   return cmp(*(Word*)u,*(Word*)v);
}

inline void*
word_length(void* u){
  return Gomu::to_integer(((Word*)u)->size());
}

inline void*
word_inverse(void* u){
  return new Word(((Word*)u)->inverse());
}

inline void*
word_concatenate(void* u,void *v){
  return new Word(((Word*)u)->concatenate(*(Word*)v));
}

//------------
// ArtinWordA
//------------

inline string
ArtinWordA_display(void* w){
  return ((Word*)w)->display(ArtinA_disp);
}

//------------
// DualWordA
//------------

inline string
DualWordA_display(void* w){
  return ((Word*)w)->display(DualA_disp);
}

