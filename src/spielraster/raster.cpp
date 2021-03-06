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

#include "raster.h"

#include <sstream>
#include <set>
#include <iterator>

/**
 ** Standardkonstruktor
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2014-10-25
 **/
Raster::Raster()
{ }

/**
 ** Konstruktor
 ** 
 ** @param     breite   Breite des Rasters
 ** @param     laenge   Länge des Rasters
 **
 ** @return    -
 **
 ** @version   2014-11-22
 **/
Raster::Raster(int const breite, int const laenge) :
  breite_{breite},
  laenge_{laenge}
#ifndef USE_BITSET
  ,felder_(breite * laenge, false)
#endif
{
#ifdef USE_BITSET
  assert(this->groesse() <= RASTER_MAX_GROESSE);
#endif
}

/**
 ** Standardkonstruktor
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2014-11-01
 **/
Raster::~Raster()
{ }

/**
 ** Das Raster aus istr einlesen
 ** 
 ** @param     istr   Einlesestrom
 **
 ** @return    Einlesestrom
 **
 ** @version   2014-10-25
 **/
istream& 
Raster::einlesen(istream& istr)
{
  /* Beispiel:
     GAMEBOARDSTART 10,10
     ..........
     ..........
     ..........
     ....#.....
     ....#.....
     ....#.....
     ....#.....
     ..........
     ..........
     ..........
     GAMEBOARDEND
     */

  string zeile;

  std::getline(istr, zeile);
  // Größe des Rasters einlesen
  if (zeile.compare(0, 14, "GAMEBOARDSTART")) {
    cerr << "Fehler beim Laden des Rasters. Erwarte GAMEBOARDSTART:\n"
      << zeile << endl;
    return istr;
  }
  std::istringstream isstr(string(zeile, 15));
  isstr >> breite_;
  isstr.get();
  isstr >> laenge_;
#ifdef USE_BITSET
  assert(this->groesse() <= RASTER_MAX_GROESSE);
#else
  this->felder_ = vector<bool>(this->breite() * this->laenge());
#endif

  // Die einzelnen Rasters einlesen
  for (int y = 0; y < this->laenge(); ++y) {
    for (int x = 0; x < this->breite(); ++x) {
      this->felder_[x + y * this->breite()] = (istr.get() == '#');
    }
    while (   (istr.peek() == '\r')
           || (istr.peek() == '\n') )
      istr.get();
  }

  // Abschluss des Rasters
  std::getline(istr, zeile);
  if (zeile.compare(0, 12, "GAMEBOARDEND")) {
    cerr << "Fehler beim Laden des Rasters. Erwarte GAMEBOARDEND:\n"
      << zeile << endl;
    return istr;
  }

  return istr;
} // istream& Raster::einlesen(istream& istr)

/**
 ** Das Raster auf ostr ausgeben
 ** 
 ** @param     ostr   Ausgabestrom
 ** @param     raster  Raster zum ausgeben
 **
 ** @return    Ausgabestrom
 **
 ** @version   2014-10-25
 **/
ostream&
operator<<(ostream& ostr, Raster const& raster)
{ return raster.ausgeben(ostr); }

/**
 ** Das Raster auf ostr ausgeben
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
Raster::ausgeben(ostream& ostr) const
{
  for (int y = 0; y < this->laenge(); ++y) {
    for (int x = 0; x < this->breite(); ++x)
      ostr << ((*this)(x, y) ? '#' : '.');
    ostr << '\n';
  }

  return ostr;
} // ostream& Raster::ausgeben(ostream& ostr) const

/**
 ** -> Rückgabe
 ** 
 ** @param     -
 **
 ** @return    die Breite des Rasters
 **
 ** @version   2014-10-25
 **/
int
Raster::breite() const
{
  return this->breite_;
} // int Raster::breite() const

/**
 ** -> Rückgabe
 ** 
 ** @param     -
 **
 ** @return    die Länge des Rasters
 **
 ** @version   2014-10-25
 **/
int
Raster::laenge() const
{
  return this->laenge_;
} // int Raster::laenge() const

/**
 ** -> Rückgabe
 ** 
 ** @param     -
 **
 ** @return    die Größe des Rasters
 **
 ** @version   2014-12-08
 **/
int
Raster::groesse() const
{
  return (this->breite() * this->laenge());
} // int Raster::groesse() const

/**
 ** -> Rückgabe
 ** 
 ** @param     x  x-Position
 ** @param     y  y-Position
 **
 ** @return    Die Belegung an der Position
 **
 ** @version   2014-10-25
 **/
bool
Raster::operator()(int const x, int const y) const
{
  if (   (x < 0)
      || (y < 0)
      || (x >= this->breite())
      || (y >= this->laenge()))
    return true;
  return this->felder_[x + y * this->breite()];
} // bool Raster::operator()(int const x, int const y) const

/**
 ** -> Rückgabe
 ** 
 ** @param     x  x-Position des Rasters
 ** @param     y  y-Position des Rasters
 **
 ** @return    Die Belegung an der Position
 **
 ** @version   2014-10-25
 **/
bool
Raster::operator()(Position const& position) const
{
  return (*this)(position.x(), position.y());
} // bool Raster::operator()(Position const& position) const

/**
 ** setzt die Position auf besetzt
 ** 
 ** @param     x  x-Position
 ** @param     y  y-Position
 ** @param     wert   wert für des Feld, Vorgabe: belegt
 **
 ** @return    -
 **
 ** @version   2014-10-25
 **/
void
Raster::belege(int const x, int const y, bool const wert)
{
  if (Position{x, y})
  this->felder_[x + y * this->breite()] = wert;
  return;
} // void Raster::belege(int const x, int const y, bool const wert)


/**
 ** setzt die Position auf besetzt
 ** 
 ** @param     position   Position des Feldes
 ** @param     wert   wert für des Feld, Vorgabe: belegt
 **
 ** @return    -
 **
 ** @version   2014-10-25
 **/
void
Raster::belege(Position const& position, bool const wert)
{
  if (position)
  return this->belege(position.x(), position.y(), wert);
} // void Raster::belege(Position const& position, bool const wert)

/**
 ** Übernimmt die besetzten Felder von raster
 ** 
 ** @param     raster   zu übernehmendes Raster
 **
 ** @return    -
 **
 ** @version   2014-10-25
 **/
void
Raster::belege(Raster const& raster)
{
  assert((this->breite() == raster.breite())
         && (this->laenge() == raster.laenge()));

#ifdef USE_BITSET
  this->felder_ |= raster.felder_;
#else
  auto f2 = begin(raster.felder_);
  for (auto f1 = begin(this->felder_); f1 != end(this->felder_); ++f1)
    *f1 = (*f1 || *f2), ++f2;
#endif

  return ;
} // void Raster::belege(Raster const& raster)

/**
 ** Invertiere das Raster
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2014-11-27
 **/
void
Raster::invertiere()
{
  this->felder_.flip();
  return;
} // void Raster::invertiere()

/**
 ** -> Rückgabe
 ** 
 ** @param     -
 **
 ** @return    invertiertes Raster
 **
 ** @version   2014-11-27
 **/
Raster
Raster::invertiert() const
{
  auto r = *this;
  r.invertiere();
  return r;
} // Raster Raster::invertiert() const

/**
 ** -> Rückgabe
 ** 
 ** @param     -
 **
 ** @return    Anzahl der belegten Felder
 **
 ** @version   2014-11-27
 **/
int
Raster::felder_belegt() const
{
#ifdef USE_BITSET
  Felder f;
  f.set();
  f << (RASTER_MAX_GROESSE - this->groesse());
  return static_cast<int>((this->felder_ & f).count());
#else
  int n = 0;
  for (auto const& f : this->felder_)
    if (f)
      ++n;
  return n;
#endif
} // int Raster::felder_belegt() const

/**
 ** -> Rückgabe
 ** 
 ** @param     -
 **
 ** @return    Anzahl der freien Felder
 **
 ** @version   2014-11-22
 **/
int
Raster::felder_frei() const
{
  return (this->breite() * this->laenge() - this->felder_belegt());
} // int Raster::felder_frei() const

/**
 ** -> Rückgabe
 ** 
 ** @param     position    Position
 **
 ** @return    Anzahl der freien Nachbarfelder um position
 **
 ** @version   2014-10-31
 **/
int
Raster::nachbarn_frei(Position const& position) const
{
  return (   ((*this)(position + Richtung::NORDEN) ? 0 : 1)
          + ((*this)(position + Richtung::SUEDEN) ? 0 : 1)
          + ((*this)(position + Richtung::OSTEN) ? 0 : 1)
          + ((*this)(position + Richtung::WESTEN) ? 0 : 1));
} // int Raster::nachbarn_frei(Position const& position) const


/**
 ** -> Rückgabe
 ** 
 ** @param     position    Position
 **
 ** @return    ein (der) freie Nachbar
 **
 ** @version   2014-12-04
 **/
Position
Raster::freier_nachbar(Position const& position) const
{
  for (auto const& r : richtungen)
    if (!(*this)(position + r))
      return (position + r);
  return Position::KEINE;
} // Position Raster::freier_nachbar(Position const& position) const

/**
 ** -> Rückgabe
 ** 
 ** @param     position           Position
 ** @param     position_ueberprüfen   ob die Position überprüft werden soll (default: true)
 **
 ** @return    Der freie Raum unter 'position'
 **
 ** @version   2014-12-04
 **/
Raster
Raster::raum(Position const& position,
             bool const position_ueberpruefen) const
{
  Raster raum(this->breite(), this->laenge()); // Raster, das gefüllt wird
  if (position_ueberpruefen && (*this)(position))
    return raum;

  Raster raster(*this); // Raster, das gefüllt wird
  std::set<Position> positionen; // Noch zu betrachtende Positionen
  int n = 1; // Anzahl der Felder
  raster.belege(position);
  raum.belege(position);
  positionen.insert(position);
  while (!positionen.empty()) {
    auto p = begin(positionen);
    for (auto r : ::richtungen) {
      auto const p2 = *p + r;
      if (!raster(p2)) {
        raster.belege(p2);
        raum.belege(p2);
        n += 1;
        positionen.insert(p2);
      }
    } // for (auto r : ::richtungen)
    positionen.erase(p);
  } // while (!positionen.empty())

  return raster;
}  // Raster Raster::raum(Position const& position, bool const position_ueberpruefen = true) const

/**
 ** -> Rückgabe
 ** 
 ** @param     position           Position
 ** @param     position_ueberprüfen   ob die Position überprüft werden soll (default: true)
 **
 ** @return    Größe des Raumes; ist position belegt und position_ueberpruefen = true (nicht default), ist die Größe 0
 **
 ** @version   2014-11-11
 **/
int
Raster::raumgroesse(Position const& position,
                    bool const position_ueberpruefen) const
{
#if 0
  return (this->raum(position, position_ueberpruefen).felder_belegt());
#endif
  if (position_ueberpruefen && (*this)(position))
    return 0;

  Raster raster(*this); // Raster, das gefüllt wird
  std::set<Position> positionen; // Noch zu betrachtende Positionen
  int n = 1; // Anzahl der Felder
  raster.belege(position);
  positionen.insert(position);
  while (!positionen.empty()) {
    auto p = begin(positionen);
    for (auto r : ::richtungen) {
      auto const p2 = *p + r;
      if (!raster(p2)) {
        raster.belege(p2);
        n += 1;
        positionen.insert(p2);
      }
    } // for (auto r : ::richtungen)
    positionen.erase(p);
  } // while (!positionen.empty())

  return n;
}  // int Raster::raumgroesse(Position const& position, bool const position_ueberpruefen = true) const

/**
 ** -> Rückgabe
 ** Im Unterschied zu 'raumgroesse' wird von Sackgassen nur eine berücksichtigt.
 ** es gilt: raumgroesse_erreichbar <= raumgroesse
 ** 
 ** @param     position               Position
 ** @param     position_ueberprüfen   ob die Position überprüft werden soll (default: false)
 **
 ** @return    Größe des erreichbaren Raumes; ist position belegt und position_ueberpruefen = true (default), ist die Größe 0
 **
 ** @version   2014-11-11
 **/
int
Raster::raumgroesse_erreichbar(Position const& position,
                               bool const position_ueberpruefen) const
{
  if (position_ueberpruefen && (*this)(position))
    return 0;

  int n_max = 0; // Maximale Anzahl an Feldern
  for (auto r : ::richtungen) {
    if ((*this)(position + r))
      continue;
    Raster raster(*this); // Raster, das gefüllt wird
    if (!(*this)(position)) {
      raster.belege(position);
    }
    std::set<Position> positionen; // Noch zu betrachtende Positionen
    std::set<Position> sackgassen; // Sackgassen, diese werden separat berücksichtigt
    int n = 0; // Anzahl der Felder
    {
      auto p = position + r;
      while (raster.nachbarn_frei(p) == 1) {
        raster.belege(p);
        n += 1;
        p = raster.freier_nachbar(p);
      }
      positionen.insert(p);
    }
    while (!positionen.empty()) {
      while (!positionen.empty()) {
        auto p = *begin(positionen);
        positionen.erase(p);
        raster.belege(p);
        n += 1;
        while (raster.nachbarn_frei(p) == 1) {
          p = raster.freier_nachbar(p);
          raster.belege(p);
          positionen.erase(p);
          n += 1;
        }
        for (auto r : ::richtungen) {
          auto const p2 = p + r;
          if (   raster(p2)
              || (positionen.find(p2) != positionen.end()))
            continue;

          // Auf einen Engpass prüfen, dieser kann eine Sackgasse sein
          if (   (raster.nachbarn_frei(p2) <= 1)
             ) {
            if (sackgassen.find(p2) != sackgassen.end()) {
              // Einen zweiten Weg zu der Position gefunden, also keine Sackgasse
              sackgassen.erase(p2);
              raster.belege(p2);
              n += 1;
            } else {
              sackgassen.insert(p2);
            }
          } else {
            positionen.insert(p2);
          }
        } // for (auto r : ::richtungen)
      } // while (!positionen.empty())

      // Schleifen erkennen
      for (auto p = begin(sackgassen); p != end(sackgassen); ++p) {
        auto raum = raster.raum(*p);
        auto p2 = p;
        for (++p2; p2 != end(sackgassen); ) {
          if (raum(*p2)) {
            positionen.insert(*p2);
            positionen.insert(*p);
            p2 = sackgassen.erase(p2);
          } else {
            ++p2;
          }
        } // for (p2 > p)
      } // for (p : sackgassen)
      for (auto const& p : positionen) {
        sackgassen.erase(p);
      }
    } // while (!positionen.empty())

    // Von allen Sackgassen kann nur eine gefüllt werden -- wähle die größte
    int m = 0; // maximale Größe der Sackgassen
    for (auto p : sackgassen) {
      int m2 = 0;
      // Korridore (nur ein Weg möglich) bis zum Ende durchgehen
      while (raster.nachbarn_frei(p) == 1) {
        raster.belege(p);
        m2 += 1;
        p = raster.freier_nachbar(p);
      }
      if (!raster.nachbarn_frei(p)) {
        m = std::max(m, m2 + 1);
        continue;
      }
      // Sackgasse füllen
      m2 += raster.raumgroesse_erreichbar(p);

      m = std::max(m, m2);
    } // for (p : sackgassen)
    n_max = std::max(n + m, n_max);
  } // for (auto r : ::richtungen)
  if (!(*this)(position))
    n_max += 1;
  return std::min(n_max, this->raumgroesse(position));
}  // int Raster::raumgroesse_erreichbar(Position const& position, bool const position_ueberpruefen = false) const

/**
 ** -> Rückgabe
 ** 
 ** @param     pa    erste Position
 ** @param     pb    zweite Position
 **
 ** @return    kürzeste Entfernung zwischen den beiden Positionen, -1, wenn es keinen Weg zwischen beiden gibt
 **
 ** @version   2014-11-01
 **/
int
Raster::kuerzeste_entfernung(Position const& pa, Position const& pb) const
{
  if (pa == pb)
    return 0;
  if ((pb.x() < 0) || (pb.y() < 0)) // Außerhalb vom Spielraster, zum Beispiel ein gestorbener Bot
    return -1;

  Raster raster(*this); // Raster, das gefüllt wird
  std::set<Position> positionen; // Noch zu betrachtende Positionen
  positionen.insert(pa);
  int n = 1; // Entfernung
  std::set<Position> positionenb; // Noch zu betrachtende Positionen der nächsten Ebene
  while (!positionen.empty()) {
    positionenb.clear();
    for (auto p : positionen) {
      for (auto r : ::richtungen) {
        auto const p2 = p + r;
        if (p2 == pb)
          return n;
        if (!raster(p2)) {
          raster.belege(p2);
          positionenb.insert(p2);
        }
      } // for (auto r : ::richtungen)
    } // for (auto p : positionen)
    positionen = positionenb;
    n += 1;
  } // while (!positionen.empty())

  return -1;
} // int Raster::kuerzeste_entfernung(Position const& pa, Position const& pb) const

/**
 ** -> Rückgabe
 ** 
 ** @param     raster1   erstes Raster
 ** @param     raster2   zweites Raster
 **
 ** @return    ob raster1 < raster2 ist
 **
 ** @version   2014-11-09
 **
 ** @todo      Performance stark verbessern
 **/
bool
operator<(Raster const& raster1, Raster const& raster2)
{
  if (raster1.laenge() < raster2.laenge())
    return true;
  if (raster1.laenge() > raster2.laenge())
    return false;
  if (raster1.breite() < raster2.breite())
    return true;
  if (raster1.breite() > raster2.breite())
    return false;


  for (unsigned i = 0; i < raster1.felder_.size(); ++i) {
    if (raster1.felder_[i] && !raster2.felder_[i])
      return true;
    if (!raster1.felder_[i] && raster2.felder_[i])
      return false;
  }
  return false;
} // bool operator<(Raster const& raster1, Raster const& raster2)
