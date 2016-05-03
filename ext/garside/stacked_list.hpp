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

#ifndef STACKED_LIST_HPP
#define STACKED_LIST_HPP

#include <iostream>
#include <cstdint>
#include <deque>
#include <stack>
#include <cassert>

using namespace std;

#define SIZE 1024

typedef uint16_t NInd;
typedef int16_t NData;


//**********************
//* Class declarations *
//**********************

//! Node of a StackedList
class StackedListNode{
public:
  StackedListNode();
  NInd previous,next;
  NData data;
};

//! A Both directionnal list structure based on stacks
class StackedList{
public:

private:
  //! Fill the current StackedList with ann arry of size s
  void fill(NData* array,size_t s);

  //Create a new node for the Stacked lisy
  //\return index of the new created node
  NInd new_node();

public:
  //! Size of the stacked list 0 for empty
  //! Attention, there is exactly size+1 nodes used in the list
  size_t size;
  
  //! An array of Node used or not in the list
  StackedListNode nodes[SIZE];

  //! Indices of all available nodes
  NInd indices[SIZE];

  //! The empty constructor
  StackedList();

  //! Construct a StackedList from an array of size s
  StackedList(NData* array,size_t s);

  //! Clear the current StackedList
  void clear();
  
  //! Erase the node i in the list
  void erase(NInd i);

  //! Return first data of the list
  NData first();
  
  //! Show all information of the nodes
  void full_show() const;

  //! Init the current StackedList to be of size s
  void init(size_t s);
  
  //! Init the current StackedList with an array of size s
  void init(NData* array,size_t s);

  //! Insert a new node with data before the node i
  //! \return index of the newly inserted node
  NInd insert_before(NInd i,NData data);

  //! Insert a new node with data aftre the node i
  //! \return index of the newly inserted node
  NInd insert_after(NInd i,NData data);

  //! Test if the StackedList is empty
  bool is_empty() const;

  //! Return last data of the list
  NData last();
};


//***********************
//* Auxiliary Functions *
//***********************

ostream& operator<<(ostream& os,const StackedList&);

//**********************
//* Inline definitions *
//**********************

//-------------------
// StackedList::Node
//-------------------

inline
StackedListNode::StackedListNode(){}

//-------------
// StackedList
//-------------

inline
StackedList::StackedList(){
  clear();
  nodes[0].data=0;
  nodes[0].previous=0;
  nodes[0].next=0;
}

inline
StackedList::StackedList(NData* array,size_t s){
  clear();
  fill(array,s);
}

inline void
StackedList::clear(){
  size=0;
  for(size_t i=0;i<SIZE;++i) indices[i]=i;
  nodes[0].previous=0;
  nodes[0].next=0;
}

inline void
StackedList::init(NData* array,size_t s){
  clear();
  fill(array,s);
}

inline bool
StackedList::is_empty() const{
  return size==0;
}

inline NData
StackedList::first(){
  return nodes[nodes[0].next].data;
}

inline NData
StackedList::last(){
  return nodes[nodes[0].previous].data;
}

inline NInd
StackedList::new_node(){
  return indices[++size];
}


#endif
