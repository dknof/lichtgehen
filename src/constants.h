/*
   tronbot

   Copyright (C) 2014 by Diether Knof

   This file is part of tronbot.

   Tronbot is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   Software Foundation, either version 3 of the License, or
   your option) any later version.

   Tronbot is distributed in the hope that it will be useful,
   but without any warranty; without even the implied warranty of
   merchantability or fitness for a particular purpose.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

   Diese Datei ist Teil von tronbot.

   Trobot ist Freie Software: Sie können es unter den Bedingungen
   der GNU General Public License, wie von der Free Software Foundation,
   Version 3 der Lizenz oder (nach Ihrer Wahl) jeder späteren
   veröffentlichten Version, weiterverbreiten und/oder modifizieren.

   Trobot wird in der Hoffnung, dass es nützlich sein wird, aber
   ohne jede Gewährleistung, bereitgestellt; sogar ohne die implizite
   Gewährleistung der Marktfähigkeit oder Eignung für einen bestimmten Zweck.
   Siehe die GNU General Public License für weitere Details.

   Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
   Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.
   */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::ostream;
using std::istream;

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <array>
using std::array;
#include <memory>
using std::unique_ptr;
#ifndef __cpp_lib_make_unique
// unique_ptr implementation -*- C++ -*-
// vom gcc
namespace std {
  template<typename _Tp>
    struct _MakeUniq
    { typedef unique_ptr<_Tp> __single_object; };

  template<typename _Tp>
    struct _MakeUniq<_Tp[]>
    { typedef unique_ptr<_Tp[]> __array; };

  template<typename _Tp, size_t _Bound>
    struct _MakeUniq<_Tp[_Bound]>
    { struct __invalid_type { }; };

  /// std::make_unique for single objects
  template<typename _Tp, typename... _Args>
    inline typename _MakeUniq<_Tp>::__single_object
    make_unique(_Args&&... __args)
    { return unique_ptr<_Tp>(new _Tp(std::forward<_Args>(__args)...)); }

  /// std::make_unique for arrays of unknown bound
  template<typename _Tp>
    inline typename _MakeUniq<_Tp>::__array
    make_unique(size_t __num)
    { return unique_ptr<_Tp>(new typename remove_extent<_Tp>::type[__num]()); }

  /// Disable std::make_unique for arrays of known bound
  template<typename _Tp, typename... _Args>
    inline typename _MakeUniq<_Tp>::__invalid_type
    make_unique(_Args&&...) = delete;
} // namespace std
#endif

using std::make_unique;

#include <cassert>

extern std::ostream* cdebug_;
#define cdebug (*cdebug_)
#define CLOG cdebug << __FILE__ << '#' << __LINE__ << ' '

namespace std {
  template<class X>
inline X min(X const& a, X const& b, X const& c)
{ return std::min(a, std::min(b, c)); }
  template<class X>
inline X min(X const& a, X const& b, X const& c, X const& d)
{ return std::min(std::min(a, b), std::min(c, d)); }
  template<class X>
inline X max(X const& a, X const& b, X const& c)
{ return std::max(a, std::max(b, c)); }
  template<class X>
inline X max(X const& a, X const& b, X const& c, X const& d)
{ return std::max(std::max(a, b), std::max(c, d)); }
} // namespace std

// Zeitbeschränkung in Sekunden
#ifndef ZEITBESCHRAENKUNG
#define ZEITBESCHRAENKUNG 1
#endif
// Die Tiefe bei der Tiefensuche dynamisch ermitteln
#define TIEFE_DYNAMISCH

// creates a segmentation fault
#define SEGFAULT if (true) { cerr << "Created segmentation fault:\n" \
  << __FILE__ << " # " << __LINE__ << endl; \
  abort(); } else (void)0

#endif // #ifndef CONSTANTS_H
