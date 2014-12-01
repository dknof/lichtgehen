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

#ifndef SPIELRASTER_H
#define SPIELRASTER_H

#include "constants.h"

#include "raster.h"
#include "botposition.h"

/** Ein Spielraster mit Bots
 **/
class Spielraster : public Raster {
  public:
  struct RaumInfo {
    int groesse = 0; // die Größe des Raums
    int bot_anz = 0; // die Anzahl der (gegnerischen) Bots
    int entfernung_naechster_bot = -1; // die Entfernung zum nächsten Bot
  }; // struct RaumInfo

  public:
  // Konstruktor
  Spielraster();

  // Spielraster auf ostr ausgeben
  ostream& ausgeben_schoen(ostream& ostr) const;
  // Spielraster auf ostr ausgeben
  ostream& ausgeben(ostream& ostr) const override;


  // Bots

  // Die Anzahl der Bots
  int bot_anz() const;
  // Die Anzahl der Bots noch im Spiel
  int bots_im_spiel() const;
  // Ob der Bot mit der Nummer noch im Spiel ist
  bool bot_im_spiel(int bot) const;

  // setzt einen Bot auf das Raster
  void setze_bot(istream& istr);
  // setzt einen Bot auf das Raster
  void setze_bot(int n, BotPosition const& bot_position);
  // entfernt einen Bot vom Spielfed
  void entferne_bot(int n);

  // die aktuelle Position des Bots
  BotPosition const& position(int n) const;
  // der Weg des Bots
  BotWeg const& weg(int n) const;

  // bewegt den Bot um ein Feld
  void bewege_bot(int n, Bewegungsrichtung br);

  //
  // Informationen über das Spielraster
  //

  // die Runde
  int runde() const;
  // das Spielraster in der angegeben Runde
  Spielraster historie(int runde) const;

  // Informationen über den Raum des Bots
  Spielraster::RaumInfo rauminfo(int bot) const;
  Spielraster::RaumInfo rauminfo(int bot,
                                 Bewegungsrichtung bewegungsrichtung) const;

  // Einflussbereich

  // Größe des Einflussbereichs ermitteln
  int einflussbereich_groesse(int bot) const;
  // Größe des Einflussbereichs ermitteln
  int einflussbereich_groesse(int bot, Bewegungsrichtung br) const;
  // Einflussbereich ermitteln
  Raster einflussbereich(int bot) const;
  // Einflussbereich ermitteln
  Raster einflussbereich(int bot, Bewegungsrichtung br) const;
  // Einflussbereich ermitteln
  Raster einflussbereich(int bot, Position p) const;
  // Die Größe des erreichbaren Einflussbereich
  int einflussbereich_groesse_erreichbar(int bot) const;

  private:
  // Anzahl der Bots
  int bot_anz_ = 0;
  // Der Wegverlauf der Bots
  vector<BotWeg> bot_weg_;
}; // class Spielraster : public Raster

// Spielraster ausgeben
ostream& operator<<(ostream& ostr, Spielraster const& spielraster);

#endif // #ifndef SPIELRASTER_H
