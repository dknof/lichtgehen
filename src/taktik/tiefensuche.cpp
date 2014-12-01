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

#define TIEFE_MAX 3

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
    Taktik{"Tiefensuche", "Bestimme alle möglichen möglichen Wege und beschreite den „besten“ (nur für zwei Bots)."}
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

      auto const bewertung_vv = this->tiefensuche(spielraster, bot, bot2,
                                                  Bewegungsrichtung::VORWAERTS,
                                                  Bewegungsrichtung::VORWAERTS,
                                                  1);
      auto const bewertung_vl = this->tiefensuche(spielraster, bot, bot2,
                                                  Bewegungsrichtung::VORWAERTS,
                                                  Bewegungsrichtung::LINKS,
                                                  1);
      auto const bewertung_vr = this->tiefensuche(spielraster, bot, bot2,
                                                  Bewegungsrichtung::VORWAERTS,
                                                  Bewegungsrichtung::RECHTS,
                                                  1);
      auto const bewertung_lv = this->tiefensuche(spielraster, bot, bot2,
                                                  Bewegungsrichtung::LINKS,
                                                  Bewegungsrichtung::VORWAERTS,
                                                  1);
      auto const bewertung_ll = this->tiefensuche(spielraster, bot, bot2,
                                                  Bewegungsrichtung::LINKS,
                                                  Bewegungsrichtung::LINKS,
                                                  1);
      auto const bewertung_lr = this->tiefensuche(spielraster, bot, bot2,
                                                  Bewegungsrichtung::LINKS,
                                                  Bewegungsrichtung::RECHTS,
                                                  1);
      auto const bewertung_rv = this->tiefensuche(spielraster, bot, bot2,
                                                  Bewegungsrichtung::RECHTS,
                                                  Bewegungsrichtung::VORWAERTS,
                                                  1);
      auto const bewertung_rl = this->tiefensuche(spielraster, bot, bot2,
                                                  Bewegungsrichtung::RECHTS,
                                                  Bewegungsrichtung::LINKS,
                                                  1);
      auto const bewertung_rr = this->tiefensuche(spielraster, bot, bot2,
                                                  Bewegungsrichtung::RECHTS,
                                                  Bewegungsrichtung::RECHTS,
                                                  1);

      // Minimiere den Verlust
      auto const bewertung_v = std::min(bewertung_vv, bewertung_vl, bewertung_vr);
      auto const bewertung_l = std::min(bewertung_lv, bewertung_ll, bewertung_lr);
      auto const bewertung_r = std::min(bewertung_rv, bewertung_rl, bewertung_rr);

#if 1
#if 0
      cdebug << "vv: " << bewertung_vv << '\n';
      cdebug << "vl: " << bewertung_vl << '\n';
      cdebug << "vr: " << bewertung_vr << '\n';
      cdebug << "lv: " << bewertung_lv << '\n';
      cdebug << "ll: " << bewertung_ll << '\n';
      cdebug << "lr: " << bewertung_lr << '\n';
      cdebug << "rv: " << bewertung_rv << '\n';
      cdebug << "rl: " << bewertung_rl << '\n';
      cdebug << "rr: " << bewertung_rr << '\n';
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
    } // Taktik::Ergebnis Tiefensuche::ergebnis(Spielraster const& spielraster, int const bot)

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
   **
   ** @return    Wert der Bewegungen
   **
   ** @version   2014-11-29
   **/
  Tiefensuche::Bewertung
    Tiefensuche::tiefensuche(Spielraster const& spielraster,
                             int const bot1, int const bot2,
                             Bewegungsrichtung const r1,
                             Bewegungsrichtung const r2,
                             int const tiefe) const
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

      if (tiefe == TIEFE_MAX)
        return Bewertung(Bewertung::Spielstand::OFFEN,
                         tiefe,
                         this->bewertung(spielraster, bot1, bot2));

      // rekursiver Aufruf (aufwendig)
      auto const bewertung_vv = this->tiefensuche(sr, bot1, bot2,
                                                  Bewegungsrichtung::VORWAERTS,
                                                  Bewegungsrichtung::VORWAERTS,
                                                  tiefe + 1);
      auto const bewertung_vl = this->tiefensuche(sr, bot1, bot2,
                                                  Bewegungsrichtung::VORWAERTS,
                                                  Bewegungsrichtung::LINKS,
                                                  tiefe + 1);
      auto const bewertung_vr = this->tiefensuche(sr, bot1, bot2,
                                                  Bewegungsrichtung::VORWAERTS,
                                                  Bewegungsrichtung::RECHTS,
                                                  tiefe + 1);
      auto const bewertung_lv = this->tiefensuche(sr, bot1, bot2,
                                                  Bewegungsrichtung::LINKS,
                                                  Bewegungsrichtung::VORWAERTS,
                                                  tiefe + 1);
      auto const bewertung_ll = this->tiefensuche(sr, bot1, bot2,
                                                  Bewegungsrichtung::LINKS,
                                                  Bewegungsrichtung::LINKS,
                                                  tiefe + 1);
      auto const bewertung_lr = this->tiefensuche(sr, bot1, bot2,
                                                  Bewegungsrichtung::LINKS,
                                                  Bewegungsrichtung::RECHTS,
                                                  tiefe + 1);
      auto const bewertung_rv = this->tiefensuche(sr, bot1, bot2,
                                                  Bewegungsrichtung::RECHTS,
                                                  Bewegungsrichtung::VORWAERTS,
                                                  tiefe + 1);
      auto const bewertung_rl = this->tiefensuche(sr, bot1, bot2,
                                                  Bewegungsrichtung::RECHTS,
                                                  Bewegungsrichtung::LINKS,
                                                  tiefe + 1);
      auto const bewertung_rr = this->tiefensuche(sr, bot1, bot2,
                                                  Bewegungsrichtung::RECHTS,
                                                  Bewegungsrichtung::RECHTS,
                                                  tiefe + 1);

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
        auto const m = std::min(bewertung_v, bewertung_l, bewertung_r);
        if (m == bewertung_v)
          return std::max(bewertung_vv, bewertung_lv, bewertung_rv);
        else if (m == bewertung_l)
          return std::max(bewertung_vl, bewertung_ll, bewertung_rl);
        else // if (m == bewertung_r)
          return std::max(bewertung_vr, bewertung_lr, bewertung_rr);
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
    } // Taktik::Ergebnis Tiefensuche::ergebnis(Spielraster const& spielraster, int const bot) const

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
