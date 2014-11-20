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

#ifndef UI_GTKMM_HAUPTFENSTER_H
#define UI_GTKMM_HAUPTFENSTER_H

#include "gtkmm.h"
#include <gtkmm/window.h>
namespace Gtk {
  class DrawingArea;
  class Label;
}

namespace UI_Gtkmm {
  /** das Hauptfenster
   ** @todo  Spielraster
   ** @todo  Hilfe
   ** @todo  Beenden
   ** @todo  Bedienung
   ** @todo  Informationen
   **/
  class Hauptfenster : public Gtk::Window {
    public:
      // Konstruktor
      Hauptfenster(UI_Gtkmm& ui);

      // aktualisiere das Spielraster
      void aktualisiere_spielraster();
    private:
      // initializiere das Hauptfenster
      void init();

    private:
      // Verweis auf die UI
      UI_Gtkmm* const ui;
      // Spielraster
      Gtk::DrawingArea* spielraster = nullptr;
      // Rundenanzeige
      Gtk::Label* runde = nullptr;
      // Botinfo
      Gtk::Label* bot1 = nullptr;
      Gtk::Label* bot2 = nullptr;
  }; // class Hauptfenster : public Gtk::Window
} // namespace UI_Gtkmm

#endif // #ifndef UI_GTKMM_HAUPTFENSTER_H
