///////////////////////////////////////////////////////////////////////////
// Copyright (C) 2009  Whit Armstrong                                    //
//                                                                       //
// This program is free software: you can redistribute it and/or modify  //
// it under the terms of the GNU General Public License as published by  //
// the Free Software Foundation, either version 3 of the License, or     //
// (at your option) any later version.                                   //
//                                                                       //
// This program is distributed in the hope that it will be useful,       //
// but WITHOUT ANY WARRANTY; without even the implied warranty of        //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         //
// GNU General Public License for more details.                          //
//                                                                       //
// You should have received a copy of the GNU General Public License     //
// along with this program.  If not, see <http://www.gnu.org/licenses/>. //
///////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include "r.objects.hpp"
#include "Rutilities.hpp"

using std::vector;
using std::string;
using std::stringstream;
using std::cout;
using std::cerr;
using std::endl;


Robject* Robject::factory(SEXP x) {
  SEXP r_class = getAttrib(x,R_ClassSymbol);
  const std::string r_class_str(CHAR(STRING_ELT(r_class,0)));
  if(r_class == R_NilValue) {
    //throw()
    return NULL;
  }
  if(r_class_str == "matrix") {
    switch(TYPEOF(x)) {
    case INTSXP: return new RMatrix<int>(x);
    case REALSXP: return new RMatrix<double>(x);
    case LGLSXP: return new RMatrix<bool>(x);
    case CHARSXP: return new RMatrix<string>(x);
    default: return NULL;
    }
  }

  if(r_class_str == "data.frame") {
    return new RDataFrame(x);
  }

  switch(TYPEOF(x)) {
  case INTSXP: return new RVector<int>(x);
  case REALSXP: return new RVector<double>(x);
  case LGLSXP: return new RVector<bool>(x);
  case CHARSXP: return new RVector<string>(x);
  default: return NULL;
  }

  // throw !bad_object_creation
  return NULL;
}

bool RDataFrame::hasRownames() const {
  return (getRownames() == R_NilValue) ? false : true;
}

bool RDataFrame::hasColnames() const {
  return (getColnames() == R_NilValue) ? false : true;
}

SEXP RDataFrame::getRownames() const {
  return getAttrib(sexp_, install("row.names"));
}

SEXP RDataFrame::getColnames() const {
  return getAttrib(sexp_, R_NamesSymbol);
}

void RDataFrame::getColnames(std::vector<std::string>& ans) const {
  sexp2string(getColnames(),back_inserter(ans));
}

void RDataFrame::getSEXPS(std::vector<SEXP>& ans) const {
  for(R_len_t i = 0; i < length(sexp_); i++) {
    ans.push_back(VECTOR_ELT(sexp_,i));
  }
}

R_len_t RDataFrame::nrow() const {
  if(sexp_==R_NilValue) {
    return 0;
  }
  return length(VECTOR_ELT(sexp_,0));
}

R_len_t RDataFrame::ncol() const {
  if(sexp_==R_NilValue) {
    return 0;
  }
  return length(sexp_);
}

template<typename T>
bool RMatrix<T>::hasRownames() const {
  return (getRownames() == R_NilValue) ? false : true;
}

template<typename T>
bool RMatrix<T>::hasColnames() const {
  return (getColnames() == R_NilValue) ? false : true;
}

template<typename T>
SEXP RMatrix<T>::getRownames() const {
  SEXP dimnames = getAttrib(sexp_, R_DimNamesSymbol);

  if(dimnames==R_NilValue) {
    return R_NilValue;
  }

  return VECTOR_ELT(dimnames, 0);
}

template<typename T>
SEXP RMatrix<T>::getColnames() const {
 SEXP dimnames = getAttrib(sexp_, R_DimNamesSymbol);

  if(dimnames==R_NilValue) {
    return R_NilValue;
  }
  return VECTOR_ELT(dimnames, 1);
}

template<typename T>
void RMatrix<T>::getColnames(std::vector<std::string>& ans) const {
  sexp2string(getColnames(),back_inserter(ans));
}

template<typename T>
void RMatrix<T>::getSEXPS(std::vector<SEXP>& ans) const {
  for(R_len_t i = 0; i < length(sexp_); i++) {
    ans.push_back(sexp_);
  }
}

template<typename T>
R_len_t RMatrix<T>::nrow() const {
  if(sexp_==R_NilValue) {
    return 0;
  }

  return nrows(sexp_);
}

template<typename T>
R_len_t RMatrix<T>::ncol() const {
  if(sexp_==R_NilValue) {
    return 0;
  }
  return ncols(sexp_);
}

template<typename T>
bool RVector<T>::hasRownames() const {
  return (getRownames() == R_NilValue) ? false : true;
}

template<typename T>
bool RVector<T>::hasColnames() const {
  return false;
}

template<typename T>
SEXP RVector<T>::getColnames() const {
  return R_NilValue;
}

template<typename T>
SEXP RVector<T>::getRownames() const {
  return getAttrib(sexp_, R_NamesSymbol);
}

template<typename T>
void RVector<T>::getColnames(std::vector<std::string>& ans) const {
  return;
}

template<typename T>
void RVector<T>::getSEXPS(std::vector<SEXP>& ans) const {
    ans.push_back(sexp_);
}

template<typename T>
R_len_t RVector<T>::nrow() const {
  if(sexp_==R_NilValue) {
    return 0;
  }
  return length(sexp_);
}

template<typename T>
R_len_t RVector<T>::ncol() const {
  if(sexp_==R_NilValue) {
    return 0;
  }
  return 1;
}
