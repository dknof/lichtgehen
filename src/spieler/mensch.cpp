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

#include "mensch.h"

#include "../ui/ui.h"

/**
 ** Standardkonstruktor
 ** 
 ** @param     ui            UI
 **
 ** @return    -
 **
 ** @version   2015-01-25
 **/
Mensch::Mensch(UI& ui) :
  Spieler("Mensch"),
  ui(ui)
{ }

/**
 ** -> Rückgabe
 ** Die Richtung wir dvon der UI übernommen, belegte Felder werden aber nicht betreten
 ** 
 ** @param     -
 **
 ** @return    Richtung, in die der Mensch geht
 **
 ** @version   2015-01-24
 **/
Bewegungsrichtung
Mensch::bewegung()
{
  auto const bp = this->spielraster().position(this->nummer()); // Position
  auto const bw = (this->ui.naechste_richtung() - bp.richtung()); // Bewegung
  if (!this->spielraster().nachbarn_frei(bp))
    return bw;
  // Wenn das Feld belegt ist, nehme das nächste
  if (this->spielraster()(bp + bw))
    return this->bewegung();
  return bw;
} // Bewegungsrichtung Mensch::bewegung()
