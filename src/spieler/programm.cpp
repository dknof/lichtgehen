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
  Spieler(name == "" ? pfad : name)
{
  this->starte_programm(pfad);
} // Programm::Programm(string const& pfad)

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
{
#if 0

  fclose(write_to_child);
  fclose(read_from_child);
  close(fdin[1]);
  close(fdout[0]);
#endif

  // noch auf den Kindprozess (das gestartete Programm) warten
  int status;
  wait(&status);
} // Programm::~Programm()

/**
 ** startet das Programm und verbindet istr und ostr mit den entsprechenden Strömen
 ** 
 ** @param     pfad   Pfad zum Programm
 **
 ** @return    -
 **
 ** @version   2015-01-24
 **/
void
Programm::starte_programm(string const& pfad)
{
  int fdin[2];  // stdin für das Programm
  int fdout[2]; // stdout für das Programm

  // Erstelle erste Pipe
  if (pipe(fdin) == -1) {
    perror("creating pipe: failed");
    exit(EXIT_FAILURE);
  }

  // Erstelle zweite Pipe
  if (pipe(fdout) == -1) {
    perror("creating pipe: failed");
    exit(EXIT_FAILURE);
  }

  // Eigener Thread für das Programm

  int pid = fork();

  if (pid == -1) {
    // Fehler
    perror("fork failed: cat process");
    exit(EXIT_FAILURE);
  } // if (pid == -1)

  if (pid == 0) {
    // Thread
    // -> starte Programm

    // Streams verbinden:
    // fdin -> stdin
    close(fdin[1]);
    close(0);
    dup(fdin[0]);

    // fdout -> stdout
    close(fdout[0]);
    close(1);
    dup(fdout[1]);

    execl(pfad.c_str(), this->name().c_str(), (char *) 0);
    perror("exec failed!");
    exit(20);
  } // if (pid == 0)

  // Standardprozess
  close(fdin[0]);
  close(fdout[1]);

  this->ostr = std::make_unique<opipestream>(fdin[1]);
  this->istr = std::make_unique<ipipestream>(fdout[0]);

  if (this->ostr == nullptr) {
    perror("write to pipe failed");
    exit(EXIT_FAILURE);
  }
  if (this->istr == nullptr) {
    perror("read from pipe failed");
    exit(EXIT_FAILURE);
  }


  return ;
} // void Programm::starte_programm(string pfad)

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

  spielraster.ausgeben(*this->ostr);
  *this->ostr << "SET " << this->nummer() + 1 << '\n';
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
    cout << "POS " << s + 1 << this->spielraster().position(s) << '\n';
    *this->ostr << "POS " << s + 1 << ' '
      << this->spielraster().position(s).x() + 1 << ','
      << this->spielraster().position(s).y() + 1 << ' '
      << this->spielraster().position(s).richtung() << '\n';
  }
  *this->ostr << "ROUND " << runde + 1 << '\n';
  *this->ostr << std::flush;

  return ;
} // void Programm::runde(int const runde)

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
    std::getline(*this->istr, zeile);
    CLOG << zeile << endl;
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
