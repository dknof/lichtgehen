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

#ifndef TAKTIK_TIEFENSUCHE_H
#define TAKTIK_TIEFENSUCHE_H

#include "taktik.h"
#include <set>

namespace TaktikNS {
/** Taktik Tiefensuche
 ** Alle möglichen Wege durchgehen und den „besten“ beschreiten
 ** Nur für zwei Bots programmiert
 **/
class Tiefensuche : public Taktik {
  public:
    // Ergebniswert der Rekursion
    struct Bewertung {
      using Wert = double;
      enum class Spielstand {
        VERLOREN, GEWONNEN, UNENTSCHIEDEN, OFFEN
      } spielstand;
      int tiefe;
      Wert wert = 0;
      // Konstruktoren
      Bewertung(Spielstand const s, int const t) : spielstand(s), tiefe(t) {}
      Bewertung(Spielstand const s, int const t, double const w) : spielstand(s), tiefe(t), wert(w) {}
    }; // struct Bewertung
  public:
    // Konstruktor
    Tiefensuche();
    // Destruktor
    ~Tiefensuche();

    // gibt das Ergebnis der Taktik zurück (ob sie greift und die Richtung)
    Ergebnis ergebnis(Spielraster const& spielraster,
                      int bot) override;

  protected:
    // bewertet das Spielfeld
    Bewertung::Wert bewertung(Spielraster const& spielraster,
                              int bot1, int bot2) const;

  private:
    // bewertet rekursiv die Schritte
    Bewertung tiefensuche(Spielraster const& spielraster, int bot1, int bot2,
                          Bewegungsrichtung r1, Bewegungsrichtung r2,
                          int tiefe) const;

  protected:
    // maximale Tiefe
    int tiefe_max;
}; // class Tiefensuche : public Taktik

// Vergleich von den Ergebnissen
bool operator<(Tiefensuche::Bewertung const& lhs,
               Tiefensuche::Bewertung const& rhs);
// Vergleich von den Ergebnissen
bool operator>(Tiefensuche::Bewertung const& lhs,
               Tiefensuche::Bewertung const& rhs);
// Vergleich von den Ergebnissen
bool operator==(Tiefensuche::Bewertung const& lhs,
                Tiefensuche::Bewertung const& rhs);
// Ausgabe der Bewertung
ostream& operator<<(ostream& ostr, Tiefensuche::Bewertung const& b);

} // namespace TaktikNS

#endif // #ifndef TAKTIK_TIEFENSUCHE_H
