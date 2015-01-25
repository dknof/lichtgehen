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
  for (int b = 0; b < this->spieler_anz(); ++b)
    raster.belege(this->position(b), false);
  ostr << raster;
  ostr << "GAMEBOARDEND\n";
#if 0
  for (int b = 0; b < this->spieler_anz(); ++b) {
    auto const p = this->position(b);
    ostr << "POS " << b+1 << " "
      << p.x()+1 << "," << p.y()+1 << " " << p.richtung() << '\n';
  }
#endif
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
        // Spieler suchen
        int n; // Nummer des Spielers
        SpielerWeg::const_iterator pos; // Position des Spielers
        for (n = 0; n < this->spieler_anz(); ++n) {
          for (pos = begin(this->weg(n)); pos != end(this->weg(n)); ++pos) {
            if (*pos == Position(x, y)) {
              break;
            }
          } // for (p)
          if (pos != end(this->weg(n)))
            break;
        } // for (n)
        if (n < this->spieler_anz()) {
          // Spielerverlauf ausgeben
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
  // Positionen/Rundenanzahl der Spielers ausgeben
  for (int n = 0; n < this->spieler_anz(); ++n) 
    if (this->position(n))
      ostr << "Spieler " << n << ": " << this->position(n) << '\n';
    else
      ostr << "Spieler " << n << ": " << this->weg(n).size() - 2 << " Runden\n";

  return ostr;
} // ostream& Raster::ausgeben_schoen(ostream& ostr) const

/**
 ** -> Rückgabe
 ** 
 ** @param     -
 **
 ** @return    die Anzahl der Spieler
 **
 ** @version   2014-10-25
 **/
int
Spielraster::spieler_anz() const
{
  return this->spieler_anz_;
} // int Spielraster::spieler_anz() const

/**
 ** -> Rückgabe
 ** 
 ** @param     -
 **
 ** @return    die Anzahl der Spieler im Spiel
 **
 ** @version   2014-11-08
 **/
int
Spielraster::spieler_im_spiel() const
{
  int n = 0;
  for (int i = 0; i < this->spieler_anz(); ++i)
    if (this->spieler_im_spiel(i))
      n += 1;
  return n;
} // int Spielraster::spieler_im_spiel() const

/**
 ** -> Rückgabe
 ** 
 ** @param     spieler   Nummer des Spielers
 **
 ** @return    ob der Spieler noch im Spiel ist
 **
 ** @version   2014-11-10
 **/
bool
Spielraster::spieler_im_spiel(int const spieler) const
{
  return this->position(spieler);
} // bool Spielraster::spieler_im_spiel(int const spieler) const

/**
 ** setzt den Spieler auf ein Raster
 ** 
 ** @param     istr   Eingabestrom mit den Daten
 **
 ** @return    -
 **
 ** @version   2014-10-30
 **/
void
Spielraster::setze_spieler(istream& istr)
{
  istr.get(); istr.get(); istr.get(); istr.get(); // "SET " oder "POS "
  int n;
  istr >> n;
  SpielerPosition bp(istr);
  this->setze_spieler(n - 1, bp);
  return ;
} // void Spielraster::setze_spieler(istream& istr)

/**
 ** setzt den Spieler auf ein Raster
 ** 
 ** @param     n    Nummer des Spielers
 ** @param     bp   SpielerPosition
 **
 ** @return    -
 **
 ** @version   2014-10-27
 **/
void
Spielraster::setze_spieler(int const n, SpielerPosition const& spieler_position)
{
  if (n > this->spieler_anz()) {
    cerr << "Spieleranzahl nicht wie erwartet: Anzahl = " << this->spieler_anz() << ", aber spieler " << n << " soll gesetzt werden\n";
    exit(1);
  }
  if (n == this->spieler_anz()) {
    this->spieler_anz_ += 1;
    this->spieler_weg_.push_back(SpielerWeg());
  }
  this->spieler_weg_[n].push_back(spieler_position);
  this->belege(spieler_position);

  return; 
} // void Spielraster::setze_spieler(int const n, SpielerPosition const& spieler_position)

/**
 ** entfernt einen Spieler vom Spielraster
 ** 
 ** @param     n    Nummer des Spielers
 **
 ** @return    -
 **
 ** @version   2014-10-27
 **/
void
Spielraster::entferne_spieler(int const n)
{
  // setze den Spieler auf eine Position außerhalb vom Spielraster
  this->spieler_weg_[n].push_back(SpielerPosition{Position::KEINE, Richtung::NORDEN});
  return;
} // void Spielraster::entferne_spieler(int const n)

/**
 ** -> Rückgabe
 ** 
 ** @param     n    Nummer des Spielers
 **
 ** @return    die Position des Spielers
 **
 ** @version   2014-10-27
 **/
SpielerPosition const&
Spielraster::position(int const n) const
{
  return this->weg(n).back();
} // SpielerPosition const& Spielraster::position(int const n) const

/**
 ** -> Rückgabe
 ** 
 ** @param     n    Nummer des Spielers
 **
 ** @return    der Weg des Spielers
 **
 ** @version   2014-10-27
 **/
SpielerWeg const&
Spielraster::weg(int const n) const
{
  return this->spieler_weg_[n];
} // SpielerWeg const& Spielraster::weg(int const n) const

/**
 ** bewegt den Spieler um einen Schritt
 ** 
 ** @param     n    Nummer des Spielers
 ** @param     br   Bewegungsrichtung
 **
 ** @return    -
 **
 ** @version   2014-11-13
 **/
void 
Spielraster::bewege_spieler(int const n, Bewegungsrichtung const br)
{
  SpielerPosition const bp{this->position(n) + br};
  if ((*this)(bp)) {
    this->entferne_spieler(n);
    return ;
  }

  this->spieler_weg_[n].push_back(bp);
  this->belege(bp);

  return ;
} // void Spielraster::bewege_spieler(int const n, Bewegungsrichtung const br)

/**
 ** -> Rückgabe
 ** 
 ** @param     spieler       Nummer des Spielers
 **
 ** @return    Informationen über den Raum, in dem sich der Spieler befindet
 **
 ** @version   2014-11-11
 **/
Spielraster::RaumInfo
Spielraster::rauminfo(int const spieler) const
{
  if (!this->spieler_im_spiel(spieler))
    return RaumInfo{};
  RaumInfo rauminfo;
  rauminfo.groesse = this->raumgroesse(this->position(spieler));
  rauminfo.spieler_anz = 0;

  // Entfernung zum nächsten Spieler
  rauminfo.entfernung_naechster_spieler = this->breite() * this->laenge() + 1;
  for (int b = 0; b < this->spieler_anz(); ++b) {
    if (b == spieler)
      continue;
    if (!this->spieler_im_spiel(b))
      continue;
    auto const n = this->kuerzeste_entfernung(this->position(spieler),
                                             this->position(b));
    if (n >= 0) {
      rauminfo.spieler_anz += 1;
      rauminfo.entfernung_naechster_spieler = std::min(rauminfo.entfernung_naechster_spieler, n);
    }
  } // for (int b = 0; b < this->spieler_anz(); ++b)
  if (rauminfo.spieler_anz == 0)
    rauminfo.entfernung_naechster_spieler = -1;
  return rauminfo;
} // Spielraster::RaumInfo Spielraster::rauminfo(int const spieler) const

/**
 ** -> Rückgabe
 ** 
 ** @param     spieler       Nummer des Spielers
 ** @param     bewegungsrichtung   Richtung des Raumes
 **
 ** @return    Informationen über den Raum des Spielers in Richtung „bewegungsrichtung”
 **
 ** @version   2014-11-11
 **
 ** @todo      Durch kopieren und modifizieren der Algorithmen Raster::raumgroesse / Raster::kuerzeste_entfernung lässt sich die Performance steigern. Dies lohnt hier aber vermutlich nicht, also wird der Code hier lieber simpel gehalten.
 **/
Spielraster::RaumInfo
Spielraster::rauminfo(int const spieler,
                      Bewegungsrichtung const bewegungsrichtung) const
{
  RaumInfo rauminfo;
  rauminfo.groesse = this->raumgroesse(this->position(spieler) + bewegungsrichtung, true);
  rauminfo.spieler_anz = 0;
  rauminfo.entfernung_naechster_spieler = this->breite() * this->laenge() + 1;
  for (int b = 0; b < this->spieler_anz(); ++b) {
    if (b == spieler)
      continue;
    auto const n = this->kuerzeste_entfernung(this->position(spieler),
                                             this->position(b));
    if (n >= 0) {
      rauminfo.spieler_anz += 1;
      rauminfo.entfernung_naechster_spieler = std::min(rauminfo.entfernung_naechster_spieler, n);
    }
  } // for (int b = 0; b < this->spieler_anz(); ++b)
  if (rauminfo.spieler_anz == 0)
    rauminfo.entfernung_naechster_spieler = -1;
  return rauminfo;
} // Spielraster::RaumInfo Spielraster::rauminfo(int const spieler, Bewegungsrichtung const bewegungsrichtung) const

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
  auto r = this->spieler_weg_[0].size();
  for (auto const& w : this->spieler_weg_)
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
  for (auto const& w : sr.spieler_weg_) {
    if (static_cast<int>(w.size()) >= runde + 2)
      for (auto f = (begin(w) + runde + 1); f != end(w); ++f)
        sr.belege(*f, false);
  }
  // Weg der Spieler beschneiden
  for (auto& w : sr.spieler_weg_)
    w.resize(std::min(static_cast<int>(w.size()), runde + 1));

  return sr;
} // int Spielraster::runde() const

/**
 ** -> Rückgabe
 ** 
 ** @param     spieler       Nummer des Spielers
 **
 ** @return    Groesse des Einflussbereichs
 **
 ** @version   2014-11-22
 **/
int 
Spielraster::einflussbereich_groesse(int const spieler) const
{
  return this->einflussbereich(spieler).felder_belegt();
} // int Spielraster::einflussbereich_groesse(int spieler) const

/**
 ** -> Rückgabe
 ** 
 ** @param     spieler   Nummer des Spielers
 ** @param     br    Richtung, für die geprüft wird
 **
 ** @return    Groesse des Einflussbereichs in dem Feld Richtung br
 **
 ** @version   2014-11-22
 **/
int 
Spielraster::einflussbereich_groesse(int const spieler, Bewegungsrichtung const br) const
{
  return this->einflussbereich(spieler, br).felder_belegt();
} // int Spielraster::einflussbereich_groesse(int spieler, Bewegungsrichtung const br) const

/**
 ** -> Rückgabe
 ** Ermittelt den Einflussbereich eines Spieler
 ** 
 ** @param     spieler       Nummer des Spielers
 **
 ** @return    Einflussbereich des Spielers
 **
 ** @version   2014-11-22
 **/
Raster 
Spielraster::einflussbereich(int const spieler) const
{
  return einflussbereich(spieler, this->position(spieler));
} // Raster Spielraster::einflussbereich(int const spieler) const

/**
 ** -> Rückgabe
 ** Ermittelt den Einflussbereich eines Spieler
 ** 
 ** @param     spieler       Nummer des Spielers
 ** @param     br    Richtung, für die geprüft wird
 **
 ** @return    Einflussbereich des Spielers
 **
 ** @version   2014-11-22
 **/
Raster 
Spielraster::einflussbereich(int const spieler, Bewegungsrichtung const br) const
{
  return einflussbereich(spieler, this->position(spieler) + br);
} // Raster Spielraster::einflussbereich(int spieler, Bewegungsrichtung br) const

/**
 ** -> Rückgabe
 ** Ermittelt den Einflussbereich eines Spieler
 ** 
 ** @param     spieler       Nummer des Spielers
 ** @param     position  Position des Spielers
 **
 ** @return    Einflussbereich des Spielers
 **
 ** @version   2014-11-22
 **/
Raster 
Spielraster::einflussbereich(int const spieler, Position const position) const
{
  if (  (*this)(position)
      && (position != this->position(spieler)) )
    return Raster{this->breite(), this->laenge()};
  auto raster = static_cast<Raster>(*this);
  auto raster_spieler = Raster{this->breite(), this->laenge()};

  std::set<Position> offene_positionen_spieler; // offene Positionen des Spielers
  std::set<Position> offene_positionen_sonst; // offene Positionen der anderen Spieler
  for (int b = 0; b < this->spieler_anz(); ++b) {
    auto const p = this->position(b);
    if (!p)
      continue;
    if (b == spieler)
      offene_positionen_spieler.insert(position);
    else
      offene_positionen_sonst.insert(p);
  } // for (b)

  std::set<Position> offene_positionen_spieler_neu; // neue offene Positionen des Spielers
  std::set<Position> offene_positionen_sonst_neu; // neue offene Positionen der anderen Spieler
  while (   !offene_positionen_spieler.empty()
         || !offene_positionen_sonst.empty()) {
    offene_positionen_spieler_neu.clear();
    offene_positionen_sonst_neu.clear();
    // Erst den Einflussbereich der anderen Spieler erweitern, anschließend den eigenen
    for (auto p : offene_positionen_sonst) {
      for (auto r : ::richtungen) {
        if (!raster(p + r)) {
          raster.belege(p + r);
          offene_positionen_sonst_neu.insert(p + r);
        }
      }
    } // for (p)
    for (auto p : offene_positionen_spieler) {
      for (auto r : ::richtungen) {
        if (!raster(p + r)) {
          raster.belege(p + r);
          raster_spieler.belege(p + r);
          offene_positionen_spieler_neu.insert(p + r);
        }
      }
    } // for (p)
    offene_positionen_spieler = offene_positionen_spieler_neu;
    offene_positionen_sonst = offene_positionen_sonst_neu;
  } // while (offene_positionen_spieler || offene_positionen_sonst)

  return raster_spieler;
} // Raster Spielraster::einflussbereich(int spieler, Position position) const

/**
 ** -> Rückgabe
 ** Ermittelt, wie viele Felder vom Einflussbereich durch einen Durchlauf erreicht werden können.
 ** 
 ** @param     spieler       Nummer des Spielers
 **
 ** @return    Anzahl der Felder, die der Spieler aus seinem Einflussbereich und nur über den Einflussbereich erreichen kann
 **
 ** @version   2014-11-28
 **/
int 
Spielraster::einflussbereich_groesse_erreichbar(int const spieler) const
{
  Raster r = *this;
  r.belege(this->einflussbereich(spieler).invertiert());
  return r.raumgroesse_erreichbar(this->position(spieler));
} // int Spielraster::einflussbereich_groesse_erreichbar(int spieler) const
