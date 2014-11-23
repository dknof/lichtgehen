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
#include "hilfe.h"
#include "ueber.h"
#include "../../spielraster/spielraster.h"

#include <gtkmm.h>

namespace UI_Gtkmm {
  auto const bot_colors
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
        this->spielraster = Gtk::manage(new Gtk::DrawingArea());
        box_oben->pack_start(*this->spielraster, Gtk::PACK_EXPAND_WIDGET);
        this->spielraster->set_size_request(600, 600);
        this->spielraster->signal_draw().connect(sigc::mem_fun(*this, &Hauptfenster::aktualisiere_spielraster), false);
        { // Informationsbereich
          auto box_info = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 20));
          box_info->set_border_width(20);
          box_oben->pack_end(*box_info, Gtk::PACK_SHRINK);
          // Runde
          this->runde = Gtk::manage(new Gtk::Label("Runde", Gtk::ALIGN_CENTER));
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
            this->runde->set_attributes(attrlist);
          }
          box_info->add(*this->runde);
          // Felder frei
          this->felder_frei = Gtk::manage(new Gtk::Label("Felder frei"));
          box_info->add(*this->felder_frei);
          // Bots
          for (int b = 0; b < this->ui->spielraster->bot_anz(); ++b) {
            this->bot.push_back(Gtk::manage(new Gtk::Label("Bot " + std::to_string(b))));
            auto& bot = *this->bot[b];
            Pango::AttrList attrlist;
            {
              //auto attr = Pango::Attribute::create_attr_weight(Pango::WEIGHT_BOLD);
              //attrlist.insert(attr);
            }
            {
              auto color = Pango::Color();
              color.parse(bot_colors[std::min(b, static_cast<int>(bot_colors.size()) - 1)].to_string());
              auto attr = Pango::Attribute::create_attr_foreground(0.5, 0.5, 0.5);
              attr.set_color(color);
              attrlist.insert(attr);
            }
            bot.set_attributes(attrlist);
            box_info->add(bot);
          }
        } // Informationsbereich
      } // Oberer Bereich 
      { // Historie
        this->historie = Gtk::manage(new Gtk::Scale(Gtk::ORIENTATION_HORIZONTAL));
        this->historie->set_increments(1, 10);
        this->historie->set_range(1, 1);
        this->historie->set_round_digits(0);
        hauptbox->pack_start(*this->historie, Gtk::PACK_SHRINK);
        this->historie->signal_value_changed().connect(sigc::mem_fun(*this, &Hauptfenster::aktualisiere), false);
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
        beenden->signal_clicked().connect(sigc::ptr_fun(&Gtk::Main::quit));
      } // Unterer Bereich 


      this->show_all_children();
      this->show_all();

      //this->signal_key_press_event().connect(sigc::mem_fun(*this, &MainWindow::on_key_press_event));
      //
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

      auto const spielraster = this->ui->spielraster->historie(this->historie->get_value() - 1);

      // Runde
      if (this->ui->spielraster->runde() == this->historie->get_value())
        this->runde->set_label("Runde " + std::to_string(spielraster.runde()));
      else
        this->runde->set_label("Runde "
                               + std::to_string(static_cast<int>(this->historie->get_value()))
                               + " (" + std::to_string(this->ui->spielraster->runde()) + ")");
      { // die Historie gegebenenfalls erweitern, dabei den Wert weiterschieben
        int r = this->historie->get_value();
        if (r == this->ui->spielraster->runde() - 1) {
          this->historie->set_value(this->ui->spielraster->runde());
          if (this->historie->get_value() == r)
            r = this->ui->spielraster->runde();
        }
        this->historie->set_range(1, this->ui->spielraster->runde());
        this->historie->set_value(r);
      }

      // Felder frei
      this->felder_frei->set_label(std::to_string(spielraster.felder_frei()) + " Felder frei");

      // Bots
      for (int b = 0; b < spielraster.bot_anz(); ++b) {
        auto text = "Bot " + std::to_string(b);
        if (spielraster.position(b)) {
          text += " " + std::to_string(spielraster.position(b));
          text += ", " + std::to_string(spielraster.einflussbereich_groesse(b)) + " Felder";
        } else {
          text += ", tot, Runde " + std::to_string(spielraster.weg(b).size());
        }
        this->bot[b]->set_label(text);
      } // for (b)
      // Weitere Informationen

      // Spielraster neu zeichnen lassen
      this->spielraster->queue_draw();

      while (this->ui->main->events_pending())
        this->ui->main->iteration(false);

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
      auto const spielraster = this->ui->spielraster->historie(this->historie->get_value() - 1);
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

      // Einflussbereich der Bots malen
      cr->save();
      for (int b = 0; b < spielraster.bot_anz(); ++b) {
        auto const& color = bot_colors[std::min(b, static_cast<int>(bot_colors.size()) - 1)];
        cr->set_source_rgba(color.get_red_p(),
                            color.get_green_p(),
                            color.get_blue_p(),
                            0.2);
        auto raster = spielraster.einflussbereich(b);
        for (int x = 0; x < breite; ++x) {
          for (int y = 0; y < laenge; ++y) {
            if (raster(x, y))
              cr->rectangle(x, y, 1, 1);
          } // for (y)
        } // for (x)
        for (auto const& p : spielraster.weg(b))
          cr->rectangle(p.x(), p.y(), 1, 1);
        cr->fill();
      } // for (b)
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
      cr->save();
      cr->set_line_width(0.6);
      cr->set_line_join(Cairo::LINE_JOIN_ROUND);
      cr->set_line_cap(Cairo::LINE_CAP_ROUND);

      for (int b = 0; b < spielraster.bot_anz(); ++b) {
        auto const& color = bot_colors[std::min(b, static_cast<int>(bot_colors.size()) - 1)];
        cr->set_source_rgb(color.get_red_p(),
                           color.get_green_p(),
                           color.get_blue_p());
        auto const p = spielraster.weg(b)[0];
        switch (p.richtung()) {
        case Richtung::NORDEN:
          cr->move_to(p.x() + 0.5, p.y() + 0.7);
          break;
        case Richtung::SUEDEN:
          cr->move_to(p.x() + 0.5, p.y() + 0.3);
          break;
        case Richtung::OSTEN:
          cr->move_to(p.x() + 0.3, p.y() + 0.5);
          break;
        case Richtung::WESTEN:
          cr->move_to(p.x() + 0.7, p.y() + 0.5);
          break;
        } // switch (p.richtung())
        for (auto const& p : spielraster.weg(b))
          if (p)
            cr->line_to(p.x() + 0.5, p.y() + 0.5);
        cr->stroke();
      } // for (b)
      cr->restore();


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
            for (int i = 0; i < this->ui->spielraster->felder_frei(); ++i)
              this->ui->schwebende_richtungen.push_back(Richtung::NORDEN);
            this->nummer_eingabe = 0;
            this->historie->set_value(this->ui->spielraster->runde());
            return true;
          case GDK_KEY_Right:
          case GDK_KEY_l:
            for (int i = 0; i < this->ui->spielraster->felder_frei(); ++i)
              this->ui->schwebende_richtungen.push_back(Richtung::OSTEN);
            this->nummer_eingabe = 0;
            this->historie->set_value(this->ui->spielraster->runde());
            return true;
          case GDK_KEY_Down:
          case GDK_KEY_k:
            for (int i = 0; i < this->ui->spielraster->felder_frei(); ++i)
              this->ui->schwebende_richtungen.push_back(Richtung::SUEDEN);
            this->nummer_eingabe = 0;
            this->historie->set_value(this->ui->spielraster->runde());
            return true;
          case GDK_KEY_Left:
          case GDK_KEY_h:
            for (int i = 0; i < this->ui->spielraster->felder_frei(); ++i)
              this->ui->schwebende_richtungen.push_back(Richtung::WESTEN);
            this->nummer_eingabe = 0;
            this->historie->set_value(this->ui->spielraster->runde());
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
          this->historie->set_value(this->ui->spielraster->runde());
          return true;
        case GDK_KEY_Right:
        case GDK_KEY_l:
          for (int i = 0; i < std::max(1, this->nummer_eingabe); ++i)
            this->ui->schwebende_richtungen.push_back(Richtung::OSTEN);
          this->nummer_eingabe = 0;
          this->historie->set_value(this->ui->spielraster->runde());
          return true;
        case GDK_KEY_Down:
        case GDK_KEY_k:
          for (int i = 0; i < std::max(1, this->nummer_eingabe); ++i)
            this->ui->schwebende_richtungen.push_back(Richtung::SUEDEN);
          this->nummer_eingabe = 0;
          this->historie->set_value(this->ui->spielraster->runde());
          return true;
        case GDK_KEY_Left:
        case GDK_KEY_h:
          for (int i = 0; i < std::max(1, this->nummer_eingabe); ++i)
            this->ui->schwebende_richtungen.push_back(Richtung::WESTEN);
          this->nummer_eingabe = 0;
          this->historie->set_value(this->ui->spielraster->runde());
          return true;
        case GDK_KEY_Page_Up:
          this->historie->set_value(this->historie->get_value() - 1);
          return true;
        case GDK_KEY_Page_Down:
          this->historie->set_value(this->historie->get_value() + 1);
          return true;
        case GDK_KEY_Home:
          this->historie->set_value(0);
          return true;
        case GDK_KEY_End:
          this->historie->set_value(this->ui->spielraster->runde());
          return true;
        case GDK_KEY_space:
          cout << "Leertaste\n";
          break;
        case GDK_KEY_F1:
          this->ui->hilfe->show();
          return true;
        case GDK_KEY_Escape:
          this->close();
          return true;
        default:
          return false;
        } // switch (key->keyval)
      }

      return false;
    } // bool UI_GTKMM::key_press(GdkEventKey const* const key)

} // namespace UI_Gtkmm
