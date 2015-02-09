/*
   lichtgehen

   Copyright (C) 2014 by Diether Knof

   This file is part of lichtgehen.

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

   Diese Datei ist Teil von lichtgehen.

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

#include "text.h"
#include "../spielraster/spielraster.h"

/**
 ** Konstruktor
 ** 
 ** @param     ostr   Ausgabestrom für den Text
 **
 ** @return    -
 **
 ** @version   2014-11-20
 **/
UI_Text::UI_Text(ostream& ostr) :
  ostr(ostr)
{ }

/**
 ** Eine neue Runde startet
 ** 
 ** @param     n   Nummer der Runde
 **
 ** @return    -
 **
 ** @version   2014-11-20
 **/
void
UI_Text::runde(int const n)
{
  this->ostr << "---\n"
    << "Runde " << n << '\n'
    << *this->spielraster;
  return;
} // void UI_Text::runde(int n)

/**
 ** Das Spiel ist zuende
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2014-11-20
 **/
void
UI_Text::spiel_endet()
{
  this->ostr << "---\n"
    << "Spiel zuende\n"
    << *this->spielraster;
  return;
} // void UI_Text::spiel_endet()

/**
 ** gibt die nächste Richtung für die Bewegung zurück
 ** 
 ** @param     -
 **
 ** @return    Norden
 **
 ** @version   2014-11-20
 **/
Richtung
UI_Text::hole_richtung()
{ return Richtung::NORDEN; }
