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
    hauptbox->pack_start(*box_oben, Gtk::PACK_EXPAND_WIDGET);
    // Spielfläche
    this->spielraster = Gtk::manage(new Gtk::DrawingArea());
    box_oben->pack_start(*this->spielraster, Gtk::PACK_EXPAND_WIDGET);
    this->spielraster->set_size_request(600, 600);
    this->spielraster->signal_draw().connect(sigc::mem_fun(*this, &Hauptfenster::aktualisiere_spielraster), false);
    { // Informationsbereich
      auto box_info = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 20));
      box_info->set_border_width(20);
      box_oben->pack_end(*box_info, Gtk::PACK_SHRINK);
      // Runde
      this->runde = Gtk::manage(new Gtk::Label("<big><b>Runde</b></big>", Gtk::ALIGN_CENTER));
      this->runde->set_use_markup();
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
    auto box_unten = Gtk::manage(new Gtk::ButtonBox(Gtk::ORIENTATION_HORIZONTAL));
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
  this->show_all();

  return ;
} // Hauptfenster::init()

/**
 ** aktualisiert alle Elemente
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2014-11-21
 **/
void
Hauptfenster::aktualisiere()
{
  if (!this->ui->spielraster)
    return ;

  // Runde
  // Bots
  // Weitere Informationen

  return ;
} // void Hauptfenster::aktualisiere()

/**
 ** aktualisiert das Spielraster, d.h. malt es neu
 ** 
 ** @param     cr    Schnittstelle zum Zeichnen
 **
 ** @return    -
 **
 ** @version   2014-11-21
 **/
bool
Hauptfenster::aktualisiere_spielraster(Cairo::RefPtr<Cairo::Context> const& cr)
{
  if (!this->ui->spielraster)
    return false;
  auto const spielraster = *this->ui->spielraster;
  auto const breite = spielraster.breite();
  auto const laenge = spielraster.laenge();

  auto const allocation = this->spielraster->get_allocation();
  cr->scale(static_cast<double>(allocation.get_width()) / breite,
            static_cast<double>(allocation.get_height()) / laenge);

  cr->save();
  cr->set_source_rgb(1, 1, 1);
  cr->paint();
  cr->restore();

  // Felder malen
  cr->save();
  cr->set_source_rgb(0, 0, 0);
  for (int x = 0; x < breite; ++x) {
    for (int y = 0; y < laenge; ++y) {
      if (spielraster(x, y))
        cr->rectangle(x, y, 1, 1);
    } // for (y)
  } // for (x)
  cr->fill();
  cr->restore();

  // Pfade der Bots wieder freimachen
    cr->save();
  cr->set_source_rgb(1, 1, 1);
  for (int b = 0; b < spielraster.bot_anz(); ++b)
    for (auto const f : spielraster.weg(b))
      cr->rectangle(f.x(), f.y(), 1, 1);
    cr->fill();
    cr->restore();

  cr->save();
  cr->set_source_rgb(0.5, 0.5, 0.5);
  cr->set_line_width(breite / static_cast<double>(allocation.get_width()));
  // Linien malen
  for (int x = 0; x <= breite; ++x)
    cr->move_to(x, 0), cr->line_to(x, laenge);
  cr->stroke();
  cr->restore();
  cr->save();
  cr->set_source_rgb(0.5, 0.5, 0.5);
  cr->set_line_width(laenge / static_cast<double>(allocation.get_height()));
  for (int y = 0; y <= laenge; ++y)
    cr->move_to(0, y), cr->line_to(breite, y);
  cr->stroke();
  cr->restore();

  // Pfade der Bots malen
  for (int b = 0; b < spielraster.bot_anz(); ++b) {
    cr->save();
    switch (b) {
    case 0: cr->set_source_rgb(1, 0, 0); break;
    case 1: cr->set_source_rgb(0, 1, 0); break;
    case 2: cr->set_source_rgb(0, 0, 1); break;
    case 3: cr->set_source_rgb(1, 1, 0); break;
    case 4: cr->set_source_rgb(1, 0, 1); break;
    case 5: cr->set_source_rgb(0, 1, 1); break;
    default: cr->set_source_rgb(0.5, 0.5, 0.5); break;
    } // switch (b)
    auto const weg = spielraster.weg(b);
    if (weg.size() == 1) {
      auto const p = spielraster.position(b);
      switch (p.richtung()) {
      case Richtung::NORDEN:
        cr->rectangle(p.x() + 0.2, p.y() + 0.2, 0.6, 0.8);
        break;
      case Richtung::SUEDEN:
        cr->rectangle(p.x() + 0.2, p.y(), 0.6, 0.8);
        break;
      case Richtung::OSTEN:
        cr->rectangle(p.x() + 0.2, p.y() + 0.2, 0.8, 0.6);
        break;
      case Richtung::WESTEN:
        cr->rectangle(p.x(), p.y() + 0.2, 0.8, 0.6);
        break;
      } // switch (p.richtung())
    } else {
      for (auto f = begin(weg); f+1 != end(weg); ++f) {
        auto const p = *f;
        switch ((f+1)->richtung()) {
        case Richtung::NORDEN:
          cr->rectangle(p.x() + 0.2, p.y() - 0.2, 0.6, 1);
          break;
        case Richtung::SUEDEN:
          cr->rectangle(p.x() + 0.2, p.y() + 0.2, 0.6, 1);
          break;
        case Richtung::OSTEN:
          cr->rectangle(p.x() + 0.2, p.y() + 0.2, 1, 0.6);
          break;
        case Richtung::WESTEN:
          cr->rectangle(p.x() - 0.2, p.y() + 0.2, 1, 0.6);
          break;
        } // switch (p.richtung())
      } // for (f)
    } // if !(weg.size() == 1)
    //for (auto const f : spielraster.weg(b))
    //cr->rectangle(f.x(), f.y(), 1, 1);
    cr->fill();
    cr->restore();

    // @todo: Einflussbereich darstellen
  } // for (b)


  return true;
} // bool Hauptfenster::aktualisiere_spielraster()

} // namespace UI_Gtkmm
