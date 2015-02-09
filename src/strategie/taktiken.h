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

#ifndef TAKTIKEN_H
#define TAKTIKEN_H

#include "constants.h"
#include "strategie.h"
#include "../taktik/taktik.h"

namespace StrategieNS {
/** Sammlung von Taktiken
 ** Die erste Taktik, die passt, liefert die Richtung.
 **/
class Taktiken : public Strategie {
  public:
    // Konstruktor
    Taktiken();
    // Konstruktor mit einer Taktik
    Taktiken(string name);
    // Konstruktor mit mehreren Taktiken
    Taktiken(std::initializer_list<string> name);

    // Destruktor
    virtual ~Taktiken() override;

    // die Strategie ausgeben
    virtual ostream& ausgeben(ostream& ostr) const override;

    // Taktik hinzufügen
    void hinzufuegen(unique_ptr<Taktik> taktik);

    // Die Bewegungsrichtung nach der Strategie ermitteln
    virtual Bewegungsrichtung bewegung(Spielraster const& spielraster,
                                       int spieler_nummer) override;

  protected:
    // Die Taktiken
    std::vector<unique_ptr<Taktik>> taktiken;
}; // class Taktiken : public Strategie
} // namespace StrategieNS

#endif // #ifndef TAKTIKEN_H
