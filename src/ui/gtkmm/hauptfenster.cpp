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

#include "hauptfenster.h"
#include "../../spielraster/spielraster.h"

#include <gtkmm.h>

namespace UI_Gtkmm {
/**
 ** Konstruktor
 ** 
 ** @param     ui   das UI
 **
 ** @return    -
 **
 ** @version   2014-11-19
 **/
Hauptfenster::Hauptfenster(UI_Gtkmm& ui) :
  Gtk::Window(),
  ui(&ui)
{
  this->init();
} // Hauptfenster::Hauptfenster(UI_Gtkmm& ui)

/**
 ** initializiere das UI
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2014-11-18
 **/
void
Hauptfenster::init()
{
  // Das Hauptfenster
  this->set_title("Tronbot");

  auto hauptbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
  this->add(*hauptbox);

  { // Oberer Bereich 
    auto box_oben = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 20));
    hauptbox->add(*box_oben);
    // Spielfläche
    this->spielraster = Gtk::manage(new Gtk::DrawingArea());
    box_oben->add(*this->spielraster);
    this->spielraster->set_size_request(100, 100);
    { // Informationsbereich
      auto box_info = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 20));
      box_oben->pack_end(*box_info, Gtk::PACK_SHRINK);
      // Runde
      this->runde = Gtk::manage(new Gtk::Label("Runde"));
      box_info->add(*this->runde);
      // Bots
      this->bot1 = Gtk::manage(new Gtk::Label("Bot1"));
      this->bot2 = Gtk::manage(new Gtk::Label("Bot2"));
      box_info->add(*this->bot1);
      box_info->add(*this->bot2);
      // freie Felder
      // Größe Einflussbereich
    } // Informationsbereich
  } // Oberer Bereich 

  { // Unterer Bereich 
    auto box_unten = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 20));
    box_unten->set_homogeneous();
    hauptbox->pack_end(*box_unten, Gtk::PACK_SHRINK);

    // Hilfeschaltfläche + Hilfefenster
    auto hilfe = Gtk::manage(new Gtk::Button(Gtk::Stock::HELP));
    box_unten->add(*hilfe);
    // @todo: Signal

    // Beenden-Schaltfläche
    auto beenden = Gtk::manage(new Gtk::Button(Gtk::Stock::QUIT));
    box_unten->add(*beenden);
    beenden->signal_clicked().connect(sigc::ptr_fun(&Gtk::Main::quit));
  } // Unterer Bereich 


  this->show_all_children();
  this->aktualisiere_spielraster();
  this->show_all();
  CLOG << endl;

  return ;
} // Hauptfenster::init()

/**
 ** aktualisiert das Spielraster, d.h. malt es neu
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2014-11-20
 **/
void
Hauptfenster::aktualisiere_spielraster()
{
  if (!this->ui->spielraster)
    return ;
  auto const breite = this->spielraster->get_allocated_width();
  auto const hoehe = this->spielraster->get_allocated_height();
  // Felder malen
  // Linien malen
  for (int x = 0; x < this->ui->spielraster->breite(); ++x) {
  } // for (x)
  for (int y = 0; y < this->ui->spielraster->laenge(); ++y) {
  } // for (y)
  (void)breite;
  (void)hoehe;
  while (this->ui->main->events_pending()) {
    this->ui->main->iteration(false);
  }


  return ;
} // void Hauptfenster::aktualisiere_spielraster()

} // namespace UI_Gtkmm
