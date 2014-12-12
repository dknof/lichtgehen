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

#include "tiefensuche.h"
#include <cstdlib>
#include <chrono>

#ifdef USE_THREADS
#include <array>
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
    Taktik{"Tiefensuche", "Bestimme alle möglichen Wege und beschreite den „besten“ (nur für zwei Bots)."},
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
   ** @param     bot   die Nummer des Bots
   **
   ** @return    Richtung mit dem „besten“ Wert
   **
   ** @version   2014-11-08
   **/
  Taktik::Ergebnis
    Tiefensuche::ergebnis(Spielraster const& spielraster,
                          int const bot)
    {
      // nur für zwei Bots
      if (spielraster.rauminfo(bot).bot_anz != 1)
        return false;

      // bot 2 suchen
      int bot2;
      for (bot2 = 0; bot2 < spielraster.bot_anz(); ++bot2) {
        if (bot2 == bot)
          continue;
        if (spielraster.kuerzeste_entfernung(spielraster.position(bot),
                                             spielraster.position(bot2))
            >= 0)
          break;
      }
      if (bot2 == spielraster.bot_anz())
        return false;

      auto const felder_frei = spielraster.felder_frei();
      this->tiefe_max = (  (felder_frei >= 2000) ? 0
                         : (felder_frei >=  160) ? 1
                         : (felder_frei >=   80) ? 2
                         : (felder_frei >=   40) ? 3
                         : (felder_frei >=   20) ? 4
                         : felder_frei);

      Taktik::Ergebnis ergebnis = false;
#if 1
      // Tiefe ausprobieren
      auto const zeit_start = std::chrono::system_clock::now();
      auto dauer = std::chrono::duration<double>{zeit_start - zeit_start};
      this->tiefe_max = 0;
      do {
#ifdef USE_THREADS
        ergebnis = this->tiefensuche_thread(spielraster, bot, bot2);
#else
        ergebnis = this->tiefensuche(spielraster, bot, bot2);
#endif
        dauer = std::chrono::system_clock::now() - zeit_start;
        this->tiefe_max += 1;
      } while (dauer.count() < 0.1 * 0.9 * ZEITBESCHRAENKUNG);
#else
      ergebnis = tiefensuche(spielraster, bot, bot2);
#endif

      return ergebnis;
    } // Taktik::Ergebnis Tiefensuche::ergebnis(Spielraster spielraster, int bot)

#if 0
  /**
   ** -> Rückgabe
   ** Suche mit Tiefensuche die „beste“ Richtung
   ** 
   ** @param     spielraster   das Spielraster
   ** @param     bot   die Nummer des Bots
   ** @param     bot2   die Nummer des zweiten Bots
   **
   ** @return    Richtung mit dem „besten“ Wert
   **
   ** @version   2014-11-08
   **/
  Taktik::Ergebnis
    Tiefensuche::tiefensuche(Spielraster const& spielraster,
                          int const bot, int const bot2) const
    {
      auto const bewertung_vv = this->tiefensuche(spielraster, bot, bot2,
                                                  Bewegungsrichtung::VORWAERTS,
                                                  Bewegungsrichtung::VORWAERTS,
                                                  0);
      auto const bewertung_vl = this->tiefensuche(spielraster, bot, bot2,
                                                  Bewegungsrichtung::VORWAERTS,
                                                  Bewegungsrichtung::LINKS,
                                                  0);
      auto const bewertung_vr = this->tiefensuche(spielraster, bot, bot2,
                                                  Bewegungsrichtung::VORWAERTS,
                                                  Bewegungsrichtung::RECHTS,
                                                  0);
      auto const bewertung_lv = this->tiefensuche(spielraster, bot, bot2,
                                                  Bewegungsrichtung::LINKS,
                                                  Bewegungsrichtung::VORWAERTS,
                                                  0);
      auto const bewertung_ll = this->tiefensuche(spielraster, bot, bot2,
                                                  Bewegungsrichtung::LINKS,
                                                  Bewegungsrichtung::LINKS,
                                                  0);
      auto const bewertung_lr = this->tiefensuche(spielraster, bot, bot2,
                                                  Bewegungsrichtung::LINKS,
                                                  Bewegungsrichtung::RECHTS,
                                                  0);
      auto const bewertung_rv = this->tiefensuche(spielraster, bot, bot2,
                                                  Bewegungsrichtung::RECHTS,
                                                  Bewegungsrichtung::VORWAERTS,
                                                  0);
      auto const bewertung_rl = this->tiefensuche(spielraster, bot, bot2,
                                                  Bewegungsrichtung::RECHTS,
                                                  Bewegungsrichtung::LINKS,
                                                  0);
      auto const bewertung_rr = this->tiefensuche(spielraster, bot, bot2,
                                                  Bewegungsrichtung::RECHTS,
                                                  Bewegungsrichtung::RECHTS,
                                                  1);

      // Minimiere den Verlust
      auto const bewertung_v = std::min(bewertung_vv, bewertung_vl, bewertung_vr);
      auto const bewertung_l = std::min(bewertung_lv, bewertung_ll, bewertung_lr);
      auto const bewertung_r = std::min(bewertung_rv, bewertung_rl, bewertung_rr);

#if 0
#if 1
      cdebug << '\n';
      cdebug << "v    " << bewertung_vv << "   \t" << bewertung_vl << "   \t" << bewertung_vr << "\n";
      cdebug << "l    " << bewertung_lv << "   \t" << bewertung_ll << "   \t" << bewertung_lr << "\n";
      cdebug << "r    " << bewertung_rv << "   \t" << bewertung_rl << "   \t" << bewertung_rr << "\n";

#endif

      cdebug << bot << ": "
        << "V = " << bewertung_v << ", "
        << "L = " << bewertung_l << ", "
        << "R = " << bewertung_r << "\n";
#endif

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

      return false;
    } // Taktik::Ergebnis Tiefensuche::tiefensuche(Spielraster const& spielraster, int const bot, int const bot2) const
#endif

  /**
   ** -> Rückgabe
   ** Suche mit Tiefensuche die „beste“ Richtung, verwended 9 threads
   ** 
   ** @param     spielraster   das Spielraster
   ** @param     bot   die Nummer des Bots
   ** @param     bot2   die Nummer des zweiten Bots
   **
   ** @return    Richtung mit dem „besten“ Wert
   **
   ** @version   2014-11-08
   **/
  Taktik::Ergebnis
    Tiefensuche::tiefensuche_thread(Spielraster const& spielraster,
                          int const bot, int const bot2) const
    {
#ifdef USE_THREADS
#define THREAD(b1, b2) std::async(std::launch::async, std::bind(&Tiefensuche::tiefensuche_iteration, this, spielraster, bot, bot2, b1, b2, 0))
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
      auto const bewertung_vv = threads[0].get();
      auto const bewertung_vl = threads[1].get();
      auto const bewertung_vr = threads[2].get();
      auto const bewertung_lv = threads[3].get();
      auto const bewertung_ll = threads[4].get();
      auto const bewertung_lr = threads[5].get();
      auto const bewertung_rv = threads[6].get();
      auto const bewertung_rl = threads[7].get();
      auto const bewertung_rr = threads[8].get();

      // Minimiere den Verlust
      auto const bewertung_v = std::min(bewertung_vv, bewertung_vl, bewertung_vr);
      auto const bewertung_l = std::min(bewertung_lv, bewertung_ll, bewertung_lr);
      auto const bewertung_r = std::min(bewertung_rv, bewertung_rl, bewertung_rr);

#if 0
#if 1
      cdebug << '\n';
      cdebug << "v    " << bewertung_vv << "   \t" << bewertung_vl << "   \t" << bewertung_vr << "\n";
      cdebug << "l    " << bewertung_lv << "   \t" << bewertung_ll << "   \t" << bewertung_lr << "\n";
      cdebug << "r    " << bewertung_rv << "   \t" << bewertung_rl << "   \t" << bewertung_rr << "\n";

#endif

      cdebug << bot << ": "
        << "V = " << bewertung_v << ", "
        << "L = " << bewertung_l << ", "
        << "R = " << bewertung_r << "\n";
#endif

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
#endif // #ifdef USE_THREADS

      return false;
    } // Taktik::Ergebnis Tiefensuche::tiefensuche_thread(Spielraster const& spielraster, int const bot, int const bot2) const

  /**
   ** -> Rückgabe
   ** bewertet das Spielfeld rekursiv
   ** Der Wert ist vom ersten Bot zu sehen. 
   ** 1: er gewinnt, -1: er verliert, 0: unentschieden
   ** 
   ** @param     spielraster   das Spielraster
   ** @param     bot1          die Nummer des ersten Bots
   ** @param     bot2          die Nummer des zweiten Bots
   ** @param     r1            die Bewegungsrichtung des ersten Bots
   ** @param     r2            die Bewegungsrichtung des zweiten Bots
   ** @param     tiefe         verbleibende Tiefe
   **
   ** @return    Wert der Bewegungen
   **
   ** @version   2014-11-29
   **/
  Tiefensuche::Bewertung
    Tiefensuche::tiefensuche_iteration(Spielraster const& spielraster,
                                       int const bot1, int const bot2,
                                       Bewegungsrichtung const r1,
                                       Bewegungsrichtung const r2,
                                       int tiefe) const
    {
      BotPosition const bp1 = spielraster.position(bot1);
      BotPosition const bp2 = spielraster.position(bot2);

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

      // Beide Bots konnten einen Schritt machen

      // die Schritte durchführen
      Spielraster sr{spielraster};
      sr.bewege_bot(bot1, r1);
      sr.bewege_bot(bot2, r2);

      if (tiefe >= this->tiefe_max)
        return Bewertung(Bewertung::Spielstand::OFFEN,
                         tiefe,
                         this->bewertung(sr, bot1, bot2));

      tiefe += 1;
      // rekursiver Aufruf (aufwendig)
      auto const bewertung_vv = this->tiefensuche_iteration(sr, bot1, bot2,
                                                            Bewegungsrichtung::VORWAERTS,
                                                            Bewegungsrichtung::VORWAERTS,
                                                            tiefe);
      auto const bewertung_vl = this->tiefensuche_iteration(sr, bot1, bot2,
                                                            Bewegungsrichtung::VORWAERTS,
                                                            Bewegungsrichtung::LINKS,
                                                            tiefe);
      auto const bewertung_vr = this->tiefensuche_iteration(sr, bot1, bot2,
                                                            Bewegungsrichtung::VORWAERTS,
                                                            Bewegungsrichtung::RECHTS,
                                                            tiefe);
      auto const bewertung_lv = this->tiefensuche_iteration(sr, bot1, bot2,
                                                            Bewegungsrichtung::LINKS,
                                                            Bewegungsrichtung::VORWAERTS,
                                                            tiefe);
      auto const bewertung_ll = this->tiefensuche_iteration(sr, bot1, bot2,
                                                            Bewegungsrichtung::LINKS,
                                                            Bewegungsrichtung::LINKS,
                                                            tiefe);
      auto const bewertung_lr = this->tiefensuche_iteration(sr, bot1, bot2,
                                                            Bewegungsrichtung::LINKS,
                                                            Bewegungsrichtung::RECHTS,
                                                            tiefe);
      auto const bewertung_rv = this->tiefensuche_iteration(sr, bot1, bot2,
                                                            Bewegungsrichtung::RECHTS,
                                                            Bewegungsrichtung::VORWAERTS,
                                                            tiefe);
      auto const bewertung_rl = this->tiefensuche_iteration(sr, bot1, bot2,
                                                            Bewegungsrichtung::RECHTS,
                                                            Bewegungsrichtung::LINKS,
                                                            tiefe);
      auto const bewertung_rr = this->tiefensuche_iteration(sr, bot1, bot2,
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
    } // Taktik::Ergebnis Tiefensuche::tiefensuche_iteration(Spielraster const& spielraster, int const bot) const

  /**
   ** -> Rückgabe
   ** 
   ** @param     spielraster   das Spielraster
   ** @param     bot1          die Nummer des ersten Bots
   ** @param     bot2          die Nummer des zweiten Bots
   **
   ** @return    Bewertung des Spielrasters
   **
   ** @version   2014-11-29
   **/
  double
    Tiefensuche::bewertung(Spielraster const& spielraster,
                           int bot1, int bot2) const
    {
      return (spielraster.einflussbereich_groesse_erreichbar(bot1)
              - spielraster.einflussbereich_groesse_erreichbar(bot2));
    } // double Tiefensuche::bewertung(Spielraster const& spielraster, int bot1, int bot2) const

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
        case Tiefensuche::Bewertung::Spielstand::OFFEN:
          // todo: Überdenken
          if ((lhs.wert < 0) && (rhs.wert < 0))
            return (lhs.tiefe < rhs.tiefe);
          else if (lhs.wert == rhs.wert)
            return (lhs.tiefe < rhs.tiefe);
          else
            return (lhs.wert < rhs.wert);
        } // switch (rhs.spielstand)
      case Tiefensuche::Bewertung::Spielstand::OFFEN:
        switch (rhs.spielstand) {
        case Tiefensuche::Bewertung::Spielstand::VERLOREN:
          return false;
        case Tiefensuche::Bewertung::Spielstand::GEWONNEN:
          return true;
        case Tiefensuche::Bewertung::Spielstand::UNENTSCHIEDEN:
        case Tiefensuche::Bewertung::Spielstand::OFFEN:
          // todo: Überdenken
          if ((lhs.wert < 0) && (rhs.wert < 0)) {
            if (lhs.tiefe == rhs.tiefe)
              return (lhs.wert < rhs.wert);
            return (lhs.tiefe < rhs.tiefe);
          } else if (lhs.wert == rhs.wert)
            return (lhs.tiefe < rhs.tiefe);
          else
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

} // namespace TaktikNS
