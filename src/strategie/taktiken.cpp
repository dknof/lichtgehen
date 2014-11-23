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

#include "taktiken.h"

namespace StrategieNS {

  /**
   ** Standardkonstruktor
   ** 
   ** @param     -
   **
   ** @return    -
   **
   ** @version   2014-10-30
   **/
  Taktiken::Taktiken() :
    Strategie{"Taktiken", "verschiedene Taktiken durchgehen"}
  {
    // ganz simple Taktiken ohne richtige Wahl direkt hinzufügen
    this->hinzufuegen(Taktik::create("Tot"));
    this->hinzufuegen(Taktik::create("Einziger Weg"));
  } // Taktiken::Taktiken()

  /**
   ** Standardkonstruktor mit einer Taktik
   ** 
   ** @param     name    Name der Taktik
   **
   ** @return    -
   **
   ** @version   2014-10-30
   **/
  Taktiken::Taktiken(string const name) :
    Strategie{"Taktiken", "verschiedene Taktiken durchgehen"}
  {
    // ganz simple Taktiken ohne richtige Wahl direkt hinzufügen
    this->hinzufuegen(Taktik::create("Tot"));
    this->hinzufuegen(Taktik::create("Einziger Weg"));
    this->hinzufuegen(Taktik::create(name));
  } // Taktiken::Taktiken(string name)

  /**
   ** Standardkonstruktor mit mehreren Taktiken
   ** 
   ** @param     namen    Name der Taktiken
   **
   ** @return    -
   **
   ** @version   2014-10-30
   **/
  Taktiken::Taktiken(std::initializer_list<string> name) :
    Strategie{"Taktiken", "verschiedene Taktiken durchgehen"}
  {
    // ganz simple Taktiken ohne richtige Wahl direkt hinzufügen
    this->hinzufuegen(Taktik::create("Tot"));
    this->hinzufuegen(Taktik::create("Einziger Weg"));
    for (auto const n : name)
      this->hinzufuegen(Taktik::create(n));
  } // Taktiken::Taktiken(string name)

  /**
   ** Standarddestruktor
   ** 
   ** @param     -
   **
   ** @return    -
   **
   ** @version   2014-10-25
   **/
  Taktiken::~Taktiken()
  { }

  /**
   ** Die Taktiken auf ostr ausgeben
   ** 
   ** @param     ostr   Ausgabestrom
   **
   ** @return    Ausgabestrom
   **
   ** @version   2014-10-30
   **/
  ostream& 
    Taktiken::ausgeben(ostream& ostr) const
    {
      this->Strategie::ausgeben(ostr);

      int i = 0;
      for (auto const& t : this->taktiken) { 
        ostr << i << ": " << *t << '\n';
        ++i;
      }
      return ostr;
    } // ostream& Taktiken::ausgeben(ostream& ostr) const

  /**
   ** eine Taktik hinzufügen
   ** 
   ** @param     taktik   Taktik zum hinzufügen
   **
   ** @return    -
   **
   ** @version   2014-10-25
   **/
  void
    Taktiken::hinzufuegen(unique_ptr<Taktik> taktik)
    {
      this->taktiken.push_back(std::move(taktik));
      return;
    } // void Taktiken::hinzufuegen(unique_ptr<Taktik> taktik)

  /**
   ** Die Bewegungsrichtung nach den Taktiken ermitteln
   ** 
   ** @param     spielraster   das Spielraster
   ** @param     bot_nummer  die Nummer des Bots
   **
   ** @return    -
   **
   ** @version   2014-10-30
   **/
  Bewegungsrichtung
    Taktiken::bewegung(Spielraster const& spielraster, int const bot_nummer)
    {
      for (auto& t : this->taktiken) {
        cdebug << "Teste Taktik " << *t << '\n';
        auto ergebnis = t->ergebnis(spielraster, bot_nummer);
        if (ergebnis) {
          cdebug << bot_nummer << ": " << ergebnis.richtung << ": " << *t << '\n';
          return ergebnis.richtung;
        }
      }
      auto const bp = BotPosition{spielraster.position(bot_nummer)};
      if (!spielraster(bp + Bewegungsrichtung::VORWAERTS))
        return Bewegungsrichtung::VORWAERTS;
      else if (!spielraster(bp + Bewegungsrichtung::LINKS))
        return Bewegungsrichtung::LINKS;
      else
        return Bewegungsrichtung::RECHTS;
    } // Bewegungsrichtung Taktiken::bewegung(Spielraster spielraster, int bot_nummer)

} // namespace StrategieNS
