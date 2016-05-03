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

#include "braids.hpp"
#include "init.hpp"

//******************
//* Global objects *
//******************

MonoidFamily ArtinA_mf("Artin A-type",ArtinA_disp,ArtinA_gnum);
MonoidFamily dualA_mf("Dual A-type",DualA_disp,DualA_gnum);

//***********************
//* Auxiliary functions *
//***********************

//---------------
// braids_init()
//---------------

void braids_init(){
  ArtinA_mf.set_left_complement(&ArtinA_left_sc);
  ArtinA_mf.set_right_complement(&ArtinA_right_sc);
  ArtinA_mf.data=(void*)type_ArtinWordA;
  dualA_mf.set_left_complement(&DualA_left_sc);
  dualA_mf.set_right_complement(&DualA_right_sc);
  dualA_mf.data=(void*)type_DualWordA;
}

//------------------------------------------------
// ArtinA_left_sc(Generator,Generator,Generator*)
//------------------------------------------------

int ArtinA_left_sc(const Generator& x,const Generator& y,Generator* comp){
  //Comp statisfy comp*x=...*y
  if(x==y) return 0;
  if(abs(x-y)==1){
    comp[0]=x;
    comp[1]=y;
    return 2;
  }
  else{
    comp[0]=y;
    return 1;
  }
}

//-------------------------------------------------
// ArtinA_rigth_sc(Generator,Generator,Generator*)
//-------------------------------------------------

int ArtinA_right_sc(const Generator& x,const Generator& y,Generator* comp){
  //Comp satisfy x*comp=y*...
  if(x==y) return 0;
  if(abs(x-y)==1){
    comp[0]=y;
    comp[1]=x;
    return 2;
  }
  else{
    comp[0]=y;
    return 1;
  }
}

//--------------------
// DualA_gnum(size_t)
//--------------------

inline size_t
DualA_gnum(size_t n){
  size_t res=0;
  for(size_t i=1;i<n;++i){
    res+=i;
  }
  return res;
}

//-----------------------------------------------
// DualA_left_sc(Generator,Generator,Generator*)
//-----------------------------------------------

int DualA_left_sc(const Generator& x,const Generator& y,Generator* comp){
  //Case 1
  if(x==y) return 0;
  uint p=get_i(x);
  uint q=get_j(x);
  uint r=get_i(y);
  uint s=get_j(y);
  //Case 5 and 6
  if(p==r){
    comp[0]=(q<s)?y:generator(s,q);
    return 1;
  }
  //Case 7 and 8
  if(q==s){
    comp[0]=(p<r)?y:generator(r,p);
    return 1;
  }
  //Case 3
  if(p==s){
    comp[0]=y;
    return 1;
  }
  //Case 5
  if(q==r){
    comp[0]=generator(p,s);
    return 1;
  }
  //Case 9
  if(p<r and r<q and q<s){
    comp[0]=generator(r,q);
    comp[1]=generator(p,s);
    return 2;
  }
  //Case 10
  if(r<p and p<s and s<q){
    comp[0]=generator(s,q);
    comp[1]=generator(r,p);
    return 2;
  }
  //Case 2
  comp[0]=y;
  return 1;
}

//------------------------------------------------
// DualA_right_sc(Generator,Generator,Generator*)
//------------------------------------------------

int DualA_right_sc(const Generator& x,const Generator& y,Generator* comp){
  //Case 1
  if(x==y) return 0;
  uint p=get_i(x);
  uint q=get_j(x);
  uint r=get_i(y);
  uint s=get_j(y);
  //Case 5 and 6
  if(p==r){
    comp[0]=(q<s)?generator(q,s):y;
    return 1;
  }
  //Case 7 and 8
  if(q==s){
    comp[0]=(p<r)?generator(p,r):y;
    return 1;
  }
  //Case 3
  if(p==s){
    comp[0]=generator(r,q);
    return 1;
  }
  //Case 5
  if(q==r){
    comp[0]=y;
    return 1;
  }
  //Case 9
  if(p<r and r<q and q<s){
    comp[0]=generator(q,s);
    comp[1]=generator(p,r);
    return 2;
  }
  //Case 10
  if(r<p and p<s and s<q){
    comp[0]=generator(p,s);
    comp[1]=generator(r,q);
    return 2;
  }
  //Case 2
  comp[0]=y;
  return 1;
}
