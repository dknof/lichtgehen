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

#ifndef STRATEGIE_H
#define STRATEGIE_H

#include "constants.h"
#include "../spielraster/spielraster.h"

/** Basisklasse für Strategieen
 ** Eine Strategie gibt eine Bewegung zurück.
 ** Im Vergleich zur Taktik kann sie Informationen über den Spielverlauf sammeln.
 **/
class Strategie {
  public:
    // erstellt eine Strategie entsprechend dem Namen
    static unique_ptr<Strategie> create(string const& name);
    static unique_ptr<Strategie> create(std::initializer_list<string> name);

  protected:
    // Konstruktor
    Strategie(string const& name, string const& beschreibung);

  public:
    // Destruktor
    virtual ~Strategie();

    // die Strategie ausgeben
    virtual ostream& ausgeben(ostream& ostr) const;

    // Die Bewegungsrichtung nach der Strategie ermitteln
    virtual Bewegungsrichtung bewegung(Spielraster const& spielraster,
                                       int bot_nummer) = 0;

  protected:
    // setzt Namen und Beschreibung
    void setze_name(string const& name, string const& beschreibung);

  public:
    // Der Name der Strategie
    string name;
    // Der Beschreibung der Strategie
    string beschreibung;
}; // class Strategie

// die Strategie ausgeben
ostream& operator<<(ostream& ostr, Strategie const& strategie);

#endif // #ifndef STRATEGIE_H
