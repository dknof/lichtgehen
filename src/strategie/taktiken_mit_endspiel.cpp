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

#include "taktiken_mit_endspiel.h"

namespace StrategieNS {

  /**
   ** Standardkonstruktor
   ** 
   ** @param     -
   **
   ** @return    -
   **
   ** @version   2014-11-11
   **/
  TaktikenMitEndspiel::TaktikenMitEndspiel() :
    Taktiken{}
  {
    this->setze_name("Taktiken mit Endspiel", "verschiedene Taktiken durchgehen, zum Spielende auf Endspieltaktiken wechseln");
  } // TaktikenMitEndspiel::TaktikenMitEndspiel()

  /**
   ** Standardkonstruktor mit einer Taktik
   ** 
   ** @param     name    Name der Taktik
   **
   ** @return    -
   **
   ** @version   2014-11-11
   **/
  TaktikenMitEndspiel::TaktikenMitEndspiel(string const name) :
    Taktiken{name}
  {
    this->setze_name("Taktiken mit Endspiel", "verschiedene Taktiken durchgehen, zum Spielende auf Endspieltaktiken wechseln");
  } // TaktikenMitEndspiel::TaktikenMitEndspiel(string name)

  /**
   ** Standardkonstruktor mit mehreren TaktikenMitEndspiel
   ** 
   ** @param     namen    Name der TaktikenMitEndspiel
   **
   ** @return    -
   **
   ** @version   2014-11-11
   **/
  TaktikenMitEndspiel::TaktikenMitEndspiel(std::initializer_list<string> name) :
    Taktiken{name}
  {
    this->setze_name("Taktiken mit Endspiel", "verschiedene Taktiken durchgehen, zum Spielende auf Endspieltaktiken wechseln");
  } // TaktikenMitEndspiel::TaktikenMitEndspiel(std::initializer_list<string> name)

  /**
   ** Standarddestruktor
   ** 
   ** @param     -
   **
   ** @return    -
   **
   ** @version   2014-11-11
   **/
  TaktikenMitEndspiel::~TaktikenMitEndspiel()
  { }

  /**
   ** Die TaktikenMitEndspiel auf ostr ausgeben
   ** 
   ** @param     ostr   Ausgabestrom
   **
   ** @return    Ausgabestrom
   **
   ** @version   2014-11-11
   **/
  ostream& 
    TaktikenMitEndspiel::ausgeben(ostream& ostr) const
    {
      this->Taktiken::ausgeben(ostr);
      if (this->spielstatus & SpielStatus::EIGENER_RAUM)
        ostr << "Eigener Raum\n";
      if (this->spielstatus & SpielStatus::LETZTE_FELDER)
        ostr << "Letzte Felder\n";
      return ostr;
    } // ostream& TaktikenMitEndspiel::ausgeben(ostream& ostr) const

  /**
   ** Endspiel prüfen
   ** Die Bewegungsrichtung nach den Taktiken ermitteln
   ** 
   ** @param     spielraster   das Spielraster
   ** @param     spieler_nummer  die Nummer des Spielers
   **
   ** @return    -
   **
   ** @version   2014-11-11
   **/
  Bewegungsrichtung
    TaktikenMitEndspiel::bewegung(Spielraster const& spielraster,
                                  int const spieler_nummer)
    {
      this->teste_spielstatus(spielraster, spieler_nummer);
      return this->Taktiken::bewegung(spielraster, spieler_nummer);
    } // Bewegungsrichtung TaktikenMitEndspiel::bewegung(Spielraster spielraster, int spieler_nummer)

  /**
   ** Teste die Änderung des Spielstatus
   ** 
   ** @param     -
   **
   ** @return    -
   **
   ** @version   2014-11-11
   **/
  void
    TaktikenMitEndspiel::teste_spielstatus(Spielraster const& spielraster,
                                           int const spieler_nummer)
    {
      auto neuer_status = this->spielstatus;

      auto const rauminfo = spielraster.rauminfo(spieler_nummer);

      // Teste, auf letzte Felder
      if (!(this->spielstatus & LETZTE_FELDER)) {
        if (rauminfo.groesse <= 2 + 5)
          neuer_status |= LETZTE_FELDER;
      } // if (!(this->spielstatus & LETZTE_FELDER))

      // Teste, auf eigenen Raum
      if (!(this->spielstatus & EIGENER_RAUM)) {
        if (rauminfo.spieler_anz == 0)
          neuer_status |= EIGENER_RAUM;
      } // if (!(this->spielstatus & EIGENER_RAUM))

      // Prüfe, ob sich der Status geändert hat
      if (neuer_status == this->spielstatus)
        return ;

      // Die Tiefensuche ist nur für zwei Spieler implementiert
      if (   (neuer_status == LETZTE_FELDER)
          && (rauminfo.spieler_anz > 1) )
        return ;

      this->taktiken.clear();
      this->hinzufuegen(Taktik::create("Tot"));
      this->hinzufuegen(Taktik::create("Einziger Weg"));
      //this->hinzufuegen(Taktik::create("in größten Raum"));
      switch (neuer_status) {
      case LETZTE_FELDER:
        this->hinzufuegen(Taktik::create("Tiefensuche"));
        break;
      case EIGENER_RAUM:
        this->hinzufuegen(Taktik::create("Raum ausfüllen (Tiefensuche)"));
        break;
      case LETZTE_FELDER | EIGENER_RAUM:
        //this->hinzufuegen(Taktik::create("Raum ausfüllen (Tiefensuche bis Ende)"));
        this->hinzufuegen(Taktik::create("Raum ausfüllen (Tiefensuche)"));
        break;
      default:
        // kann nicht auftreten
        break;
      } // switch (neuer_status)

      this->spielstatus = neuer_status;

      return ;
    } // void TaktikenMitEndspiel::teste_spielstatus(Spielraster spielraster, int spieler_nummer)

} // namespace StrategieNS
