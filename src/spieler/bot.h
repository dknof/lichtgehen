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

#ifndef BOT_H
#define BOT_H

#include "spieler.h"
#include "constants.h"

#include "../strategie/strategie.h"
#include "../spielraster/spielraster.h"

/** Ein Bot
 **/
class Bot : public Spieler {
  public:
    // Konstruktor
    explicit Bot(string const& name = "Bot");
    // Destruktor
    ~Bot();

    // setzt die Strategie
    void setze_strategie(unique_ptr<Strategie> strategie);

    // die Bewegung
    Bewegungsrichtung bewegung();

  private:
    // die Strategie
    unique_ptr<Strategie> strategie_ = nullptr;
}; // class Bot

#endif // #ifndef BOT_H
