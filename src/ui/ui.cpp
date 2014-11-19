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

#include "hauptfenster.h"
#include <gtkmm/main.h>

struct UI::Pimpl {
  unique_ptr<Gtk::Main> main; // Hauptroutine
  unique_ptr<UI_Gtkmm::Hauptfenster> hauptfenster; // Hauptfenster
}; // struct UI::Pimpl

/**
 ** Konstruktor
 ** 
 ** @param     argc   Anzahl Kommandozeilenargumente
 ** @param     argv   Kommandozeilenargumente
 **
 ** @return    -
 **
 ** @version   2014-11-18
 **/
UI::UI(int& argc, char* argv[]) :
  pimpl{make_unique<UI::Pimpl>()}
{
  this->pimpl->main = make_unique<Gtk::Main>(argc, argv);
  this->pimpl->hauptfenster = make_unique<UI_Gtkmm::Hauptfenster>(*this);

  // Einen zweiten Thread eröffnen, um die GUI darzustellen.
  // Der Thread wird benötigt, da ein Thread auf cin wartet.
  while (true) {
    this->pimpl->main->iteration();
  }

} // UI::UI(int argc, char* argv[])

/**
 ** Destruktor
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2014-11-18
 **/
UI::~UI()
{
} // UI::~UI()

/**
 ** Ein Spiel startet
 ** 
 ** @param     spielraster    das Spielraster
 **
 ** @return    -
 **
 ** @version   2014-11-19
 **/
void
UI::spiel_startet(Spielraster const& spielraster)
{
  this->spielraster = &spielraster;
  this->pimpl->hauptfenster->aktualisiere_spielraster();
  return;
} // void UI::spiel_startet(Spielraster const& spielraster)

/**
 ** Eine neue Runde startet
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2014-11-19
 **/
void
UI::neue_runde()
{
  this->pimpl->hauptfenster->aktualisiere_spielraster();
  return;
} // void UI::neue_runde()

/**
 ** Das Spiel ist zuende
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2014-11-19
 **/
void
UI::spiel_endet()
{
  this->pimpl->hauptfenster->aktualisiere_spielraster();
  return;
} // void UI::nspiel_endeteue_runde()
