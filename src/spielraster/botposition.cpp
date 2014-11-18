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

#include "botposition.h"

/**
 ** Konstruktor
 ** 
 ** @param     position        Position
 ** @param     richtung   Richtung
 **
 ** @return    -
 **
 ** @version   2014-10-25
 **/
BotPosition::BotPosition(Position const& position, Richtung const richtung) :
  Position{position}, richtung_{richtung}
{ }

/**
 ** Konstruktor
 ** 
 ** @param     istr   Einlesestrom
 **
 ** @return    Einlesestrom
 **
 ** @version   2014-10-25
 **/
BotPosition::BotPosition(istream& istr) :
  Position{istr}, richtung_{Richtung::NORDEN}
{
  while (istr.good() && isspace(istr.peek()))
    istr.get();

  auto const c = istr.get();
  switch (c) {
  case 'N':
    this->richtung_ = Richtung::NORDEN;
    break;
  case 'E':
    this->richtung_ = Richtung::OSTEN;
    break;
  case 'S':
    this->richtung_ = Richtung::SUEDEN;
    break;
  case 'W':
    this->richtung_ = Richtung::WESTEN;
    break;
  default:
    cerr << "Richtung '" << c << "' unbekannt.\n";
  } // switch (c)
} // BotPosition::BotPosition(istream& istr)

/**
 ** Die BotPosition auf ostr ausgeben
 ** 
 ** @param     ostr         Ausgabestrom
 ** @param     botposition  BotPosition zum ausgeben
 **
 ** @return    Ausgabestrom
 **
 ** @version   2014-10-25
 **/
ostream&
operator<<(ostream& ostr, BotPosition const& botposition)
{ 
  ostr << static_cast<Position const&>(botposition) << ' ' << botposition.richtung();
  return ostr;
} // ostream& operator<<(ostream& ostr, BotPosition const& botposition)

/**
 ** -> Rückgabe
 ** 
 ** @param     -
 **
 ** @return    die Richtung
 **
 ** @version   2014-10-25
 **/
Richtung
BotPosition::richtung() const
{
  return this->richtung_;
} // Richtung BotPosition::richtung() const

/**
 ** Bewegt die BotPosition entsprechend bewegungsrichtung
 ** 
 ** @param     bewegungsrichtung   Bewegungsrichtung
 **
 ** @return    die neue Position
 **
 ** @version   2014-10-25
 **/
BotPosition&
BotPosition::operator+=(Bewegungsrichtung const bewegungsrichtung)
{
  this->richtung_ += bewegungsrichtung;
  static_cast<Position&>(*this) += this->richtung();
  return *this;
} // BotPosition& BotPosition::operator+=(Bewegungsrichtung const bewegungsrichtung)

/**
 ** -> Rückgabe
 ** 
 ** @param     lhs   Position
 ** @param     rhs   Bewegungsrichtung
 **
 ** @return    die Position um einen Schritt erweitert weiter
 **
 ** @version   2014-11-13
 **/
BotPosition
operator+(BotPosition bp, Bewegungsrichtung const rhs)
{
  return (bp += rhs);
} // BotPosition operator+(Botposition lhs, Bewegungsrichtung const rhs)
