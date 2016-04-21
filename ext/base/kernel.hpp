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

#include "../../module.hpp"

using namespace Gomu;

//! Assigneme function
//! \param lhs left hand side of assignment
//! \param rhs right hand side of assignment
Value assignment(Context&,Value& lhs,Value& rhs);

//! Unload a symbol
Value del(Context&,Value&);

//! Return the type of a value
Value type(Context&,Value&);

//! Return an array of member functions of a type
Value symbols(Context&,Value&);
