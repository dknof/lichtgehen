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

#include "gui.h"

#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/drawingarea.h>

struct Gui::Pimpl {
  unique_ptr<Gtk::Main> main; // Hauptroutine
  unique_ptr<Gtk::Window> hauptfenster; // Hauptfenster
  Gtk::DrawingArea* spielraster; // Spielraster
}; // struct Gui::Pimpl

/**
 ** Konstruktor
 ** 
 ** @param     argc   Anzahl Kommandozeilenargumente
 ** @param     argv   Kommandozeilenargumente
 ** @param     spielraster   das Spielraster
 **
 ** @return    -
 **
 ** @version   2014-11-18
 **/
Gui::Gui(int& argc, char* argv[], Spielraster const& spielraster) :
  spielraster(spielraster),
  pimpl{make_unique<Gui::Pimpl>()}
{
  this->pimpl->main = make_unique<Gtk::Main>(argc, argv);
  this->init();
} // Gui::Gui(int argc, char* argv[]) :

/**
 ** Destruktor
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2014-11-18
 **/
Gui::~Gui()
{
} // Gui::~Gui()

/**
 ** initializiere die GUI
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2014-11-18
 **/
void
Gui::init()
{
  // Das Hauptfenster
  this->pimpl->hauptfenster = make_unique<Gtk::Window>();
  this->pimpl->hauptfenster->set_title("Tronbot");

  auto hauptbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
  this->pimpl->hauptfenster->add(*hauptbox);

  { // Oberer Bereich 
    auto box_oben = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 20));
    hauptbox->add(*box_oben);
    // Spielfläche
    this->pimpl->spielraster = Gtk::manage(new Gtk::DrawingArea());
    box_oben->add(*this->pimpl->spielraster);
    this->pimpl->spielraster->set_size_request(100, 100);
    { // Informationsbereich
      // Runde
      // Bots
      // freie Felder
      // Größe Einflussbereich
    } // Informationsbereich
  } // Oberer Bereich 

  { // Unterer Bereich 
    auto box_unten = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 20));
    box_unten->set_homogeneous();
    hauptbox->add(*box_unten);

    // Hilfeschaltfläche + Hilfefenster
    auto hilfe = Gtk::manage(new Gtk::Button("Hilfe"));
    box_unten->add(*hilfe);
    //m_button.signal_clicked().connect(sigc::mem_fun(*this, &HelloWorld::on_button_clicked));

    // Beenden-Schaltfläche
    auto beenden = Gtk::manage(new Gtk::Button("Beenden"));
    box_unten->add(*beenden);
    //m_button.signal_clicked().connect(sigc::mem_fun(*this, &HelloWorld::on_button_clicked));
  } // Unterer Bereich 


  this->pimpl->hauptfenster->show_all();

  // Einen zweiten Thread eröffnen, um die GUI darzustellen.
  // Der Thread wird benötigt, da ein Thread auf cin wartet.
  while (true) {
    this->pimpl->main->iteration();
  }

  return ;
} // Gui::void()
