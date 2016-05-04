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

#ifndef MONOID_HPP
#define MONOID_HPP

#include <cstdint>
#include "../../array.hpp"
#include "stacked_list.hpp"

#define MAX_COMPLEMENT_SIZE 64

//***************************
//* Early class definitions *
//***************************

class Reversing;
class LeftReversing;
class RightReversing;
class PresentedMonoid;
class Word;

//************
//* Typedefs *
//************

//! Monoid generator
typedef int16_t Generator;
//! Complement function of a monoid
typedef int(*SetComplement)(const Generator& x,const Generator& y,Generator* comp);
//! Display function for monoid generator
typedef string(*DisplayGenerator)(const Generator& x);
//! Return the number of generators of the monoid of rank n among a monoid familly
typedef size_t(*GeneratorsNumber)(size_t n);
//! Return the rank of a Generator
typedef size_t(*GeneratorRank)(const Generator& x);
//! Ranked Generator bijection
typedef Generator(*RankedGeneratorBijection)(size_t r,const Generator& x,int p);
//! Return a ranked word
typedef Word(*RankedWordFactory)(size_t r);
  
//********************* 
//* Class definitions *
//*********************

//-----------
// Reversing
//-----------

//! Common data and function between left and right reversing algorithms
class Reversing{
public:
  //! Internal word
  StackedList word;
  //! Next detected position to reverse
  deque<NInd> to_reverse;
  //! Destination structure for complement
  Generator comp[MAX_COMPLEMENT_SIZE];
  //! Complement function
  SetComplement set_comp;

  //! Clear internal word
  void clear();

  //! Display internal word
  void disp_word() const;

  //! Return internal word
  Word get_word() const;
  
  //! Init internal word to be of size s
  void init_word(size_t s);
  
  //! Number of detected position to reverse. O implies the word is reversed
  size_t remaining_size() const;

  //! Set internal word
  void set_word(const Word& w);
};

//----------------
// Left reversing
//----------------

//! A class for left reversing algorithm
class LeftReversing:public Reversing{  
public:
  //! Unique constructor
  LeftReversing(SetComplement sc);
  
  //! Test if full reversing gives a positive word when internal word is u.v^(-1)
  bool check_positivity();

  //! Return numerator of the word
  Word denominator();

  //! Reverse untill the is no more reversing step
  void full_reverse();
  
  //! Return numerator of the word
  Word numerator();

  //! Perform one reversing step
  void reverse();

  //! Set internal word to be w
  void set_word(const Word& w);

  //! Set internal word to be num.den^(-1)
  void set_word(const Word& num,const Word& den);
};

//-----------------
// Right reversing
//-----------------

//! A class for right reversing
class RightReversing:public Reversing{  
public:
  //! Unique constructor
  RightReversing(SetComplement sc);

  //! Test if full reversing gives a positive word when internal word is u^(-1).v
  bool check_positivity();
  
  //! Return numerator of the word
  Word denominator();
  
  //! Reverse untill the is no more reversing ste
  void full_reverse();

  //! Return numerator of the word
  Word numerator();

  //! Perform one reversing 
  void reverse();

  //! Set internal word
  void set_word(const Word& w);

  //! Set internal word to be den^(-1).num
  void set_word(const Word& den,const Word& num);
};

//-------------
// MonoidTrait
//-------------
//! Class for procedure attached to monoid

class MonoidTrait{
public:
  //! Pointer to a LeftReversing
  LeftReversing* left_reversing;
  //! Pointer to a RightReversing
  RightReversing* right_reversing;
  //! Extra data
  void* data;
  //! Empty constructor
  MonoidTrait();

  //! Destructor
  ~MonoidTrait();

  //! Test if two words are equivalent
  bool are_equivalent(const Word& u,const Word& v);

  //! Test if the family has a left complement
  bool has_left_complement() const;
   
  //! Test if the family has a right complement
  bool has_right_complement() const;
  
  //! Test if a is left divisible by b, i.e.,if it exists c such that a=b.c */
  bool is_left_divisible(const Word& a,const Word& b);

  //! Return a Couple (f,c) such that f equals true if a is left divisible by b,
  //! i.e.,if it exists c such that a=b.c 
  pair <bool,Word> is_left_divisible_x(const Word& a,const Word& b);
 
  //! Test if a is right divisible by b, i.e.,if it exists c such that a=c.b
  bool is_right_divisible(const Word& a,const Word& b);

  //! Return a Couple (f,c) such that f equals true if a is right divisible by b,
  //! i.e.,if it exists c such that a=c.b
  pair<bool,Word> is_right_divisible_x(const Word& a,const Word& b);

  //! Return left complement of x and y
  Word left_complement(const Generator& x,const Generator& y);
  
  //! Return the left denominator
  Word left_denominator();
  
  //! Return the left gcd of a and b, i.e., a maximal element c 
  //! such that there exist x with a=c.x and y with b=c.y 
  Word left_gcd(const Word& a,const Word& b);

  //! Return a Couple (c,d) where c is the left gcd of a and d is such that a=c.d
  pair<Word,Word> left_gcd_x(const Word& a,const Word& b);
  
  //! Return the left lcm of a and b, i.e., a minimal element c
  //! such that there exist x with c=x.a and y with c=y.a
  Word left_lcm(const Word& a,const Word& b);

  //! Return the left lcm complement of a and b, i.e.,
  //! an element d such that d.a is equal to the left lcm of a and b
  Word left_lcm_complement(const Word& a,const Word& b);
  
  //! Return the left numerator
  Word left_numerator();
  
  //! Left reverse the word w
  Word left_reverse(const Word& w);

  //! Left reverse the u.v^(-1)
  Word left_reverse(const Word& u,const Word& v);

  //! Return right complement of x and y
  Word right_complement(const Generator& x,const Generator& y);
  
  //! Return the right denominator
  Word right_denominator();
  
  //! Return the left gcd of a and b, i.e., a maximal element c
  //! such that there exist x with a=c.x and y with b=c.y
  Word right_gcd(const Word& a,const Word& b);

  //!  Return a Couple (c,d) where c is the right gcd of a and d is such that a=d.c
  pair<Word,Word> right_gcd_x(const Word& a,const Word& b);
  
  //! Return the right lcm of a and b, i.e., a minimal element c
  //! such that there exist x with c=a.x and y with c=a.y 
  Word right_lcm(const Word& a,const Word& b);
  
  //! Return the right lcm complement of a and b, i.e.,
  //! an element d such that a.d is equal to the right lcm of a and b 
  Word right_lcm_complement(const Word& a,const Word& b);

  //! Return right numerator
  Word right_numerator();
  
  //! Right reverse the word w
  Word right_reverse(const Word& w);

  //! Right reverse the u^(-1).v
  Word right_reverse(const Word& u,const Word& v);
  
  //! Set left complement
  void set_left_complement(SetComplement sc);

  //! Set right complement
  void set_right_complement(SetComplement sc); 
};

//--------------
// MonoidFamily
//--------------
//! A class for familly of monoid

class MonoidFamily:public MonoidTrait{
public:
  //! Function to display generators
  DisplayGenerator gdisp;
  //! Function returning the number of generators for a given rank 
  GeneratorsNumber gnum;
  //! Return rank of a generator
  GeneratorRank grank;
  //! Label of the monoid family
  string label;
  //! Ranked Garside automorphism germ
  RankedGeneratorBijection ranked_phi_germ;
  //! Ranked Garside element factory
  RankedWordFactory ranked_garside_word_factory;
  
  //! Unique constructor
  MonoidFamily(string l,DisplayGenerator d,GeneratorsNumber n,GeneratorRank r);

  //! Destructor
  ~MonoidFamily();

  //! Apply phi_r^p to the word
  void apply_phi(size_t r,Word& w,int p=1);
  
  //! Display
  string display() const;
    
  //! Return garside_element of rank r
  Word garside_element(size_t r);
  
  //! Return number of generators for rank n
  size_t generators_number(size_t n);

  //! Test if the family has a Garside automorphism
  bool has_garside_automorphism() const;

  //! Test if the family has a Garside element
  bool has_garside_element() const;
  
  //! Return the word obtained under phi_r^p
  Word phi(size_t r,const Word& w,int p=1);

  //! Return phi-normal form of an element
  Word phi_normal(const Word& w);

  //! Return phi-normal form of an element of rank r
  Word phi_normal(size_t r,const Word& w);
  
  //! Return ranked phi-tail of an element
  Word phi_tail(size_t r,const Word& w);

  //! Return ranked phi-tail of an element together with remainder
  pair<Word,Word> phi_tail_x(size_t r,const Word& w);

  //! Return the ranked phi-splitting of an element
  Array<Word> phi_splitting(size_t r,const Word& w);

  //! Return rank of a Word
  size_t rank(const Word& w);

    //! Set ranked phi germ
  void set_ranked_phi_germ(RankedGeneratorBijection rpg);
  
  //! Set ranked garside word factory
  void set_ranked_garside_word_factory(RankedWordFactory rgwf);  
};

//------
// Word
//------

//! Class for word 
class Word:public Array<Generator>{
public:
  //! Empty constructor
  Word();

  //! Construct a word from a list of Generator
  Word(const initializer_list<Generator>& l);
  
  //! Recopy constructor
  Word(const Word&);
  
  //! Move constructor
  Word(Word&&);
  
  //! Construct a word from an array
  Word(const Array<Generator>&);
  Word(Array<Generator>&&);

  //! Assignement operator with copy
  Word& operator=(const Word& w);

  //! Assignement operator with move
  Word& operator=(Word&& w);
  
  //! Concatenate a word to this one
  Word concatenate(const Word& w) const;

  //! Return the word inverse of this one
  Word inverse() const;

  //! Display a word
  string display(DisplayGenerator d) const;
};

//***********************
//* Auxiliary functions *
//***********************

//! Comparison function for Generator
//! \param x a generator
//! \param y a generator
//! \return -1 if x<y, 0 if x==y and 1 if x>y
int cmp(const Generator& x,const Generator& y);


//! Display a generator with letter
string disp_alpha(const Generator& x);

//! Multiply word
//! \param u a word
//! \param w a word
//! \return the word uv
Word operator*(const Word& u,const Word& v);


//***********************
//* Inline declarations *
//***********************

//-----------
// Reversing
//-----------

inline void
Reversing::clear(){
  to_reverse.clear();
}

inline void
Reversing::disp_word() const{
  cout<<word<<endl;
}

inline void
Reversing::init_word(size_t s){
  word.init(s);
}

inline size_t
Reversing::remaining_size() const{
  return to_reverse.size();
}

inline void
Reversing::set_word(const Word& w){
  word.init((NData*)w.array,w.size());
}

//----------------
// Left reversing
//----------------

inline
LeftReversing::LeftReversing(SetComplement sc){
  set_comp=sc;
}

inline void
LeftReversing::full_reverse(){
  while(not to_reverse.empty())
    reverse();
}


//-----------------
// Right reversing
//-----------------

inline
RightReversing::RightReversing(SetComplement sc){
  set_comp=sc;
}

inline void
RightReversing::full_reverse(){
  while(not to_reverse.empty()) reverse();
}

//--------------
// MonoidFamily
//--------------

inline
MonoidFamily::~MonoidFamily(){
}

inline string
MonoidFamily::display() const{
  return label+" monoid family";
}

inline Word
MonoidFamily::garside_element(size_t r){
  return ranked_garside_word_factory(r);
}

inline size_t
MonoidFamily::generators_number(size_t n){
  return gnum(n);
}

inline bool
MonoidFamily::has_garside_element() const{
  return ranked_garside_word_factory!=nullptr;
}

inline bool
MonoidFamily::has_garside_automorphism() const{
  return ranked_phi_germ!=nullptr;
}

inline Word
MonoidFamily::phi_normal(const Word& w){
  return phi_normal(rank(w),w);
}

				    
inline void
MonoidFamily::set_ranked_phi_germ(RankedGeneratorBijection rpg){
  ranked_phi_germ=rpg;
}

inline void
MonoidFamily::set_ranked_garside_word_factory(RankedWordFactory rgwf){
  ranked_garside_word_factory=rgwf;
}

//-------------
// MonoidTrait
//-------------

inline bool
MonoidTrait::has_left_complement() const{
  return left_reversing!=nullptr;
}

inline bool
MonoidTrait::has_right_complement() const{
  return right_reversing!=nullptr;
}

inline bool
MonoidTrait::is_left_divisible(const Word& a,const Word& b){
  right_reversing->set_word(b,a);
  return right_reversing->check_positivity();
}

inline bool
MonoidTrait::is_right_divisible(const Word& a,const Word& b){
  left_reversing->set_word(a,b);
  return left_reversing->check_positivity();
}

inline Word
MonoidTrait::left_denominator(){
  return left_reversing->denominator();
}

inline Word
MonoidTrait::left_lcm_complement(const Word& a,const Word& b){
  left_reverse(b,a);
  return left_numerator();
}

inline Word
MonoidTrait::left_lcm(const Word& a,const Word& b){
  return left_lcm_complement(a,b)*a;
}

inline Word
MonoidTrait::left_numerator(){
  return left_reversing->numerator();
}

inline Word
MonoidTrait::left_reverse(const Word& w){
  left_reversing->set_word(w);
  left_reversing->full_reverse();
  return left_reversing->get_word();
}

inline Word
MonoidTrait::left_reverse(const Word& u,const Word& v){
  left_reversing->set_word(u,v);
  left_reversing->full_reverse();
  return left_reversing->get_word();
}

inline Word
MonoidTrait::right_denominator(){
  return right_reversing->denominator();
}

inline Word
MonoidTrait::right_lcm(const Word& a,const Word& b){
  return a*right_lcm_complement(a,b);
}

inline Word
MonoidTrait::right_lcm_complement(const Word& a,const Word& b){
  right_reverse(a,b);
  return right_numerator();
}

inline Word
MonoidTrait::right_numerator(){
  return right_reversing->numerator();
}

inline Word
MonoidTrait::right_reverse(const Word& w){
  right_reversing->set_word(w);
  right_reversing->full_reverse();
  return right_reversing->get_word();
}

inline Word
MonoidTrait::right_reverse(const Word& u,const Word& v){
  right_reversing->set_word(u,v);
  right_reversing->full_reverse();
  return right_reversing->get_word();
}

inline void
MonoidTrait::set_left_complement(SetComplement sc){
  left_reversing=new LeftReversing(sc);
}

inline void
MonoidTrait::set_right_complement(SetComplement sc){
  right_reversing=new RightReversing(sc);
}

//------
// Word
//------

inline
Word::Word(){}

inline
Word::Word(const Word& w):Array(w){}

inline
Word::Word(Word&& w):Array(w){}

inline
Word::Word(const Array<Generator>& a):Array(a){}

inline
Word::Word(Array<Generator>&& a):Array(a){}

inline Word
Word::concatenate(const Word& w) const{
  return Word(append(w));
}

inline Word&
Word::operator=(const Word& w){
  Array::operator=(w);
}

inline Word&
Word::operator=(Word&& w){
  Array::operator=(w);
}

//***********************
//* Auxiliary functions *
//***********************

inline int
cmp(const Generator& x,const Generator& y){
  if(x<y) return -1;
  if(x==y) return 0;
  return 1;
}

inline string
disp_alpha(const Generator& x){
  if(x==0) return "e";
  string res="";
  if(x>0) return res+char(x-1+'a');
  return res+char(-x-1+'A');
}

inline Word
operator*(const Word& u,const Word& v){
  return u.append(v);
}
				    
#endif
