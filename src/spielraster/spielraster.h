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

#ifndef SPIELRASTER_H
#define SPIELRASTER_H

#include "constants.h"

#include "raster.h"
#include "spielerposition.h"

/** Ein Spielraster mit Spieler
 **/
class Spielraster : public Raster {
  public:
  struct RaumInfo {
    int groesse = 0; // die Größe des Raums
    int spieler_anz = 0; // die Anzahl der (gegnerischen) Spieler
    int entfernung_naechster_spieler = -1; // die Entfernung zum nächsten Spieler
  }; // struct RaumInfo

  public:
  // Konstruktor
  Spielraster();

  // Spielraster auf ostr ausgeben
  ostream& ausgeben_schoen(ostream& ostr) const;
  // Spielraster auf ostr ausgeben
  ostream& ausgeben(ostream& ostr) const override;


  // Spieler

  // Die Anzahl der Spieler
  int spieler_anz() const;
  // Die Anzahl der Spieler noch im Spiel
  int spieler_im_spiel() const;
  // Ob der Spielermit der Nummer noch im Spiel ist
  bool spieler_im_spiel(int spieler) const;

  // setzt einen Spieler auf das Raster
  void setze_spieler(istream& istr);
  // setzt einen Spieler auf das Raster
  void setze_spieler(int n, SpielerPosition const& spieler_position);
  // entfernt einen Spieler vom Spielfed
  void entferne_spieler(int n);

  // die aktuelle Position des Spieler
  SpielerPosition const& position(int n) const;
  // der Weg des Spieler
  SpielerWeg const& weg(int n) const;

  // bewegt den Spieler um ein Feld
  void bewege_spieler(int n, Bewegungsrichtung br);

  //
  // Informationen über das Spielraster
  //

  // die Runde
  int runde() const;
  // das Spielraster in der angegeben Runde
  Spielraster historie(int runde) const;

  // Informationen über den Raum des Spieler
  Spielraster::RaumInfo rauminfo(int spieler) const;
  Spielraster::RaumInfo rauminfo(int spieler,
                                 Bewegungsrichtung bewegungsrichtung) const;

  // Einflussbereich

  // Größe des Einflussbereichs ermitteln
  int einflussbereich_groesse(int spieler) const;
  // Größe des Einflussbereichs ermitteln
  int einflussbereich_groesse(int spieler, Bewegungsrichtung br) const;
  // Einflussbereich ermitteln
  Raster einflussbereich(int spieler) const;
  // Einflussbereich ermitteln
  Raster einflussbereich(int spieler, Bewegungsrichtung br) const;
  // Einflussbereich ermitteln
  Raster einflussbereich(int spieler, Position p) const;
  // Die Größe des erreichbaren Einflussbereich
  int einflussbereich_groesse_erreichbar(int spieler) const;

  private:
  // Anzahl der Spieler
  int spieler_anz_ = 0;
  // Der Wegverlauf der Spieler
  vector<SpielerWeg> spieler_weg_;
}; // class Spielraster : public Raster

// Spielraster ausgeben
ostream& operator<<(ostream& ostr, Spielraster const& spielraster);

#endif // #ifndef SPIELRASTER_H
