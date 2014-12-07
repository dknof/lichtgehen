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

#include "spielraster.h"

#include <sstream>
#include <map>
#include <set>

/**
 ** Standardkonstruktor
 ** 
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2014-10-25
 **/
Spielraster::Spielraster()
{ }

/**
 ** Das Spielraster auf ostr schön ausgeben
 ** 
 ** @param     ostr   Ausgabestrom
 ** @param     spielraster  Spielraster zum ausgeben
 **
 ** @return    Ausgabestrom
 **
 ** @version   2014-10-25
 **/
ostream&
operator<<(ostream& ostr, Spielraster const& spielraster)
{ return spielraster.ausgeben_schoen(ostr); }

/**
 ** Das Spielraster auf ostr ausgeben
 ** Die Ausgabe entspricht dem Format zum Laden
 ** 
 ** @param     ostr   Ausgabestrom
 **
 ** @return    Ausgabestrom
 **
 ** @version   2014-12-01
 **/
ostream& 
Spielraster::ausgeben(ostream& ostr) const
{
  ostr << "GAMEBOARDSTART "
    << this->breite() << "," << this->laenge() << '\n';
  Raster raster = *this;
  for (int b = 0; b < this->bot_anz(); ++b)
    raster.belege(this->position(b), false);
  ostr << raster;
  ostr << "GAMEBOARDEND\n";
  for (int b = 0; b < this->bot_anz(); ++b) {
    auto const p = this->position(b);
    ostr << "POS " << b+1 << " "
      << p.x()+1 << "," << p.y()+1 << " " << p.richtung() << '\n';
  }
  return ostr;
} // ostream& Spielraster::ausgeben(ostream& ostr) const

/**
 ** Das Spielraster auf ostr ausgeben
 ** 
 ** @param     ostr   Ausgabestrom
 **
 ** @return    Ausgabestrom
 **
 ** @version   2014-10-25
 **/
ostream& 
Spielraster::ausgeben_schoen(ostream& ostr) const
{
  typedef std::pair<Richtung, Richtung> RP;
  std::map<RP, string> richtung_symbol;
  richtung_symbol[RP(Richtung::NORDEN, Richtung::NORDEN)] = "│"; // = "↑";
  richtung_symbol[RP(Richtung::NORDEN, Richtung::OSTEN )] = "┌"; // = "↱";
  richtung_symbol[RP(Richtung::NORDEN, Richtung::WESTEN)] = "┐"; // = "↰";
  richtung_symbol[RP(Richtung::OSTEN,  Richtung::OSTEN )] = "─"; // = "→";
  richtung_symbol[RP(Richtung::OSTEN,  Richtung::SUEDEN)] = "┐"; // = "⬎";
  richtung_symbol[RP(Richtung::OSTEN,  Richtung::NORDEN)] = "┘"; // = "⬏";
  richtung_symbol[RP(Richtung::SUEDEN, Richtung::SUEDEN)] = "│"; // = "↓";
  richtung_symbol[RP(Richtung::SUEDEN, Richtung::OSTEN )] = "└"; // = "↳";
  richtung_symbol[RP(Richtung::SUEDEN, Richtung::WESTEN)] = "┘"; // = "↲";
  richtung_symbol[RP(Richtung::WESTEN, Richtung::WESTEN)] = "─"; // = "←";
  richtung_symbol[RP(Richtung::WESTEN, Richtung::NORDEN)] = "└"; // = "⬑";
  richtung_symbol[RP(Richtung::WESTEN, Richtung::SUEDEN)] = "┌"; // = "⬐";

  ostr << "┌";
  for (int x = 0; x < this->breite(); ++x)
    ostr << "─";
  ostr << "┐\n";
  for (int y = 0; y < this->laenge(); ++y) {
    ostr << "│";
    for (int x = 0; x < this->breite(); ++x) {
      if ((*this)(x, y)) {
#if 1 // farbige Ausgabe
        // Bot suchen
        int n; // Nummer des Bots
        BotWeg::const_iterator pos; // Position des Bots
        for (n = 0; n < this->bot_anz(); ++n) {
          for (pos = begin(this->weg(n)); pos != end(this->weg(n)); ++pos) {
            if (*pos == Position(x, y)) {
              break;
            }
          } // for (p)
          if (pos != end(this->weg(n)))
            break;
        } // for (n)
        if (n < this->bot_anz()) {
          // Botverlauf ausgeben
          ostr << "\033[" << (31 + 3 * n) << ";1m";
          if (*pos == this->weg(n)[0]) { // Startposition
            switch (((this->weg(n).size() == 1) ? pos : pos + 1)->richtung()) {
              //switch (pos->richtung()) // ←↑→↓↧↦↥↤↱↲↳↴↵↩↪
            case Richtung::NORDEN: ostr << "╵"; break; // ↥
            case Richtung::OSTEN:  ostr << "╶"; break; // ↦
            case Richtung::SUEDEN: ostr << "╷"; break; // ↧
            case Richtung::WESTEN: ostr << "╴"; break; // ↤
            } // switch (pos->richtung())
          } else if ((*pos == this->position(n))
                     || !*(pos + 1))
          { // aktuelle Position
            switch (pos->richtung()) { // ←↑→↓↧↦↥↤↱↲↳↴↵↩↪
            case Richtung::NORDEN: ostr << "╷"; break; // ↑
            case Richtung::OSTEN:  ostr << "╴"; break; // →
            case Richtung::SUEDEN: ostr << "╵"; break; // ↓
            case Richtung::WESTEN: ostr << "╶"; break; // ←
            } // switch (pos->richtung())
          } else {
            ostr << richtung_symbol[RP(pos->richtung(), (pos + 1)->richtung())];
          }
          ostr << "\033[0;m";
        } else {
          ostr << "█"; // #
        }
#else
        ostr << '#';
#endif
      } else {
        ostr << "·";
      }
    } // for (x)
    ostr << "│";
    ostr << '\n';
  } // for (y)
  ostr << "└";
  for (int x = 0; x < this->breite(); ++x)
    ostr << "─";
  ostr << "┘\n";
  // Positionen/Rundenanzahl der Bots ausgeben
  for (int n = 0; n < this->bot_anz(); ++n) 
    if (this->position(n))
      ostr << "Bot " << n << ": " << this->position(n) << '\n';
    else
      ostr << "Bot " << n << ": " << this->weg(n).size() - 2 << " Runden\n";

  return ostr;
} // ostream& Raster::ausgeben_schoen(ostream& ostr) const

/**
 ** -> Rückgabe
 ** 
 ** @param     -
 **
 ** @return    die Anzahl der Bots
 **
 ** @version   2014-10-25
 **/
int
Spielraster::bot_anz() const
{
  return this->bot_anz_;
} // int Spielraster::bot_anz() const

/**
 ** -> Rückgabe
 ** 
 ** @param     -
 **
 ** @return    die Anzahl der Bots im Spiel
 **
 ** @version   2014-11-08
 **/
int
Spielraster::bots_im_spiel() const
{
  int n = 0;
  for (int i = 0; i < this->bot_anz(); ++i)
    if (this->bot_im_spiel(i))
      n += 1;
  return n;
} // int Spielraster::bots_im_spiel() const

/**
 ** -> Rückgabe
 ** 
 ** @param     bot   Nummer des Bots
 **
 ** @return    ob der Bot noch im Spiel ist
 **
 ** @version   2014-11-10
 **/
bool
Spielraster::bot_im_spiel(int const bot) const
{
  return this->position(bot);
} // bool Spielraster::bots_im_spiel(int const bot) const

/**
 ** setzt den Bot auf ein Raster
 ** 
 ** @param     istr   Eingabestrom mit den Daten
 **
 ** @return    -
 **
 ** @version   2014-10-30
 **/
void
Spielraster::setze_bot(istream& istr)
{
  istr.get(); istr.get(); istr.get(); istr.get(); // "SET " oder "POS "
  int n;
  istr >> n;
  BotPosition bp(istr);
  this->setze_bot(n - 1, bp);
  return ;
} // void Spielraster::setze_bot(istream& istr)

/**
 ** setzt den Bot auf ein Raster
 ** 
 ** @param     n    Nummer des Bots
 ** @param     bp   BotPosition
 **
 ** @return    -
 **
 ** @version   2014-10-27
 **/
void
Spielraster::setze_bot(int const n, BotPosition const& bot_position)
{
  if (n > this->bot_anz()) {
    cerr << "Botanzahl nicht wie erwartet: Anzahl = " << this->bot_anz() << ", aber bot " << n << " soll gesetzt werden\n";
    exit(1);
  }
  if (n == this->bot_anz()) {
    this->bot_anz_ += 1;
    this->bot_weg_.push_back(BotWeg());
  }
  this->bot_weg_[n].push_back(bot_position);
  this->belege(bot_position);

  return; 
} // void Spielraster::setze_bot(int const n, BotPosition const& bot_position)

/**
 ** entfernt einen Bot vom Spielraster
 ** 
 ** @param     n    Nummer des Bots
 **
 ** @return    -
 **
 ** @version   2014-10-27
 **/
void
Spielraster::entferne_bot(int const n)
{
  // setze den Bot auf eine Position außerhalb vom Spielraster
  this->bot_weg_[n].push_back(BotPosition{Position::KEINE, Richtung::NORDEN});
  return;
} // void Spielraster::entferne_bot(int const n)

/**
 ** -> Rückgabe
 ** 
 ** @param     n    Nummer des Bots
 **
 ** @return    die Position des Bots
 **
 ** @version   2014-10-27
 **/
BotPosition const&
Spielraster::position(int const n) const
{
  return this->weg(n).back();
} // BotPosition const& Spielraster::position(int const n) const

/**
 ** -> Rückgabe
 ** 
 ** @param     n    Nummer des Bots
 **
 ** @return    der Weg des Bots
 **
 ** @version   2014-10-27
 **/
BotWeg const&
Spielraster::weg(int const n) const
{
  return this->bot_weg_[n];
} // BotWeg const& Spielraster::weg(int const n) const

/**
 ** bewegt den Bot um einen Schritt
 ** 
 ** @param     n    Nummer des Bots
 ** @param     br   Bewegungsrichtung
 **
 ** @return    -
 **
 ** @version   2014-11-13
 **/
void 
Spielraster::bewege_bot(int const n, Bewegungsrichtung const br)
{
  BotPosition const bp{this->position(n) + br};
  if ((*this)(bp)) {
    this->entferne_bot(n);
    return ;
  }

  this->bot_weg_[n].push_back(bp);
  this->belege(bp);

  return ;
} // void Spielraster::bewege_bot(int const n, Bewegungsrichtung const br)

/**
 ** -> Rückgabe
 ** 
 ** @param     bot       Nummer des Bots
 **
 ** @return    Informationen über den Raum, in dem sich der Bot befindet
 **
 ** @version   2014-11-11
 **/
Spielraster::RaumInfo
Spielraster::rauminfo(int const bot) const
{
  if (!this->bot_im_spiel(bot))
    return RaumInfo{};
  RaumInfo rauminfo;
  rauminfo.groesse = this->raumgroesse(this->position(bot));
  rauminfo.bot_anz = 0;

  // Entfernung zum nächsten Bot
  rauminfo.entfernung_naechster_bot = this->breite() * this->laenge() + 1;
  for (int b = 0; b < this->bot_anz(); ++b) {
    if (b == bot)
      continue;
    if (!this->bot_im_spiel(b))
      continue;
    auto const n = this->kuerzeste_entfernung(this->position(bot),
                                             this->position(b));
    if (n >= 0) {
      rauminfo.bot_anz += 1;
      rauminfo.entfernung_naechster_bot = std::min(rauminfo.entfernung_naechster_bot, n);
    }
  } // for (int b = 0; b < this->bot_anz(); ++b)
  if (rauminfo.bot_anz == 0)
    rauminfo.entfernung_naechster_bot = -1;
  return rauminfo;
} // Spielraster::RaumInfo Spielraster::rauminfo(int const bot) const

/**
 ** -> Rückgabe
 ** 
 ** @param     bot       Nummer des Bots
 ** @param     bewegungsrichtung   Richtung des Raumes
 **
 ** @return    Informationen über den Raum des Bots in Richtung „bewegungsrichtung”
 **
 ** @version   2014-11-11
 **
 ** @todo      Durch kopieren und modifizieren der Algorithmen Raster::raumgroesse / Raster::kuerzeste_entfernung lässt sich die Performance steigern. Dies lohnt hier aber vermutlich nicht, also wird der Code hier lieber simpel gehalten.
 **/
Spielraster::RaumInfo
Spielraster::rauminfo(int const bot,
                      Bewegungsrichtung const bewegungsrichtung) const
{
  RaumInfo rauminfo;
  rauminfo.groesse = this->raumgroesse(this->position(bot) + bewegungsrichtung, true);
  rauminfo.bot_anz = 0;
  rauminfo.entfernung_naechster_bot = this->breite() * this->laenge() + 1;
  for (int b = 0; b < this->bot_anz(); ++b) {
    if (b == bot)
      continue;
    auto const n = this->kuerzeste_entfernung(this->position(bot),
                                             this->position(b));
    if (n >= 0) {
      rauminfo.bot_anz += 1;
      rauminfo.entfernung_naechster_bot = std::min(rauminfo.entfernung_naechster_bot, n);
    }
  } // for (int b = 0; b < this->bot_anz(); ++b)
  if (rauminfo.bot_anz == 0)
    rauminfo.entfernung_naechster_bot = -1;
  return rauminfo;
} // Spielraster::RaumInfo Spielraster::rauminfo(int const bot, Bewegungsrichtung const bewegungsrichtung) const

/**
 ** -> Rückgabe
 ** 
 ** @param     -
 **
 ** @return    die Runde
 **
 ** @version   2014-11-22
 **/
int 
Spielraster::runde() const
{
  auto r = this->bot_weg_[0].size();
  for (auto const& w : this->bot_weg_)
    r = std::max(r, w.size());

  return r;
} // int Spielraster::runde() const

/**
 ** -> Rückgabe
 ** 
 ** @param     runde   Runde
 **
 ** @return    das Spielraster in der angegebenen Runde
 **
 ** @version   2014-11-22
 **/
Spielraster 
Spielraster::historie(int const runde) const
{
  if (runde >= this->runde())
    return *this;

  auto sr = *this;
  // Felder wieder freisetzen
  for (auto const& w : sr.bot_weg_) {
    if (static_cast<int>(w.size()) >= runde + 2)
      for (auto f = (begin(w) + runde + 1); f != end(w); ++f)
        sr.belege(*f, false);
  }
  // Weg der Bots beschneiden
  for (auto& w : sr.bot_weg_)
    w.resize(std::min(static_cast<int>(w.size()), runde + 1));

  return sr;
} // int Spielraster::runde() const

/**
 ** -> Rückgabe
 ** 
 ** @param     bot       Nummer des Bots
 **
 ** @return    Groesse des Einflussbereichs
 **
 ** @version   2014-11-22
 **/
int 
Spielraster::einflussbereich_groesse(int const bot) const
{
  return this->einflussbereich(bot).felder_belegt();
} // int Spielraster::einflussbereich_groesse(int bot) const

/**
 ** -> Rückgabe
 ** 
 ** @param     bot   Nummer des Bots
 ** @param     br    Richtung, für die geprüft wird
 **
 ** @return    Groesse des Einflussbereichs in dem Feld Richtung br
 **
 ** @version   2014-11-22
 **/
int 
Spielraster::einflussbereich_groesse(int const bot, Bewegungsrichtung const br) const
{
  return this->einflussbereich(bot, br).felder_belegt();
} // int Spielraster::einflussbereich_groesse(int bot, Bewegungsrichtung const br) const

/**
 ** -> Rückgabe
 ** Ermittelt den Einflussbereich eines Bots
 ** 
 ** @param     bot       Nummer des Bots
 **
 ** @return    Einflussbereich des Bots
 **
 ** @version   2014-11-22
 **/
Raster 
Spielraster::einflussbereich(int const bot) const
{
  return einflussbereich(bot, this->position(bot));
} // Raster Spielraster::einflussbereich(int const bot) const

/**
 ** -> Rückgabe
 ** Ermittelt den Einflussbereich eines Bots
 ** 
 ** @param     bot       Nummer des Bots
 ** @param     br    Richtung, für die geprüft wird
 **
 ** @return    Einflussbereich des Bots
 **
 ** @version   2014-11-22
 **/
Raster 
Spielraster::einflussbereich(int const bot, Bewegungsrichtung const br) const
{
  return einflussbereich(bot, this->position(bot) + br);
} // Raster Spielraster::einflussbereich(int bot, Bewegungsrichtung br) const

/**
 ** -> Rückgabe
 ** Ermittelt den Einflussbereich eines Bots
 ** 
 ** @param     bot       Nummer des Bots
 ** @param     position  Position des Bots
 **
 ** @return    Einflussbereich des Bots
 **
 ** @version   2014-11-22
 **/
Raster 
Spielraster::einflussbereich(int const bot, Position const position) const
{
  if (  (*this)(position)
      && (position != this->position(bot)) )
    return Raster{this->breite(), this->laenge()};
  auto raster = static_cast<Raster>(*this);
  auto raster_bot = Raster{this->breite(), this->laenge()};

  std::set<Position> offene_positionen_bot; // offene Positionen des Bots
  std::set<Position> offene_positionen_sonst; // offene Positionen der anderen Bots
  for (int b = 0; b < this->bot_anz(); ++b) {
    auto const p = this->position(b);
    if (!p)
      continue;
    if (b == bot)
      offene_positionen_bot.insert(position);
    else
      offene_positionen_sonst.insert(p);
  } // for (b)

  std::set<Position> offene_positionen_bot_neu; // neue offene Positionen des Bots
  std::set<Position> offene_positionen_sonst_neu; // neue offene Positionen der anderen Bots
  while (   !offene_positionen_bot.empty()
         || !offene_positionen_sonst.empty()) {
    offene_positionen_bot_neu.clear();
    offene_positionen_sonst_neu.clear();
    // Erst den Einflussbereich der anderen Bots erweitern, anschließend den eigenen
    for (auto p : offene_positionen_sonst) {
      for (auto r : ::richtungen) {
        if (!raster(p + r)) {
          raster.belege(p + r);
          offene_positionen_sonst_neu.insert(p + r);
        }
      }
    } // for (p)
    for (auto p : offene_positionen_bot) {
      for (auto r : ::richtungen) {
        if (!raster(p + r)) {
          raster.belege(p + r);
          raster_bot.belege(p + r);
          offene_positionen_bot_neu.insert(p + r);
        }
      }
    } // for (p)
    offene_positionen_bot = offene_positionen_bot_neu;
    offene_positionen_sonst = offene_positionen_sonst_neu;
  } // while (offene_positionen_bot || offene_positionen_sonst)

  return raster_bot;
} // Raster Spielraster::einflussbereich(int bot, Position position) const

/**
 ** -> Rückgabe
 ** Ermittelt, wie viele Felder vom Einflussbereich durch einen Durchlauf erreicht werden können.
 ** 
 ** @param     bot       Nummer des Bots
 **
 ** @return    Anzahl der Felder, die der Bot aus seinem Einflussbereich und nur über den Einflussbereich erreichen kann
 **
 ** @version   2014-11-28
 **/
int 
Spielraster::einflussbereich_groesse_erreichbar(int const bot) const
{
  Raster r = *this;
  r.belege(this->einflussbereich(bot).invertiert());
  return r.raumgroesse_erreichbar(this->position(bot));
} // int Spielraster::einflussbereich_groesse_erreichbar(int bot) const
