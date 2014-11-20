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

#include "constants.h"

#include "spielraster/spielraster.h"
#include "bot/bot.h"
#include "ui/ui.h"

#include <string>
using std::string;
#include <sstream>
#include <unistd.h>

std::ostream* cdebug_;

void main_wettbewerb();
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
#if 1 // debug
cdebug_ = &cerr;
#else // no debug
cdebug_ = new std::ostringstream;
#endif

#if 0
main_wettbewerb();
#else
main_eigenes_spiel(argc, argv);
#endif

return 0;
} // int main()

/**
 ** Hauptfunktion für ein eigenes Spiel (mit eigenen Bots)
 **
 ** @param     argc   Anzahl der Argumente (-> GUI)
 ** @param     argv   Argumente (-> GUI)
 **
 ** @return    -
 **
 ** @version   2014-11-18
 **
 ** @todo      Die Kollisionsabfrage zählt die Runden nicht genau (treffen sich zwei Bots schafft der erste einen Schritt mehr), so reicht es aber zum Testen aus.
 **/
void
main_eigenes_spiel(int& argc, char* argv[])
{
  Spielraster spielraster; // das Spielraster
  spielraster.einlesen(cin);

  // UI erzeugen
  //auto ui = UI::create("none", argc, argv);
  auto ui = UI::create("cout", argc, argv);
  //auto ui = UI::create("cerr", argc, argv);
  //auto ui = UI::create("gtkmm", argc, argv);

  { // Positionen der Bots lesen
    while (cin.peek() && cin.good()) {
      spielraster.setze_bot(cin);
#if 0
      break; // Test: nur ein Bot
#endif
      while (isspace(cin.peek()))
        cin.get();
    } // while (cin.good())
  }
  vector<unique_ptr<Bot>> bots; // Die Bots
  // Bots erzeugen
  for (int i = 0; i < spielraster.bot_anz(); ++i) {
    auto bot = make_unique<Bot>(spielraster);
    bot->setze_nummer(i);
    bot->setze_strategie(Strategie::create({"in größten Raum", "Raum ausfüllen"}));
    //bot->setze_strategie(Strategie::create({"Raum ausfüllen (Tiefensuche)"}));
    bots.push_back(std::move(bot));
  }

  // Spielen
  int runde = 0;
  auto naechster_schritt = vector<Bewegungsrichtung>(spielraster.bot_anz());
  ui->spiel_startet(spielraster);
  while (spielraster.bots_im_spiel()) {
    runde += 1;
    ui->runde(runde);
    // Schritte abfragen
    for (int b = 0; b < spielraster.bot_anz(); ++b) {
      if (spielraster.position(b)) {
        naechster_schritt[b] = bots[b]->bewegung();
      }
    }

    // Schritte gehen
    for (int b = 0; b < spielraster.bot_anz(); ++b) {
      if (!spielraster.position(b))
        continue;
      auto bp = spielraster.position(b) + naechster_schritt[b];
      if (spielraster(bp)) {
        for (int i = 0; i < b; ++i) {
          if (spielraster.position(i) == bp) {
            spielraster.entferne_bot(i);
            spielraster.bewege_bot(b, naechster_schritt[b]);
          }
        }
        spielraster.entferne_bot(b);
      } else {
        spielraster.setze_bot(b, bp);
      }
    }
  } // while (spielraster.bots_im_spiel())

  ui->spiel_endet();

  return ;
} // void main_eigenes_spiel(int argc, char* argv[])


/**
 ** Hauptroutine für den Wettbewerb
 **
 ** @param     -
 **
 ** @return    -
 **
 ** @version   2014-11-08
 **/
void
main_wettbewerb()
{
  Spielraster spielraster;
  Bot bot(spielraster);
  //bot.setze_strategie(Strategie::create("Vorwärts"));
  //bot.setze_strategie(Strategie::create("Linksherum"));
  //bot.setze_strategie(Strategie::create({"vorwärts 0.1", "links 0.5"}));
  bot.setze_strategie(Strategie::create({"in größten Raum", "Raum ausfüllen"}));
  //bot.setze_strategie(Strategie::create({"in größten Raum"}));

  string zeile;
  while (cin.good()) {
    std::getline(cin, zeile);
    //cdebug << "> " << zeile << '\n';
    std::istringstream istr(zeile);

    if (zeile.compare( 0, 15, "GAMEBOARDSTART ") == 0) {
      // GAMEBOARDSTART N,M - Es wird ein neues Spielbrett mit X Spalten und Y Zeilen übertragen. Endet mit GAMEBOARDEND
      string text = zeile + "\n";
      do {
        std::getline(cin, zeile);
        text += zeile + "\n";
      } while (cin.good()
               && zeile != "GAMEBOARDEND") ;
      std::istringstream istr(text);
      spielraster.einlesen(istr);

    } else if (zeile.compare( 0, 4, "SET ") == 0) {
      // SET P - Eigene Spielernummer festlegen
      bot.setze_nummer(std::stoi(string(zeile, 4)) - 1);

#if 0
      if (zeile == "SET 1")
        bot.setze_strategie(Strategie::create("Vorwärts"));
      else if (zeile == "SET 2")
        bot.setze_strategie(Strategie::create("Linksherum"));
#endif

    } else if (zeile.compare( 0, 4, "POS ") == 0) {
      // POS P N,M DIR - Position N,M des Bots P auf dem Spielbrett festlegen, wobei der Bot in Richtung DIR = (NORTH|EAST|SOUTH|WEST) schaut.
      spielraster.setze_bot(istr);

    } else if (zeile.compare( 0, 6, "ROUND ") == 0) {
      // ROUND R - Runde R beginnt.  Das Spielbrett beginnt links oben bei (1,1).  Nach diesem Befehl erwartet der Server eine Antwort vom Client.

#if 0
      //if (cdebug_ == &cerr) usleep(100*1000);
      cdebug << "___\n";
      cdebug << "Entfernung: " << spielraster.kuerzeste_entfernung(spielraster.position(0), spielraster.position(1)) << '\n';
      cdebug << spielraster;
#endif
      if (spielraster.bot_im_spiel(bot.nummer()))
        cout << bot.bewegung() << '\n';

    } else if (zeile.compare( 0, 4, "OUT ") == 0) {
      // OUT P - Spieler P ist ausgeschieden.

      spielraster.entferne_bot(std::stoi(string(zeile, 4)) - 1);
    } else if (zeile == "END") {
      // END - Das Spiel ist zu Ende.
      break;

    } else {
      cerr << "Unbekannte Befehlszeile:\n" << zeile << '\n';
      break;
    } // if (zeile == …)
  } // while (true)

  //usleep(20*1000);
  //cdebug << "___\n" << spielraster << endl;
  //usleep(20*1000);

  return ;
} // void main_wettbewerb()


/*
   Kommandos vom Bot an den Server:

   RIGHT  – Dreht den Bot um 90 Grad nach rechts.
   LEFT   – Dreht den Bot um 90 Grad nach links.
   AHEAD  – Lässt die Ausrichtung des Bots, wie sie ist.
   */

