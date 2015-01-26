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

#include "programm.h"

#include "../ui/ui.h"

#include "../klassen/pstream.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include <fstream>

/**
 ** Standardkonstruktor
 ** 
 ** @param     pfad    Pfad zum Programm
 ** @param     name    Name des Programms
 **
 ** @return    -
 **
 ** @version   2015-01-24
 **/
Programm::Programm(string const& pfad,
                   string const& name) :
  Spieler(name == "" ? pfad : name),
  iostr(iopipestream::erzeuge_zum_programm(pfad, name))
{ }

/**
 ** Destruktor
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2015-01-24
 **/
Programm::~Programm()
{ }

/**
 ** das Spiel startet
 ** 
 ** @param     spielraster   das Spielraster
 **
 ** @return    -
 **
 ** @version   2014-10-25
 **/
void
Programm::spiel_startet(Spielraster const& spielraster)
{
  this->Spieler::spiel_startet(spielraster);

  spielraster.ausgeben(*this->iostr);
  *this->iostr << "SET " << this->nummer() + 1 << '\n';
  return ;
} // void Spieler::spiel_startet(Spielraster const& spielraster)

/**
 ** setzt die Nummer
 ** 
 ** @param     nummer    die Nummer
 **
 ** @return    -
 **
 ** @version   2014-10-25
 **/
void
Programm::setze_nummer(int const nummer)
{
  this->Spieler::setze_nummer(nummer);
  return ;
} // void Programm::setze_nummer(int const nummer)

/**
 ** die Runde startet
 ** 
 ** @param     runde   Nummer der Runde
 **
 ** @return    -
 **
 ** @version   2014-10-25
 **/
void
Programm::runde(int const runde)
{
  this->Spieler::runde(runde);

  for (int s = 0; s < this->spielraster().spieler_anz(); ++s) {
    SpielerPosition p = this->spielraster().position(s);
    cout << "POS " << s + 1 << p << '\n';
    if (!p) {
      p = this->spielraster().weg(s)[this->spielraster().weg(s).size() - 2];
    }
    *this->iostr << "POS " << s + 1 << ' '
      << p.x() + 1 << ','
      << p.y() + 1 << ' '
      << p.richtung() << '\n';
  }
  *this->iostr << "ROUND " << runde + 1 << '\n';

  return ;
} // void Programm::runde(int const runde)

/**
 ** das Spiel endet
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2014-10-25
 **/
void
Programm::spiel_endet()
{
  *this->iostr << "END" << '\n';

  this->Spieler::spiel_endet();
  return ;
} // void Spieler::spiel_endet()


/**
 ** setzt die Nummer
 ** 
 ** @param     nummer    die Nummer
 **
 ** @return    -
 **
 ** @version   2014-11-21
 **/
Bewegungsrichtung
Programm::bewegung()
{
  // lese die Ausgabe vom Programm
  string zeile = "";
  while (true) {
    std::getline(*this->iostr, zeile);
    if (zeile == "AHEAD")
      return Bewegungsrichtung::VORWAERTS;
    else if (zeile == "LEFT")
      return Bewegungsrichtung::LINKS;
    else if (zeile == "RIGHT")
      return Bewegungsrichtung::RECHTS;
    else {
      cerr << "Bewegung '" << zeile << "' unbekannt.\n";
      exit(0);
    }
  }
  return Bewegungsrichtung::VORWAERTS;
} // Bewegungsrichtung Programm::bewegung()
