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
#include <memory>
using std::unique_ptr;
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
}


// creates a segmentation fault
#define SEGFAULT if (true) { cerr << "Created segmentation fault:\n" \
  << __FILE__ << " # " << __LINE__ << endl; \
  abort(); } else (void)0

#endif // #ifndef CONSTANTS_H
