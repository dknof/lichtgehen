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

#include "richtung.h"

// alle Richtungen
std::array<Richtung, 4> const richtungen = {{Richtung::NORDEN, Richtung::OSTEN, Richtung::SUEDEN, Richtung::WESTEN}};
// alle Bewegungsrichtungen
std::array<Bewegungsrichtung, 3> const bewegungsrichtungen = {{Bewegungsrichtung::VORWAERTS, Bewegungsrichtung::LINKS, Bewegungsrichtung::RECHTS}};

/**
 ** Die Richtung auf ostr ausgeben
 ** 
 ** @param     ostr      Ausgabestrom
 ** @param     richtung  Richtung zum ausgeben
 **
 ** @return    Ausgabestrom
 **
 ** @version   2014-10-25
 **/
ostream&
operator<<(ostream& ostr, Richtung const& richtung)
{ 
  switch (richtung) {
  case Richtung::NORDEN:
    ostr << 'N';
    break;
  case Richtung::OSTEN:
    ostr << 'E';
    break;
  case Richtung::SUEDEN:
    ostr << 'S';
    break;
  case Richtung::WESTEN:
    ostr << 'W';
    break;
  } // switch (richtung);
  return ostr;
} // ostream& operator<<(ostream& ostr, Richtung const& richtung)

/**
 ** Verändert die Richtung im Uhrzeigersinn
 ** 
 ** @param     richtung    Richtung 
 **
 ** @return    die geänderte Richtung
 **
 ** @version   2014-10-25
 **/
Richtung&
operator++(Richtung& richtung)
{
  switch (richtung) {
  case Richtung::NORDEN:
    richtung = Richtung::OSTEN;
    break;
  case Richtung::OSTEN:
    richtung = Richtung::SUEDEN;
    break;
  case Richtung::SUEDEN:
    richtung = Richtung::WESTEN;
    break;
  case Richtung::WESTEN:
    richtung = Richtung::NORDEN;
    break;
  };
  return richtung;
} // Richtung& operator++(Richtung& richtung)

/**
 ** Verändert die Richtung gegen den Uhrzeigersinn
 ** 
 ** @param     richtung    Richtung 
 **
 ** @return    die geänderte Richtung
 **
 ** @version   2014-10-25
 **/
Richtung&
operator--(Richtung& richtung)
{
  switch (richtung) {
  case Richtung::NORDEN:
    richtung = Richtung::WESTEN;
    break;
  case Richtung::OSTEN:
    richtung = Richtung::NORDEN;
    break;
  case Richtung::SUEDEN:
    richtung = Richtung::OSTEN;
    break;
  case Richtung::WESTEN:
    richtung = Richtung::SUEDEN;
    break;
  };
  return richtung;
} // Richtung& operator--(Richtung& richtung)

/**
 ** Ändert richtung entsprechend bewegungsrichtung
 ** 
 ** @param     richtung            Richtung 
 ** @param     bewegungsrichtung   Bewegungsrichtung
 **
 ** @return    geänderte Richtung
 **
 ** @version   2014-10-25
 **/
Richtung&
operator+=(Richtung& richtung, Bewegungsrichtung const bewegungsrichtung)
{
  switch (bewegungsrichtung) {
  case Bewegungsrichtung::VORWAERTS:
    return richtung;
  case Bewegungsrichtung::RECHTS:
    return ++richtung;
  case Bewegungsrichtung::LINKS:
    return --richtung;
  }; // switch (bewegungsrichtung)
  return richtung;
} // Richtung& operator+=(Richtung& richtung, Bewegungsrichtung const bewegungsrichtung)

/**
 ** -> Rückgabe
 ** 
 ** @param     richtung            Richtung 
 ** @param     bewegungsrichtung   Bewegungsrichtung
 **
 ** @return    geänderte Richtung
 **
 ** @version   2014-11-13
 **/
Richtung
operator+(Richtung richtung, Bewegungsrichtung const bewegungsrichtung)
{
  return (richtung += bewegungsrichtung);
} // Richtung operator+(Richtung richtung, Bewegungsrichtung const bewegungsrichtung)

/**
 ** -> Rückgabe
 ** 
 ** @param     lhs   aktuelle Richtung
 ** @param     rhs   vorige Richtung
 **
 ** @return    die Bewegungsrichtung von rhs nach lhs
 **
 ** @version   2014-11-13
 **/
Bewegungsrichtung
operator-(Richtung const lhs, Richtung const rhs)
{
  if (lhs == rhs)
    return Bewegungsrichtung::VORWAERTS;
  else if (lhs == rhs + Bewegungsrichtung::LINKS)
    return Bewegungsrichtung::LINKS;
  else if (lhs == rhs + Bewegungsrichtung::RECHTS)
    return Bewegungsrichtung::RECHTS;
  else // Fehler, rückwärts gegangen
    return Bewegungsrichtung::VORWAERTS;
} // Bewegungsrichtung operator-(Richtung const lhs, Richtung const rhs)

/**
 ** Die Bewegungsrichtung auf ostr ausgeben
 ** 
 ** @param     ostr      Ausgabestrom
 ** @param     bewegungsrichtung  Bewegungsrichtung zum ausgeben
 **
 ** @return    Ausgabestrom
 **
 ** @version   2014-10-25
 **/
ostream&
operator<<(ostream& ostr, Bewegungsrichtung const& bewegungsrichtung)
{ 
  switch (bewegungsrichtung) {
  case Bewegungsrichtung::VORWAERTS:
    ostr << "AHEAD";
    break;
  case Bewegungsrichtung::RECHTS:
    ostr << "RIGHT";
    break;
  case Bewegungsrichtung::LINKS:
    ostr << "LEFT";
    break;
  } // switch (bewegungsrichtung);
  return ostr;
} // ostream& operator<<(ostream& ostr, Bewegungsrichtung const& bewegungsrichtung)

