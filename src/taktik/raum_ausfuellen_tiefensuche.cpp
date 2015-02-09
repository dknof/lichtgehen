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

#include "raum_ausfuellen_tiefensuche.h"

#include <chrono>

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
   ** @version   2014-12-05
   **/
  RaumAusfuellenTiefensuche::RaumAusfuellenTiefensuche() :
    Taktik{"Raum ausfüllen (Tiefensuche)", "Den Raum möglichst gut ausfüllen, rekursiv mit Tiefensuche wird ein optimaler Weg bestimmt."}
  { }

  /**
   ** -> Rückgabe
   ** Suche mit Tiefensuche den längsten Weg.
   ** 
   ** @param     spielraster   das Spielraster
   ** @param     bot_nummer   die Nummer des Bots
   **
   ** @return    Richtung mit dem längsten Weg
   **
   ** @version   2014-12-14
   **/
  Taktik::Ergebnis
    RaumAusfuellenTiefensuche::ergebnis(Spielraster const& spielraster,
                                        int const bot_nummer)
    {
      auto const& bp = spielraster.position(bot_nummer);
#ifdef TIEFE_DYNAMISCH
      // Tiefe ausprobieren
      auto const zeit_start = std::chrono::system_clock::now();
      auto dauer = std::chrono::duration<double>{zeit_start - zeit_start};
      int tiefe_max = 0;
      Bewertung ev;
      Bewertung el;
      Bewertung er;
      do {
#ifdef USE_THREADS
#define THREAD(b) std::async(std::launch::async, std::bind(&RaumAusfuellenTiefensuche::wegweite, this, spielraster, bp + b, tiefe_max))
        std::array<std::future<int>, 3> threads
          = {THREAD(Bewegungsrichtung::VORWAERTS),
            THREAD(Bewegungsrichtung::LINKS),
            THREAD(Bewegungsrichtung::RECHTS)};
#undef THREAD
        ev = Bewertung(threads[0].get(),
                       spielraster.nachbarn_frei(bp + Bewegungsrichtung::VORWAERTS));
        el = Bewertung(threads[1].get(),
                       spielraster.nachbarn_frei(bp + Bewegungsrichtung::LINKS));
        er = Bewertung(threads[2].get(),
                       spielraster.nachbarn_frei(bp + Bewegungsrichtung::RECHTS));
#else
        ev = Bewertung(this->wegweite(spielraster, bp + Bewegungsrichtung::VORWAERTS, tiefe_max),
                       spielraster.nachbarn_frei(bp + Bewegungsrichtung::VORWAERTS));
        el = Bewertung(this->wegweite(spielraster, bp + Bewegungsrichtung::LINKS, tiefe_max),
                       spielraster.nachbarn_frei(bp + Bewegungsrichtung::LINKS));
        er = Bewertung(this->wegweite(spielraster, bp + Bewegungsrichtung::RECHTS, tiefe_max),
                       spielraster.nachbarn_frei(bp + Bewegungsrichtung::RECHTS));
#endif
        dauer = std::chrono::system_clock::now() - zeit_start;
        tiefe_max += 1;
      } while (dauer.count() < 0.1 * 0.9 * ZEITBESCHRAENKUNG);
#else
      int const tiefe_max = 4;
      auto const ev
        = Bewertung(this->wegweite(spielraster, bp + Bewegungsrichtung::VORWAERTS, tiefe_max),
                    spielraster.nachbarn_frei(bp + Bewegungsrichtung::VORWAERTS));
      auto const el
        = Bewertung(this->wegweite(spielraster, bp + Bewegungsrichtung::LINKS, tiefe_max),
                    spielraster.nachbarn_frei(bp + Bewegungsrichtung::LINKS));
      auto const er
        = Bewertung(this->wegweite(spielraster, bp + Bewegungsrichtung::RECHTS, tiefe_max),
                    spielraster.nachbarn_frei(bp + Bewegungsrichtung::RECHTS));
#endif
#if 0
      CLOG << Bewegungsrichtung::VORWAERTS << ": " << ev << endl;
      CLOG << Bewegungsrichtung::LINKS     << ": " << el << endl;
      CLOG << Bewegungsrichtung::RECHTS    << ": " << er << endl;
#endif

#if 0
      // bei viel freier Fläche: möglichst am Rand halten
      if (   (el.weite >= 50)
          && ((el.weite * 101)/100 + 5 >= std::max(er.weite, ev.weite)) )
        return Bewegungsrichtung::LINKS;
      if (   (ev.weite >= 50)
          && ((ev.weite * 101)/100 + 5 >= std::max(el.weite, er.weite)) )
        return Bewegungsrichtung::VORWAERTS;
#endif

      if (el >= ev)
        if (el >= er)
          return Bewegungsrichtung::LINKS;
        else
          return Bewegungsrichtung::RECHTS;
      if (ev >= er)
        return Bewegungsrichtung::VORWAERTS;
      else
        return Bewegungsrichtung::RECHTS;
    } // Taktik::Ergebnis RaumAusfuellenTiefensuche::ergebnis(Spielraster const& spielraster, int const bot_nummer)

  /**
   ** -> Rückgabe
   ** Suche mit Tiefensuche den längsten Weg.
   ** Sollte bis etwa 25 Rasterer verwendet werden.
   ** 28 Rasterer: 1,05 s
   ** 5x5 Rasterer: 0,255 s
   ** 
   ** @param     raster              Spielraster
   ** @param     p                   Position auf dem Raster
   ** @param     tiefe_verbleibend   verbleibende Tiefe zu prüfen
   **
   ** @return    Längster Weg ab der Position
   **
   ** @version   2014-12-05
   **/
  int
    RaumAusfuellenTiefensuche::wegweite(Raster raster,
                                        Position p,
                                        int tiefe_verbleibend) const
    {
      if (raster(p))
        return 0;
      raster.belege(p);
      int n = 1;
      while (raster.nachbarn_frei(p) == 1) {
        p = raster.freier_nachbar(p);
        raster.belege(p);
        n += 1;
      }
      if (raster.nachbarn_frei(p) == 0) {
        return n;
      }
      if (tiefe_verbleibend == 0) {
        return (n + raster.raumgroesse_erreichbar(p));
      }

      tiefe_verbleibend -= 1;
      int wegweite_max = 0;
      for (auto& r : ::richtungen) {
        wegweite_max = std::max(wegweite_max,
                                this->wegweite(raster, p + r, tiefe_verbleibend));
      }
      return n + wegweite_max;
    } // int RaumAusfuellenTiefensuche::wegweite(Raster raster, BotPosition const p, int tiefe_verbleibend)

  /**
   ** -> Rückgabe
   ** vergleicht zwei Bewertungen
   ** 
   ** @param     lhs   linker Operand
   ** @param     rhs   linker Operand
   **
   ** @return    wahr, wenn die Weite von lhs größer ist als von rhs oder beide weiten gleich sind und die Anzahl an freien Nachbarn von lhs kleiner(!) oder gleich der von rhs ist
   **
   ** @version   2014-12-06
   **/
  bool
    operator>=(RaumAusfuellenTiefensuche::Bewertung const& lhs,
               RaumAusfuellenTiefensuche::Bewertung const& rhs)
    {
      return (   (lhs.weite > rhs.weite)
              || (   (lhs.weite == rhs.weite)
                  && (lhs.nachbarn_frei <= rhs.nachbarn_frei) ) );
    } // bool operator>=(RaumAusfuellenTiefensuche::Bewertung lhs, RaumAusfuellenTiefensuche::Bewertung rhs);

  /**
   ** Gibt die Bewertung auf ostr aus
   ** 
   ** @param     ostr   Ausgabestrom
   ** @param     e      Bewertung
   **
   ** @return    Ausgabestrom
   **
   ** @version   2014-12-06
   **/
  ostream&
    operator<<(ostream& ostr,
               RaumAusfuellenTiefensuche::Bewertung const& e)
    {
      ostr << e.weite << ", " << e.nachbarn_frei;
      return ostr;
    } // ostream& operator<<(ostream& ostr, RaumAusfuellenTiefensuche::Bewertung e)

} // namespace TaktikNS
