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

#include "gtkmm.h"

#include "hauptfenster.h"
#include "hilfe.h"
#include "ueber.h"
#include <gtkmm/main.h>

namespace UI_Gtkmm {
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
    main{make_unique<Gtk::Main>(argc, argv)},
    hauptfenster{make_unique<Hauptfenster>(*this)},
    hilfe{make_unique<Hilfe>()},
    ueber{make_unique<Ueber>()}
  {
    this->hilfe->set_transient_for(*this->hauptfenster);
    this->ueber->set_transient_for(*this->hauptfenster);

    // @todo: Einen zweiten Thread eröffnen, um die GUI darzustellen.
    // Der Thread wird benötigt, da ein Thread auf cin wartet.
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
      this->hauptfenster->present();
      this->hauptfenster->aktualisiere();
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
      this->hauptfenster->runde(n);
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
      this->hauptfenster->spiel_endet();
      if (this->hauptfenster->is_visible())
        this->main->run(*this->hauptfenster);
      return;
    } // void UI_Gtkmm::spiel_endet()

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
    UI_Gtkmm::naechste_richtung()
    { 
      while (   this->schwebende_richtungen.empty()
             && this->hauptfenster->is_visible()) {
        this->main->iteration();
        usleep(10000);
      };
      if (!this->hauptfenster->is_visible())
        return Richtung::NORDEN;
      auto const r = this->schwebende_richtungen.front();
      this->schwebende_richtungen.pop_front();
      return r;
    } // Richtung UI_Gtkmm::naechste_richtung()

} // namespace UI_Gtkmm
