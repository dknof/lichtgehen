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

#include "position.h"

/**
 ** Konstruktor
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2014-10-25
 **/
Position::Position(int const x, int const y) :
  x_{x}, y_{y}
{ }

/**
 ** Konstruktor aus String
 ** 
 ** @param     istr   Einlesestream
 **
 ** @return    -
 **
 ** @version   2014-10-25
 **/
Position::Position(istream& istr)
{
  istr >> x_;
  istr.get();
  istr >> y_;
  --this->x_;
  --this->y_;
} // Position::Position(istream& istr)

/**
 ** Die Position auf ostr ausgeben
 ** 
 ** @param     ostr   Ausgabestrom
 ** @param     position  Position zum ausgeben
 **
 ** @return    Ausgabestrom
 **
 ** @version   2014-10-25
 **/
ostream&
operator<<(ostream& ostr, Position const& position)
{ 
  ostr << position.x() << ',' << position.y();
  return ostr;
} // ostream& operator<<(ostream& ostr, Position const& position)

/**
 ** -> Rückgabe
 ** 
 ** @param     -
 **
 ** @return    die x-Position
 **
 ** @version   2014-10-25
 **/
int
Position::x() const
{
  return this->x_;
} // int Position::x() const

/**
 ** -> Rückgabe
 ** 
 ** @param     -
 **
 ** @return    die y-Position
 **
 ** @version   2014-10-25
 **/
int
Position::y() const
{
  return this->y_;
} // int Position::y() const

/**
 ** -> Rückgabe
 ** 
 ** @param     -
 **
 ** @return    ob dies eine gültige Position ist
 **
 ** @version   2014-11-08
 **/
Position::operator bool() const
{
  return ((this->x() >= 0) && (this->y() >= 0));
} // Position::operator bool() const


/**
 ** verändert die Position entsprechend 'richtung'
 ** 
 ** @param     richtung    Richtung 
 **
 ** @return    die neue Position
 **
 ** @version   2014-10-25
 **/
Position&
Position::operator+=(Richtung const richtung)
{
  switch (richtung) {
  case Richtung::NORDEN:
    this->y_ -= 1;
    break;
  case Richtung::OSTEN:
    this->x_ += 1;
    break;
  case Richtung::SUEDEN:
    this->y_ += 1;
    break;
  case Richtung::WESTEN:
    this->x_ -= 1;
    break;
  };
  return *this;
} // Position& Position::operator+=(Richtung const richtung)

/**
 ** -> Rückgabe
 ** 
 ** @param     pa   erste Position
 ** @param     pb   zweite Position
 **
 ** @return    ob die beiden Positionen gleich sind
 **
 ** @version   2014-10-27
 **/
bool
operator==(Position const& pa, Position const& pb)
{
  return (   (pa.x() == pb.x())
          && (pa.y() == pb.y()) );
} // bool operator==(Position const& pa, Position const& pb)

/**
 ** -> Rückgabe
 ** 
 ** @param     pa   erste Position
 ** @param     pb   zweite Position
 **
 ** @return    ob die erste Position vor der zweiten liegt
 **
 ** @version   2014-10-31
 **/
bool
operator<(Position const& pa, Position const& pb)
{
  return (   (pa.y() < pb.y())
          || (   (pa.y() == pb.y())
              && (pa.x() < pb.x()) ) );
} // bool operator<(Position const& pa, Position const& pb)

/**
 ** -> Rückgabe
 ** 
 ** @param     lhs    Position 
 ** @param     rhs    Richtung 
 **
 ** @return    die um Richtung geänderte Position
 **
 ** @version   2014-10-25
 **/
Position
operator+(Position lhs, Richtung const rhs)
{
  return (lhs += rhs);
} // Position operator+(Position lhs, Richtung const rhs)
