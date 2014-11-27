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

#ifndef RASTER_H
#define RASTER_H

#include "constants.h"

#include "position.h"
#include <bitset>
// Maximale Größe des Rasters
#define RASTER_MAX_GROESSE (128*128)
#define USE_BITSET

/** Ein Raster
 ** Die einzelnen Punkte sind besetzt oder nicht besetzt
 **/
class Raster {
friend bool operator<(Raster const& raster1, Raster const& raster2);
#ifdef USE_BITSET
typedef std::bitset<RASTER_MAX_GROESSE> Felder;
#else
typedef vector<bool> Felder;
#endif
  public:
  // Konstruktor
  Raster();
  // Konstruktor
  Raster(int breite, int laenge);
  // Destruktor
  virtual ~Raster();

  // Raster aus istr einlesen
  virtual istream& einlesen(istream& istr);
  // Raster auf ostr ausgeben
  virtual ostream& ausgeben(ostream& ostr) const;

  // Breite des Rasteres
  int breite() const;
  // Länge des Rasteres
  int laenge() const;

  // Das Raster
  bool operator()(int x, int y) const;
  // Das Raster
  bool operator()(Position const& position) const;

  // belegt das Raster
  void belege(int x, int y, bool wert = true);
  // belegt das Raster
  void belege(Position const& position, bool wert = true);
  // belegt das Raster
  void belege(Raster const& raster);

  // invertiere das Raster
  void invertiere();
  // invertiertes Raster
  Raster invertiert() const;

  //
  // Informationen über das Raster
  //

  // Anzahl der belegten Felder
  int felder_belegt() const;
  // Anzahl der freien Felder
  int felder_frei() const;

  // Die Anzahl der freien Nachbarraster
  int nachbarn_frei(Position const& position) const;
  // Die Größe des Raumes unter position
  int raumgroesse(Position const& position,
                  bool position_uberpruefen = true) const;
  // Die Größe von position erreichbaren Raumes
  int raumgroesse_erreichbar(Position const& position,
                             bool position_uberpruefen = true) const;
  // Kürzeste Entfernun zwischen beiden Positionen
  int kuerzeste_entfernung(Position const& pa, Position const& pb) const;

  private:
  // Breite der Rasters
  int breite_;
  // Länge des Rasters
  int laenge_;
  // Die einzelnen Felder
  Felder felder_;
}; // class Raster

// Raster ausgeben
ostream& operator<<(ostream& ostr, Raster const& raster);
// Raster vergleichen
bool operator<(Raster const& raster1, Raster const& raster2);

#endif // #ifndef RASTER_H
