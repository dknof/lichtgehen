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

#include "gtkmm.h"

#include "hauptfenster.h"
#include <gtkmm/main.h>

namespace UI_Gtkmm {
  struct UI_Gtkmm::Pimpl {
    unique_ptr<Gtk::Main> main; // Hauptroutine
    unique_ptr<Hauptfenster> hauptfenster; // Hauptfenster
  }; // struct UI_Gtkmm::Pimpl

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
  UI_Gtkmm::UI_Gtkmm(int& argc, char* argv[]) :
    pimpl{make_unique<UI_Gtkmm::Pimpl>()}
  {
    this->pimpl->main = make_unique<Gtk::Main>(argc, argv);
    this->pimpl->hauptfenster = make_unique<Hauptfenster>(*this);

    // Einen zweiten Thread eröffnen, um die GUI darzustellen.
    // Der Thread wird benötigt, da ein Thread auf cin wartet.
    while (true) {
      this->pimpl->main->iteration();
    }

  } // UI_Gtkmm::UI_Gtkmm(int argc, char* argv[])

  /**
   ** Destruktor
   ** 
   ** @param     -
   **
   ** @return    -
   **
   ** @version   2014-11-20
   **/
  UI_Gtkmm::~UI_Gtkmm()
  { }

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
    UI_Gtkmm::spiel_startet()
    {
      this->pimpl->hauptfenster->aktualisiere_spielraster();
      return;
    } // void UI_Gtkmm::spiel_startet()

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
    UI_Gtkmm::runde(int const n)
    {
      this->pimpl->hauptfenster->aktualisiere_spielraster();
      return;
    } // void UI_Gtkmm::runde(int n)

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
    UI_Gtkmm::spiel_endet()
    {
      this->pimpl->hauptfenster->aktualisiere_spielraster();
      return;
    } // void UI_Gtkmm::spiel_endet()
} // namespace UI_Gtkmm
