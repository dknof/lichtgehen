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

#ifndef UI_GTKMM_H
#define UI_GTKMM_H

#include "constants.h"
#include "../ui.h"
#include "../../spielraster/richtung.h"
class Spielraster;

namespace Gtk {
  class Main;
}
namespace UI_Gtkmm {
  class Hauptfenster;
  /** Die UI
   **/
  class UI_Gtkmm : public UI {
    public:
      // Konstruktor
      UI_Gtkmm(int& argc, char* argv[]);
      // Destruktor
      ~UI_Gtkmm();

      // das Spiel startet
      void spiel_startet();
      // neue Runde
      void runde(int n);
      // das Spiel ist zuende
      void spiel_endet();

      // gibt die nächste Richtung (Benutzereingabe) zurück
      Richtung hole_richtung();

    private:
      // initializiere die UI
      void init();

    public:
      unique_ptr<Gtk::Main> main; // Hauptroutine
    private: // Die Elemente der UI
      unique_ptr<Hauptfenster> hauptfenster; // Hauptfenster
  }; // class UI_Gtkmm : public UI
} // namespace UI_Gktmm

#endif // #ifndef UI_GTKMM_H
