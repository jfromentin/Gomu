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

#include "stacked_list.hpp"

//***************
//* StackedList *
//****************


//--------------------------
// StackedList::erase(NInd)
//--------------------------

void StackedList::erase(NInd i){
  NInd p=nodes[i].previous;
  NInd n=nodes[i].next;
  nodes[p].next=n;
  nodes[n].previous=p;
  indices[size--]=i;
}

//----------------------------------
// StackedList::fill(NData*,size_t)
//----------------------------------

void
StackedList::fill(NData* array,size_t s){
  size=s;
  nodes[0].data=0;
  nodes[0].previous=s;
  nodes[0].next=1;
  for(size_t ind=0;ind<s;++ind){
    nodes[ind+1].previous=ind;
    nodes[ind+1].next=ind+2;
    nodes[ind+1].data=array[ind];
  }
  nodes[s].next=0;
}

//---------------------------
// StackedList::init(size_t)
//--------------------------

void
StackedList::init(size_t s){
  clear();
  size=s;
  nodes[0].previous=s;
  nodes[0].next=1;
  for(size_t ind=0;ind<s;++ind){
    nodes[ind+1].previous=ind;
    nodes[ind+1].next=ind+2;
  }
  nodes[s].next=0;
}


//----------------------------------------
// StackedtList::insert_after(NInd,NData)
//----------------------------------------

NInd
StackedList::insert_after(NInd i,NData data){
  // p <-> i <-> n becomes p <-> i <-> j <-> n
  NInd j=new_node();
  NInd n=nodes[i].next;
  nodes[j].previous=i;
  nodes[j].next=n;
  nodes[j].data=data;
  nodes[i].next=j;
  nodes[n].previous=j;
  return j;
}

//----------------------------------------
// StackedList::insert_before(NInd,Ndata)
//----------------------------------------

NInd
StackedList::insert_before(NInd i,NData data){
  //p <-> i <-> n becomes p <-> j <-> i <-> n
  NInd j=new_node();
  NInd p=nodes[i].previous;
  nodes[j].previous=p;
  nodes[j].next=i;
  nodes[j].data=data;
  nodes[i].previous=j;
  nodes[p].next=j;
  return j;
}

void
StackedList::full_show() const{
  NInd ind=0;
  size_t i=0;
  while(i<10){
    cout<<"@"<<ind<<" : "<<nodes[ind].previous<<"<- "<<nodes[ind].data<<" -> "<<nodes[ind].next<<endl;
    ind=nodes[ind].next;
    if(ind==0) return;
    ++i;
  }
  cout<<"@"<<ind<<" : "<<nodes[ind].previous<<"<- "<<nodes[ind].data<<" -> "<<nodes[ind].next<<endl;
}

//***********************
//* Auxiliary Functions *
//***********************

ostream& operator<<(ostream& os,const StackedList& list){
  os<<" Stack =";
  for(size_t i=0;i<=list.size;++i) os<<' '<<list.indices[i];
  os<<'#';
  for(size_t i=list.size+1;i<=20;++i) os<<' '<<list.indices[i];
  os<<endl;
  if(list.is_empty()) return os<<"()";
  NInd ind=list.nodes[0].next;
  os<<'('<<list.nodes[ind].data;
  size_t i=0;
  while((ind=list.nodes[ind].next)!=0){
    os<<','<<list.nodes[ind].data;
    ++i;
    if(i>10) return os<<"...)";
  }
  return os<<')';
};
