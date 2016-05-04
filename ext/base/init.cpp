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

extern "C"{

  bool True=true;
  bool False=false;
  
  Gomu::Type types[]={
    TYPE_SENTINEL
  };
 
  //--- Functions ---//
  Gomu::Module::Function functions[]={
    {"Integer","len",{"String"},(void*)string_len},
    {"Integer","len",{"Array"},(void*)array_len},
    {"Integer","negate",{"Integer"},(void*)integer_negate},
    {"Integer","operator+",{"Integer","Integer"},(void*)integer_add},
    {"Integer","operator*",{"Integer","Integer"},(void*)integer_mul},
    {"Integer","operator/",{"Integer","Integer"},(void*)integer_quo},
    {"Integer","operator%",{"Integer","Integer"},(void*)integer_rem},
    {"Integer","operator-",{"Integer","Integer"},(void*)integer_sub},
    FUNC_SENTINEL
  };
  
  //--- Member functions ---//
  Gomu::Module::Function member_functions[]={
    {"Integer","len",{"String"},(void*)string_len},
    {"Array","types",{"Module"},(void*)module_types},
    FUNC_SENTINEL
  };
  
  //--- Contextual functions ---//
  Gomu::Module::Function contextual_functions[]={
    {"Void","check",{"Module"},(void*)module_check},
    {"Void","delete",{"Symbol"},(void*)del},
    {"Void","execute",{"String"},(void*)execute},
    {"Generic","operator=",{"Symbol","Generic"},(void*)assignment},
    {"Boolean","operator==",{"Generic","Generic"},(void*)equality},
    {"Array","symbols",{"Type"},(void*)member_symbols},
    {"Type","type",{"Generic"},(void*)type},
    FUNC_SENTINEL
  };


  Gomu::Module::Symbol symbols[]={
    {"true","Boolean",(void*)&True},
    {"false","Boolean",(void*)&False},
    SYMB_SENTINEL
  };
}

