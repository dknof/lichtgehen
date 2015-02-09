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

#ifndef TAKTIK_H
#define TAKTIK_H

#include "constants.h"

#include "../spielraster/spielraster.h"

/** Basisklasse für eine Taktik
 ** Eine Taktik prüft, ob eine bestimmte Situation gegeben ist und liefert eine entsprechende Bewegungsrichtung zurück.
 ** Eine Taktik kann auch 'false' zurückliefern, wenn die Situation für die Taktik nicht passt.
 ** Eine Taktik berücksichtigt nur den aktuellen Zustand, sie sammelt selber keine Informationen. Für Informationen des Spielverlaufs ist die Strategie zu verwenden.
 ** @todo   Auch Felder sperren und dies bei weiteren Taktiken berücksichtigen
 **/
class Taktik {
  public:
    struct Ergebnis {
      inline Ergebnis(bool greift) : // greift muss 'false' sein
        greift(greift), richtung(Bewegungsrichtung::VORWAERTS)
      { }
      inline Ergebnis(Bewegungsrichtung richtung) :
        greift(true), richtung(richtung)
      { }
      explicit operator bool()
      { return this->greift; }

      bool greift;
      Bewegungsrichtung richtung;
    }; // struct Ergebnis

  public:
    // erstellt eine Taktik entsprechend dem Namen
    static unique_ptr<Taktik> create(string const& name);

  protected:
    // Konstruktor
    Taktik(string const& name, string const& beschreibung);
  public:
    // Destruktor
    virtual ~Taktik() = 0;

  public:
    // die Taktik ausgeben
    virtual ostream& ausgeben(ostream& ostr) const;

    // gibt das Ergebnis der Taktik zurück (ob sie greift und die Richtung)
    virtual Ergebnis ergebnis(Spielraster const& spielraster, int bot_nummer) = 0;

  public:
    // Der Name der Taktik
    string const name;
    // Die Beschreibung der Taktik
    string const beschreibung;
}; // class Taktik

// die Taktik ausgeben
ostream& operator<<(ostream& ostr, Taktik const& taktik);

#endif // #ifndef TAKTIK_H
