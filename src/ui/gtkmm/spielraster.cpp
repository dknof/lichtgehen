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
#include "spielraster.h"
#include "../../spielraster/spielraster.h"

#include <gdkmm.h>

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
   ** Speichert das Spielraster in ein pdf oder png
   ** 
   ** @param     spielraster   Spielraster
   ** @param     datei         Dateiname
   **
   ** @return    -
   **
   ** @version   2015-02-07
   **/
  void
    speicher(Spielraster const& spielraster, string const& datei)
    {
      if (datei.length() < 5) {
        cerr << "Typ der Datei '" << datei << "' unbekannt, Name ist zu kurz.\n";
        exit(EXIT_FAILURE);
      }
      if (datei.find(".") == string::npos) {
        cerr << "Typ der Datei '" << datei << "' nicht erkannt, kein '.' gefunden.\n";
        exit(EXIT_FAILURE);
      }
      auto const typ = string(datei, datei.find_last_of(".") + 1);
      if (   (typ != "pdf")
          && (typ != "png") ) {
        cerr << "Typ '" << typ << "' der Datei '" << datei << "' nicht unterstützt, nur 'pdf' und 'png' werden unterstützt\n";
        exit(EXIT_FAILURE);
      }
      if (typ == "png") {
        cerr << "Die Unterstützung für Typ '" << typ << "' der Datei '" << datei << "' ist noch fehlerhaft.\n";
        exit(EXIT_FAILURE);
      }

      auto const breite = spielraster.breite();
      auto const laenge = spielraster.laenge();

      Cairo::RefPtr<Cairo::Surface> surface;
      if (typ == "pdf")
        surface = Cairo::PdfSurface::create(datei, 10 * breite, 10 * laenge);
      else
        surface = Cairo::ImageSurface::create_from_png(datei);

      if (!surface) {
        cerr << "Fehler beim Erstellen des Cairo::Surface von Typ '" << typ << "' (Datei '" << datei << "').\n";
        exit(EXIT_FAILURE);
      }

      auto cr = Cairo::Context::create(surface);
      cr->scale(10.0, 10.0);

      zeichne(spielraster, cr);

      cr->show_page();
      return ;
    } // void speicher(Spielraster const& spielraster, string const& datei)

  /**
   ** Zeichnet das Spielraster
   ** 
   ** @param     spielraster   Spielraster
   ** @param     cr            Zeichenbereich
   **
   ** @return    -
   **
   ** @version   2015-02-07
   **/
  void
    zeichne(Spielraster const& spielraster,
            Cairo::RefPtr<Cairo::Context> const& cr)
    {
      auto const breite = spielraster.breite();
      auto const laenge = spielraster.laenge();

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
      for (int b = 0; b < spielraster.spieler_anz(); ++b)
        for (auto const f : spielraster.weg(b))
          cr->rectangle(f.x(), f.y(), 1, 1);
      cr->fill();
      cr->restore();

      // Einflussbereich der Bots malen
      cr->save();
      for (int b = 0; b < spielraster.spieler_anz(); ++b) {
        auto const& color = spieler_colors[std::min(b, static_cast<int>(spieler_colors.size()) - 1)];
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

      // Linien malen
      cr->save();
      cr->set_source_rgb(0.5, 0.5, 0.5);
      cr->set_line_width(0.05);
      for (int x = 0; x <= breite; ++x)
        cr->move_to(x, 0), cr->line_to(x, laenge);
      cr->stroke();
      cr->restore();
      cr->save();
      cr->set_source_rgb(0.5, 0.5, 0.5);
      cr->set_line_width(0.05);
      for (int y = 0; y <= laenge; ++y)
        cr->move_to(0, y), cr->line_to(breite, y);
      cr->stroke();
      cr->restore();

      // Pfade der Bots malen
      cr->save();
      cr->set_line_width(0.6);
      cr->set_line_join(Cairo::LINE_JOIN_ROUND);
      //cr->set_line_cap(Cairo::LINE_CAP_ROUND);

      for (int b = 0; b < spielraster.spieler_anz(); ++b) {
        auto const& color = spieler_colors[std::min(b, static_cast<int>(spieler_colors.size()) - 1)];
        cr->set_source_rgb(color.get_red_p(),
                           color.get_green_p(),
                           color.get_blue_p());
        auto const p = spielraster.weg(b)[0];
        switch (spielraster.weg(b).size() == 1 ? p.richtung() : spielraster.weg(b)[1].richtung()) {
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
        {
          // letzter Zug
          auto p = spielraster.weg(b).back();
          if (!p) {
            p = spielraster.weg(b)[spielraster.weg(b).size() - 2];
            if (p) {
              switch (p.richtung()) {
              case Richtung::NORDEN:
                cr->rel_line_to(0, -0.5);
                break;
              case Richtung::SUEDEN:
                cr->rel_line_to(0, 0.5);
                break;
              case Richtung::OSTEN:
                cr->rel_line_to(0.5, 0);
                break;
              case Richtung::WESTEN:
                cr->rel_line_to(-0.5, 0);
                break;
              } // switch (p.richtung())
              //auto const p2 = p + p.richtung();
              //cr->line_to(p2.x() + 0.5, p2.y() + 0.5);
            }
          }
        }
        cr->stroke();
      } // for (b)
      cr->restore();

      cr->save();
      cr->set_line_width(0.01);
      cr->set_line_join(Cairo::LINE_JOIN_ROUND);
      for (int b = 0; b < spielraster.spieler_anz(); ++b) {
        auto const& color = spieler_colors[std::min(b, static_cast<int>(spieler_colors.size()) - 1)];
        cr->set_source_rgb(color.get_red_p(),
                           color.get_green_p(),
                           color.get_blue_p());
        auto const p = spielraster.weg(b).back();
        if (!p)
          continue;
        cr->move_to(p.x() + 0.5, p.y() + 0.5);
        switch (p.richtung()) {
        case Richtung::NORDEN:
          cr->rel_line_to(-0.4, 0);
          cr->rel_line_to(0.4, -0.4);
          cr->rel_line_to(0.4, 0.4);
          break;
        case Richtung::SUEDEN:
          cr->rel_line_to(-0.4, 0);
          cr->rel_line_to(0.4, 0.4);
          cr->rel_line_to(0.4, -0.4);
          break;
        case Richtung::OSTEN:
          cr->rel_line_to(0, -0.4);
          cr->rel_line_to(0.4, 0.4);
          cr->rel_line_to(-0.4, 0.4);
          break;
        case Richtung::WESTEN:
          cr->rel_line_to(0, -0.4);
          cr->rel_line_to(-0.4, 0.4);
          cr->rel_line_to(0.4, 0.4);
          break;
        } // switch (p.richtung())
        cr->close_path();
        cr->fill();
      } // for (b)
      cr->restore();

      return ;
    } // void zeichne(Spielraster const& spielraster
} // namespace UI_Gtkmm
