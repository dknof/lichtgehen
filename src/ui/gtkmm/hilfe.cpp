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

#include "constants.h"
#include "hilfe.h"

namespace UI_Gtkmm {
  /**
   ** Konstruktor
   ** 
   ** @param     -
   **
   ** @return    -
   **
   ** @version   2014-11-22
   **/
  Hilfe::Hilfe() :
    Gtk::MessageDialog("Hilfe")
  {
    this->signal_realize().connect(sigc::mem_fun(*this, &Hilfe::init));
    this->signal_response().connect(sigc::hide(sigc::mem_fun(*this, &Hilfe::hide)));
  } // Hilfe::Hilfe()

  /**
   ** initializiere das UI
   ** 
   ** @param     -
   **
   ** @return    -
   **
   ** @version   2014-11-21
   **/
  void
    Hilfe::init()
    {
      this->set_title("Tronbot – Hilfe");
      this->set_message("Hilfe");
      this->set_secondary_text("<b>Darstellung</b>\n"
                               "Spielraster\n"
                               "* schwarze Felder: Blöcke\n"
                               "* farbige Linien: Weg des entsprechenden Bots\n"
                               "* farbige Felder: Einflussbereich des entsprechenden Bots (er kann das Feld vor allen anderen erreichen)\n"
                               "Runde: Nummer der Runde\n"
                               "Freie Felder: Anzahl der freien Felder des Spielrasters, egal ob sie von einem Bot erreicht werden können\n"
                               "Botinfo\n"
                               "* Position und Bewegungsrichtung / tot\n"
                               "* Felder im Einflussbereich (siehe oben) / Runde des Ausscheidens\n"
                               "\n"
                               "Schieberegler\n"
                               "Erlaubt es, in die Historie des Spielrasters zu schauen\n"
                               "\n"
                               "<b>Tastaturbedienung</b>\n"
                               "F1: diese Hilfe aufrufen\n"
                               "↑: gehe Richtung Norden\n"
                               "→: gehe Richtung Osten\n"
                               "↓: gehe Richtung Süden\n"
                               "←: gehe Richtung Westen\n"
                               "Umschalt + Pfeiltaste: gehe bis zur nächsten Wand in die Richtung\n"
                               "Zahl + Pfeiltaste: gehe entsprechend viele Schritte in die Richtung\n"
                               "Bewegung ist auch mit den vi-Bewegungstasten hjkl möglich\n"
                               "Bild ↓: in der Historie eine Runde weiter gehen\n"
                               "Bild ↑: in der Historie eine Runde zurück gehen\n"
                               "Pos1: in der Historie zur ersten Runde springen\n"
                               "Ende: in der Historie zur aktuellen Runde springen\n"
                               "r: angezeigtes Spielfeld ausgeben und in Test.txt schreiben (zum Testen der Spielsituation)\n"
                               "R: angezeigtes Spielfeld „schön“ ausgeben\n"
                               "p: angezeigtes Spielfeld in der Datei „Spielraster.pdf“ speichern\n"
                               "P: Spielverlauf (als Bilder) in der Datei „Spielraster_Verlauf.pdf“ speichern\n"
                               "\n"
                               "<b>Sonstiges</b>\n"
                               "* Schritte gegen eine Wand werden ignoriert (außer es gibt keine andere Möglichkeit)\n"
                               "* Die Richtungsbefehle werden der Reihe nach abgearbeitet, auch wenn das Spiel bei der Eingabe noch nicht soweit war\n"
                               , true);
      // todo: Leertaste: entsprechend der Strategie laufen

      return ;
    } // Hilfe::init()
} // namespace UI_Gtkmm
