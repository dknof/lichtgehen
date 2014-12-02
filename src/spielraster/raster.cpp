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

#include "raster.h"

#include <sstream>
#include <set>

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
#ifndef USE_BITSET
  assert(breite * laenge <= RASTER_MAX_GROESSE);
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
  assert(this->breite() * this->laenge() <= RASTER_MAX_GROESSE);
#ifndef USE_BITSET
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
  f << (RASTER_MAX_GROESSE - this->breite() * this->laenge());
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
  if (position_ueberpruefen && (*this)(position))
    return 0;

  Raster raster(*this); // Raster, das gefüllt wird
  std::set<Position> positionen; // Noch zu betrachtende Positionen
  positionen.insert(position);
  int n = 1; // Anzahl der Felder
  while (!positionen.empty()) {
    auto p = begin(positionen);
    for (auto r : ::richtungen) {
      Position const p2 = *p + r;
      if (!raster(p2)) {
        raster.belege(p2);
        n += 1;
        positionen.insert(p2);
      }
    } // for (auto r : ::richtungen)
    positionen.erase(p);
  } // while (!positionen.empty())

  return n;
}  // int Raster::raumgroesse(Position const& position, bool const position_ueberpruelfen = true) const

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

  if ((*this)(position))
    return std::max(this->raumgroesse_erreichbar(position + Richtung::NORDEN, true),
                    this->raumgroesse_erreichbar(position + Richtung::OSTEN, true),
                    this->raumgroesse_erreichbar(position + Richtung::SUEDEN, true),
                    this->raumgroesse_erreichbar(position + Richtung::WESTEN, true));

  Raster raster(*this); // Raster, das gefüllt wird
  std::set<Position> positionen; // Noch zu betrachtende Positionen
  std::set<Position> sackgassen; // Sackgassen, diese werden separat berücksichtigt
  positionen.insert(position);
  int n = ((*this)(position) ? 0 : 1); // Anzahl der Felder
  raster.belege(position);
  while (!positionen.empty()) {
    auto p = begin(positionen);
    for (auto r : ::richtungen) {
      Position const p2 = *p + r;
      if (!raster(p2)) {
        // Auf einen Engpass prüfen, dieser kann eine Sackgasse sein
        if (raster.nachbarn_frei(p2) <= 1) {
          sackgassen.insert(p2);
          raster.belege(p2);
        } else {
          raster.belege(p2);
          n += 1;
          positionen.insert(p2);
        }
      }
    } // for (auto r : ::richtungen)
    positionen.erase(p);
  } // while (!positionen.empty())

  if (sackgassen.empty()) {
    return n;
  }

  if (sackgassen.size() == 1)
    return (n + raster.raumgroesse_erreichbar(*sackgassen.begin()) + 1);

#if 0
  // Schleifen erkennen
  for (auto const& p : sackgassen) {
    bool schleife = false;
    for (auto const& p2 : sackgassen) {
      if (p2 == p)
        continue;
      if (raster.kuerzeste_entfernung(p, p2) >= 0) {
        n += 1;
        schleife = true;
        sackgassen.erase(p2);
      }
    }
    if (schleife) {
      // Überschätzung, wenn es nur eine Schleife gibt, da statt dem Weg zurück auch eine größere Sackgasse gewählt werden kann.
      // Unterschätzung, wenn es mehr als eine Schleife gibt, da nur ein Hin- und Rückweg angesetzt wird.
      n += raster.raumgroesse_erreichbar(p) + 1;
      sackgassen.erase(p);
    }
  } // for (p : sackgassen)
#endif

  // Eine Sackgasse kann gefüllt werden
  int m = 0; // maximale Größe der Sackgassen
  for (auto const& p : sackgassen) {
    m = std::max(m, raster.raumgroesse_erreichbar(p) + 1);
  }

  return (n + m);
}  // int Raster::raumgroesse_erreichbar(Position const& position, bool const position_ueberpruelfen = false) const

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
        Position const p2 = p + r;
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
