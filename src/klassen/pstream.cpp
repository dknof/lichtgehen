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

#include "pstream.h"

#include <cstdio>
#include <unistd.h>

/**
 ** Standardkonstruktor
 ** 
 ** @param     pipefd    Dateidescriptor zum Ausgabestrom
 **
 ** @return    -
 **
 ** @version   2015-01-25
 **/
opipestream::buf::buf(int const pipefd) :
  pipefd(pipefd),
  fd(nullptr)
{
  this->fd = fdopen(this->pipefd, "w");
  if (this->fd == nullptr) {
    perror("read from pipe failed");
    exit(EXIT_FAILURE);
  }
} // opipestream::buf::buf(int const pipefd)

/**
 ** Destruktor
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2015-01-25
 **/
opipestream::buf::~buf()
{
  std::fclose(this->fd);
  close(this->pipefd);
} // opipestream::buf::~buf()

/**
 ** Daten in die Pipe schreiben
 ** 
 ** @param     c_   zu schreibendes Zeichen
 **
 ** @return    geschriebenes Zeichen
 **
 ** @version   2015-01-25
 **/
opipestream::buf::int_type
opipestream::buf::overflow(int_type const c_)
{
  if( traits_type::eq_int_type( c_, traits_type::eof() ) )
    return traits_type::not_eof( c_ );
  char const c = traits_type::to_char_type( c_ );
  // zeichenweise wegschreiben
  if (std::fputc(c, this->fd) == EOF)
    return traits_type::eof();

  std::fflush(this->fd);
  return c_;
} // int_type opipestream::buf::overflow(int_type c_ = traits_type::eof())

/**
 ** Standardkonstruktor
 ** 
 ** @param     pipefd    Dateidescriptor zum Eingabestrom
 **
 ** @return    -
 **
 ** @version   2015-01-25
 **/
ipipestream::buf::buf(int const pipefd) :
  pipefd(pipefd),
  fd(nullptr)
{
  this->fd = fdopen(this->pipefd, "r");
  if (this->fd == nullptr) {
    perror("read from pipe failed");
    exit(EXIT_FAILURE);
  }
} // ipipestream::buf::buf(int const pipefd)

/**
 ** Destruktor
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2015-01-25
 **/
ipipestream::buf::~buf()
{
  std::fclose(this->fd);
  close(this->pipefd);
} // ipipestream::buf::~buf()

/**
 ** Daten aus der Pipe lesen
 ** 
 ** @param     -
 **
 ** @return    gelesenes Zeichen
 **
 ** @version   2015-01-25
 **/
ipipestream::buf::int_type
ipipestream::buf::underflow()
{
  int const c = fgetc(this->fd);
  if (c == EOF)
    return traits_type::eof();
  ungetc(c, this->fd);
  return c;
} // int_type ipipestream::buf::underflow()

/**
 ** Daten aus der Pipe lesen
 ** 
 ** @param     -
 **
 ** @return    gelesenes Zeichen
 **
 ** @version   2015-01-25
 **/
ipipestream::buf::int_type
ipipestream::buf::uflow()
{
  int const c = fgetc(this->fd);
  if (c == EOF)
    return traits_type::eof();
  return c;
} // int_type ipipestream::buf::uflow()
