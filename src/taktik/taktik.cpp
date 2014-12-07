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

#include "taktik.h"

// die einzelnen Taktiken
#include "tot.h"
#include "einzigerweg.h"
#include "richtung.h"
#include "gegner_verfolgen.h"
#include "groesster_raum.h"
#include "groesstes_einflussgebiet.h"

// Endspiel
#include "tiefensuche.h"
#include "raum_ausfuellen.h"
#include "raum_ausfuellen_tiefensuche.h"
#include "raum_ausfuellen_tiefensuche_bis_ende.h"

/**
 ** erstellt eine Taktik entsprechend dem Namen
 ** 
 ** @param     name   Name der zu erzeugenden Taktik
 **
 ** @return    -
 **
 ** @version   2014-10-25
 **/
unique_ptr<Taktik>
Taktik::create(string const& name)
{
  using namespace TaktikNS;
  using TaktikNS::Richtung;
  if (name == "Tot")
    return make_unique<Tot>();
  else if (name == "Einziger Weg")
    return make_unique<EinzigerWeg>();
  else if (name == "vorwärts")
    return make_unique<Richtung>(Bewegungsrichtung::VORWAERTS);
  else if (name == "links")
    return make_unique<Richtung>(Bewegungsrichtung::LINKS);
  else if (name == "rechts")
    return make_unique<Richtung>(Bewegungsrichtung::RECHTS);
  else if (name.compare(0, string("vorwärts").length(), "vorwärts") == 0) {
    return make_unique<Richtung>(Bewegungsrichtung::VORWAERTS,
                                 std::stod(string(name, string("vorwärts").length())));
  } else if (name.compare(0, 5, "links") == 0) {
    return make_unique<Richtung>(Bewegungsrichtung::LINKS,
                                 std::stod(string(name, 5)));
  } else if (name.compare(0, 6, "rechts") == 0) {
    return make_unique<Richtung>(Bewegungsrichtung::RECHTS,
                                 std::stod(string(name, 6)));
  }
  else if (name == "in größten Raum")
    return make_unique<GroessterRaum>();
  else if (name == "zum größten Einflussgebiet")
    return make_unique<GroesstesEinflussgebiet>();
  else if (name == "zur Mitte")
    return nullptr; // Gehe Richtung Mitte (Schwerpunkt der freien Felder)
  else if (name == "Abstand wahren")
    return nullptr; // Gege weg vom Gegner
  else if (name == "Gegner verfolgen")
    return make_unique<GegnerVerfolgen>();
  else if (name == "Abhauen")
    return nullptr; // Gehe von allen Gegnern weg
  else if (name == "Raum ausfüllen")
    return make_unique<RaumAusfuellen>();
  else if (name == "Raum ausfüllen (Tiefensuche)")
    return make_unique<RaumAusfuellenTiefensuche>();
  else if (name == "Raum ausfüllen (Tiefensuche bis Ende)")
    return make_unique<RaumAusfuellenTiefensucheBisEnde>();
  else if (name == "Tiefensuche")
    return make_unique<Tiefensuche>();
  else if (name == "Raum abzweigen")
    return nullptr; // großen Raum abzweigen

  cerr << "Taktik '" << name << "' unbekannt\n";
  return nullptr;
} // static unique_ptr<Taktik> Taktik::create(string const& name)

/**
 ** Standardkonstruktor
 ** 
 ** @param     name   der Name der Taktik
 ** @param     beschreibung   die Beschreibung der Taktik
 **
 ** @return    -
 **
 ** @version   2014-10-25
 **/
Taktik::Taktik(string const& name, string const& beschreibung) :
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
Taktik::~Taktik()
{ }

/**
 ** Die Taktik auf ostr ausgeben
 ** 
 ** @param     ostr   Ausgabestrom
 ** @param     taktik  Taktik zum ausgeben
 **
 ** @return    Ausgabestrom
 **
 ** @version   2014-10-25
 **/
ostream&
operator<<(ostream& ostr, Taktik const& taktik)
{ return taktik.ausgeben(ostr); }

/**
 ** Die Taktik auf ostr ausgeben
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
Taktik::ausgeben(ostream& ostr) const
{
  ostr << this->name << " -- " << this->beschreibung;
  return ostr;
} // ostream& Taktik::ausgeben(ostream& ostr) const

