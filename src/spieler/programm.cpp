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
 ** @param     spielraster   Spielraster
 ** @param     pfad          Pfad zum Programm
 **
 ** @return    -
 **
 ** @version   2015-01-24
 **/
Programm::Programm(Spielraster const& spielraster,
               string const& pfad) :
  Spieler(spielraster, pfad),
  istr(),
  ostr()
{
  this->starte_programm(pfad);
} // Programm::Programm(Spielraster const& spielraster, string const& pfad)

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
    // fdout -> stdout
    close(fdin[1]); //close write side from parents
    close(0); //close stdin
    dup(fdin[0]); //connect pipe from execl cat to stdin

    close(fdout[0]); //close read side from parents
    close(1); //close stdout
    dup(fdout[1]); //connect pipe from execl cat to stdout

    CLOG << endl;
    execl(pfad.c_str(), this->name().c_str(), (char *) 0);
    perror("exec failed!");
    exit(20);
  } // if (pid == 0)

  // Standardprozess
  close(fdin[0]);
  close(fdout[1]);

  this->ostr = std::make_unique<opipestream>(fdin[1]);
  this->istr = fdopen(fdout[0], "r");

  if (this->ostr == nullptr) {
    perror("write to pipe failed");
    exit(EXIT_FAILURE);
  }
  if (this->istr == nullptr) {
    perror("read from pipe failed uiae");
    exit(EXIT_FAILURE);
  }

  //this->istr = std::make_unique<std::ifstream>(read_from_child);
  //this->ostr = std::make_unique<std::ofstream>(write_to_child);

  return ;
} // void Programm::starte_programm(string pfad)

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
  // schreibe den letzten Zug

  // lese die Ausgabe vom Programm
  string zeile;
  char zeile_c[256];
  zeile_c[255] = 0;
  while (!ferror(this->istr)) {
    fgets(zeile_c, 255, this->istr);
    zeile = string(zeile_c);
    //std::getline(cin, zeile);
    if (zeile == "AHEAD")
      return Bewegungsrichtung::VORWAERTS;
    else if (zeile == "LEFT")
      return Bewegungsrichtung::LINKS;
    else if (zeile == "RIGHT")
      return Bewegungsrichtung::RECHTS;
    else
      cerr << "Bewegung '" << zeile << "' unbekannt.\n";
  }
  CLOG << endl;
  return Bewegungsrichtung::VORWAERTS;;
} // Bewegungsrichtung Programm::bewegung()
