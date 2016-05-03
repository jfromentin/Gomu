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

#include "monoid.hpp"

//*************
//* Reversing *
//*************

Word
Reversing::get_word() const{
  size_t s=word.size;
  Word res(s);
  if(s==0) return res;
  NInd ind=word.nodes[0].next;
  res[0]=word.nodes[ind].data;
  size_t i=1;
  while((ind=word.nodes[ind].next)!=0){
    res[i++]=word.nodes[ind].data;
  }
  return res;
}

//*****************
//* LeftReversing *
//*****************

//-----------------------------------
// LeftReversing::check_positivity()
//-----------------------------------

bool
LeftReversing::check_positivity(){
  while(not to_reverse.empty()){
    reverse();
    if(word.size>0 and word.first()<0) return false;
  }
  return true;
}

//------------------------------
// LeftReversing::denominator()
//------------------------------

Word
LeftReversing::denominator(){
  size_t ind=word.nodes[0].next;
  if(ind==0 or word.nodes[ind].data>0) return Word();
  size_t s=1;
  while((ind=word.nodes[ind].next)!=0 and word.nodes[ind].data<0){ 
    ++s;
  }
  Word res(s);
  s=0;
  while((ind=word.nodes[ind].previous)!=0){
    res[s++]=-word.nodes[ind].data;
  }
  return res;
}

//----------------------------
// LeftReversing::numerator()
//----------------------------

Word
LeftReversing::numerator(){
  size_t ind=word.nodes[0].previous;
  if(ind==0 or word.nodes[ind].data<0) return Word();
  size_t s=1;
  while((ind=word.nodes[ind].previous)!=0 and word.nodes[ind].data>0){ 
    ++s;
  }
  Word res(s);
  s=0;
  while((ind=word.nodes[ind].next)!=0){
    res[s++]=word.nodes[ind].data;
  }
  return res;
}

//--------------------------
// LeftReversing::reverse()
//--------------------------

void
LeftReversing::reverse(){
  //replace ___x.Y___ by ___U.v___
  NInd i=to_reverse.back();
  to_reverse.pop_back();
  Generator x=word.nodes[i].data;
  NInd j=word.nodes[i].next;
  NInd p=word.nodes[i].previous;
  Generator y=-word.nodes[j].data;
  NInd n=word.nodes[j].next;
  //the word is __$.x.Y.#__ with $=[p],X=[i],y=[j],#=[n]
  size_t s=set_comp(x,y,comp);
  //TODO :: Unroll loop
  for(size_t ind=0;ind<s;++ind){
    word.insert_after(i,-comp[ind]);
  }
  word.erase(i);
  s=set_comp(y,x,comp);
  for(size_t ind=0;ind<s;++ind) word.insert_before(j,comp[ind]);
  word.erase(j);

  //the word is now __$.U.v.#___ with $=[p] and #=[n]
  if(s>0){
    if(word.nodes[p].data>0){
      if(word.nodes[word.nodes[p].next].data<0){
	to_reverse.push_back(p);
      }
    }
    if(word.nodes[n].data<0){
      if(word.nodes[word.nodes[n].previous].data>0){
	to_reverse.push_back(word.nodes[n].previous);
      }
    }
  }
  else{
    if(word.nodes[p].data>0 and word.nodes[n].data<0){
      to_reverse.push_back(p);
    }
  }
}

//----------------------------------------
// LeftReversing::set_word(const Word& w)
//----------------------------------------

void
LeftReversing::set_word(const Word& w){
  clear();
  Reversing::set_word(w);
  if(w.is_empty()) return;
  for(size_t i=0;i<w.size()-1;++i){
    if(w.read(i)>0 and w.read(i+1)<0){
      to_reverse.push_back(i+1); 
    }
  }
}

//----------------------------------------------------------
// LeftReversing::set_word(const Word& num,const Word& den)
//----------------------------------------------------------

void
LeftReversing::set_word(const Word& num,const Word& den){
  clear();
  size_t ns=num.size();
  size_t ds=den.size();
  Reversing::init_word(ns+ds);
  for(int i=0;i<ns;++i){
    word.nodes[i+1].data=num[i];
  }
  for(int i=0;i<ds;++i){
    word.nodes[i+ns+1].data=-den[ds-i-1];
  }
  if(ns*ds!=0) to_reverse.push_back(ns);
}

//******************
//* RightReversing *
//******************

//------------------------------------
// RightReversing::check_positivity()
//------------------------------------

bool
RightReversing::check_positivity(){
  while(not to_reverse.empty()){
    reverse();
    if(word.size>0 and word.last()<0) return false;
  }
  return true;
}

//-------------------------------
// RightReversing::denominator()
//-------------------------------

Word
RightReversing::denominator(){
  size_t ind=word.nodes[0].previous;
  if(ind==0 or word.nodes[ind].data>0) return Word();
  size_t s=1;
  while((ind=word.nodes[ind].previous)!=0 and word.nodes[ind].data<0){ 
    ++s;
  }
  Word res(s);
  size_t i=0;
  while((ind=word.nodes[ind].next)!=0){
    res[s-i-1]=-word.nodes[ind].data;
    ++i;
  }
  return res;
}

//-----------------------------
// RightReversing::numerator()
//-----------------------------

Word
RightReversing::numerator(){
  size_t ind=word.nodes[0].next;
  if(ind==0 or word.nodes[ind].data<0) return Word();
  size_t s=1;
  while((ind=word.nodes[ind].next)!=0 and word.nodes[ind].data>0){ 
    ++s;
  }
  Word res(s);
  size_t i=0;
  while((ind=word.nodes[ind].previous)!=0){
    res[s-i-1]=word.nodes[ind].data;
    ++i;
  }
  return res;
}

//---------------------------
// RightReversing::reverse()
//---------------------------

void RightReversing::reverse(){
  //replace ___X.y___ by ___u.V___
  NInd i=to_reverse.back();
  to_reverse.pop_back();
  Generator x=-word.nodes[i].data;
  NInd j=word.nodes[i].next;
  NInd p=word.nodes[i].previous;
  Generator y=word.nodes[j].data;
  NInd n=word.nodes[j].next;

  //the word is __$.X.y.#__ with $=[p],X=[i],y=[j],#=[n]
  size_t s=set_comp(x,y,comp);
  //TODO :: Unroll loop
  for(size_t ind=0;ind<s;++ind) word.insert_before(i,comp[ind]);
  word.erase(i);
  s=set_comp(y,x,comp);
  for(size_t ind=0;ind<s;++ind) word.insert_after(j,-comp[ind]);
  word.erase(j);
  
  //the word is now __$.u.V.#___ with $=[p] and #=[n]
  if(s>0){
 
    if(word.nodes[p].data<0){
      if(word.nodes[word.nodes[p].next].data>0){
	to_reverse.push_back(p);
      }
    }
    if(word.nodes[n].data>0){
      if(word.nodes[word.nodes[n].previous].data<0){
	to_reverse.push_back(word.nodes[n].previous);
      }
    }
  }
  else{
    if(word.nodes[p].data<0 and word.nodes[n].data>0){
      to_reverse.push_back(p);
    }
  }
}

//-----------------------------------------
// RightReversing::set_word(const Word& w)
//-----------------------------------------

void
RightReversing::set_word(const Word& w){
  clear();
  Reversing::set_word(w);
  if(w.is_empty()) return;
  for(size_t i=0;i<w.size()-1;++i){
    if(w.read(i)<0 and w.read(i+1)>0){
      to_reverse.push_back(i+1); 
    }
  }
}

//----------------------------------------------------------
// RightReversing::set_word(const Word& num,const Word& den)
//----------------------------------------------------------

void
RightReversing::set_word(const Word& den,const Word& num){
  clear();
  size_t ds=den.size();
  size_t ns=num.size();
  Reversing::init_word(ds+ns);
  for(int i=0;i<ds;++i){
    word.nodes[i+1].data=-den[ds-i-1];
  }
  for(int i=0;i<ns;++i){
    word.nodes[i+ds+1].data=num[i];
  }
  if(ns*ds!=0) to_reverse.push_back(ds);
}


//****************
//* MonoidFamily *
//****************

MonoidFamily::~MonoidFamily(){
}

//***************
//* MonoidTrait *
//***************

//-----------------------------
// MonoidTrait::~MonoidTrait()
//-----------------------------

MonoidTrait::~MonoidTrait(){
  if(left_reversing!=nullptr) delete left_reversing;
  if(right_reversing!=nullptr) delete right_reversing;
}

//-----------------------------------------------------------
// MonoidTrait::is_left_divisible_x(const Word&,const Word&)
//-----------------------------------------------------------

pair<bool,Word>
MonoidTrait::is_left_divisible_x(const Word& a,const Word& b){
  right_reversing->set_word(b,a);
  if(right_reversing->check_positivity())
    return pair<bool,Word>(true,right_numerator());
  return pair<bool,Word>(false,Word());
}

//------------------------------------------------------------
// MonoidTrait::is_right_divisible_x(const Word&,const Word&)
//------------------------------------------------------------

pair<bool,Word>
MonoidTrait::is_right_divisible_x(const Word& a,const Word& b){
  left_reversing->set_word(a,b);
  if(left_reversing->check_positivity())
    return pair<bool,Word>(true,left_numerator());
  return pair<bool,Word>(false,Word());
}

//---------------------------------------------------
// MonoidTrait::left_complement(Generator,Generator)
//---------------------------------------------------

Word
MonoidTrait::left_complement(const Generator& x,const Generator& y){
  Generator comp[MAX_COMPLEMENT_SIZE];
  size_t l=left_reversing->set_comp(x,y,comp);
  Word res(l);
  for(size_t i=0;i<l;++i) res[i]=comp[i];
  return res;
}

//------------------------------------------------
// MonoidTrait::left_gcd(const Word&,const Word&)
//------------------------------------------------

Word
MonoidTrait::left_gcd(const Word& a,const Word& b){
  right_reverse(a,b);
  left_reverse(right_reversing->get_word());
  left_reverse(a,left_denominator());
  return left_numerator();
}

//--------------------------------------------------
// MonoidTrait::left_gcd_x(const Word&,const Word&)
//--------------------------------------------------

pair<Word,Word>
MonoidTrait::left_gcd_x(const Word& a,const Word& b){
  right_reverse(a,b);
  left_reverse(right_reversing->get_word());
  Word div=left_denominator();
  left_reverse(a,div);
  return pair<Word,Word>(left_numerator(),div);
}

//----------------------------------------------------
// MonoidTrait::right_complement(Generator,Generator)
//----------------------------------------------------

Word
MonoidTrait::right_complement(const Generator& x,const Generator& y){
  Generator comp[MAX_COMPLEMENT_SIZE];
  size_t l=right_reversing->set_comp(x,y,comp);
  Word res(l);
  for(size_t i=0;i<l;++i) res[i]=comp[i];
  return res;
}

//------------------------------------------------
// MonoidTrait::right_gcd(const Word&,const Word&)
//------------------------------------------------

Word
MonoidTrait::right_gcd(const Word& a,const Word& b){
  left_reverse(b,a);
  right_reverse(left_reversing->get_word());
  right_reverse(right_denominator(),a);
  return right_numerator();
}

//---------------------------------------------------
// MonoidTrait::right_gcd_x(const Word&,const Word&)
//---------------------------------------------------

pair<Word,Word>
MonoidTrait::right_gcd_x(const Word& a,const Word& b){
  left_reverse(b,a);
  right_reverse(left_reversing->get_word());
  Word div=right_denominator();
  right_reverse(div,a);
  return pair<Word,Word>(right_numerator(),div);
}

//********
//* Word *
//********

//------------------------------------------------
// Word::Word(const initializer_list<Generator>&)
//------------------------------------------------

Word::Word(const initializer_list<Generator>& l):Array(l.size()){
  size_t i=0;
  for(auto it=l.begin();it!=l.end();++it){
    array[i++]=*it;
  }
}

//-----------------
// Word::inverse() 
//-----------------

Word
Word::inverse() const{
  Word res(s);
  for(size_t i=0;i<s;++i){
    res.array[i]=-array[s-i-1];
  }
  return res;
}

//-----------------------------------
// Word::display(DisplayGenerator d)
//-----------------------------------

string
Word::display(DisplayGenerator d) const{
  if(s==0) return "e";
  string str=d(array[0]);
  for(size_t i=1;i<s;++i){
    str+='.';
    str+=d(array[i]);
  }
  return str;
}

//***********************
//* Auxiliary functions *
//***********************
