#ifndef Braids_array
#define Braids_array

#include <iostream>
#include <list>
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

#include <set>
#include <stack>
#include <initializer_list>
#include <string.h>
#include <cassert>

using namespace std;

//! Class for array
template <class T>
class Array{
 protected:

  //! Size of the array
  size_t s;
  //! Internal data of the array
  T* array;
  //! Construct an array from a c++ array a of size s and own it
  Array(T* a,size_t s);

public:

  //! Contruct an array of size s
  //! \param s size of the array
  Array(size_t s=0);
  
  //! Construct an array from an initialization list
   Array(const initializer_list<T>& list);
    
  //! The copy contructor
  Array(const Array<T>&);
    
  //! The move constructor
  Array(Array<T>&&);
    
  //! Construct an array from a stack
  Array(stack<T>& stack);

  //! Construct an array from a list 
  Array(const list<T>& list);
    
  //! Construct an array from a set 
  Array(const set<T>& set);
    
  //! Destructor
  ~Array();
    
  //! Assignemnet operator with copy
  //! \param array Array to copy
  Array& operator=(const Array& array);

  
  //! Assignement operator with move
  //! \param array Array to move
  Array& operator=(Array&& array);

  
  //! Test is the array is empty
  //! \return true if empty flase otherwise 
  bool is_empty() const;
    
  //! Return the size of the array$
    size_t size() const;
    
  //! Return a pointer to the begin of the array
  const T* begin() const;
    
  //!* Return a pointer a pointer just after the end of the array
  const T* end() const;
    
  //! Write the a value at a given index
  //! \param i index where write
  //! \param v value to write
  void write(size_t i,const T& v);
	
  //! Return the value at a given index
  //! \param i index where read
  //! \param return a reference to the value at index i
  T& read(size_t i) const;
  
  //! Return the value at a given index
  //! \param i index where read
  //! \param return a reference to the value at index i
  const T& at(size_t i) const;

  //! Return the value at a given index
  //! \param i index where read
  //! \param return a reference to the value at index i
  const T& operator[](size_t i) const;
    
  //! Provide access to the the value at index i
  //! \param i index where look
  //! \param return a reference to the value at index i
  T& at(size_t i);

  //! Provide access to the the value at index i
  //! \param i index where look
  //! \param return a reference to the value at index i
  T& operator[](size_t i);

  
  //! Comparison function
  bool operator<(const Array& array) const;
   
  //! Display function
  template<class U> friend std::ostream& operator<<(std::ostream& os,const Array<U>&);

};

//*************************
//* Function declarations *
//*************************

template<class T> inline
Array<T>::Array(size_t _s):s(_s),array(new T[_s]){}

template<class T> inline
Array<T>::Array(T* a,size_t _s):s(_s),array(a){}

template<class T> 
Array<T>::Array(const initializer_list<T> &list):s((int)list.size()),array(new T[s]){
  T* ptr=array;
  for(auto it=list.begin();it!=list.end();++it){
    *ptr=*it;
    ++ptr;
  }
}

template<class T> inline
Array<T>::Array(const Array<T>& a):s(a.s),array(new T[s]){
  memcpy(array,a.array,s*sizeof(T));
}

template<class T> inline
Array<T>::Array(Array<T>&& a):s(a.s),array(a.array){
  a.s=0;
  a.array=nullptr;
}

template<class T> 
Array<T>::Array(const list<T>& l):s((int)l.size()),array(new T[s]){
  T* ptr=array;
  for(typename list<T>::const_iterator it=l.begin();it!=l.end();++it){
    *ptr=*it;
    ++ptr;
  }
}

template<class T> 
Array<T>::Array(stack<T>& stack):s(stack.size()),array(new T[s]){
  T* ptr=array;
  while(not stack.empty()){
    *ptr=stack.top();
    stack.pop();
    ++ptr;
  }
}

template<class T> 
Array<T>::Array(const set<T>& l):s(l.size()),array(new T[s]){
  T* ptr=array;
  for(typename set<T>::const_iterator it=l.begin();it!=l.end();++it){
    *ptr=*it;
    ++ptr;
  }
}

template<class T> inline
Array<T>::~Array(){
  if(array!=nullptr) delete[] array;
}

template<class T> Array<T>&
Array<T>::operator=(const Array<T>& a){
  if(this!=&a){
    if(s!=a.s){
      if(array!=nullptr) delete[] array;
      array=new T[a.s];
    }
    s=a.s;
    memcpy(array,a.array,s*sizeof(T));
  }
  return *this;
}

template<class T>  Array<T>&
Array<T>::operator=(Array<T>&& a){
  if(this!=&a){
    if(array!=nullptr) delete[] array;
    s=a.s;
    a.s=0;
    array=a.array;
    a.array=nullptr;
  }
  return *this;
}

template<class T> inline bool
Array<T>::is_empty() const{
  return s==0;
}

template<class T> inline size_t
Array<T>::size() const{
  return s;
}

template<class T> inline const T*
Array<T>::begin() const{
  return array;
}

template<class T> inline const T*
Array<T>::end() const{
  return array+s;
}

template<class T> inline void
Array<T>::write(size_t i,const T& v){
  assert(i<s);
  array[i]=v;
}

template<class T> inline T&
Array<T>::read(size_t i) const{
  assert(i<s);
  return array[i];
}

template<class T> inline const T&
Array<T>::at(size_t i) const{
  assert(i<s);
  return array[i];
}

template<class T> inline T&
Array<T>::at(size_t i){
  assert(i<s);
  return array[i];
}

template<class T> inline const T&
Array<T>::operator[](size_t i) const{
  assert(i<s);
  return array[i];
}

template<class T> inline T&
Array<T>::operator[](size_t i){
  assert(i<s);
  return array[i];
}

template<class T> bool
Array<T>::operator<(const Array<T>& arr) const{
  if(s==arr.s){
    for(size_t i=0;i<s;++i){
      if(array[i]!=arr.array[i]) return array[i]<arr.array[i];
    }
    return false;
  }
  return s<arr.s;
}

inline ostream& 
operator<<(ostream& os,const Array<uint8_t>& a){
  os<<'[';
  if(not a.is_empty()){
    const uint8_t* ptr=a.begin();
    os<<(int)*ptr;
    for(++ptr;ptr!=a.end();++ptr) os<<','<<(int)*ptr;
  }
  return os<<']'; 
}

inline ostream& 
operator<<(ostream& os,const Array<int8_t>& a){
  os<<'[';
  if(not a.is_empty()){
    const int8_t* ptr=a.begin();
    os<<(int)*ptr;
    for(++ptr;ptr!=a.end();++ptr) os<<','<<(int)*ptr;
  }
  return os<<']'; 
}

template<class T> ostream& 
operator<<(ostream& os,const Array<T>& a){
  os<<'[';
  if(not a.is_empty()){
    const T* ptr=a.begin();
    os<<*ptr;
    for(++ptr;ptr!=a.end();++ptr) os<<','<<*ptr;
  }
  return os<<']'; 
}




#endif
