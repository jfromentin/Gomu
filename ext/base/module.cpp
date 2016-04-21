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

#include "module.hpp"
#include "../../interpreter.hpp"

//---------------------
// module_types(void*)
//---------------------

void* module_types(void* v){
  Module* module=(Module*)v;
  ArrayValue *res=new ArrayValue(module->ntype);
  res->type=type_string;
  for(size_t i=0;i<module->ntype;++i){
    res->tab[i]=new string(module->types[i]);
  }
  return res;
}


