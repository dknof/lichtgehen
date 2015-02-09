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

#include "strategie.h"

// die einzelnen Strategieen
#include "taktiken.h"
#include "taktiken_mit_endspiel.h"

/**
 ** erstellt eine Strategie entsprechend dem Namen
 ** 
 ** @param     name   Name der zu erzeugenden Strategie
 **
 ** @return    -
 **
 ** @version   2014-10-30
 **/
unique_ptr<Strategie>
Strategie::create(string const name)
{
  using namespace StrategieNS;

  // Endspiel: alleine im Raum
  // → Raum möglichst gut ausfüllen

  // Wenn keine Strategie mit dem Namen gefunden wird, versuche eine Taktik
  return make_unique<TaktikenMitEndspiel>(name);

  cerr << "Strategie '" << name << "' unbekannt\n";
  return nullptr;
} // static unique_ptr<Strategie> Strategie::create(string const name)

/**
 ** erstellt eine Strategie entsprechend dem Namen
 ** 
 ** @param     namen   Namen der Taktiken, aus denen die Strategie besteht
 **
 ** @return    -
 **
 ** @version   2014-10-30
 **/
unique_ptr<Strategie>
Strategie::create(std::initializer_list<string> namen)
{
  return make_unique<StrategieNS::TaktikenMitEndspiel>(namen);
} // unique_ptr<Strategie> Strategie::create(std::initializer_list<string> namen)

/**
 ** Standardkonstruktor
 ** 
 ** @param     name    der Name der Strategie
 ** @param     beschreibung    die Beschreibung der Strategie
 **
 ** @return    -
 **
 ** @version   2014-10-25
 **/
Strategie::Strategie(string const name, string const beschreibung) :
  name{name},
  beschreibung{beschreibung}
{ }

/**
 ** Standarddestruktor
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2014-10-25
 **/
Strategie::~Strategie()
{ }

/**
 ** Die Strategie auf ostr ausgeben
 ** 
 ** @param     ostr   Ausgabestrom
 ** @param     strategie  Strategie zum ausgeben
 **
 ** @return    Ausgabestrom
 **
 ** @version   2014-10-25
 **/
ostream&
operator<<(ostream& ostr, Strategie const& strategie)
{ return strategie.ausgeben(ostr); }

/**
 ** Die Strategie auf ostr ausgeben
 ** 
 ** @param     ostr   Ausgabestrom
 **
 ** @return    Ausgabestrom
 **
 ** @version   2014-10-25
 **
 ** @todo      Bots und Verlauf darstellen
 **/
ostream& 
Strategie::ausgeben(ostream& ostr) const
{
  ostr << this->name << " -- " << this->beschreibung << '\n';
  return ostr;
} // ostream& Strategie::ausgeben(ostream& ostr) const

/**
 ** setzt Namen und Beschreibung der Strategie
 ** Für doppelt abgeleitete Klassen zu verwenden
 ** 
 ** @param     name    der Name der Strategie
 ** @param     beschreibung    die Beschreibung der Strategie
 **
 ** @return    -
 **
 ** @version   2014-11-11
 **/
void
Strategie::setze_name(string const name, string const beschreibung)
{
  this->name = name;
  this->beschreibung = beschreibung;
  return ;
} // void Strategie::setze_name(string const name, string const beschreibung)
