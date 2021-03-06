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

#include "tiefensuche.h"
#include <cstdlib>
#include <chrono>
#include <algorithm>

#ifdef USE_THREADS
#include <future>
#endif

namespace TaktikNS {
  /**
   ** Standardkonstruktor
   ** 
   ** @param     -
   **
   ** @return    -
   **
   ** @version   2014-11-08
   **/
  Tiefensuche::Tiefensuche() :
    Taktik{"Tiefensuche", "Bestimme alle möglichen Wege und beschreite den „besten“ (nur für zwei Spielers)."},
    tiefe_max(0)
    { }

  /**
   ** Destruktor
   ** 
   ** @param     -
   **
   ** @return    -
   **
   ** @version   2014-11-29
   **/
  Tiefensuche::~Tiefensuche()
  { }

  /**
   ** -> Rückgabe
   ** Suche mit Tiefensuche die „beste“ Richtung
   ** 
   ** @param     spielraster   das Spielraster
   ** @param     spieler   die Nummer des Spielers
   **
   ** @return    Richtung mit dem „besten“ Wert
   **
   ** @version   2014-11-08
   **/
  Taktik::Ergebnis
    Tiefensuche::ergebnis(Spielraster const& spielraster,
                          int const spieler)
    {
      // nur für zwei Spielers
      if (spielraster.rauminfo(spieler).spieler_anz != 1)
        return false;

      // spieler 2 suchen
      int spieler2;
      for (spieler2 = 0; spieler2 < spielraster.spieler_anz(); ++spieler2) {
        if (spieler2 == spieler)
          continue;
        if (spielraster.kuerzeste_entfernung(spielraster.position(spieler),
                                             spielraster.position(spieler2))
            >= 0)
          break;
      }
      if (spieler2 == spielraster.spieler_anz())
        return false;

      auto const felder_frei = spielraster.felder_frei();
      this->tiefe_max = (  (felder_frei >= 2000) ? 0
                         : (felder_frei >=  160) ? 1
                         : (felder_frei >=   80) ? 2
                         : (felder_frei >=   40) ? 3
                         : (felder_frei >=   20) ? 4
                         : felder_frei);

      Taktik::Ergebnis ergebnis = false;
#ifdef TIEFE_DYNAMISCH
      // Tiefe ausprobieren
      auto const zeit_start = std::chrono::system_clock::now();
      auto dauer = std::chrono::duration<double>{zeit_start - zeit_start};
      this->tiefe_max = 0;
      do {
        ergebnis = this->tiefensuche(spielraster, spieler, spieler2);
        dauer = std::chrono::system_clock::now() - zeit_start;
        this->tiefe_max += 1;
      } while (dauer.count() < 0.1 * 0.9 * ZEITBESCHRAENKUNG);
#else
      ergebnis = tiefensuche(spielraster, spieler, spieler2);
#endif

      return ergebnis;
    } // Taktik::Ergebnis Tiefensuche::ergebnis(Spielraster spielraster, int spieler)

  /**
   ** -> Rückgabe
   ** Suche mit Tiefensuche die „beste“ Richtung
   ** 
   ** @param     spielraster   das Spielraster
   ** @param     spieler   die Nummer des Spielers
   ** @param     spieler2   die Nummer des zweiten Spielers
   **
   ** @return    Richtung mit dem „besten“ Wert
   **
   ** @version   2014-11-08
   **/
  Taktik::Ergebnis
    Tiefensuche::tiefensuche(Spielraster const& spielraster,
                             int const spieler, int const spieler2) const
    {
#ifdef USE_THREADS
#define THREAD(b1, b2) std::async(std::launch::async, std::bind(&Tiefensuche::iteration, this, spielraster, spieler, spieler2, b1, b2, 0))
      std::array<std::future<Bewertung>, 9> threads
        = {THREAD(Bewegungsrichtung::VORWAERTS, Bewegungsrichtung::VORWAERTS),
          THREAD(Bewegungsrichtung::VORWAERTS, Bewegungsrichtung::LINKS),
          THREAD(Bewegungsrichtung::VORWAERTS, Bewegungsrichtung::RECHTS),
          THREAD(Bewegungsrichtung::LINKS, Bewegungsrichtung::VORWAERTS),
          THREAD(Bewegungsrichtung::LINKS, Bewegungsrichtung::LINKS),
          THREAD(Bewegungsrichtung::LINKS, Bewegungsrichtung::RECHTS),
          THREAD(Bewegungsrichtung::RECHTS, Bewegungsrichtung::VORWAERTS),
          THREAD(Bewegungsrichtung::RECHTS, Bewegungsrichtung::LINKS),
          THREAD(Bewegungsrichtung::RECHTS, Bewegungsrichtung::RECHTS)};
#undef THREAD
      RichtungenErgebnis const ergebnisse(spielraster, spieler,
                                          RichtungenErgebnis::Ergebnisse({threads[0].get(),
                                          threads[1].get(),
                                          threads[2].get(),
                                          threads[3].get(),
                                          threads[4].get(),
                                          threads[5].get(),
                                          threads[6].get(),
                                          threads[7].get(),
                                          threads[8].get()
                                          }));
#else // #ifdef !USE_THREADS
      RichtungenErgebnis const ergebnisse(spielraster, spieler,
                                          RichtungenErgebnis::Ergebnisse{
                                          this->iteration(spielraster, spieler, spieler2,
                                                          Bewegungsrichtung::VORWAERTS,
                                                          Bewegungsrichtung::VORWAERTS,
                                                          0),
                                          this->iteration(spielraster, spieler, spieler2,
                                                          Bewegungsrichtung::VORWAERTS,
                                                          Bewegungsrichtung::LINKS,
                                                          0),
                                          this->iteration(spielraster, spieler, spieler2,
                                                          Bewegungsrichtung::VORWAERTS,
                                                          Bewegungsrichtung::RECHTS,
                                                          0),
                                          this->iteration(spielraster, spieler, spieler2,
                                                          Bewegungsrichtung::LINKS,
                                                          Bewegungsrichtung::VORWAERTS,
                                                          0),
                                          this->iteration(spielraster, spieler, spieler2,
                                                          Bewegungsrichtung::LINKS,
                                                          Bewegungsrichtung::LINKS,
                                                          0),
                                          this->iteration(spielraster, spieler, spieler2,
                                                          Bewegungsrichtung::LINKS,
                                                          Bewegungsrichtung::RECHTS,
                                                          0),
                                          this->iteration(spielraster, spieler, spieler2,
                                                          Bewegungsrichtung::RECHTS,
                                                          Bewegungsrichtung::VORWAERTS,
                                                          0),
                                          this->iteration(spielraster, spieler, spieler2,
                                                          Bewegungsrichtung::RECHTS,
                                                          Bewegungsrichtung::LINKS,
                                                          0),
                                          this->iteration(spielraster, spieler, spieler2,
                                                          Bewegungsrichtung::RECHTS,
                                                          Bewegungsrichtung::RECHTS,
                                                          1)
                                          });
#endif // #ifdef !USE_THREADS
      cdebug << spieler << ": " << ergebnisse;
      return ergebnisse.beste_richtung();

      return false;
    } // Taktik::Ergebnis Tiefensuche::tiefensuche(Spielraster const& spielraster, int const spieler, int const spieler2) const

  /**
   ** -> Rückgabe
   ** bewertet das Spielfeld rekursiv
   ** Der Wert ist vom ersten Spieler zu sehen. 
   ** 1: er gewinnt, -1: er verliert, 0: unentschieden
   ** 
   ** @param     spielraster   das Spielraster
   ** @param     spieler1          die Nummer des ersten Spielers
   ** @param     spieler2          die Nummer des zweiten Spielers
   ** @param     r1            die Bewegungsrichtung des ersten Spielers
   ** @param     r2            die Bewegungsrichtung des zweiten Spielers
   ** @param     tiefe         verbleibende Tiefe
   **
   ** @return    Wert der Bewegungen
   **
   ** @version   2014-11-29
   **/
  Tiefensuche::Bewertung
    Tiefensuche::iteration(Spielraster const& spielraster,
                           int const spieler1, int const spieler2,
                           Bewegungsrichtung const r1,
                           Bewegungsrichtung const r2,
                           int tiefe) const
    {
      SpielerPosition const bp1 = spielraster.position(spieler1);
      SpielerPosition const bp2 = spielraster.position(spieler2);

      if (bp1 + r1 == bp2 + r2) // ineinander gelaufen
        return Bewertung(Bewertung::Spielstand::UNENTSCHIEDEN, tiefe);
      if (spielraster(bp1 + r1)) {
        if (spielraster(bp2 + r2)) // beide gegen die Wand gelaufen
          return Bewertung(Bewertung::Spielstand::UNENTSCHIEDEN, tiefe);
        else // alleine gegen eine Wand gelaufen :-(
          return Bewertung(Bewertung::Spielstand::VERLOREN, tiefe);
      } else if (spielraster(bp2 + r2)) { // Gegner gegen eine Wand gelaufen :-)
        return Bewertung(Bewertung::Spielstand::GEWONNEN, tiefe);
      }

      // Beide Spielers konnten einen Schritt machen

      // die Schritte durchführen
      Spielraster sr{spielraster};
      sr.bewege_spieler(spieler1, r1);
      sr.bewege_spieler(spieler2, r2);

      if (tiefe >= this->tiefe_max)
        return Bewertung(Bewertung::Spielstand::OFFEN,
                         tiefe,
                         this->bewertung(sr, spieler1, spieler2));

      tiefe += 1;
      // rekursiver Aufruf (aufwendig)
      auto const bewertung_vv = this->iteration(sr, spieler1, spieler2,
                                                Bewegungsrichtung::VORWAERTS,
                                                Bewegungsrichtung::VORWAERTS,
                                                tiefe);
      auto const bewertung_vl = this->iteration(sr, spieler1, spieler2,
                                                Bewegungsrichtung::VORWAERTS,
                                                Bewegungsrichtung::LINKS,
                                                tiefe);
      auto const bewertung_vr = this->iteration(sr, spieler1, spieler2,
                                                Bewegungsrichtung::VORWAERTS,
                                                Bewegungsrichtung::RECHTS,
                                                tiefe);
      auto const bewertung_lv = this->iteration(sr, spieler1, spieler2,
                                                Bewegungsrichtung::LINKS,
                                                Bewegungsrichtung::VORWAERTS,
                                                tiefe);
      auto const bewertung_ll = this->iteration(sr, spieler1, spieler2,
                                                Bewegungsrichtung::LINKS,
                                                Bewegungsrichtung::LINKS,
                                                tiefe);
      auto const bewertung_lr = this->iteration(sr, spieler1, spieler2,
                                                Bewegungsrichtung::LINKS,
                                                Bewegungsrichtung::RECHTS,
                                                tiefe);
      auto const bewertung_rv = this->iteration(sr, spieler1, spieler2,
                                                Bewegungsrichtung::RECHTS,
                                                Bewegungsrichtung::VORWAERTS,
                                                tiefe);
      auto const bewertung_rl = this->iteration(sr, spieler1, spieler2,
                                                Bewegungsrichtung::RECHTS,
                                                Bewegungsrichtung::LINKS,
                                                tiefe);
      auto const bewertung_rr = this->iteration(sr, spieler1, spieler2,
                                                Bewegungsrichtung::RECHTS,
                                                Bewegungsrichtung::RECHTS,
                                                tiefe);
#if 0
      { // Variante A: Mein Gegner verhält sich möglichst schlecht für mich
        // (pessimistisch)
        // Minimiere den Verlust
        auto const bewertung_v = std::min(bewertung_vv, bewertung_vl, bewertung_vr);
        auto const bewertung_l = std::min(bewertung_lv, bewertung_ll, bewertung_lr);
        auto const bewertung_r = std::min(bewertung_rv, bewertung_rl, bewertung_rr);

        return std::max(bewertung_v, bewertung_l, bewertung_r);
      } // Variante A (Pessimist)
#endif


#if 1
      { // Variante B: Mein Gegner verhält sich möglichst gut für sich (er verfolgt Variante 1)
        // (optimistisch)
        // Minimiere seinen Verlust
        auto const bewertung_v = std::max(bewertung_vv, bewertung_lv, bewertung_rv);
        auto const bewertung_l = std::max(bewertung_vl, bewertung_ll, bewertung_rl);
        auto const bewertung_r = std::max(bewertung_vr, bewertung_lr, bewertung_rr);

        return std::min(bewertung_v, bewertung_l, bewertung_r);
      } // Variante B (Optimist)
#endif

#if 0
      { // Variante C
        // 1) Gewinnerstrategie gleich verwenden
        if (   ( (bewertung_vv == 1) && (bewertung_vl == 1) && (bewertung_vr == 1) )
            || ( (bewertung_lv == 1) && (bewertung_ll == 1) && (bewertung_lr == 1) )
            || ( (bewertung_rv == 1) && (bewertung_rl == 1) && (bewertung_rr == 1) ) )
          return 1;
        if (   ( (bewertung_vv == -1) && (bewertung_lv == -1) && (bewertung_rv == -1) )
            || ( (bewertung_vl == -1) && (bewertung_ll == -1) && (bewertung_rl == -1) )
            || ( (bewertung_vr == -1) && (bewertung_lr == -1) && (bewertung_rr == -1) ) )
          return -1;
        // 2) die ungünstigen Richtungen für beide Spieler entfernen
        // 3) die verbleibenen Bewertungen irgendwie mitteln

        return Bewertung(Bewertung::Spielstand::OFFEN, -1);
      } // Variante C
#endif
    } // Taktik::Ergebnis Tiefensuche::iteration(Spielraster spielraster, int spieler, int spieler2, Bewegungsrichtung b1, Bewegungsrichtung b2, int tiefe) const

  /**
   ** -> Rückgabe
   ** 
   ** @param     spielraster   das Spielraster
   ** @param     spieler1          die Nummer des ersten Spielers
   ** @param     spieler2          die Nummer des zweiten Spielers
   **
   ** @return    Bewertung des Spielrasters
   **
   ** @version   2014-11-29
   **/
  double
    Tiefensuche::bewertung(Spielraster const& spielraster,
                           int spieler1, int spieler2) const
    {
      return (spielraster.einflussbereich_groesse_erreichbar(spieler1)
              - spielraster.einflussbereich_groesse_erreichbar(spieler2));
    } // double Tiefensuche::bewertung(Spielraster const& spielraster, int spieler1, int spieler2) const

  /**
   ** -> Rückgabe
   ** 
   ** @param     lhs   erstes Ergebnis
   ** @param     rhs   zweites Ergebnis
   **
   ** @return    ob lhs schlechter ist als rhs
   **
   ** @version   2014-11-29
   **/
  bool
    operator<(Tiefensuche::Bewertung const& lhs,
              Tiefensuche::Bewertung const& rhs)
    {
      switch (lhs.spielstand) {
      case Tiefensuche::Bewertung::Spielstand::GEWONNEN:
        // Gewinnen ist immer gut
        switch (rhs.spielstand) {
        case Tiefensuche::Bewertung::Spielstand::GEWONNEN:
          // Wenn gewinnen, dann möglichst schnell
          if (lhs.tiefe > rhs.tiefe)
            return true;
          else if (lhs.tiefe < rhs.tiefe)
            return false;
          else
            return (lhs.wert < rhs.wert);
        case Tiefensuche::Bewertung::Spielstand::VERLOREN:
        case Tiefensuche::Bewertung::Spielstand::UNENTSCHIEDEN:
        case Tiefensuche::Bewertung::Spielstand::OFFEN:
          return false;
        } // switch (rhs.spielstand)
      case Tiefensuche::Bewertung::Spielstand::VERLOREN:
        // Gewinnen ist immer schlecht
        switch (rhs.spielstand) {
        case Tiefensuche::Bewertung::Spielstand::VERLOREN:
          // Wenn verlieren, dann möglichst spät
          if (lhs.tiefe < rhs.tiefe)
            return true;
          else if (lhs.tiefe > rhs.tiefe)
            return false;
          else
            return (lhs.wert < rhs.wert);
        case Tiefensuche::Bewertung::Spielstand::GEWONNEN:
        case Tiefensuche::Bewertung::Spielstand::UNENTSCHIEDEN:
        case Tiefensuche::Bewertung::Spielstand::OFFEN:
          return true;
        } // switch (rhs.spielstand)
      case Tiefensuche::Bewertung::Spielstand::UNENTSCHIEDEN:
        switch (rhs.spielstand) {
        case Tiefensuche::Bewertung::Spielstand::VERLOREN:
          return false;
        case Tiefensuche::Bewertung::Spielstand::GEWONNEN:
          return true;
        case Tiefensuche::Bewertung::Spielstand::UNENTSCHIEDEN:
          // Bei schlechtem Stand lieber kürzer spielen
          if ((lhs.wert < 0) && (rhs.wert < 0)
              && (lhs.tiefe != rhs.tiefe))
            return (lhs.tiefe > rhs.tiefe);
          if (lhs.wert == rhs.wert)
            return (lhs.tiefe < rhs.tiefe);
          return (lhs.wert < rhs.wert);
        case Tiefensuche::Bewertung::Spielstand::OFFEN:
          // Bei schlechten Stand ist unentschieden besser
          if ((lhs.wert < 0) && (rhs.wert < 0))
            return false;
          if (lhs.wert == rhs.wert)
            return (lhs.tiefe < rhs.tiefe);
          return (lhs.wert < rhs.wert);
        } // switch (rhs.spielstand)
      case Tiefensuche::Bewertung::Spielstand::OFFEN:
        switch (rhs.spielstand) {
        case Tiefensuche::Bewertung::Spielstand::VERLOREN:
          return false;
        case Tiefensuche::Bewertung::Spielstand::GEWONNEN:
          return true;
        case Tiefensuche::Bewertung::Spielstand::UNENTSCHIEDEN:
          // Bei schlechten Stand ist unentschieden besser
          if ((lhs.wert < 0) && (rhs.wert < 0))
            return true;
        case Tiefensuche::Bewertung::Spielstand::OFFEN:
          if (lhs.wert == rhs.wert)
            return (lhs.tiefe < rhs.tiefe);
          return (lhs.wert < rhs.wert);
        } // switch (rhs.spielstand)
      } // switch (lhs.spielstand)
      return false;
    } // bool operator<(Tiefensuche::Bewertung lhs, RaumAusfuellenTIefensuche::Ergebnis rhs)

  /**
   ** -> Rückgabe
   ** 
   ** @param     lhs   erstes Ergebnis
   ** @param     rhs   zweites Ergebnis
   **
   ** @return    ob lhs besser ist als rhs
   **
   ** @version   2014-11-29
   **/
  bool
    operator>(Tiefensuche::Bewertung const& lhs,
              Tiefensuche::Bewertung const& rhs)
    {
      return (rhs < lhs);
    } // bool operator>(Tiefensuche::Bewertung lhs, Tiefensuche::Bewertung rhs)

  /**
   ** -> Rückgabe
   ** 
   ** @param     lhs   erstes Ergebnis
   ** @param     rhs   zweites Ergebnis
   **
   ** @return    ob lhs und rhs gleich sind
   **
   ** @version   2014-11-29
   **/
  bool
    operator==(Tiefensuche::Bewertung const& lhs,
               Tiefensuche::Bewertung const& rhs)
    {
      return (   (rhs.spielstand == lhs.spielstand)
              && (rhs.tiefe == lhs.tiefe)
              && (rhs.wert == lhs.wert) );
    } // bool operator==(Tiefensuche::Bewertung lhs, Tiefensuche::Bewertung rhs)

  /**
   ** gibt die Bewertung aus
   ** 
   ** @param     ostr   Ausgabestrom
   ** @param     b      Bewertung
   **
   ** @return    Ausgabestrom
   **
   ** @version   2014-11-29
   **/
  ostream&
    operator<<(ostream& ostr, Tiefensuche::Bewertung const& b)
    {
      ostr << "(";
      switch (b.spielstand) {
      case Tiefensuche::Bewertung::Spielstand::VERLOREN:
        ostr << "-"; break;
      case Tiefensuche::Bewertung::Spielstand::GEWONNEN:
        ostr << "+"; break;
      case Tiefensuche::Bewertung::Spielstand::UNENTSCHIEDEN:
        ostr << "x"; break;
      case Tiefensuche::Bewertung::Spielstand::OFFEN:
        ostr << "o"; break;
      } // switch (bewertung.spielstand)
      ostr << " " << b.tiefe;
      if (b.spielstand == Tiefensuche::Bewertung::Spielstand::OFFEN)
        ostr << ", " << b.wert;
      ostr << ")";
      return ostr;
    }

  /**
   ** Konstruktor
   ** 
   ** @param     spielraster   Spielraster
   ** @param     spieler           Nummer des Spielers
   ** @param     bewertung     Bewertungen für alle Richtungskombinationen
   **
   ** @return    -
   **
   ** @version   2014-12-13
   **/
  Tiefensuche::RichtungenErgebnis::RichtungenErgebnis(Spielraster const& spielraster, int spieler, RichtungenErgebnis::Ergebnisse const& bewertung) :
    bewertung(bewertung),
    nachbarn_frei({spielraster.nachbarn_frei(spielraster.position(spieler) + Bewegungsrichtung::VORWAERTS),
                  spielraster.nachbarn_frei(spielraster.position(spieler) + Bewegungsrichtung::LINKS),
                  spielraster.nachbarn_frei(spielraster.position(spieler) + Bewegungsrichtung::RECHTS)})
    { }

  /**
   ** -> Rückgabe
   ** 
   ** @param     -
   **
   ** @return    „Beste“ Richtung entsprechend den Ergebnissen
   **
   ** @version   2014-12-13
   **/
  Bewegungsrichtung 
    Tiefensuche::RichtungenErgebnis::beste_richtung() const
    {
      // Minimiere den Verlust
      auto const bewertung_v = std::min(this->bewertung[0], this->bewertung[1], this->bewertung[2]);
      auto const bewertung_l = std::min(this->bewertung[3], this->bewertung[4], this->bewertung[5]);
      auto const bewertung_r = std::min(this->bewertung[6], this->bewertung[7], this->bewertung[8]);

      using Y = std::pair<Bewertung, int>;
      using X = std::pair<Bewegungsrichtung, Y>;
      array<X, 3>
        ergebnisse({ X(Bewegungsrichtung::VORWAERTS, Y(bewertung_v, this->nachbarn_frei[0])),
         X(Bewegungsrichtung::LINKS,  Y(bewertung_l, this->nachbarn_frei[1])),
         X(Bewegungsrichtung::RECHTS, Y(bewertung_r, this->nachbarn_frei[2])) });

      // Bewegungsrichtung ignorieren
      // Bewertung kleiner ist schlechter
      // int (nachbarn_anz) kleiner ist besser
#if 0
  return std::max_element(begin(ergebnisse), end(ergebnisse),
                          [=](auto const& a, auto const& b)
                          {
                          if (a.second.first < b.second.first)
                          return true;
                          else if (b.second.first < a.second.first)
                          return false;
                          else if (a.second.second > b.second.second)
                          return true;
                          else
                          return false;
                          }
                         )->first;
#endif
      auto m = *begin(ergebnisse);
      for (auto const& e : ergebnisse) {
        if (   (m.second.first < e.second.first)
            || (   (m.second.first == e.second.first)
                && (m.second.second > e.second.second) ) )
          m = e;
      }
      return m.first;

#if 0
  auto zufall = 1 * rand();
  if (bewertung_v > bewertung_l) {
    if (bewertung_v > bewertung_r)
      return Bewegungsrichtung::VORWAERTS;
    else if (bewertung_v < bewertung_r)
      return Bewegungsrichtung::RECHTS;
    else // (bewertung_v == bewertung_r)
      return ((zufall <= RAND_MAX / 2)
              ? Bewegungsrichtung::VORWAERTS
              : Bewegungsrichtung::RECHTS);
  } else if (bewertung_v < bewertung_l) {
    if (bewertung_l > bewertung_r)
      return Bewegungsrichtung::LINKS;
    else if (bewertung_l < bewertung_r)
      return Bewegungsrichtung::RECHTS;
    else // (bewertung_l == bewertung_r)
      return ((zufall <= RAND_MAX / 2)
              ? Bewegungsrichtung::LINKS
              : Bewegungsrichtung::RECHTS);
  } else { // (bewertung_v == bewertung_l)
    if (bewertung_v < bewertung_r)
      return Bewegungsrichtung::RECHTS;
    else if (bewertung_v > bewertung_r)
      return ((zufall <= RAND_MAX / 2)
              ? Bewegungsrichtung::VORWAERTS
              : Bewegungsrichtung::LINKS);
    else // (bewertung_v == bewertung_l == bewertung_r)
      return ((zufall <= RAND_MAX / 3)
              ? Bewegungsrichtung::VORWAERTS
              : (zufall <= RAND_MAX / 3 * 2)
              ? Bewegungsrichtung::LINKS
              : Bewegungsrichtung::RECHTS);
  }
#endif

  return Bewegungsrichtung::VORWAERTS;
} // Bewegungsrichtung Tiefensuche::RichtungenErgebnis::beste_richtung() const

/**
 ** gibt die Ergebnisse aus
 ** 
 ** @param     ostr   Ausgabestrom
 ** @param     e      Ergebnis
 **
 ** @return    Ausgabestrom
 **
 ** @version   2014-12-13
 **/
ostream&
operator<<(ostream& ostr, Tiefensuche::RichtungenErgebnis const& e)
{
  auto const& bewertung_vv = e.bewertung[0];
  auto const& bewertung_vl = e.bewertung[1];
  auto const& bewertung_vr = e.bewertung[2];
  auto const& bewertung_lv = e.bewertung[3];
  auto const& bewertung_ll = e.bewertung[4];
  auto const& bewertung_lr = e.bewertung[5];
  auto const& bewertung_rv = e.bewertung[6];
  auto const& bewertung_rl = e.bewertung[7];
  auto const& bewertung_rr = e.bewertung[8];

  auto const bewertung_v = std::min(bewertung_vv, bewertung_vl, bewertung_vr);
  auto const bewertung_l = std::min(bewertung_lv, bewertung_ll, bewertung_lr);
  auto const bewertung_r = std::min(bewertung_rv, bewertung_rl, bewertung_rr);

  ostr << "V = " << bewertung_v << ", "
    << "L = " << bewertung_l << ", "
    << "R = " << bewertung_r << "\n";

  ostr << '\n';
  ostr << "v    " << bewertung_vv << "   \t" << bewertung_vl << "   \t" << bewertung_vr << " (" << e.nachbarn_frei[0] << ")\n";
  ostr << "l    " << bewertung_lv << "   \t" << bewertung_ll << "   \t" << bewertung_lr << " (" << e.nachbarn_frei[1] << ")\n";
  ostr << "r    " << bewertung_rv << "   \t" << bewertung_rl << "   \t" << bewertung_rr << " (" << e.nachbarn_frei[2] << ")\n";

  return ostr;
} // ostream& operator<<(ostream& ostr, Tiefensuche::RichtungenErgebnis const& e)

} // namespace TaktikNS
