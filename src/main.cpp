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

#include "spielraster/spielraster.h"
#include "spieler/bot.h"
#include "spieler/mensch.h"
#include "ui/ui.h"
#include "ui/gtkmm/spielraster.h"

#include <string>
using std::string;
#include <sstream>
#include <algorithm>
#include <unistd.h>

std::ostream* cdebug_ = new std::ostringstream;

#define ZEIT_MESSEN

#ifdef ZEIT_MESSEN
#include <chrono>
#endif

void main_wettbewerb(int& argc, char* argv[]);
void main_eigenes_spiel(int& argc, char* argv[]);

/**
 ** Hauptfunktion
 **
 ** @param     argc   Anzahl der Argumente (wird ignoriert)
 ** @param     argv   Argumente (wird ignoriert)
 **
 ** @return    0
 **
 ** @version   2014-10-25
 **/
int
main(int argc, char* argv[])
{
cdebug_ = &cerr;
//cdebug_ = &cout;

#ifdef USE_EIGENES_SPIEL
main_eigenes_spiel(argc, argv);
#else // Wettbewerb
cdebug_ = new std::ostringstream;
//cdebug_ = &cout;
main_wettbewerb(argc, argv);
#endif

return 0;
} // int main()

/**
 ** Hauptfunktion für ein eigenes Spiel (mit eigenen Spielern)
 **
 ** @param     argc   Anzahl der Argumente (-> GUI)
 ** @param     argv   Argumente (-> GUI)
 **
 ** @return    -
 **
 ** @version   2014-11-18
 **
 ** @todo      Die Kollisionsabfrage zählt die Runden nicht genau (treffen sich zwei Spieler schafft der erste einen Schritt mehr), so reicht es aber zum Testen aus.
 **/
void
main_eigenes_spiel(int& argc, char* argv[])
{
  Spielraster spielraster; // das Spielraster
  spielraster.einlesen(cin);

  // UI erzeugen
#ifdef USE_UI_GTKMM
  auto ui = UI::create("gtkmm", argc, argv);
#else
  //auto ui = UI::create("none", argc, argv);
  auto ui = UI::create("cout", argc, argv);
  //auto ui = UI::create("cerr", argc, argv);
#endif

  { // Positionen der Spieler lesen
    while (cin.peek() && cin.good()) {
      spielraster.setze_spieler(cin);
#if 0
      break; // Test: nur ein Spieler
#endif
      while (isspace(cin.peek()))
        cin.get();
    } // while (cin.good())
  }
  vector<unique_ptr<Spieler>> spieler; // Die Spieler
  // Spieler erzeugen
  for (int i = 0; i < spielraster.spieler_anz(); ++i) {
    unique_ptr<Spieler> s;
    switch (i) {
    case 0:
      s = std::move(make_unique<Mensch>(*ui));
      // Die möglichen Bots, in der Reihenfolge der Bewertung vom Programmierwettbewerb
      //s = Spieler::create("../freiesmagazin-2014-10-contest/bots/Knof/src/lichtgehen", "Knof");
      //s = Spieler::create("../freiesmagazin-2014-10-contest/bots/Burow/FalkBot", "Burow"); 
      //s = Spieler::create("../freiesmagazin-2014-10-contest/bots/Scharm/lichtgehen.sh", "Scharm"); // Fehler
      //s = Spieler::create("../freiesmagazin-2014-10-contest/bots/StaudingerP/run_bot.sh", "StaudingerP");
      //s = Spieler::create("../freiesmagazin-2014-10-contest/bots/Demel/bin/bot", "Demel");
      //s = Spieler::create("../freiesmagazin-2014-10-contest/bots/StaudingerC/bot.py", "StaudingerC");
      //s = Spieler::create("../freiesmagazin-2014-10-contest/bots/Schmidhuber/build/Bot.jar", "Schmidhuber"); // Fehler
      //s = Spieler::create("../freiesmagazin-2014-10-contest/bots/Maraun/bot", "Maraun");
      //s = Spieler::create("../freiesmagazin-2014-10-contest/bots/Bock/bot", "Bock");
      //s = Spieler::create("../freiesmagazin-2014-10-contest/bots/dummybot/bot", "dummybot");
      //s = Spieler::create("../freiesmagazin-2014-10-contest/bots/Wagenfuehr/bot", "Wagenfuehr");
      break;
    default:
      auto b = make_unique<Bot>();
      b->setze_strategie(Strategie::create({"Tiefensuche",
                                           "in größten Raum",
                                           "zum größten Einflussgebiet",
                                           "Gegner verfolgen",
                                           "vorwärts 0.7",
                                           "links 0.7",
                                           }));
      s = std::move(b);
      break;
    } // switch (i)
    s->setze_nummer(i);
    spieler.push_back(std::move(s));
  }

  // Spielen
  int runde = 0;
  auto naechster_schritt = vector<Bewegungsrichtung>(spielraster.spieler_anz());
  for (auto& s : spieler)
    s->spiel_startet(spielraster);
  ui->spiel_startet(spielraster);
  while (spielraster.spieler_im_spiel()) {
    for (auto& s : spieler)
      s->runde(runde);
    ui->runde(runde);

    cdebug << "Runde " << runde << '\n';

    runde += 1;
    //cdebug << spielraster << '\n';
    //usleep(10000);
    // Schritte abfragen
    for (int b = 0; b < spielraster.spieler_anz(); ++b) {
      if (spielraster.position(b)) {
        //cdebug << b << " erreichbare Raumgröße: " << spielraster.raumgroesse_erreichbar(spielraster.position(b)) << '\n';
#ifdef ZEIT_MESSEN
        auto const zeit_start = std::chrono::system_clock::now();
#endif
        naechster_schritt[b] = spieler[b]->bewegung();
#ifdef ZEIT_MESSEN
        std::chrono::duration<double> const dauer
          = std::chrono::system_clock::now() - zeit_start;
        if (dauer.count() > 0.9 * ZEITBESCHRAENKUNG) {
          cout << b << ": " << dauer.count() << " Sekunden\n";
        }
#endif

      }
    }
    //if (runde == 21)
    //break;

    // Schritte gehen
    auto pos_neu = vector<Position>(spielraster.spieler_anz());
    for (int b = 0; b < spielraster.spieler_anz(); ++b) {
      if (spielraster.position(b))
        pos_neu[b] = spielraster.position(b) + naechster_schritt[b];
    }
    for (int b = 0; b < spielraster.spieler_anz(); ++b) {
      if (!spielraster.position(b))
        continue;
      if (std::count(begin(pos_neu), end(pos_neu), pos_neu[b]) > 1) {
        spielraster.entferne_spieler(b);
        spielraster.belege(pos_neu[b]);
      } else {
        spielraster.bewege_spieler(b, naechster_schritt[b]);
      }
    } // for (b)
#if 0
    if (spielraster.spieler_im_spiel() == 1)
      break;
#endif
    //break;
  } // while (spielraster.spieler_im_spiel())

  UI_Gtkmm::speicher(spielraster, "Spielraster.pdf");
  UI_Gtkmm::speicher_verlauf(spielraster, "Spielraster_Verlauf.pdf");
//  UI_Gtkmm::speicher(spielraster, "Spielraster.png");

  for (auto& s : spieler)
    s->spiel_endet();
  ui->spiel_endet();

  return ;
} // void main_eigenes_spiel(int argc, char* argv[])


/**
 ** Hauptroutine für den Wettbewerb
 **
 ** @param     argc   Anzahl der Argumente (-> GUI)
 ** @param     argv   Argumente (-> GUI)
 **
 ** @return    -
 **
 ** @version   2014-11-08
 **/
void
main_wettbewerb(int& argc, char* argv[])
{
  Spielraster spielraster;
#ifdef USE_UI_GTKMM
  auto ui = UI::create("gtkmm", argc, argv);
#else
  auto ui = UI::create("none", argc, argv);
  //auto ui = UI::create("cout", argc, argv);
  //auto ui = UI::create("cerr", argc, argv);
#endif
  ui = UI::create("none", argc, argv);
#ifdef USE_EINGABE
  Mensch spieler(*ui);
#else
  Bot spieler;
  spieler.setze_strategie(Strategie::create({"Tiefensuche",
                                            "in größten Raum",
                                            "zum größten Einflussgebiet",
                                            "Gegner verfolgen",
                                            "vorwärts 0.7",
                                            "links 0.7",
                                            }));
#endif

  string zeile;
  while (cin.good()) {
    std::getline(cin, zeile);
    //cdebug << "> " << zeile << '\n';
    std::istringstream istr(zeile);

    if (zeile.compare( 0, 15, "GAMEBOARDSTART ") == 0) {
      // GAMEBOARDSTART N,M - Es wird ein neues Spielbrett mit X Spalten und Y Zeilen übertragen. Endet mit GAMEBOARDEND
      auto text = zeile + "\n";
      do {
        std::getline(cin, zeile);
        text += zeile + "\n";
      } while (cin.good()
               && zeile != "GAMEBOARDEND") ;
      std::istringstream istr(text);
      spielraster.einlesen(istr);
      spieler.spiel_startet(spielraster);

    } else if (zeile.compare( 0, 4, "SET ") == 0) {
      // SET P - Eigene Spielernummer festlegen
      spieler.setze_nummer(std::stoi(string(zeile, 4)) - 1);

    } else if (zeile.compare( 0, 4, "POS ") == 0) {
      // POS P N,M DIR - Position N,M des Spielers P auf dem Spielbrett festlegen, wobei der Spieler in Richtung DIR = (NORTH|EAST|SOUTH|WEST) schaut.
      spielraster.setze_spieler(istr);

    } else if (zeile.compare( 0, 6, "ROUND ") == 0) {
      // ROUND R - Runde R beginnt.  Das Spielbrett beginnt links oben bei (1,1).  Nach diesem Befehl erwartet der Server eine Antwort vom Client.

      if (spielraster.runde() == 1)
        ui->spiel_startet(spielraster);
      ui->runde(spielraster.runde());
      if (spielraster.spieler_im_spiel(spieler.nummer()))
        cout << spieler.bewegung() << '\n';

    } else if (zeile.compare( 0, 4, "OUT ") == 0) {
      // OUT P - Spieler P ist ausgeschieden.

      spielraster.entferne_spieler(std::stoi(string(zeile, 4)) - 1);
    } else if (zeile == "END") {
      // END - Das Spiel ist zu Ende.
      ui->spiel_endet();
      break;

    } else if (zeile.compare(0, 5, "(II) )")) {
      cout << zeile << endl;
    } else {
      cerr << "Unbekannte Befehlszeile:\n" << zeile << '\n';
      break;
    } // if (zeile == …)
  } // while (true)

  UI_Gtkmm::speicher(spielraster, "Spielraster.pdf");
  UI_Gtkmm::speicher_verlauf(spielraster, "Spielraster_Verlauf.pdf");
  //UI_Gtkmm::speicher(spielraster, "Spielraster.png");

  //usleep(20*1000);
  //cdebug << "___\n" << spielraster << endl;
  //usleep(20*1000);

  return ;
} // void main_wettbewerb()


/*
   Kommandos vom Spieler an den Server:

   RIGHT  – Dreht den Spieler um 90 Grad nach rechts.
   LEFT   – Dreht den Spieler um 90 Grad nach links.
   AHEAD  – Lässt die Ausrichtung des Spielers, wie sie ist.
   */

