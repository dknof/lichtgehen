/*
   tronnot

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

#include "hauptfenster.h"
#include "hilfe.h"
#include "ueber.h"
#include "spielraster.h"
#include "../../spielraster/spielraster.h"

#include <gtkmm.h>
#include <fstream>

namespace UI_Gtkmm {
  auto const spieler_colors
    = vector<Gdk::Color>({Gdk::Color("#f00"),
                         Gdk::Color("#00f"),
                         Gdk::Color("#0c0"),
                         Gdk::Color("#f0f"),
                         Gdk::Color("#0ff"),
                         Gdk::Color("#ed0"),
                         Gdk::Color("#ccc")
                         });

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
    this->signal_realize().connect(sigc::mem_fun(*this, &Hauptfenster::init));
  } // Hauptfenster::Hauptfenster(UI_Gtkmm& ui)

  /**
   ** initializiere das Hauptfenster
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
        this->spielraster_ = Gtk::manage(new Gtk::DrawingArea());
        box_oben->pack_start(*this->spielraster_, Gtk::PACK_EXPAND_WIDGET);
        this->spielraster_->set_size_request(600, 600);
        this->spielraster_->signal_draw().connect(sigc::mem_fun(*this, &Hauptfenster::aktualisiere_spielraster), false);
        { // Informationsbereich
          auto box_info = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 20));
          box_info->set_border_width(20);
          box_oben->pack_end(*box_info, Gtk::PACK_SHRINK);
          // Runde
          this->runde_ = Gtk::manage(new Gtk::Label("Runde", Gtk::ALIGN_CENTER));
          {
            Pango::AttrList attrlist;
            {
              auto attr = Pango::Attribute::create_attr_weight(Pango::WEIGHT_BOLD);
              attrlist.insert(attr);
            }
            {
              auto attr = Pango::Attribute::create_attr_scale(1.41);
              attrlist.insert(attr);
            }
            this->runde_->set_attributes(attrlist);
          }
          box_info->add(*this->runde_);
          // Felder frei
          this->felder_frei_ = Gtk::manage(new Gtk::Label("Felder frei"));
          box_info->add(*this->felder_frei_);
          // Bots
          for (int b = 0; b < this->ui->spielraster->spieler_anz(); ++b) {
            this->spieler.push_back(Gtk::manage(new Gtk::Label("Bot " + std::to_string(b))));
            auto& spieler = *this->spieler[b];
            Pango::AttrList attrlist;
            {
              //auto attr = Pango::Attribute::create_attr_weight(Pango::WEIGHT_BOLD);
              //attrlist.insert(attr);
            }
            {
              auto color = Pango::Color();
              color.parse(spieler_colors[std::min(b, static_cast<int>(spieler_colors.size()) - 1)].to_string());
              auto attr = Pango::Attribute::create_attr_foreground(0, 0, 0);
              attr.set_color(color);
              attrlist.insert(attr);
            }
            spieler.set_attributes(attrlist);
            box_info->add(spieler);
          }
        } // Informationsbereich
      } // Oberer Bereich 
      { // Historie
        this->historie_ = Gtk::manage(new Gtk::Scale(Gtk::ORIENTATION_HORIZONTAL));
        this->historie_->set_increments(1, 10);
        this->historie_->set_range(1, 1);
        this->historie_->set_round_digits(0);
        hauptbox->pack_start(*this->historie_, Gtk::PACK_SHRINK);
        this->historie_->signal_value_changed().connect(sigc::mem_fun(*this, &Hauptfenster::aktualisiere), false);
      } // Historie

      { // Unterer Bereich 
        auto box_unten = Gtk::manage(new Gtk::ButtonBox(Gtk::ORIENTATION_HORIZONTAL));
        hauptbox->pack_end(*box_unten, Gtk::PACK_SHRINK);

        // Hilfeschaltfläche + Hilfefenster
        auto hilfe = Gtk::manage(new Gtk::Button(Gtk::Stock::HELP));
        box_unten->add(*hilfe);
        hilfe->signal_clicked().connect(sigc::mem_fun0(*this->ui->hilfe,
                                                       &Gtk::Window::present));

        // Über-Schaltfläche
        auto ueber = Gtk::manage(new Gtk::Button(Gtk::Stock::ABOUT));
        box_unten->add(*ueber);
        ueber->signal_clicked().connect(sigc::mem_fun0(*this->ui->ueber,
                                                       &Gtk::Window::present));

        // Beenden-Schaltfläche
        auto beenden = Gtk::manage(new Gtk::Button(Gtk::Stock::QUIT));
        box_unten->add(*beenden);
        beenden->signal_clicked().connect(sigc::mem_fun0(*this,
                                                         &Gtk::Window::hide));
      } // Unterer Bereich 


      this->show_all_children();
      this->show_all();

      return ;
    } // Hauptfenster::init()

  /**
   ** eine neue Runde
   ** 
   ** @param     n   Rundennummer
   **
   ** @return    -
   **
   ** @version   2015-02-04
   **/
  void
    Hauptfenster::runde(int const n)
    {
      // die Historie erweitern, dabei den Wert gegebenenfalls weiterschieben
      this->historie_->set_range(1, n + 1);
      if (this->historie_->get_value() == n) {
        this->historie_->set_value(n + 1);
        this->aktualisiere();
      }

      while (this->ui->main->events_pending())
        this->ui->main->iteration(false);

      return ;
    } // void Hauptfenster::runde(int const n)

  /**
   ** das Spiel endet
   ** 
   ** @param     -
   **
   ** @return    -
   **
   ** @version   2015-02-04
   **/
  void
    Hauptfenster::spiel_endet()
    {
      // die Historie erweitern, dabei den Wert gegebenenfalls weiterschieben
      auto const n = this->ui->spielraster->runde();
      this->historie_->set_range(1, n);
      if (this->historie_->get_value() >= n - 2) {
        this->historie_->set_value(n);
        this->aktualisiere();
      }

      return ;
    } // void Hauptfenster::spiel_endet()

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

      auto const spielraster = this->ui->spielraster->historie(this->historie_->get_value() - 1);

      // Runde
      if (this->ui->spielraster->runde() == this->historie_->get_value())
        this->runde_->set_label("       Runde " + std::to_string(spielraster.runde()) + "       ");
      else
        this->runde_->set_label("  Runde "
                               + std::to_string(static_cast<int>(this->historie_->get_value()))
                               + " (" + std::to_string(this->ui->spielraster->runde()) + ")  ");
      // Felder frei
      this->felder_frei_->set_label(std::to_string(spielraster.felder_frei()) + " Felder frei");

      // Bots
      for (int b = 0; b < spielraster.spieler_anz(); ++b) {
        auto text = "Bot " + std::to_string(b);
        if (spielraster.position(b)) {
          text += " " + std::to_string(spielraster.position(b));
          text += ", " + std::to_string(spielraster.einflussbereich_groesse_erreichbar(b)) + " (" + std::to_string(spielraster.einflussbereich_groesse(b)) + ")";
          //text += ", " + std::to_string(spielraster.einflussbereich_groesse(b)) + " Felder";
        } else {
          text += ", tot, Runde " + std::to_string(spielraster.weg(b).size());
        }
        this->spieler[b]->set_label(text);
      } // for (b)
      // Weitere Informationen

      // Spielraster neu zeichnen lassen
      this->spielraster_->queue_draw();

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

      auto const spielraster = this->ui->spielraster->historie(this->historie_->get_value() - 1);
      auto const breite = spielraster.breite();
      auto const laenge = spielraster.laenge();

      auto const allocation = this->spielraster_->get_allocation();
      cr->scale(static_cast<double>(allocation.get_width()) / breite,
                static_cast<double>(allocation.get_height()) / laenge);

      zeichne(spielraster, cr);

      return true;
    } // bool Hauptfenster::aktualisiere_spielraster(Cairo::RefPtr<Cairo::Context> const& cr)

  /**
   ** eine Taste wurde gedrückt
   **
   ** @param     key   gedrückte Taste
   **
   ** @return    true
   **
   ** @version   2014-11-21
   **/
  bool
    Hauptfenster::on_key_press_event(GdkEventKey* const key)
    {
#if 0
      struct GdkEventKey {
        GdkEventType type;
        GdkWindow *window;
        gint8 send_event;
        guint32 time;
        guint state;
        guint keyval;
        gint length;
        gchar *string;
        guint16 hardware_keycode;
        guint8 group;
      };
#endif

      if (!(key->state & ~(GDK_SHIFT_MASK | GDK_MOD1_MASK | GDK_MOD2_MASK))) {
        if (key->state == GDK_SHIFT_MASK) {
          switch (key->keyval) {
          case GDK_KEY_Up:
          case GDK_KEY_j:
            for (int i = 0; i < this->ui->spielraster->laenge(); ++i)
              this->ui->schwebende_richtungen.push_back(Richtung::NORDEN);
            this->nummer_eingabe = 0;
            this->historie_->set_value(this->ui->spielraster->runde());
            return true;
          case GDK_KEY_Right:
          case GDK_KEY_l:
            for (int i = 0; i < this->ui->spielraster->breite(); ++i)
              this->ui->schwebende_richtungen.push_back(Richtung::OSTEN);
            this->nummer_eingabe = 0;
            this->historie_->set_value(this->ui->spielraster->runde());
            return true;
          case GDK_KEY_Down:
          case GDK_KEY_k:
            for (int i = 0; i < this->ui->spielraster->laenge(); ++i)
              this->ui->schwebende_richtungen.push_back(Richtung::SUEDEN);
            this->nummer_eingabe = 0;
            this->historie_->set_value(this->ui->spielraster->runde());
            return true;
          case GDK_KEY_Left:
          case GDK_KEY_h:
            for (int i = 0; i < this->ui->spielraster->breite(); ++i)
              this->ui->schwebende_richtungen.push_back(Richtung::WESTEN);
            this->nummer_eingabe = 0;
            this->historie_->set_value(this->ui->spielraster->runde());
            return true;
          }
        }
        switch (key->keyval) {
        case GDK_KEY_0: this->nummer_eingabe *= 10; this->nummer_eingabe += 0; return true;
        case GDK_KEY_1: this->nummer_eingabe *= 10; this->nummer_eingabe += 1; return true;
        case GDK_KEY_2: this->nummer_eingabe *= 10; this->nummer_eingabe += 2; return true;
        case GDK_KEY_3: this->nummer_eingabe *= 10; this->nummer_eingabe += 3; return true;
        case GDK_KEY_4: this->nummer_eingabe *= 10; this->nummer_eingabe += 4; return true;
        case GDK_KEY_5: this->nummer_eingabe *= 10; this->nummer_eingabe += 5; return true;
        case GDK_KEY_6: this->nummer_eingabe *= 10; this->nummer_eingabe += 6; return true;
        case GDK_KEY_7: this->nummer_eingabe *= 10; this->nummer_eingabe += 7; return true;
        case GDK_KEY_8: this->nummer_eingabe *= 10; this->nummer_eingabe += 8; return true;
        case GDK_KEY_9: this->nummer_eingabe *= 10; this->nummer_eingabe += 9; return true;
        }
        switch (key->keyval) {
        case GDK_KEY_Up:
        case GDK_KEY_j:
          for (int i = 0; i < std::max(1, this->nummer_eingabe); ++i)
            this->ui->schwebende_richtungen.push_back(Richtung::NORDEN);
          this->nummer_eingabe = 0;
          this->historie_->set_value(this->ui->spielraster->runde());
          return true;
        case GDK_KEY_Right:
        case GDK_KEY_l:
          for (int i = 0; i < std::max(1, this->nummer_eingabe); ++i)
            this->ui->schwebende_richtungen.push_back(Richtung::OSTEN);
          this->nummer_eingabe = 0;
          this->historie_->set_value(this->ui->spielraster->runde());
          return true;
        case GDK_KEY_Down:
        case GDK_KEY_k:
          for (int i = 0; i < std::max(1, this->nummer_eingabe); ++i)
            this->ui->schwebende_richtungen.push_back(Richtung::SUEDEN);
          this->nummer_eingabe = 0;
          this->historie_->set_value(this->ui->spielraster->runde());
          return true;
        case GDK_KEY_Left:
        case GDK_KEY_h:
          for (int i = 0; i < std::max(1, this->nummer_eingabe); ++i)
            this->ui->schwebende_richtungen.push_back(Richtung::WESTEN);
          this->nummer_eingabe = 0;
          this->historie_->set_value(this->ui->spielraster->runde());
          return true;
        case GDK_KEY_Page_Up:
          this->historie_->set_value(this->historie_->get_value() - 1);
          return true;
        case GDK_KEY_Page_Down:
          this->historie_->set_value(this->historie_->get_value() + 1);
          return true;
        case GDK_KEY_Home:
          this->historie_->set_value(0);
          return true;
        case GDK_KEY_End:
          this->historie_->set_value(this->ui->spielraster->runde());
          return true;
        case GDK_KEY_space:
          break;
        case GDK_KEY_F1:
        case GDK_KEY_question:
          this->ui->hilfe->show();
          return true;
        case GDK_KEY_Escape:
          this->close();
          return true;
        case GDK_KEY_r:
          // Spielraster ausgeben und speichern
          this->ui->spielraster->historie(this->historie_->get_value() - 1).ausgeben(cerr);
          {
            std::ofstream ostr("Test.txt");
            if (ostr.good())
              this->ui->spielraster->historie(this->historie_->get_value() - 1).ausgeben(ostr);
          }
          return true;
        case GDK_KEY_R:
          // Raster ausgeben
          cerr << this->ui->spielraster->historie(this->historie_->get_value() - 1);
          return true;
        case GDK_KEY_p:
          // Raster in ein pdf ausgaben
          speicher(this->ui->spielraster->historie(this->historie_->get_value() - 1), "Spielraster.pdf");
          return true;
        case GDK_KEY_P:
          // Verlauf in ein pdf ausgaben
          speicher_verlauf(*this->ui->spielraster, "Spielraster_Verlauf.pdf");
          return true;
        default:
          return false;
        } // switch (key->keyval)
      }

      return false;
    } // bool UI_GTKMM::key_press(GdkEventKey const* const key)

} // namespace UI_Gtkmm
