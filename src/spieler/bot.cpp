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

#include "bot.h"

#include "../spielraster/spielraster.h"

/**
 ** Standardkonstruktor
 ** 
 ** @param     name   Name des Bots
 **
 ** @return    -
 **
 ** @version   2015-01-25
 **/
Bot::Bot(string const& name) :
  Spieler(name)
{ }

/**
 ** Destruktor
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2014-11-21
 **/
Bot::~Bot()
{ }

/**
 ** setzt die Strategie
 ** 
 ** @param     strategie    die Strategie
 **
 ** @return    -
 **
 ** @version   2014-10-25
 **/
void
Bot::setze_strategie(unique_ptr<Strategie> strategie)
{
  this->strategie_ = std::move(strategie);
  return ;
} // void Bot::setze_strategie(unique_ptr<Strategie> strategie)

/**
 ** setzt die Nummer
 ** 
 ** @param     nummer    die Nummer
 **
 ** @return    -
 **
 ** @version   2014-10-25
 **/
Bewegungsrichtung
Bot::bewegung()
{
  return this->strategie_->bewegung(this->spielraster(), this->nummer());
} // Bewegungsrichtung Bot::bewegung()
