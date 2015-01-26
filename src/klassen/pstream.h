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

#ifndef PSTREAM_H
#define PSTREAM_H

#include "constants.h"

/** Ein Ausgabestrom für eine pipe
 **/
class iopipestream : public std::iostream {
  public:
    // Startet ein Programm und gibt einen entsprechenden Stream zurück
    static unique_ptr<iopipestream> erzeuge_zum_programm(string const& pfad,
                                                         string const& name);

    /** Ein Buffer für eine pipe
     **/
    class buf : public std::streambuf {
      public:
        // Konstruktor
        explicit buf(int ipipefd, int opipefd);
        // Destruktor
        ~buf();

        // Daten dem Buffer übergeben
        int_type overflow( int_type c_ = traits_type::eof() );

        // Daten aus dem Puffer holen
        int_type underflow();
        // Daten aus dem Puffer holen
        int_type uflow();

      private:
        // pipe
        int const ipipefd;
        int const opipefd;
        // Dateideskriptor
        FILE* ifd;
        FILE* ofd;
    }; // class iopipestream::buf : public std:streambuf

  public:
    // Konstruktor
    explicit iopipestream(int ipipefd, int opipefd)
      : std::iostream(&iostrb), iostrb(ipipefd, opipefd)
    { }
  private:
    // Pipe
    buf iostrb;
}; // class iopipestream : public iostream

#ifdef VERALTET
/** Ein Ausgabestrom für eine pipe
 **/
class opipestream : public std::ostream {
  /** Ein Buffer für eine pipe
   **/
  class buf : public std::streambuf {
    public:
      // Konstruktor
      explicit buf(int pipefd);
      // Destruktor
      ~buf();

      // Daten dem Buffer übergeben
      int_type overflow( int_type c_ = traits_type::eof() );

    private:
      // pipe
      int const pipefd;
      // Dateideskriptor
      FILE* ofd;
  }; // class opipestream::buf : public std:streambuf

  public:
  // Konstruktor
  explicit opipestream(int pipefd) : std::ostream(&ostrb), ostrb(pipefd)
  { }
  private:
  // Pipe
  buf ostrb;
}; // class opipestream : public ostream

/** Ein Eingabestrom für eine pipe
 **/
class ipipestream : public std::istream {
  /** Ein Buffer für eine pipe
   **/
  class buf : public std::streambuf {
    public:
      // Konstruktor
      explicit buf(int pipefd);
      // Destruktor
      ~buf();

      // Daten aus dem Puffer holen
      int_type underflow();
      // Daten aus dem Puffer holen
      int_type uflow();

    private:
      // pipe
      int const pipefd;
      // Dateideskriptor
      FILE* ifd;
  }; // class ipipestream::buf : public std:streambuf

  public:
  // Konstruktor
  explicit ipipestream(int pipefd) : std::istream(&istrb), istrb(pipefd)
  { }
  private:
  // Pipe
  buf istrb;
}; // class ipipestream : public istream
#endif

#endif // #ifndef PSTREAM_H
