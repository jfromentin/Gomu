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

#ifndef DICTIONNARY_HPP
#define DICTIONNARY_HPP

#include <iostream>
#include <string>
#include <deque>

using namespace std;

//**********************
//* Class declarations *
//**********************

//! Class representing a dictionnary node
template<class T> class DictionnaryNode{
public:

  //!Info attached to a DictionnaryNode
  T* info;

  //!Sons of the dictionnary node
  deque<DictionnaryNode<T>*> sons;

  //! Letter of the node
  char letter;

  //! The empty constructor
  DictionnaryNode();

  //! Destructor
  ~DictionnaryNode();

  //! Add a word to the dictionnary at this node
  //! \param op the word to addd
  //! \param info information attached to the word
  void add(string op,T* info);

  //! Display word obatained from this node preceded by a prefix
  //! \param preffix the preffix of the node (ie the path from the root to the current node)
  void disp(const string& preffix) const;
};

//! A class representing a dictionnary
template<class T> class Dictionnary{
private:
  //! Root node
  DictionnaryNode<T>* root;
public:
  //! Empty constructor
  Dictionnary();

  //! Destructor
  ~Dictionnary();

  //! Add a word to the distionnary
  //! \param op the word to add
  //! \param indo information attached to the word
  //! \retrun Return true if the word has bee added, false otherwise
  bool add(string op,T* info);

  //! Display the dictionnary
  void disp() const;

  //! Find a maximal preffix of a suffix of a word in the dictionnary
  //! \param pos position of the first letter of the suffix
  //! \param str the full word
  //! \return Information attached to a maximal preffix or nullptr otherwise
  T* find_at(size_t& pos,const string& str);
};

//*************************
//* Function declarations *
//*************************

//------------------------------------
// DictionnaryNode::DictionnaryNode()
//------------------------------------

template<class T> inline
DictionnaryNode<T>::DictionnaryNode(){
  info=nullptr;
}

//------------------------------------
// DictionnaryNode::~DictionnaryNode()
//------------------------------------

template<class T>
DictionnaryNode<T>::~DictionnaryNode(){
  if(info!=nullptr) delete info;
  for(auto it=sons.begin();it!=sons.end();++it) delete *it;
}

//--------------------------------------
// DictionnaryNode::disp(const string&)
//--------------------------------------

template<class T> void
DictionnaryNode<T>::disp(const string& preffix) const{
  string npreffix=preffix+letter;
  if(info!=nullptr) cout<<npreffix<<" : "<<info->func<<endl;
  for(auto it=sons.begin();it!=sons.end();++it) (*it)->disp(npreffix);
}

//----------------------------
// Dictionnary::Dictionnary()
//----------------------------

template<class T> inline
Dictionnary<T>::Dictionnary(){
  root=new DictionnaryNode<T>();
}

//----------------------------
// Dictionnary::~Dictionnary()
//----------------------------

template<class T> inline
Dictionnary<T>::~Dictionnary(){
  if(root!=nullptr) delete root;
}

//---------------------
// Dictionnary::disp()
//---------------------

template<class T> inline void
Dictionnary<T>::disp() const{
  for(auto it=root->sons.begin();it!=root->sons.end();++it) (*it)->disp("");
}

//-----------------------------
// Dictionnary::add(string,T*)
//-----------------------------

template<class T> bool
Dictionnary<T>::add(string op,T* info){
  DictionnaryNode<T>* cur=root;
  size_t pos=0;
  size_t end=op.length();
  //Search maximal prefix
  while(pos<end){
    char l=op[pos];
    auto it=cur->sons.begin();
      for(;it!=cur->sons.end();++it){
      if((*it)->letter==l) break;
    }
    if(it==cur->sons.end()) break;//the letter not occur
    cur=*it;
    ++pos;
  }
  //If the prefix is the whole word
  if(pos==end){
    if(cur->info!=nullptr){
      cout<<"Warning : entry "<<op<<" already exists"<<endl;
      return false;
    }
    else{
      cur->info=info;
    }
  }
  //Adding suffix to dictionnary
  while(pos<end){
    DictionnaryNode<T>* ncur=new DictionnaryNode<T>();
    ncur->letter=op[pos];
    cur->sons.push_back(ncur);
    cur=ncur;
    ++pos;
  }
  cur->info=info;
  return true;
}

//---------------------------------------------
// Dictionnary::find_at(size_t&,const string&)
//---------------------------------------------

template<class T> T*
Dictionnary<T>::find_at(size_t& pos,const string& str){
  size_t end=str.length();
  size_t npos=pos;
  string path="";
  T* res=nullptr;
  DictionnaryNode<T>* cur=root;
  while(pos<end){
    char l=str[npos];
    auto it=cur->sons.begin();
    for(;it!=cur->sons.end();++it){
      if((*it)->letter==l) break;
    }
    if(it==cur->sons.end()) break;//the letter not occur
    cur=*it;
    path+=l;
    if(cur->info!=nullptr){
      res=cur->info;
      pos=npos+1;
    }
    ++npos;
  }
  return res;
}

#endif

