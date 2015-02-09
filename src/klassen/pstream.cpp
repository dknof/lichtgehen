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

#include "pstream.h"

#include <cstdio>
#include <unistd.h>

/**
 ** Startet ein Programm und gibt einen entsprechenden Stream zurück
 ** 
 ** @param     pfad   Pfad zum Programm
 **
 ** @return    Strom zur Kommunikation
 **
 ** @version   2015-01-25
 **/
unique_ptr<iopipestream>
iopipestream::erzeuge_zum_programm(string const& pfad, string const& name)
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

    execl(pfad.c_str(), name.c_str(), nullptr);
    perror("exec failed!");
    exit(20);
  } // if (pid == 0)

  // Standardprozess
  close(fdin[0]);
  close(fdout[1]);

  return std::make_unique<iopipestream>(fdout[0], fdin[1]);
} // unique_ptr<iopipestream> iopipestream::erzeuge_zum_programm(string pfad, string name)

/**
 ** Standardkonstruktor
 ** 
 ** @param     pipefd    Dateidescriptor zum Ausgabestrom
 **
 ** @return    -
 **
 ** @version   2015-01-25
 **/
iopipestream::buf::buf(int const ipipefd, int const opipefd) :
  ipipefd(ipipefd),
  opipefd(opipefd),
  ifd(nullptr),
  ofd(nullptr)
{
  this->ifd = fdopen(this->ipipefd, "r");
  this->ofd = fdopen(this->opipefd, "w");
  if (this->ifd == nullptr) {
    perror("Eingabepipe konnte nicht geöffnet werden");
    exit(EXIT_FAILURE);
  }
  if (this->ofd == nullptr) {
    perror("Ausgabepipe konnte nicht geöffnet werden");
    exit(EXIT_FAILURE);
  }
} // iopipestream::buf::buf(int ipipefd, int opipefd)

/**
 ** Destruktor
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2015-01-25
 **/
iopipestream::buf::~buf()
{
  std::fclose(this->ofd);
  std::fclose(this->ifd);
  close(this->opipefd);
  close(this->ipipefd);
} // iopipestream::buf::~buf()

/**
 ** Daten in die Pipe schreiben
 ** 
 ** @param     c_   zu schreibendes Zeichen
 **
 ** @return    geschriebenes Zeichen
 **
 ** @version   2015-01-25
 **/
iopipestream::buf::int_type
iopipestream::buf::overflow(int_type const c_)
{
  if( traits_type::eq_int_type( c_, traits_type::eof() ) )
    return traits_type::not_eof( c_ );
  char const c = traits_type::to_char_type( c_ );
  // zeichenweise wegschreiben
  if (std::fputc(c, this->ofd) == EOF)
    return traits_type::eof();

  if (c == '\n')
    std::fflush(this->ofd);

  return c_;
} // int_type iopipestream::buf::overflow(int_type c_ = traits_type::eof())

/**
 ** Daten aus der Pipe lesen
 ** 
 ** @param     -
 **
 ** @return    gelesenes Zeichen
 **
 ** @version   2015-01-25
 **/
iopipestream::buf::int_type
iopipestream::buf::underflow()
{
  int const c = fgetc(this->ifd);
  if (c == EOF)
    return traits_type::eof();
  ungetc(c, this->ifd);
  return c;
} // int_type iopipestream::buf::underflow()

/**
 ** Daten aus der Pipe lesen
 ** 
 ** @param     -
 **
 ** @return    gelesenes Zeichen
 **
 ** @version   2015-01-25
 **/
iopipestream::buf::int_type
iopipestream::buf::uflow()
{
  int const c = fgetc(this->ifd);
  if (c == EOF)
    return traits_type::eof();
  return c;
} // int_type iopipestream::buf::uflow()
