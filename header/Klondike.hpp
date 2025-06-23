#ifndef KLONDIKE_HPP
#define KLONDIKE_HPP

// ----------------------------------------------------------------------------
// Includes block.
// ----------------------------------------------------------------------------

// Utility File includes.
// This is my own home grown utilities for this specific project.
#include "Ansi.h"
#include "Util.h"

// Regular File includes.
// This is the header file for the "Solitaire" namespace and class.
#include "Solitaire.hpp"
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Namespace block.
// ----------------------------------------------------------------------------

/// @brief Adding a game to the solitaire family of games.
namespace Solitaire
{
// ----------------------------------------------------------------------------
// Class block.
// ----------------------------------------------------------------------------

/// @brief This is the class implementation for a Klondike solitaire game.
class Klondike: public Solitaire<PolarStandardPlayingCard>
{
   private:
      // ----------------------------------------------------------------------
      // Private Variables block.
      // ----------------------------------------------------------------------

      // Variables for the game.
      // This variable basically tells use whether we will draw one card or 
      // three cards.
      mutable bool draw_one_game = false;
      // ----------------------------------------------------------------------

      // ----------------------------------------------------------------------
      // Private Functions block.
      // ----------------------------------------------------------------------

      /// @brief This function checks the pile number entered to make sure it
      ///        is in bounds.
      /// @param pile_index The pile index the user has entered.
      /// @return Returns a bool. True if the number is between 1 and 7
      ///         (inclusive) and false otherwise.
      const bool CheckPileBounds(const size_t pile_index) const
      {
         return pile_index >= 1 && pile_index <=7; 
      }

      /// @brief This function checks whether the card we are moving to
      ///        the stack can actually go on the stack.
      /// @param stack_suit The suit of the stack we are moving the card to.
      /// @return Returns a bool. True if the rank of the card in the stack
      ///         is one less than the one we want to move. False otherwise.
      const bool CheckCardToStack(const size_t stack_suit) const
      {
         return (stacks[stack_suit].empty() && 
                   free.back()->GetRank() == 1) ||
                (!stacks[stack_suit].empty() &&
                 stacks[stack_suit].back()->GetRank() == 
                   free.back()->GetRank() - 1);
      }
      // ----------------------------------------------------------------------

   protected:
      // ----------------------------------------------------------------------
      // Protected Functions block.
      // ----------------------------------------------------------------------

      // Deal the cards.
      void Deal()
      {
         // First randomize the deck as it starts as an "out of the box" deck.
         deck.RandomizeDeck();

         // Initialize the stacks.
         for (size_t i = 0; i < 4; i++)
         {
            Pile<PolarStandardPlayingCard> temp = 
               Pile<PolarStandardPlayingCard>();
            stacks.push_back(std::move(temp));
         }
         
         // Now we will deal the cards.
         for (size_t i = 0; i < 7; i++)
         {
            Pile<PolarStandardPlayingCard> temp;

            for (size_t j = 0; j < i + 1; j++)
            {
               // If it is the first one we need to initialize.
               if (i != j)
               {
                  temp.push_back(std::move(deck.DrawOne()));
               }
               else 
               {
                  temp.push_back(std::move(deck.DrawOne()));
                  temp.back()->FlipCard();
               }
            }

            board.push_back(std::move(temp));
         }
      } // Deal

      /// @brief This function will automatically move cards from the free
      ///        slots to the stacks.
      /// @return Returns a bool. True if we moved a card to the stacks.
      ///         Returns false otherwise.
      bool AutoMoveFree() 
      {
         // Get how many cards are in the free slots.
         size_t size = free.size();
         // Moves free slot cards to the stack if they be moved,
         // and if cards were moved return true.
         if (!free.empty())
         {
            MoveFreeToStack(free.back()->GetSuit(), true);
            return free.size() != size;
         }
         // Otherwise as stated we return false.
         return false;
      }

      /// @brief This function moves cards from the board to another board
      ///        slot.
      /// @param pile_from The index of the pile we are moving the cards from. 
      /// @param pile_to The index we are moving the cards to.
      /// @param amount The amount of cards we are moving between the piles.
      void MoveBoardToBoard(const size_t pile_from, 
                            const size_t pile_to,
                            const size_t amount = 1)
      {
         // Check if we can move the cards.
         // Check the bounds of both indices.
         // Check if the pile and amount are in descending order.
         // Check to make sure the pile we are moving cards from is not empty.
         // Check to make sure the amount is greater than one.
         // Make sure the cards are face up.
         // Then check the rank.
         // Then check the suit colors.
         if (CheckPileBounds(pile_from) &&
             CheckPileBounds(pile_to) &&
             CheckDescendingPile(pile_from, amount) &&
             !board[pile_from - 1].empty() &&
             1 <= amount &&
             (board[pile_from - 1][board[pile_from - 1].size() - amount]
               ->GetFaceUp()) &&
             (((!board[pile_to - 1].empty()) &&
               (board[pile_to - 1].back()->GetRank() == 
                board[pile_from - 1][board[pile_from - 1].size() 
                  - amount]->GetRank() + 1) &&
               ((board[pile_from - 1][board[pile_from - 1].size() - amount]
               ->GetColor() != board[pile_to - 1].back()->GetColor()))) ||
             ((board[pile_to - 1].empty()) &&
               (board[pile_from - 1][board[pile_from - 1].size() - amount]
                     ->GetFaceUp()) &&
                  (board[pile_from - 1][board[pile_from - 1].size() 
                     - amount]->GetRank() == 13))))
         {
            // Define the temp variable.
            Pile<PolarStandardPlayingCard> temp;
            // Reserve the amount.
            temp.reserve(amount);

            // Move the cards to 'temp' and pop the blank memory slot.
            for (size_t i = amount; 0 < i; i--)
            {
               temp.push_back(std::move(board[pile_from - 1].back()));
               board[pile_from - 1].pop_back();
            }
            // Then push it to the new pile and pop the blank memory slots.
            for (size_t i = amount; 0 < i; i--)
            {
               board[pile_to - 1].push_back(std::move(temp.back()));
               temp.pop_back();
            }

            // If there is a flipped card then flip it.
            if (!board[pile_from - 1].empty() &&
               !board[pile_from - 1].back()->GetFaceUp())
            {
               board[pile_from - 1].back()->FlipCard();
            }
         }
         else
         {
            // Else output some error data.
            std::cout << "Make sure the piles selected are in range.\n"
                      << "Make sure the amount is positive.\n"
                      << "Make sure you are moving the "
                      << "right amount of cards!\n\n";
            std::cin.get();
         }
      } // MoveBoardToBoard

      /// @brief This function moves cards from the 
      ///        free ones to the board slot.
      /// @param pile_to The board slot index we are moving the card to.
      void MoveFreeToBoard(const size_t pile_to)
      {
         // Check to see if the pile we are moving to is not empty.
         // Check the bounds of the pile index.
         // Make sure the rank on the board is one higher.
         // Make sure the cards are of different colors.
         // Make sure both cards are face up.
         if ((!board[pile_to - 1].empty()) &&
             (1 <= pile_to && pile_to <= 7) &&
             (board[pile_to - 1].back()->GetRank() == 
                free.back()->GetRank() + 1) &&
             (free.back()->GetColor() != board[pile_to - 1].back()->GetColor()) &&
             (board[pile_to - 1].back()->GetFaceUp()) &&
             (free.back()->GetFaceUp()))
         {
            // Move the card and then pop the memory slot.
            board[pile_to - 1].push_back(std::move(free.back()));
            free.pop_back();
         }
         else if (board[pile_to - 1].empty() &&
                  free.back()->GetRank() == 13)
         {
            // Here we do something else for the kings.
            board[pile_to - 1].push_back(std::move(free.back()));
            free.pop_back();
         }
         else
         {
            // Some error text if necessary.
            std::cout << "Make sure the pile selected is in range.\n";
         }
      } // MoveFreeToBoard

      /// @brief This function moves the free cards to the stack.
      /// @param stack The stack suit we would like to move the card to.
      /// @param aut If this was called from the automatic function.
      void MoveFreeToStack(const std::string stack,
                           bool              aut)
      {
         // This just gives the index of the stack vector.
         size_t stack_suit = GetStackSuit(stack);

         // Make sure the free card is face up.
         // Make sure the suit matches the stack suit.
         // Check if we can even move that card to the stack.
         if ((free.back()->GetFaceUp()) &&
             (free.back()->GetSuit() == stack) &&
             (CheckCardToStack(stack_suit)))
         {
            // If we passed move the card and pop the memory slot.
            stacks[stack_suit].push_back(std::move(free.back()));
            free.pop_back();
         }
         else if (!aut)
         {
            // If we are not trying to auto fill then print an error message.
            std::cout << "Make sure the stack and card match suits.\n"
                      << "Make sure the rank is one higher!\n\n";
            std::cin.get();         
         }
      } // MoveFreeToStack

      /// @brief This function moves cards from the board to the stack.
      /// @param pile_from The index of the pile we are moving cards from.
      /// @param stack The stack suit we will be moving to.
      /// @param aut If this function was called by the auto function.
      void MoveBoardToStack(const size_t      pile_from, 
                            const std::string stack,
                            bool              aut)
      {
         // This just gives the index of the stack vector.
         size_t stack_suit = GetStackSuit(stack);

         // First check if the suits match.
         // Then check the ranks. If the stack is empty check we are adding
         // an ace otherwise check we are adding a card of one rank higher.
         if ((board[pile_from - 1].back()->GetSuit() == stack) &&
             ((stacks[stack_suit].empty() && 
               board[pile_from - 1].back()->GetRank() == 1) ||
              (!stacks[stack_suit].empty() &&
               stacks[stack_suit].back()->GetRank() == 
                  board[pile_from - 1].back()->GetRank() - 1)))
         {
            // If we passed move the card and pop the memory slot.
            stacks[stack_suit].push_back(std::move(board[pile_from - 1].back()));
            board[pile_from - 1].pop_back();

            // Then flip the next card if it is face down.
            if (!board[pile_from - 1].empty() && 
               !board[pile_from - 1].back()->GetFaceUp())
            {
               board[pile_from - 1].back()->FlipCard();
            }
         }
         else if (!aut)
         {
            // If we are not trying to auto fill then print an error message.
            std::cout << "Make sure the stack and card match suits.\n"
                      << "Make sure the rank is one higher!\n\n";
            std::cin.get();
         }
      } // MoveBoardToStack

      /// @brief This function will move stack cards to the board.
      /// @param pile_to This is the board slot index we would like to
      ///                move the card to.
      /// @param stack This is the stack we are moving the card from.
      void MoveStackToBoard(const size_t      pile_to, 
                            const std::string stack)
      {
         // This just gives the index of the stack vector.
         size_t stack_suit = GetStackSuit(stack);

         // First check that both places are not empty.
         // Then check the ranks and then the colors.
         if ((!stacks[stack_suit].empty() && !board[pile_to - 1].empty()) &&
            (board[pile_to - 1].back()->GetRank() == 
               stacks[stack_suit].back()->GetRank() + 1) &&
            (board[pile_to - 1].back()->GetColor() != 
               stacks[stack_suit].back()->GetColor()))
         {
            // If we passed move the card and pop the memory slot.
            board[pile_to - 1].push_back(std::move(stacks[stack_suit].back()));
            stacks[stack_suit].pop_back();
         }
         else
         {
            // Some error text if necessary.
            std::cout << "Make sure the stack and card match suits.\n"
                      << "Make sure the rank is one higher!\n\n";
            std::cin.get();
         }
      } // MoveBoardToStack

      /// @brief This function will draw cards from the deck to
      ///        the free slots.
      void DrawCards()
      {
         // The amount we want to draw. This will be 3 unless it is a
         // "draw_one_game" then it will get changed later.
         size_t draw_amount = 3;
         // If the amount left in the deck is less than 3 then set the 
         // "draw_amount" to the amount left in the deck.
         if (deck.GetDeck().size() < 3 && deck.GetDeck().size() > 0)
         {
            draw_amount = deck.GetDeck().size();
         }
         else if (deck.GetDeck().size() == 0)
         {
            // Now if there are no more cards in the deck then we
            // need to replenish it. This temp variable will help us
            // move the cards.
            //
            // NOTE: This will trigger even if a draw one game is played.
            Pile<PolarStandardPlayingCard> temp;

            // While the free cards are not empty push them back
            // and flip them back over.
            while (!free.empty())
            {
               // Move the card and pop the memory slot.
               temp.push_back(std::move(free.back()));
               free.pop_back();

               // Flip the card back over.
               if (temp.back()->GetFaceUp())
               {
                  temp.back()->FlipCard();
               }
            }
            
            // Then set the deck at the end from the temp variable.
            deck.SetDeck(std::move(temp));
         }

         // If we have a draw one game then set the draw amount to 1.
         if (draw_one_game)
         {
            draw_amount = 1;
         }

         // Finally if the deck is not empty...
         if (!deck.GetDeck().empty())
         {
            // Draw the amount of cards we determined.
            for (int i = 0; i < draw_amount; i++)
            {
               // Draw the card.
               free.push_back(std::move(deck.DrawOne()));

               // If the card is face down (it should be) then flip it.
               if (!free.back()->GetFaceUp())
               {
                  free.back()->FlipCard();
               }
            }
         }
      } // DrawCards

      /// @brief Prompt the user for input. Pretty simple 
      ///        but this is complicated.
      /// @param inputs This is the inputs that will be used in a context
      ///               outside this which it is why it is passed by reference.
      void GetInput(std::vector<std::string>& inputs)
      {
         // This is the inputs which we will set the 'inputs' variable later. 
         std::string input1, input2, input3, input4;

         // This first prompt asks what the user would like to do.
         do
         {
            std::cout << "Please enter your input.\n"
                      << "Type \"draw\" to draw a card, "
                      << "\"auto\" to auto fill the stacks, "
                      << "\"move\" to move cards,\n"
                      << " \"stop\" to reset your inputs, "
                      << "or \"exit\" to quit/give-up. " ;
            std::cin >> input1;
         } 
         while (input1 != "draw" && input1 != "d" && 
                input1 != "auto" && input1 != "a" && 
                input1 != "move" && input1 != "m" &&
                input1 != "exit" && input1 != "e" &&
                input1 != "stop" && input1 != "s");

         // If the input was anything but move then we don't need
         // anymore.
         if (input1 == "draw" || input1 == "d" ||
             input1 == "auto" || input1 == "a" ||
             input1 == "exit" || input1 == "e" ||
             input1 == "stop" || input1 == "s")
         {
            input2 = "";
            input3 = "";
            input4 = "";
         }
         else 
         {
            // Otherwise we need to figure out where they want to
            // move the cards from.
            do
            {
               std::cout << "Please enter where you would"
                        << " like to move cards from.\n"
                        << "Type \"free\", \"stack\", a number 1 to 7,"
                        << " \"stop\" to reset your inputs, "
                        << "or \"exit\" to quit/give-up. " ;
               std::cin >> input2;
            } 
            while (input2 != "free" && input2 != "stack" && 
                   input2 != "1" && input2 != "2" && input2 != "3" &&
                   input2 != "4" && input2 != "5" && input2 != "6" &&
                   input2 != "7" && input2 != "exit" && input2 != "e" &&
                   input2 != "stop" && input2 != "s" && input2 != "f");

            // If it was 'free' the they can move it to the stack or board.
            if (input2 == "free" || input2 == "f")
            {
               do
               {
                  std::cout << "Please enter where you would"
                           << " like to move cards to.\n"
                           << "Type \"stack\", a number 1 to 7,"
                           << " \"stop\" to reset your inputs, "
                           << "or \"exit\" to quit/give-up. " ;
                  std::cin >> input3;
               } 
               while (input3 != "stack" && input3 != "7" &&
                      input3 != "1" && input3 != "2" && input3 != "3" &&
                      input3 != "4" && input3 != "5" && input3 != "6" &&
                      input3 != "exit" && input3 != "e" &&
                      input3 != "stop" && input3 != "s");
               
               // Don't need the last 'input'.
               input4 = "";
            }
            else if (input2 == "stack")
            {
               // If it was stack then we need the suit.
               do
               {
                  std::cout << "Please enter where you would"
                           << " like to move cards to.\n"
                           << "Type the suit of the stack you "
                           << "would like to move from, "
                           << "\"stop\" to reset your inputs, "
                           << "or \"exit\" to quit/give-up. " ;
                  std::cin >> input3;
               } 
               while (input3 != "spade" && input3 != "heart" &&
                      input3 != "club" && input3 != "diamond" &&
                      input3 != "exit" && input3 != "e" &&
                      input3 != "stop" && input3 != "s");

               if (input3 == "exit" || input3 == "e" ||
                   input3 == "stop" || input3 == "s")
               {
                  // If they mess up we stop and don't need the last 'input'.
                  input4 = "";
               }
               else
               {
                  // Lastly they need to enter where the card goes. Basically
                  // the board slot.
                  do
                  {
                     std::cout << "Please enter where you would"
                              << " like to move cards to.\n"
                              << "Type a number 1 to 7,"
                              << " \"stop\" to reset your inputs, "
                              << "or \"exit\" to quit/give-up. " ;
                     std::cin >> input4;
                  } 
                  while (input4 != "7" &&
                         input4 != "1" && input4 != "2" && input4 != "3" &&
                         input4 != "4" && input4 != "5" && input4 != "6" &&
                         input4 != "exit" && input4 != "e" &&
                         input4 != "stop" && input4 != "s");
               }
            }
            else if (!(input2 != "7" &&
                       input2 != "1" && input2 != "2" && input2 != "3" &&
                       input2 != "4" && input2 != "5" && input2 != "6"))
            {
               // If they entered a board slot. The we need to figure out
               // where they want it to go.
               do
               {
                  std::cout << "Please enter where you would"
                           << " like to move cards to.\n"
                           << "Type \"stack\" or a number 1 to 7,"
                           << " \"stop\" to reset your inputs, "
                           << "or \"exit\" to quit/give-up. " ;
                  std::cin >> input3;
               } 
               while (input3 != "7" && input3 == input2 && input3 != "stack" &&
                      input3 != "1" && input3 != "2" && input3 != "3" &&
                      input3 != "4" && input3 != "5" && input3 != "6" &&
                      input3 != "exit" && input3 != "e" &&
                      input3 != "stop" && input3 != "s");

               if (input3 == "stack" || input3 == "exit" || input3 == "e" ||
                   input3 == "stop" || input3 == "s")
               {
                  // If they mess up we stop and don't need the last 'input'.
                  input4 = "";
               }
               else if (input3 == "7" ||
                        input3 == "1" || input3 == "2" || input3 == "3" ||
                        input3 == "4" || input3 == "5" || input3 == "6")
               {
                  // Get the input and find the pile.
                  size_t pile = Util::CStrToInt(input2);
                  size_t amount = -1;
                  // Get the amount they would like to move. Make sure we can
                  // Move that many cards.
                  do
                  {
                     std::cout << "Please enter the amount of cards you would"
                              << " like to move. ";
                     std::cin >> input4;
                     amount = Util::CStrToInt(input4);

                     // DEBUG
                     //std::cout << "1? " << (amount <= 0) << "\n";
                     //std::cout << "2? " << (board[pile - 1].size() < amount) 
                     //          << "\n";
                  } 
                  while ((amount <= 0 || board[pile - 1].size() < amount));
               }
            }
         }
         
         // Lastly set the 'inputs' variable.
         inputs = {input1, input2, input3, input4};
      } // GetInput

      /// @brief This function is for executing the actions from the 
      ///        inputs given.
      /// @param inputs This the inputs we will execute actions for.
      void ExecuteAction(const std::vector<std::string> inputs)
      {
         // These are the inputs broken up.
         std::string input1 = inputs[0]; 
         std::string input2 = inputs[1];
         std::string input3 = inputs[2];
         std::string input4 = inputs[3];

         // This just goes through the inputs and calls the correct function.
         if (input1 == "draw" || input1 == "d")
         {
            DrawCards();
         }
         else if (input1 == "auto" || input1 == "a")
         {
            AutoMove();
         }
         else if (input1 == "move" || input1 == "m")
         {
            if (input2 == "free" || input2 == "f")
            {
               if (input3 == "stack" && !free.empty())
               {
                  MoveFreeToStack(free.back()->GetSuit(), false);
               }
               else if (!free.empty())
               {
                  size_t pile = Util::CStrToInt(input3);
                  MoveFreeToBoard(pile);
               }
               else
               {
                  std::cout << "Make sure there are free cards to move!\n";
                  std::cin.get();
               }
            }
            else if (input2 == "stack")
            {
               size_t stack_suit = GetStackSuit(input3);
               if (!stacks[stack_suit].empty())
               {
                  size_t pile = Util::CStrToInt(input4);
                  MoveStackToBoard(pile,input3);
               }
               else
               {
                  std::cout << "Make sure there are stack cards to move!\n";
                  std::cin.get();
               }
            }
            else
            {
               size_t pile1 = Util::CStrToInt(input2);
               if (input3 == "stack" && !board[pile1 - 1].empty())
               {
                  MoveBoardToStack(pile1, 
                                   board[pile1 - 1].back()->GetSuit(),
                                   false);
               }
               else if (!board[pile1 - 1].empty())
               {
                  size_t pile2 = Util::CStrToInt(input3);
                  size_t amount = Util::CStrToInt(input4);
                  MoveBoardToBoard(pile1, pile2, amount);
               }
               else 
               {
                  std::cout << "Make sure there are board cards to move!\n";
                  std::cin.get();
               }
            }
         }
      } // ExecuteAction

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
      // Graphics!!
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

      /// @brief Since the free cards are important and dynamic it needs its
      ///        own function to get it's graphics.
      /// @return Returns a vector of strings which is the graphics
      ///         for this part.
      std::vector<std::string> FreeCardsGraphic()
      {
         // Get the "out" variable ready.
         std::vector<std::string> out;

         // Now get the graphics for the cards that have been 
         // free.
         if (!free.empty() && free.back()->GetFaceUp())
         {
            // Get the first card.
            out = free.back()->GetGraphic();

            // If there are two free cards get the next one.
            if (free.size() > 1 && free[free.size() - 2]->GetFaceUp())
            {
               for (size_t i = 0; i < out.size(); i++)
               {
                  std::string graphic = free[free.size() - 2]->GetGraphic().at(i);
                  out[i].insert(0, Util::UTF8SubStr(graphic, 4));
               }
            }

            // If there is a third card add it's graphic too.
            if (free.size() > 2 && free[free.size() - 3]->GetFaceUp())
            {
               for (size_t i = 0; i < out.size(); i++)
               {
                  std::string graphic = free[free.size() - 3]->GetGraphic().at(i);
                  out[i].insert(0, Util::UTF8SubStr(graphic, 4));
               }
            }

            // Lastly add spaces till we make it to the stacks.
            size_t spaces = 30 - out[0].length();
            for (size_t i = 0; i < out.size(); i++)
            {
               for (size_t j = 0; j < spaces; j++)
               {
                  out[i].append(" ");
               }
            }
         }
         else
         {
            // Else just add the spaces.
            for (int i = 0; i < 9; i++)
            {
               out.push_back("                              ");
            }
         }

         // Return the graphic.
         return out;
      } // FreeCardsGraphic

      /// @brief This function gets the top row which is particular to the
      ///        game being played. 
      /// @return Returns the string vector of the graphics all from the top
      ///         row of cards.
      std::vector<std::string> TopRowGraphic()
      {
         // Initialize output.
         std::vector<std::string> out;

         // Get the deck space graphic.
         if (deck.GetDeck().empty())
         {
            out = CardGraphicsAndInfo::empty;
         }
         else
         {
            out = CardGraphicsAndInfo::back;
         }

         // Add two spaces.
         for (size_t i = 0; i < out.size(); i++)
         {
            out[i].append("  ");
         }

         // Add the free card graphics.
         std::vector<std::string> temp = FreeCardsGraphic();
         for (size_t i = 0; i < out.size(); i++)
         {
            out[i].append(temp[i]);
         }

         // Add the stack graphics.
         AddStackGraphics(out, 0);
         AddStackGraphics(out, 1);
         AddStackGraphics(out, 2);
         AddStackGraphics(out, 3);

         return out;
      } // TopRowGraphic
      // ----------------------------------------------------------------------

   public:
      // ----------------------------------------------------------------------
      // Public Function block.
      // ----------------------------------------------------------------------

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
      // Game!! Must be at the end since it has everything.
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
      
      /// @brief This function actually plays the game.
      void PlayGame()
      {
         // Introduction statement.
         std::cout << "This is Klondike Solitaire!!\n";

         // Then get what kind of game they want to play. 1 or 3 draw.
         std::string draws;
         do
         {
            std::cout << "Would you like to play a draw \"3\" game"
                     << " or a draw \"1\" game? ";
            std::cin >> draws;
         } 
         while (draws != "1" && draws != "3");
         
         // If they want a loser on-draw game then flip that flag.
         if (draws == "1")
         {
            draw_one_game = true;
         }

         // Whether to start the game or not.
         std::string start;
         do
         {
            std::cout << "Type \"start\" to start. ";
            std::cin >> start;
         } 
         while (start != "start");

         // Deal the cards.
         Deal();

         // This is the bread and butter.
         // First we print the screen then checck the win condition
         // then get the users input and execute the action they want.
         do
         {
            // Print the screen function.
            PrintScreen();
         }
         while (!WinCondition() && GetInputAndExecuteAction());

         // If the user won then print the winning statement.
         // else they lost :(.
         if (WinCondition())
         {
            std::cout << "YOU WIN!\n";
         }
         else 
         {
            std::cout << "YOU LOSE!!\n";
         }
      }
      // ----------------------------------------------------------------------
}; // Klondike
// ----------------------------------------------------------------------------
} // Solitaire
// ----------------------------------------------------------------------------

#endif
