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

#include "spieler.h"
#include "bot.h"
#include "mensch.h"
#include "programm.h"

#include "../spielraster/spielraster.h"

/**
 ** erstellt einen Spieler entsprechend dem Namen
 ** 
 ** @param     typ    Typ des Spielers („Mensch“, „Bot“ oder Pfad zum Programm)
 ** @param     name   Name des zu erzeugenden Spielers
 **
 ** @return    -
 **
 ** @version   2015-01-24
 **/
unique_ptr<Spieler>
Spieler::create(string const& typ, string const& name)
{
  if (typ == "Mensch")
    //return make_unique<Mensch>(name); // ToDo: UI fehlt
  return nullptr;
  else if (typ == "Bot")
    return make_unique<Bot>(name);
  else
    return make_unique<Programm>(typ, name);

  cerr << "Spieler '" << typ << "' unbekannt\n";
  return nullptr;
} // unique_ptr<Spieler> Spieler::create(string const& name)

/**
 ** Standardkonstruktor
 ** 
 ** @param     spielraster   Spielraster
 ** @param     name          Name des Spielers
 **
 ** @return    -
 **
 ** @version   2015-01-24
 **/
Spieler::Spieler(string const& name) :
  name_(name)
{ }

/**
 ** Destruktor
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2015-01-24
 **/
Spieler::~Spieler()
{ }

/**
 ** -> Rückgabe
 ** 
 ** @param     -
 **
 ** @return    der Name des Spielers
 **
 ** @version   2015-01-24
 **/
string const&
Spieler::name() const
{
  return this->name_;
} // string const& Spieler::name() const

/**
 ** -> Rückgabe
 ** 
 ** @param     -
 **
 ** @return    die Nummer des Spielers
 **
 ** @version   2014-11-10
 **/
Spielraster const&
Spieler::spielraster() const
{
  return *this->spielraster_;
} // Spielraster const& Spieler::spielraster() const

/**
 ** -> Rückgabe
 ** 
 ** @param     -
 **
 ** @return    die Nummer des Spielers
 **
 ** @version   2014-11-10
 **/
int
Spieler::nummer() const
{
  return this->nummer_;
} // int Spieler::nummer() const

/**
 ** setzt die Nummer
 ** 
 ** @param     nummer    die Nummer
 **
 ** @return    -
 **
 ** @version   2014-10-25
 **/
void
Spieler::setze_nummer(int const nummer)
{
  this->nummer_ = nummer;
  return ;
} // void Spieler::setze_nummer(int const nummer)

/**
 ** das Spiel startet
 ** 
 ** @param     spielraster   das Spielraster
 **
 ** @return    -
 **
 ** @version   2014-10-25
 **/
void
Spieler::spiel_startet(Spielraster const& spielraster)
{
  this->spielraster_ = &spielraster;
  return ;
} // void Spieler::spiel_startet(Spielraster const& spielraster)
