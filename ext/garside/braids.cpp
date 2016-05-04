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
MonoidFamily DualA_mf("Dual A-type",DualA_disp,DualA_gnum);

//***********************
//* Auxiliary functions *
//***********************

//---------------
// braids_init()
//---------------

void braids_init(){
  ArtinA_mf.data=(void*)type_ArtinWordA;
  ArtinA_mf.set_left_complement(&ArtinA_left_sc);
  ArtinA_mf.set_right_complement(&ArtinA_right_sc);
  ArtinA_mf.set_ranked_phi_germ(&ArtinA_rpg);
  ArtinA_mf.set_ranked_garside_word_factory(&ArtinA_rgwf);
  
  DualA_mf.data=(void*)type_DualWordA;
  DualA_mf.set_left_complement(&DualA_left_sc);
  DualA_mf.set_right_complement(&DualA_right_sc);
  DualA_mf.set_ranked_phi_germ(&DualA_rpg);
  DualA_mf.set_ranked_garside_word_factory(&DualA_rgwf);
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

//----------------------------------
// ArtinA_rpg(size_t,Generator,int)
//----------------------------------

Generator ArtinA_rpg(size_t r,const Generator& x,int p){
  int power=abs(p)%2;
  if(power==0) return x;
  if(x>0) return r-x;
  return -(r+x);
}

//---------------------
// ArtinA_rgwf(size_t)
//---------------------

Word ArtinA_rgwf(size_t r){
  Word res(r*(r+1)/2);
  size_t ind=0;
  for(size_t n=r;n>0;--n){
    for(size_t i=1;i<=n;++i){
      res[ind++]=i;
    }
  }
  return res;
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

//---------------------------------
// DualA_rpg(size_t,Generator,int)
//---------------------------------

Generator DualA_rpg(size_t r,const Generator& x,int p){
  int n=r+1;
  int power=p%n;
  if(power<0) power+=n;
  int sign,absx;
  if(x>0){
    sign=1;
    absx=x;
  }
  else{
    sign=-1;
    absx=-x;
  }
  int i=(get_i(absx)-1+power)%n+1;
  int j=(get_j(absx)-1+power)%n+1;
  if(i>j) swap(i,j);
  return sign*generator(i,j);
}

//--------------------
// DualA_rgwf(size_t)
//--------------------

Word DualA_rgwf(size_t r){
  Word res(r);
  for(size_t i=0;i<r;++i){
    res[i]=generator(i+1,i+2);
  }
  return res;
}
