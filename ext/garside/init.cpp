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

#include "init.hpp"

//******************
//* Global objects *
//******************

Gomu::Type* type_ArtinWordA;
Gomu::Type* type_DualWordA;
Gomu::Type* type_monoid_family;
Gomu::Type* type_word;

//*************************
//* Extension inilisation *
//*************************

extern "C"{
  Word empty_word;
  Word x1({1});
  Word x2({2});
  Word x3({3});
  Word x4({4});
  Word X1({-1});
  Word X2({-2});
  Word X3({-3});
  Word X4({-4});
  Word x12({generator(1,2)});
  Word x13({generator(1,3)});
  Word x23({generator(2,3)});
  Word x14({generator(1,4)});
  Word x24({generator(2,4)});
  Word x34({generator(3,4)});
  Word x15({generator(1,5)});
  Word x25({generator(2,5)});
  Word x35({generator(3,5)});
  Word x45({generator(4,5)});
  Word X12({(Generator)(-generator(1,2))});
  Word X13({(Generator)(-generator(1,3))});
  Word X23({(Generator)(-generator(2,3))});
  Word X14({(Generator)(-generator(1,4))});
  Word X24({(Generator)(-generator(2,4))});
  Word X34({(Generator)(-generator(3,4))});
  Word X15({(Generator)(-generator(1,5))});
  Word X25({(Generator)(-generator(2,5))});
  Word X35({(Generator)(-generator(3,5))});
  Word X45({(Generator)(-generator(4,5))});

  void init(){
    braids_init();
  }
  
  Gomu::Module::Type types[]={
    {"ArtinWordA",ArtinWordA_display,word_delete,word_copy,word_compare,&type_ArtinWordA},
    {"DualWordA",DualWordA_display,word_delete,word_copy,word_compare,&type_DualWordA},

    {"ArtinMonoidFamilyA",mf_display,mf_delete,Gomu::no_copy,Gomu::no_comp,&type_monoid_family},
    {"DualMonoidFamilyA",mf_display,mf_delete,Gomu::no_copy,Gomu::no_comp,&type_monoid_family},
    
    {"MonoidFamily",mf_display,mf_delete,Gomu::no_copy,Gomu::no_comp,&type_monoid_family},
    {"Word",word_display,word_delete,word_copy,word_compare,&type_word},
    TYPE_SENTINEL
  };
  
  Gomu::Module::Function functions[]={
    {"ArtinWordA","operator*",{"ArtinWordA","ArtinWordA"},(void*)word_concatenate},
    {"DualWordA","operator*",{"DualWordA","DualWordA"},(void*)word_concatenate},
    {"Boolean","operator===",{"ArtinWordA","ArtinWordA"},(void*)ArtinWordA_equivalent},
    {"Boolean","operator===",{"DualWordA","DualWordA"},(void*)DualWordA_equivalent},
    {"Word","word",{"Array"},(void*)word_from_array},
    FUNC_SENTINEL
  };
  
  Gomu::Module::Function member_functions[]={
    //ArtinMonoidFamilyA
    {"ArtinWordA","garside_element",{"ArtinMonoidFamilyA","Integer"},(void*)mf_garside_element},
    {"Boolean","is_left_divisible",{"ArtinMonoidFamilyA","ArtinWordA","ArtinWordA"},(void*)mt_is_left_divisible},
    {"Tuple","is_left_divisible_x",{"ArtinMonoidFamilyA","ArtinWordA","ArtinWordA"},(void*)mt_is_left_divisible_x},
    {"Boolean","is_right_divisible",{"ArtinMonoidFamilyA","ArtinWordA","ArtinWordA"},(void*)mt_is_right_divisible},
    {"Tuple","is_right_divisible_x",{"ArtinMonoidFamilyA","ArtinWordA","ArtinWordA"},(void*)mt_is_right_divisible_x},
    {"ArtinWordA","left_complement",{"ArtinMonoidFamilyA","ArtinWordA","ArtinWordA"},(void*)mt_left_complement},
    {"ArtinWordA","left_denominator",{"ArtinMonoidFamilyA"},(void*)mt_left_denominator},
    {"ArtinWordA","left_lcm",{"ArtinMonoidFamilyA","ArtinWordA","ArtinWordA"},(void*)mt_left_lcm},
    {"ArtinWordA","left_lcm_complement",{"ArtinMonoidFamilyA","ArtinWordA","ArtinWordA"},(void*)mt_left_lcm_complement},
    {"ArtinWordA","left_gcd",{"ArtinMonoidFamilyA","ArtinWordA","ArtinWordA"},(void*)mt_left_gcd},
    {"Tuple","left_gcd_x",{"ArtinMonoidFamilyA","ArtinWordA","ArtinWordA"},(void*)mt_left_gcd_x},
    {"ArtinWordA","left_numerator",{"ArtinMonoidFamilyA"},(void*)mt_left_numerator},
    {"ArtinWordA","left_reverse",{"ArtinMonoidFamilyA","ArtinWordA"},(void*)mt_left_reverse},
    {"ArtinWordA","left_reverse",{"ArtinMonoidFamilyA","ArtinWordA","ArtinWordA"},(void*)mt_left_reverse2},
    {"ArtinWordA","phi",{"ArtinMonoidFamilyA","Integer","ArtinWordA"},(void*)mf_phi},
    {"ArtinWordA","phi",{"ArtinMonoidFamilyA","Integer","ArtinWordA","Integer"},(void*)mf_phi_power},
    {"ArtinWordA","phi_normal_form",{"ArtinMonoidFamilyA","ArtinWordA"},(void*)mf_phi_normal},
    {"ArtinWordA","phi_tail",{"ArtinMonoidFamilyA","Integer","ArtinWordA"},(void*)mf_phi_tail},
    {"Tuple","phi_tail_x",{"ArtinMonoidFamilyA","Integer","ArtinWordA"},(void*)mf_phi_tail_x},
    {"Array","phi_splitting",{"ArtinMonoidFamilyA","Integer","ArtinWordA"},(void*)mf_phi_splitting},
    {"Integer","rank",{"ArtinMonoidFamilyA","ArtinWordA"},(void*)mf_rank},
    {"ArtinWordA","right_complement",{"ArtinMonoidFamilyA","ArtinWordA","ArtinWordA"},(void*)mt_right_complement},
    {"ArtinWordA","right_denominator",{"ArtinMonoidFamilyA"},(void*)mt_right_denominator},
    {"ArtinWordA","right_lcm",{"ArtinMonoidFamilyA","ArtinWordA","ArtinWordA"},(void*)mt_right_lcm},
    {"ArtinWordA","right_lcm_complement",{"ArtinMonoidFamilyA","ArtinWordA","ArtinWordA"},(void*)mt_right_lcm_complement},
    {"ArtinWordA","right_gcd",{"ArtinMonoidFamilyA","ArtinWordA","ArtinWordA"},(void*)mt_right_gcd},
    {"Tuple","right_gcd_x",{"ArtinMonoidFamilyA","ArtinWordA","ArtinWordA"},(void*)mt_right_gcd_x},
    {"ArtinWordA","right_numerator",{"ArtinMonoidFamilyA"},(void*)mt_right_numerator},
    {"ArtinWordA","right_reverse",{"ArtinMonoidFamilyA","ArtinWordA"},(void*)mt_right_reverse},
    {"ArtinWordA","right_reverse",{"ArtinMonoidFamilyA","ArtinWordA","ArtinWordA"},(void*)mt_right_reverse2},

    //ArtinWordA
    {"Integer","length",{"ArtinWordA"},(void*)word_length},
    {"ArtinWordA","inverse",{"ArtinWordA"},(void*)word_inverse},

    //DualMonoidFamilyA
    {"DualWordA","garside_element",{"DualMonoidFamilyA","Integer"},(void*)mf_garside_element},
    {"Boolean","is_left_divisible",{"DualMonoidFamilyA","DualWordA","DualWordA"},(void*)mt_is_left_divisible},
    {"Tuple","is_left_divisible_x",{"DualMonoidFamilyA","DualWordA","DualWordA"},(void*)mt_is_left_divisible_x},
    {"Boolean","is_right_divisible",{"DualMonoidFamilyA","DualWordA","DualWordA"},(void*)mt_is_right_divisible},
    {"Tuple","is_right_divisible_x",{"DualMonoidFamilyA","DualWordA","DualWordA"},(void*)mt_is_right_divisible_x},
    {"DualWordA","left_complement",{"DualMonoidFamilyA","DualWordA","DualWordA"},(void*)mt_left_complement},
    {"DualWordA","left_denominator",{"DualMonoidFamilyA"},(void*)mt_left_denominator},
    {"DualWordA","left_lcm",{"DualMonoidFamilyA","DualWordA","DualWordA"},(void*)mt_left_lcm},
    {"DualWordA","left_lcm_complement",{"DualMonoidFamilyA","DualWordA","DualWordA"},(void*)mt_left_lcm_complement},
    {"DualWordA","left_gcd",{"DualMonoidFamilyA","DualWordA","DualWordA"},(void*)mt_left_gcd},
    {"Tuple","left_gcd_x",{"DualMonoidFamilyA","DualWordA","DualWordA"},(void*)mt_left_gcd_x},
    {"DualWordA","left_numerator",{"DualMonoidFamilyA"},(void*)mt_left_numerator},
    {"DualWordA","left_reverse",{"DualMonoidFamilyA","DualWordA"},(void*)mt_left_reverse},
    {"DualWordA","left_reverse",{"DualMonoidFamilyA","DualWordA","DualWordA"},(void*)mt_left_reverse2},
    {"DualWordA","phi",{"DualMonoidFamilyA","Integer","DualWordA"},(void*)mf_phi},
    {"DualWordA","phi",{"DualMonoidFamilyA","Integer","DualWordA","Integer"},(void*)mf_phi_power},
    {"DualWordA","phi_normal_form",{"DualMonoidFamilyA","DualWordA"},(void*)mf_phi_normal},
    {"DualWordA","phi_tail",{"DualMonoidFamilyA","Integer","DualWordA"},(void*)mf_phi_tail},
    {"Tuple","phi_tail_x",{"DualMonoidFamilyA","Integer","DualWordA"},(void*)mf_phi_tail_x},
    {"Array","phi_splitting",{"DualMonoidFamilyA","Integer","DualWordA"},(void*)mf_phi_splitting},
    {"Integer","rank",{"DualMonoidFamilyA","DualWordA"},(void*)mf_rank},
    {"DualWordA","right_complement",{"DualMonoidFamilyA","DualWordA","DualWordA"},(void*)mt_right_complement},
    {"DualWordA","right_denominator",{"DualMonoidFamilyA"},(void*)mt_right_denominator},
    {"DualWordA","right_lcm",{"DualMonoidFamilyA","DualWordA","DualWordA"},(void*)mt_right_lcm},
    {"DualWordA","right_lcm_complement",{"DualMonoidFamilyA","DualWordA","DualWordA"},(void*)mt_right_lcm_complement},
    {"DualWordA","right_gcd",{"DualMonoidFamilyA","DualWordA","DualWordA"},(void*)mt_right_gcd},
    {"Tuple","right_gcd_x",{"DualMonoidFamilyA","DualWordA","DualWordA"},(void*)mt_right_gcd_x},
    {"DualWordA","right_numerator",{"DualMonoidFamilyA"},(void*)mt_right_numerator},
    {"DualWordA","right_reverse",{"DualMonoidFamilyA","DualWordA"},(void*)mt_right_reverse},
    {"DualWordA","right_reverse",{"DualMonoidFamilyA","DualWordA","DualWordA"},(void*)mt_right_reverse2},
    
    //DualWordA
    {"Integer","length",{"DualWordA"},(void*)word_length},
    {"ArtinWordA","inverse",{"DualWordA"},(void*)word_inverse},
    
    //MonoidFamily
    {"Integer","generators_number",{"MonoidFamily","Integer"},(void*)mf_generators_number},
    
    //Word
    {"Integer","length",{"Word"},(void*)word_length},
    {"Word","inverse",{"Word"},(void*)word_inverse},
    FUNC_SENTINEL
  };
    
  Gomu::Module::Symbol symbols[]={
    {"a0","ArtinWordA",(void*)&empty_word},
    {"a1","ArtinWordA",(void*)&x1},
    {"a2","ArtinWordA",(void*)&x2},
    {"a3","ArtinWordA",(void*)&x3},
    {"a4","ArtinWordA",(void*)&x4},
    {"A1","ArtinWordA",(void*)&X1},
    {"A2","ArtinWordA",(void*)&X2},
    {"A3","ArtinWordA",(void*)&X3},
    {"A4","ArtinWordA",(void*)&X4},
    {"a00","DualWordA",(void*)&empty_word},
    {"a12","DualWordA",(void*)&x12},
    {"a13","DualWordA",(void*)&x13},
    {"a23","DualWordA",(void*)&x23},
    {"a14","DualWordA",(void*)&x14},
    {"a24","DualWordA",(void*)&x24},
    {"a34","DualWordA",(void*)&x34},
    {"a15","DualWordA",(void*)&x15},
    {"a25","DualWordA",(void*)&x25},
    {"a35","DualWordA",(void*)&x35},
    {"a45","DualWordA",(void*)&x45},
    {"A12","DualWordA",(void*)&X12},
    {"A13","DualWordA",(void*)&X13},
    {"A23","DualWordA",(void*)&X23},
    {"A14","DualWordA",(void*)&X14},
    {"A24","DualWordA",(void*)&X24},
    {"A34","DualWordA",(void*)&X34},
    {"A15","DualWordA",(void*)&X15},
    {"A25","DualWordA",(void*)&X25},
    {"A35","DualWordA",(void*)&X35},
    {"A45","DualWordA",(void*)&X45},
    {"ArtinA","ArtinMonoidFamilyA",(void*)&ArtinA_mf},
    {"DualA","DualMonoidFamilyA",(void*)&DualA_mf},
    SYMB_SENTINEL
  };
};
    
//*************************
//* Fonctions definitions *
//*************************

//--------------------------------------------
// Word garside_element(MonoidFamily,Integer)
//--------------------------------------------

void* mf_garside_element(void* m,void* r){
  MonoidFamily* monoid=(MonoidFamily*)m;
  if(not monoid->has_garside_element())
    RuntimeError("Monoid doesn't have Garside element");
 return (void*)(new Word(monoid->garside_element(Gomu::get_slong(r))));
}

//-------------------------------------
// Word phi(MonoidFamily,Integer,Word)
//-------------------------------------

void* mf_phi(void* m,void* r,void* w){
  MonoidFamily* monoid=(MonoidFamily*)m;
  if(not monoid->has_garside_automorphism())
    RuntimeError("Monoid has not Garside automorphism");
  size_t rank=Gomu::get_slong(r);
  return new Word(monoid->phi(rank,*(Word*)w));
}

//---------------------------------------------
// Word phi(MonoidFamily,Integer,Word,Integer)
//---------------------------------------------

void* mf_phi_power(void* m,void* r,void* w,void* p){
  MonoidFamily* monoid=(MonoidFamily*)m;
  if(not monoid->has_garside_automorphism())
    RuntimeError("Monoid has not Garside automorphism");
  size_t rank=Gomu::get_slong(r);
  int power=Gomu::get_slong(p);
  return new Word(monoid->phi(rank,*(Word*)w,power));
}

//------------------------------------------
// Word phi_tail(MonoidFamily,Integer,Word)
//------------------------------------------

void* mf_phi_tail(void* m,void* r,void* w){
  MonoidFamily* monoid=(MonoidFamily*)m;
  if(not monoid->has_garside_automorphism())
    RuntimeError("Monoid has not Garside automorphism");
  size_t rank=Gomu::get_slong(r);
  return new Word(monoid->phi_tail(rank,*(Word*)w));
}

//---------------------------------------------------
// (Word,Word) phi_tail_x(MonoidFamily,Integer,Word)
//---------------------------------------------------

void* mf_phi_tail_x(void* m,void* r,void* w){
  MonoidFamily* monoid=(MonoidFamily*)m;
  if(not monoid->has_garside_automorphism())
    RuntimeError("Monoid has not Garside automorphism");
  size_t rank=Gomu::get_slong(r);
  pair<Word,Word> p=monoid->phi_tail_x(rank,*(Word*)w);
  Gomu::TupleValue* res=new Gomu::TupleValue(2);
  Gomu::Type* type=(Gomu::Type*)monoid->data;
  res->tab[0]=Gomu::Value(type,new Word(p.first));
  res->tab[1]=Gomu::Value(type,new Word(p.second));
  return (void*)res;
}

//------------------------------------------------------
// Array[Word] phi_splitting(MonoidFamily,Integer,Word)
//------------------------------------------------------

void* mf_phi_splitting(void* m,void* r,void* w){
  MonoidFamily* monoid=(MonoidFamily*)m;
  size_t rank=Gomu::get_slong(r);
  Array<Word> a=monoid->phi_splitting(rank,*(Word*)w);
  Gomu::ArrayValue* res=new Gomu::ArrayValue(a.size());
  res->type=(Gomu::Type*)monoid->data;
  for(size_t i=0;i<res->size;++i){
    res->tab[i]=(void*)(new Word(a.read(i)));
  }
  return (void*)res;
}

//--------------------------------------------------
// Boolean is_left_divisible(MonoidTrait,Word,Word)
//--------------------------------------------------

void* mt_is_left_divisible(void* m,void* a,void* b){
  MonoidTrait* monoid=(MonoidTrait*)m;
  return Gomu::to_boolean(monoid->is_left_divisible(*(Word*)a,*(Word*)b));
}

//---------------------------------------------------------------
// pair<Boolean,Word> is_left_divisible_x(MonoidTrait,Word,Word)
//---------------------------------------------------------------

void* mt_is_left_divisible_x(void* m,void* a,void* b){
  MonoidTrait* monoid=(MonoidTrait*)m;
  pair<bool,Word> p=monoid->is_left_divisible_x(*(Word*)a,*(Word*)b);
  Gomu::TupleValue* res=new Gomu::TupleValue(2);
  res->tab[0]=Gomu::Value(Gomu::type_boolean,Gomu::to_boolean(p.first));
  res->tab[1]=Gomu::Value((Gomu::Type*)monoid->data,new Word(p.second));
  return (void*)res;
}

//-----------------------------------------------
// bool is_right_divisible(MonoidTrait,Word,Word)
//-----------------------------------------------

void* mt_is_right_divisible(void* m,void* a,void* b){
  MonoidTrait* monoid=(MonoidTrait*)m;
  return Gomu::to_boolean(monoid->is_right_divisible(*(Word*)a,*(Word*)b));
}

//----------------------------------------------------------------
// pair<Boolean,Word> is_right_divisible_x(MonoidTrait,Word,Word)
//----------------------------------------------------------------

void* mt_is_right_divisible_x(void* m,void* a,void* b){
  MonoidTrait* monoid=(MonoidTrait*)m;
  pair<bool,Word> p=monoid->is_right_divisible_x(*(Word*)a,*(Word*)b);
  Gomu::TupleValue* res=new Gomu::TupleValue(2);
  res->tab[0]=Gomu::Value(Gomu::type_boolean,Gomu::to_boolean(p.first));
  res->tab[1]=Gomu::Value((Gomu::Type*)monoid->data,new Word(p.second));
  return (void*)res;
}

//---------------------------------------------
// Word left_complement(MonoidTrait,Word,Word)
//---------------------------------------------

void* mt_left_complement(void* m,void* a,void* b){
  MonoidTrait* monoid=(MonoidTrait*)m;
  Word* u=(Word*)a;
  Word* v=(Word*)b;
  if(u->size()!=1 or v->size()!=1) RuntimeError("Words must be of length 1");
  return new Word(monoid->left_complement(u->array[0],v->array[0]));  
}

//------------------------------------
// Word left_denominator(MonoidTrait)
//------------------------------------

void* mt_left_denominator(void* m){
  MonoidTrait* monoid=(MonoidTrait*)m;
    if(not monoid->has_left_complement())
    RuntimeError("Monoid is not left complemented");
  return new Word(monoid->left_denominator());
}

//---------------------------------------
// Word left_gcd(MonoidTrait,Word,Word)
//---------------------------------------

void* mt_left_gcd(void* m,void* a,void *b){
  MonoidTrait* monoid=(MonoidTrait*)m;
  return new Word(monoid->left_gcd(*(Word*)a,*(Word*)b));
}
    
//-----------------------------------------------
// (Word,Word) left_gcd_x(MonoidTrait,Word,Word)
//-----------------------------------------------

void* mt_left_gcd_x(void* m,void* a,void *b){
  MonoidTrait* monoid=(MonoidTrait*)m;
  pair<Word,Word> p=monoid->left_gcd_x(*(Word*)a,*(Word*)b);
  Gomu::TupleValue* res=new Gomu::TupleValue(2);
  Gomu::Type* type=(Gomu::Type*)monoid->data;
  res->tab[0]=Gomu::Value(type,new Word(p.first));
  res->tab[1]=Gomu::Value(type,new Word(p.second));
  return (void*)res;
}

//--------------------------------------
// Word left_lcm(MonoidTrait,Word,Word)
//--------------------------------------

void* mt_left_lcm(void* m,void* a,void *b){
  MonoidTrait* monoid=(MonoidTrait*)m;
  return new Word(monoid->left_lcm(*(Word*)a,*(Word*)b));
}

//-------------------------------------------------
// Word left_lcm_complement(MonoidTrait,Word,Word)
//-------------------------------------------------

void* mt_left_lcm_complement(void* m,void* a,void *b){
  MonoidTrait* monoid=(MonoidTrait*)m;
  return new Word(monoid->left_lcm_complement(*(Word*)a,*(Word*)b));
}

//----------------------------------
// Word left_numerator(MonoidTrait)
//----------------------------------

void* mt_left_numerator(void* m){
  MonoidTrait* monoid=(MonoidTrait*)m;
  if(not monoid->has_left_complement())
    RuntimeError("Monoid is not left complemented");
  return new Word(monoid->left_numerator());
}

//-------------------------------------
// Word left_reverse(MonoidTrait,Word)
//-------------------------------------

void* mt_left_reverse(void* m,void* w){
  MonoidTrait* monoid=(MonoidTrait*)m;
  if(not monoid->has_left_complement())
    RuntimeError("Monoid is not left complemented");
  return (void*)new Word(monoid->left_reverse(*(Word*)w));
}

//------------------------------------------
// Word left_reverse(MonoidTrait,Word,Word)
//------------------------------------------

void* mt_left_reverse2(void* m,void* num,void* den){
  MonoidTrait* monoid=(MonoidTrait*)m;
  if(not monoid->has_left_complement())
    RuntimeError("Monoid is not left complemented");
  return (void*)new Word(monoid->left_reverse(*(Word*)num,*(Word*)den));
}

//----------------------------------------------
// Word right_complement(MonoidTrait,Word,Word)
//----------------------------------------------

void* mt_right_complement(void* m,void* a,void* b){
  MonoidTrait* monoid=(MonoidTrait*)m;
  Word* u=(Word*)a;
  Word* v=(Word*)b;
  if(u->size()!=1 or v->size()!=1) RuntimeError("Words must be of length 1");
  return new Word(monoid->right_complement(u->array[0],v->array[0]));  
}

//-------------------------------------
// Word right_denominator(MonoidTrait)
//-------------------------------------

void* mt_right_denominator(void* m){
  MonoidTrait* monoid=(MonoidTrait*)m;
  if(not monoid->has_right_complement())
    RuntimeError("Monoid is not right complemented");
  return new Word(monoid->right_denominator());
}

//---------------------------------------
// Word right_gcd(MonoidTrait,Word,Word)
//---------------------------------------

void* mt_right_gcd(void* m,void* a,void *b){
  MonoidTrait* monoid=(MonoidTrait*)m;
  return new Word(monoid->right_gcd(*(Word*)a,*(Word*)b));
}

//-----------------------------------------------
// (Word,Word) right_gcd_x(MonoidTrait,Word,Word)
//-----------------------------------------------

void* mt_right_gcd_x(void* m,void* a,void *b){
  MonoidTrait* monoid=(MonoidTrait*)m;
  pair<Word,Word> p=monoid->right_gcd_x(*(Word*)a,*(Word*)b);
  Gomu::TupleValue* res=new Gomu::TupleValue(2);
  Gomu::Type* type=(Gomu::Type*)monoid->data;
  res->tab[0]=Gomu::Value(type,new Word(p.first));
  res->tab[1]=Gomu::Value(type,new Word(p.second));
  return (void*)res;
}

//---------------------------------------
// Word right_lcm(MonoidTrait,Word,Word)
//---------------------------------------

void* mt_right_lcm(void* m,void* a,void *b){
  MonoidTrait* monoid=(MonoidTrait*)m;
  return new Word(monoid->right_lcm(*(Word*)a,*(Word*)b));
}

//--------------------------------------------------
// Word right_lcm_complement(MonoidTrait,Word,Word)
//--------------------------------------------------

void* mt_right_lcm_complement(void* m,void* a,void *b){
  MonoidTrait* monoid=(MonoidTrait*)m;
  return new Word(monoid->right_lcm_complement(*(Word*)a,*(Word*)b));
}

//-----------------------------------
// Word right_numerator(MonoidTrait)
//-----------------------------------

void* mt_right_numerator(void* m){
  MonoidTrait* monoid=(MonoidTrait*)m;
  if(not monoid->has_right_complement())
    RuntimeError("Monoid is not right complemented");
  return new Word(monoid->right_numerator());
}

//--------------------------------------
// Word right_reverse(MonoidTrait,Word)
//--------------------------------------

void* mt_right_reverse(void* m,void* w){
  MonoidTrait* monoid=(MonoidTrait*)m;
  if(not monoid->has_right_complement())
    RuntimeError("Monoid is not right complemented");
  return (void*)new Word(monoid->right_reverse(*(Word*)w));
}

//-------------------------------------------
// Word right_reverse(MonoidTrair,Word,Word)
//-------------------------------------------

void* mt_right_reverse2(void* m,void* den,void* num){
  MonoidTrait* monoid=(MonoidTrait*)m;
  if(not monoid->has_right_complement())
    RuntimeError("Monoid is not right complemented");
  return (void*)new Word(monoid->right_reverse(*(Word*)den,*(Word*)num));
}

//-----------------------
// Word word(ArrayValue)
//-----------------------

void* word_from_array(void* arr){
  Gomu::ArrayValue* array=(Gomu::ArrayValue*)arr;
  if(array->type!=Gomu::type_integer)
    RuntimeError("An array of integer is needed");
  size_t size=array->size;
  Word* res=new Word(size);
  for(size_t i=0;i<size;++i){
    res->write(i,Gomu::get_slong(array->tab[i]));
  }
  return res;		 
}
