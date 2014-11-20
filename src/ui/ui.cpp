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

#include "ui.h"

#include "text.h"
#include "gtkmm/gtkmm.h"

/**
 ** Konstruktor
 ** 
 ** @param     argc   Anzahl Kommandozeilenargumente
 ** @param     argv   Kommandozeilenargumente
 **
 ** @return    -
 **
 ** @version   2014-11-20
 **/
unique_ptr<UI>
UI::create(string const name, int& argc, char* argv[])
{
  if (name == "none")
    return make_unique<UI>();
  else if (name == "text")
    return make_unique<UI_Text>(cout);
  else if (name == "cout")
    return make_unique<UI_Text>(cout);
  else if (name == "cerr")
    return make_unique<UI_Text>(cerr);
  else if (name == "gtkmm")
    return make_unique<UI_Gtkmm::UI_Gtkmm>(argc, argv);
  else
    return nullptr;
} // static unique_ptr<UI> UI::create(string const name, int& argc, char* argv[])

/**
 ** Konstruktor
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2014-11-20
 **/
UI::UI()
{ }

/**
 ** Destruktor
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2014-11-20
 **/
UI::~UI()
{ }

/**
 ** -> Rückgabe
 ** 
 ** @param     -
 **
 ** @return    das Spielraster
 **
 ** @version   2014-11-20
 **/
Spielraster const& 
UI::spielraster() const
{ return *this->spielraster_; }

/**
 ** Ein Spiel startet
 ** 
 ** @param     spielraster    das Spielraster
 **
 ** @return    -
 **
 ** @version   2014-11-20
 **/
void
UI::spiel_startet(Spielraster const& spielraster)
{
  this->spielraster_ = &spielraster;
  this->spiel_startet();
  return;
} // void UI::spiel_startet(Spielraster const& spielraster)

/**
 ** Ein Spiel startet
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2014-11-20
 **/
void
UI::spiel_startet()
{ }

/**
 ** Eine neue Runde startet
 ** 
 ** @param     n   Nummer der Runde
 **
 ** @return    -
 **
 ** @version   2014-11-20
 **/
void
UI::runde(int const n)
{ }

/**
 ** Das Spiel ist zuende
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2014-11-20
 **/
void
UI::spiel_endet()
{ }

/**
 ** gibt die nächste Richtung für die Bewegung zurück
 ** 
 ** @param     -
 **
 ** @return    Norden
 **
 ** @version   2014-11-20
 **/
Richtung
UI::hole_richtung()
{ return Richtung::NORDEN; }
