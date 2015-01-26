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

#ifndef SPIELER_H
#define SPIELER_H

#include "constants.h"

#include "../spielraster/spielraster.h"

/** Ein Spieler (abstrakte Basisklasse)
 **/
class Spieler {
  public:
    // Erzeuge einen Spieler
    static std::unique_ptr<Spieler> create(string const& typ,
                                           string const& name);

  public:
    // Konstruktor
    explicit Spieler(string const& name);
    // Destruktor
    virtual ~Spieler();

    // die Nummer
    string const& name() const;

    // die Nummer
    int nummer() const;
    // setzt die Nummer
    virtual void setze_nummer(int const nummer);

    // das Spielraster
    Spielraster const& spielraster() const;


    // Spielaktionen

    // das Spiel startet
    virtual void spiel_startet(Spielraster const& spielraster);
    // die Runde startet
    virtual void runde(int runde);
    // das Spiel startet
    virtual void spiel_endet();

    // die Bewegung
    virtual Bewegungsrichtung bewegung() = 0;

  protected:
    // Spielraster
    Spielraster const* spielraster_ = nullptr;
  private:
    // Nummer
    int nummer_ = -1;
    // Name
    string name_ = "";
}; // class Spieler

#endif // #ifndef SPIELER_H
