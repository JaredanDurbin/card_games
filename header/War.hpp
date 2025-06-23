#ifndef WAR_HPP
#define WAR_HPP

// ----------------------------------------------------------------------------
// Includes block.
// ----------------------------------------------------------------------------

// Standard Library includes.
#include <random>
#include <algorithm>

// Utility File includes.
// This is my own home grown utilities for this specific project.
#include "Ansi.h"

// Regular File includes.
// This is the header file for the implementation of a standard deck
// of cards class.
#include "StandardDeck.hpp"
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Class block.
// ----------------------------------------------------------------------------

/// @brief A class for playing the card game war.
class War
{
   private:
      // ----------------------------------------------------------------------
      // Private Variables block.
      // ----------------------------------------------------------------------

      // The hands for the game. Second is the player and 
      // first is the computer.
      mutable SplitDeck<StandardPlayingCard> hands;
      // The type of game they will play. 'start' is manual.
      // 'auto' is an automatic game.
      mutable std::string game = "";
      // ----------------------------------------------------------------------

      // ----------------------------------------------------------------------
      // Private Functions block.
      // ----------------------------------------------------------------------

      /// @brief Deal all the cards.
      void Deal()
      {
         // Get a deck of cards.
         StandardDeck<StandardPlayingCard> deck;
         // Shuffle the deck.
         deck.RandomizeDeck();
         // Split it into the hands.
         hands = deck.Split(26);
      }

      /// @brief Introduce the game.
      /// @return The type of game they will play. 'start' is manual.
      ///         'auto' is an automatic game.
      std::string Introduction()
      {
         std::cout << "This is the card game WAR!\n";
         std::string input = "";
         do
         {
            std::cout << "Type \"start\" to start the game or \"auto\" to automatically play.\n";
            std::cin >> input;
         } 
         while (input != "start" && input != "auto");

         return input;
      }

      /// @brief Move the cards from the loser to the winner.
      /// @param cards_input The cards that are going to the loser.
      /// @param winner The winner's hand.
      void MoveCards(Pile<StandardPlayingCard> &cards_input,
                     Pile<StandardPlayingCard> &winner)
      {
         // Randomize how the cards got into the hand.
         std::random_device rd;
         std::mt19937 rng(rd());
         std::shuffle(cards_input.begin(), cards_input.end(), rng);
         
         // Add the cards to the hand.
         auto iter = winner.begin();
         while (!cards_input.empty())
         {
            iter = winner.insert(iter,std::move(cards_input.back()));
            cards_input.pop_back();
         }
      } // MoveCards

      /// @brief If a war occurs 
      /// @param previous_cards 
      void GoToWar(Pile<StandardPlayingCard> &previous_cards)
      {
         for (int i = 0; i < 3; i++)
         {
            if (hands.first.size() > 1 &&
                hands.second.size() > 1)
            {
               previous_cards.push_back(std::move(hands.first.back()));
               previous_cards.push_back(std::move(hands.second.back()));
               hands.first.pop_back();
               hands.second.pop_back();
            }
         }
      } // GoToWar

      /// @brief Get the backs of the amount of cards we need.
      /// @param amount The amount of backs of cards we need.
      /// @return The backs of cards graphics.
      std::vector<std::string> MakeBacks(const size_t amount)
      {
         // Initialize the output variable.
         std::vector<std::string> out = {};
         // We will be adding to a line so initialize it here.
         std::string line("");
         // Get the backs but only the first 5 columns.
         for (size_t i = 0; i < CardGraphicsAndInfo::back.size(); i++)
         {
            for (size_t j = 0; j < amount; j++)
            {
               line += CardGraphicsAndInfo::back[i].substr(5);
            }
            out.push_back(line);
            line.clear();
         }

         return out;
      }

      /// @brief Output the graphics and also play the game.
      /// @param tie_input Tells if we tied or not and need to output
      ///                  different graphics.
      void OutputGraphics(const bool tie_input)
      {
         // Clear the screen and go to the corner.
         std::cout << Ansi_t::clrscr();
         std::cout << Ansi_t::gotoRC(0,0);

         // Get the top card on both players hands.
         std::vector<std::string> opp_card = hands.first.back()->GetGraphic();
         std::vector<std::string> pla_card = hands.second.back()->GetGraphic();

         // Did we tie before this?
         if (!tie_input)
         {
            // No. Display the opponents card.
            for (size_t i = 0; i < opp_card.size() - 1; i++)
            {
               std::cout << opp_card[i] << "\n";
            }
            // Also output how many cards they have left.
            std::cout << opp_card.back() << "   Opponent's Deck: " 
                      << hands.first.size() << "\n";
            // Display the player's card.
            for (size_t i = 0; i < pla_card.size() - 1; i++)
            {
               std::cout << pla_card[i] << "\n";
            }
            // Display how many cards the user has left.
            std::cout << pla_card.back() << "   Player's Deck: " 
                      << hands.second.size() << "\n\n";
         }
         else
         {
            // Yes. We are going to war and need to display more stuff.

            // Get the amount of cards in the computer's hand.
            size_t amount = hands.first.size();
            // If the amount of cards in the player's hand is less than
            // use that amount.
            if (amount > hands.second.size())
            {
               amount = hands.second.size();
            }

            // We need to get the backs of some cards.
            // Initialize the string vector here.
            std::vector<std::string> backs = {};
            // At most we need 3 cards but if the amount is less then
            // we use that.
            if (amount >= 3)
            {
               backs = MakeBacks(3);
            }
            else 
            {
               backs = MakeBacks(amount);
            }

            // Display the opponents card.
            for (size_t i = 0; i < opp_card.size() - 1; i++)
            {
               std::cout << opp_card[i] << backs[i] << "\n";
            }
            // Also output how many cards they have left.
            std::cout << opp_card.back() << backs.back() 
                      << "   Opponent's Deck: " 
                      << hands.first.size() << "\n";
            // Display the player's card.
            for (size_t i = 0; i < pla_card.size() - 1; i++)
            {
               std::cout << pla_card[i] << backs[i] << "\n";
            }
            // Display how many cards the user has left.
            std::cout << pla_card.back() << backs.back() 
                      << "   Player's Deck: " 
                      << hands.second.size() << "\n\n";
         }
      } // OutputGraphics

      /// @brief Play a round of war.
      void PlayARound()
      {
         // Initialize the "tie" variable. 
         bool tie = false;
         // The cards that have been played.
         Pile<StandardPlayingCard> stack;
         // Loop until tie is broken.
         do
         {
            // This outputs the graphics and show the user what is happening.
            OutputGraphics(tie);

            // Determine if the player won. 
            if ((hands.first.back()->GetRank() < 
                    hands.second.back()->GetRank() &&
                 hands.first.back()->GetRank() != 1) ||
                (hands.second.back()->GetRank() == 1 && 
                 hands.first.back()->GetRank() != 1))
            {
               // If they did then move the cards to the stack.
               std::cout << "You won this round!\n\n";
               stack.push_back(std::move(hands.first.back()));
               stack.push_back(std::move(hands.second.back()));
               // Pop the memory slots.
               hands.first.pop_back();
               hands.second.pop_back();
               // Move the cards to the players hand.
               MoveCards(stack,hands.second);
               // There was no tie and we can leave the loop.
               tie = false;
               break;
            }
            else if ((hands.first.back()->GetRank() > 
                        hands.second.back()->GetRank()) || 
                     (hands.first.back()->GetRank() == 1 && 
                      hands.second.back()->GetRank() != 1))
            {
               // Else the computer won. Move the cards to the stack.
               std::cout << "You opponent won this round!\n\n";
               stack.push_back(std::move(hands.first.back()));
               stack.push_back(std::move(hands.second.back()));
               // Pop the memory slots.
               hands.first.pop_back();
               hands.second.pop_back();
               // Move the cards to the computer hand.
               MoveCards(stack,hands.first);
               // There was no tie and we can leave the loop.
               tie = false;
               break;
            }
            else
            {
               // There was a tie. I don't know if this can happen.
               if (hands.first.size() == 0 ||
                   hands.second.size() == 0)
               {
                  std::cout << "The game ended on a WAR!" << std::endl;
                  tie = false;
                  break;
               }

               // Get an input and tell them they are going to WAR.
               std::string in = "";
               std::cout << "Let's go to war!!" << std::endl << std::endl;
               if (game == "start")
               {
                  std::cin >> in;
                  std::cout << std::endl << std::endl;
               }
               // Push back the cards to the stack for later.
               stack.push_back(std::move(hands.first.back()));
               stack.push_back(std::move(hands.second.back()));
               // Pop the memory slots.
               hands.first.pop_back();
               hands.second.pop_back();
               // Go to war and say it was a tie so they keep going.
               GoToWar(stack);
               tie = true;
            }
         } 
         while (tie);
      } // PlayARound

      /// @brief Play a manual game of war.
      void PlayAUserGame()
      {
         // Initialize the "users_input" variable.
         std::string user_input = "";
         // While they both have cards keep playing.
         while (hands.first.size() > 0 &&
                hands.second.size() > 0 && 
                (user_input != "stop" &&
                 user_input != "s"))
         {
            // Prompt them to draw.
            do
            {
               std::cout << "Enter whether to \"draw\" a card or \"stop\" the game: ";
               std::cin >> user_input;
               std::cout << "\n\n";
            } 
            while ((user_input != "draw" &&
                    user_input != "d") &&
                   (user_input != "stop" &&
                    user_input != "s"));
            
            // If they want to cntinue then play a round.
            if (user_input == "draw" ||
                user_input == "d")
            {
               PlayARound();
            }
            else
            {
               break;
            }
         }
      }

      /// @brief This plays a game automatically.
      void PlayAAutoGame()
      {
         size_t count = 0;
         // Stop it at some point so we don't go infinite.
         // (Don't know if it is possible to go infinite but maybe.)
         while (hands.first.size() > 0 &&
                hands.second.size() > 0 &&
                count < 1000)
         {
            PlayARound();
            count++;
         }
      }
      // ----------------------------------------------------------------------
   
   public:
      // ----------------------------------------------------------------------
      // Public Functions block.
      // ----------------------------------------------------------------------

      /// @brief This function plays the actual game and is all that is needed.
      void PlayGame()
      {
         // Introduce the game and get what type of game they want to play.
         game = Introduction();
         // Deal the cards. Does not matter if it is an manual or auto game.
         Deal();

         // Play the game they want.
         if (game == "start")
         {
            PlayAUserGame();
         }
         else if (game == "auto")
         {
            PlayAAutoGame();
         }

         if (hands.first.size() == 0)
         {
            std::cout << "YOU WON!!" << std::endl;
         }

         if (hands.second.size() == 0)
         {
            std::cout << "YOU LOST!!" << std::endl;
         }
         
         std::cout << "Thanks for playing!\n";
      }
   // ----------------------------------------------------------------------
}; // War
// -------------------------------------------------------------------------

#endif