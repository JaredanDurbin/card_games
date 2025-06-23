#ifndef FREECELL_HPP
#define FREECELL_HPP

// ----------------------------------------------------------------------------
// Includes block.
// ----------------------------------------------------------------------------

// Utility File includes.
// This is my own home grown utilities for this specific project.
#include "Util.h"

// Regular File includes.
// This is the header file for the "Solitaire" namespace and class.
#include "Solitaire.hpp"
// This is the header file for the "StandardDeck" class.
#include "StandardDeck.hpp"
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

/// @brief This is the class implementation for a Freecell solitaire game.
class Freecell: public Solitaire<StandardPlayingCard>
{
   private:
      // ----------------------------------------------------------------------
      // Private Functions block.
      // ----------------------------------------------------------------------

      /// @brief This function checks the pile number entered to make sure it
      ///        is in bounds.
      /// @param pile_index The pile index the user has entered.
      /// @return Returns a bool. True if the number is between 1 and 8
      ///         (inclusive) and false otherwise.
      const bool CheckPileBounds(const size_t pile_index) const
      {
         return pile_index >= 1 && pile_index <= 8; 
      }

      /// @brief This funtion checks to make sure card(s) from a pile can be 
      ///        moved to another pile.
      /// @param pile_from The pile index we are moving the card(s) from.
      /// @param pile_to The pile index we are moving the card(s) to.
      /// @param amount The amount of card(s) we are moving.
      /// @return Returns a bool. Looks at the top card from the amount
      ///         counted from the bottom. Returns true if that cards color
      ///         is the opposite of the card we are moving it onto and 
      ///         if the rank of the card we are moving that card to is of 
      ///         a rank one higher. False otherwise.
      const bool CheckCanMoveCardOnCard(const size_t pile_from, 
                                        const size_t pile_to,
                                        const size_t amount) const
      {
         return board[pile_from - 1][board[pile_from - 1].size() - amount]
                   ->GetColor() != board[pile_to - 1].back()->GetColor() &&
                board[pile_to - 1].back()->GetRank() == 
                   board[pile_from - 1][board[pile_from - 1].size() 
                   - amount]->GetRank() + 1;
      }

      /// @brief This function checks to make sure that the amount we are
      ///        moving is actually an amount we can move.
      /// @param pile_to The pile we are moving the card(s) to.
      /// @param amount The amount of card(s) we are moving.
      /// @return Returns a bool. True if the amount of free cells 
      ///         times the amount of empty board piles plus one is bigger than
      ///         the amount we want to move. False otherwise.
      const bool CheckMoveAmount(const size_t pile_to,
                                 const size_t amount) const
      {
         // First we count the amount of empty board piles.
         size_t empties = 0;
         for (size_t i = 0; i < board.size(); i++)
         {
            if (board[i].empty())
            {
               empties++;
            }
         }

         // If there are no empty board piles or we counted one and that
         // is the pile we are moving our cards to...
         if (empties == 0 ||
             (empties == 1 && board[pile_to - 1].empty()))
         {
            // Then return whether the amount is less than the number of 
            // free cells.
            // NOTE: Because these are unsigned they overflow to the max
            //       int when negative.
            return 100000 > 5 - free.size() - amount;
         }

         // Otherwise return whether the amount is less than the number of 
         // free cells times the number of empty board slots plus one.
         // NOTE: Because these are unsigned they overflow to the max
         //       int when negative.
         return 100000 > ((empties + 1) * (5 - free.size())) - amount;
      }

      /// @brief This function checks whether the card we are moving to
      ///        the stack can actually go on the stack.
      /// @param stack_suit The suit of the stack we are moving the card to.
      /// @param index The index of the pile we are moving the card from.
      /// @return Returns a bool. True if the rank of the card in the stack
      ///         is one less than the one we want to move. False otherwise.
      const bool CheckCardToStack(const size_t stack_suit,
                                  const size_t index) const
      {
         return (stacks[stack_suit].empty() && 
                   free[index - 1]->GetRank() == 1) ||
                (!stacks[stack_suit].empty() && 
                 stacks[stack_suit].back()->GetRank() == 
                   free[index - 1]->GetRank() - 1);
      }
      // ----------------------------------------------------------------------

   protected:
      // ----------------------------------------------------------------------
      // Protected Functions block.
      // ----------------------------------------------------------------------

      /// @brief This function will deal all the cards.
      ///        This is the first function call that should happen.
      void Deal()
      {
         // First randomize the deck as it starts as an "out of the box" deck.
         deck.RandomizeDeck();

         // Initialize the free piles.
         free = Pile<StandardPlayingCard>();
         // Initialize the stacks.
         for (size_t i = 0; i < 4; i++)
         {
            Pile<StandardPlayingCard> temp = 
               Pile<StandardPlayingCard>();
            stacks.push_back(std::move(temp));
         }

         // Get the size of the deck.
         size_t deck_size = deck.GetDeck().size();
         // Now start to actually deal the cards.
         for (size_t i = 0; i < deck_size; i++)
         {
            // For the first eight we need to initialize the board slots.
            // Else just push back a card.
            if (i < 8)
            {
               board.push_back({});
               board[i].push_back(std::move(deck.DrawOne()));
            }
            else
            {
               board[i % 8].push_back(std::move(deck.DrawOne()));
            }
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
         for (size_t i = 0; i < size + 1 && !free.empty(); i++)
         {
            MoveFreeToStack(free[i]->GetSuit(), i + 1, true);
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
         // Check to make sure the amount we are moving is allowed.
         // Check to see if the pile we are moving to is not the pile we are 
         // moving from.
         // If the pile we are moving to is not empty check to see if we can 
         // move the card onto that card.
         //
         // Else print some data to alert the user.
         if (CheckPileBounds(pile_from) &&
             CheckPileBounds(pile_to) &&
             CheckDescendingPile(pile_from, amount) &&
             !board[pile_from - 1].empty() &&
             amount >= 1 &&
             CheckMoveAmount(pile_to, amount) &&
             pile_to != pile_from &&
             (board[pile_to - 1].empty() ||
              CheckCanMoveCardOnCard(pile_from,
                                     pile_to,
                                     amount)))
         {
            // Define the temp variable.
            Pile<StandardPlayingCard> temp;
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
         }
         else
         {
            std::cout << "Make sure the piles selected are in range.\n"
                      << "Make sure the amount is positive.\n"
                      << "Make sure you are moving the "
                      << "right amount of cards!\n\n";
            std::cin.get();
         }
      } // MoveBoardToBoard

      /// @brief This function moves a card from the board to a stack slot.
      ///        It will also do error checking for if we can move the card.
      /// @param pile_from This is the index of the pile we will move 
      ///                  the car from.
      /// @param stack This is the stack that we will move the card to.
      /// @param aut This will just not output error messages if it is true.
      void MoveBoardToStack(const size_t      pile_from,
                            const std::string stack,
                            bool              aut)
      {
         // This just gives the index of the stack vector.
         size_t stack_suit = GetStackSuit(stack);

         // Check first if the card can be moved to that stack.
         // Check then if the stack is empty and the card is an ace.
         // Or check to see if the card has a rank one higher.
         if ((board[pile_from - 1].back()->GetSuit() == stack) &&
             ((stacks[stack_suit].empty() && 
               board[pile_from - 1].back()->GetRank() == 1) ||
              (!stacks[stack_suit].empty() &&
               stacks[stack_suit].back()->GetRank() == 
               board[pile_from - 1].back()->GetRank() - 1)))
         {
            // If we passed move the card to the stack and pop 
            // the empty memory slot on the board.
            stacks[stack_suit].push_back(
                               std::move(board[pile_from - 1].back()));
            board[pile_from - 1].pop_back();
         }
         else if (!aut)
         {
            // If we are not trying to auto fill then print an error message.
            std::cout << "Make sure the stack and card match suits.\n"
                      << "Make sure the rank is one higher!\n\n";
            std::cin.get();
         }
      } // MoveBoardToStack

      /// @brief This function moves cards from the board to free slots.
      /// @param pile_from The index of the board pile we want to move.
      void MoveBoardToFree(const size_t pile_from)
      {
         // First check to make sure that board pile is not empty.
         // Then make sure there are free slots left.
         if (!board[pile_from - 1].empty() &&
             free.size() < 4)
         {
            // If we passed move the card and pop the memory slot.
            free.push_back(std::move(board[pile_from - 1].back()));
            board[pile_from - 1].pop_back();
         }
         else
         {
            // If we failed then write some error messages.
            std::cout << "Make sure the there are free cells!\n\n";
            std::cin.get();
         }
      } // MoveBoardToFree

      /// @brief This function moves cards from the free slots to the board.
      /// @param pile_to This is the index of the board slot we will move the
      ///                card to.
      /// @param index The index of the free slot we would like to move.
      void MoveFreeToBoard(const size_t pile_to,
                           const size_t index)
      {
         // First make sure the free slots are not empty.
         // Then make sure the index chosen is one that is available to move.
         // Check to see if the board slot is empty,
         // or the rank is one less and the color is the opposite.
         if (!free.empty() &&
             1 <= index && index <= free.size() &&
             (board[pile_to - 1].empty() ||
              (free[index - 1]->GetColor() != 
                  board[pile_to - 1].back()->GetColor() &&
               free[index - 1]->GetRank() + 1 == 
                  board[pile_to - 1].back()->GetRank())))
         {
            // If we passed move the card and pop the memory slot.
            board[pile_to - 1].push_back(std::move(free[index - 1]));
            free.erase(free.begin() + (index - 1));
         }
         else
         {
            // If we failed print some error info.
            std::cout << "Make sure the the index is correct!\n\n";
            std::cin.get();
         }
      } // MoveFreeToBoard

      /// @brief This function will move cards from the free slots
      //         to the stack.
      /// @param stack This is the stack that we will move the card to.
      /// @param index The index of the free slot we would like to move.
      /// @param aut This will just not output error messages if it is true.
      void MoveFreeToStack(const std::string stack,
                           const size_t      index,
                           bool              aut)
      {
         // This just gives the index of the stack vector.
         size_t stack_suit = GetStackSuit(stack);

         // First make sure the free slots are not empty.
         // Then make sure the index chosen is one that is available to move.
         // Then make sure the suit and stack match.
         // Then check if we can actually move the card there.
         if (!free.empty() &&
             1 <= index && index <= free.size() &&
             free[index - 1]->GetSuit() == stack &&
             CheckCardToStack(stack_suit, index))
         {
            // If we passed move the card and pop the memory slot.
            stacks[stack_suit].push_back(std::move(free[index - 1]));
            free.erase(free.begin() + (index - 1));
         }
         else if (!aut)
         {
            // If we are not trying to auto fill then print an error message.
            std::cout << "Make sure the the index is correct!\n\n";
            std::cin.get();
         }
      } // MoveFreeToStack

      /// @brief This function moves cards from the 
      /// @param pile_to This is the index of the board slot we will move the
      ///                card to.
      /// @param stack This is the stack that we will move the card to.
      void MoveStackToBoard(const size_t      pile_to, 
                            const std::string stack)
      {
         // This just gives the index of the stack vector.
         size_t stack_suit = GetStackSuit(stack);

         // First make sure the stack we want to move from is not empty.
         // Make sure the board slot is not empty.
         // Then make sure the pile bounds are correct.
         // Then check tht the card can be moved on top of the target card.
         if (!stacks[stack_suit].empty() && 
             !board[pile_to - 1].empty() &&
             CheckPileBounds(pile_to) &&
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
            // If we failed print some error info.
            std::cout << "Make sure the stack and card match suits.\n"
                      << "Make sure the rank is one higher!\n\n";
            std::cin.get();
         }
      } // MoveStackToBoard

      /// @brief Move cards from the stacks to the free slots.
      /// @param stack This is the stack that we will move the card to.
      void MoveStackToFree(const std::string stack)
      {
         // This just gives the index of the stack vector.
         size_t stack_suit = GetStackSuit(stack);

         // Make sure the stack is not empty,
         // and make sure that there is a free slot too.
         if (!stacks[stack_suit].empty() &&
             free.size() < 4)
         {
            // If we passed move the card and pop the memory slot.
            free.push_back(std::move(stacks[stack_suit].back()));
            stacks[stack_suit].pop_back();
         }
         else
         {
            // If we failed print some error info.
            std::cout << "Make sure the stack and card match suits.\n";
            std::cin.get();
         }
      } // MoveStackToBoard

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
      // Graphics!!
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

      /// @brief Prompt the user for input. Pretty simple 
      ///        but this is complicated.
      /// @param inputs This is the inputs that will be used in a context
      ///               outside this which it is why it is passed by reference.
      void GetInput(std::vector<std::string>& inputs)
      {
         // This is the inputs which we will set the 'inputs' variable later. 
         std::string input1, input2, input3;

         // This first prompt asks where the user would like to move cards from.
         do
         {
            std::cout << "Please enter where you would"
                     << " like to move cards from.\n"
                     << "Type \"free\", \"stack\", a number 1 to 7,"
                     << " \"stop\" to reset your inputs, "
                     << "or \"exit\" to quit/give-up. " ;
            std::cin >> input1;
         } 
         while (input1 != "free" && input1 != "stack" && input1 != "8" &&
                input1 != "1" && input1 != "2" && input1 != "3" &&
                input1 != "4" && input1 != "5" && input1 != "6" &&
                input1 != "7" && input1 != "exit" && input1 != "e" &&
                input1 != "stop" && input1 != "s" &&
                input1 != "auto" && input1 != "a");
         
         // If it is "auto" then call the auto move functions.
         if (input1 == "auto" || input1 == "a")
         {
            AutoMove();
         }
         else if (input1 == "free")
         {
            // If the input is free then we need to get the index.
            do
            {
               std::cout << "Please enter what index you would"
                         << " like to move cards from.\n"
                         << "Type a free index,"
                         << " \"stop\" to reset your inputs, "
                         << "or \"exit\" to quit/give-up. " ;
               std::cin >> input2;
            } 
            while (input2 != "1" && input2 != "2" &&
                   input2 != "3" && input2 != "4" &&
                   input2 != "exit" && input2 != "e" &&
                   input2 != "stop" && input2 != "s");

            // Then where the user would like the card to go.
            do
            {
               std::cout << "Please enter where you would"
                         << " like to move cards to.\n"
                         << "Type \"stack\", a number 1 to 7,"
                         << " \"stop\" to reset your inputs, "
                         << "or \"exit\" to quit/give-up. " ;
               std::cin >> input3;
            } 
            while (input3 != "stack" && input3 != "7" && input3 != "8" &&
                   input3 != "1" && input3 != "2" && input3 != "3" &&
                   input3 != "4" && input3 != "5" && input3 != "6" &&
                   input3 != "exit" && input3 != "e" &&
                   input3 != "stop" && input3 != "s");
         }
         else if (input1 == "stack")
         {
            // If the user said "stack" then we need to know what suit stack
            // we will move cards from.
            do
               {
                  std::cout << "Please enter where you would"
                           << " like to move cards to.\n"
                           << "Type the suit of the stack you "
                           << "would like to move from, "
                           << "\"stop\" to reset your inputs, "
                           << "or \"exit\" to quit/give-up. " ;
                  std::cin >> input2;
               } 
               while (input2 != "spade" && input2 != "heart" &&
                      input2 != "club" && input2 != "diamond" &&
                      input2 != "exit" && input2 != "e" &&
                      input2 != "stop" && input2 != "s");

               if (input2 == "exit" || input2 == "e" ||
                   input2 == "stop" || input2 == "s")
               {
                  input3 = "";
               }
               else
               {
                  // Then we need the board slot to move the card to.
                  do
                  {
                     std::cout << "Please enter where you would"
                              << " like to move cards to.\n"
                              << "Type a number 1 to 7,"
                              << " \"stop\" to reset your inputs, "
                              << "or \"exit\" to quit/give-up. " ;
                     std::cin >> input3;
                  } 
                  while (input3 != "7" &&
                         input3 != "1" && input3 != "2" && input3 != "3" &&
                         input3 != "4" && input3 != "5" && input3 != "6" &&
                         input3 != "exit" && input3 != "e" &&
                         input3 != "stop" && input3 != "s");
               }
         }
         else if (!(input1 != "7" && input1 != "8" &&
                  input1 != "1" && input1 != "2" && input1 != "3" &&
                  input1 != "4" && input1 != "5" && input1 != "6"))
         {
            // If the user said the board slot they want to move cards from.
            do
            {
               std::cout << "Please enter where you would"
                         << " like to move cards to.\n"
                         << "Type \"stack\", \"free\", or a number 1 to 7,"
                         << " \"stop\" to reset your inputs, "
                         << "or \"exit\" to quit/give-up. " ;
               std::cin >> input2;
            } 
            while (input2 != "free" && input2 != "8" &&
                   input2 != "7" && input2 != "stack" &&
                   input2 != "1" && input2 != "2" && input2 != "3" &&
                   input2 != "4" && input2 != "5" && input2 != "6" &&
                   input2 != "exit" && input2 != "e" &&
                   input2 != "stop" && input2 != "s");

            // Next we need to figure out where they want the cards.
            // If it is stack or free we will figure things out.
            // Otherwise....
            if (input2 == "stack" || input2 == "free" ||
                input2 == "exit" || input2 == "e" ||
                input2 == "stop" || input2 == "s")
            {
               input3 = "";
            }
            else if (!(input1 != "7" && input1 != "8" &&
                     input1 != "1" && input1 != "2" && input1 != "3" &&
                     input1 != "4" && input1 != "5" && input1 != "6"))
            {
               // If it is another board slot then we need the amount to move.
               size_t pile = Util::CStrToInt(input1);
               size_t amount = -1;
               do
               {
                  std::cout << "Please enter the amount of cards you would"
                            << " like to move. ";
                  std::cin >> input3;
                  amount = Util::CStrToInt(input3);

                  // DEBUG
                  //std::cout << "1? " << (amount <= 0) << "\n";
                  //std::cout << "2? " << (board[pile - 1].size() < amount) 
                  //          << "\n";
               } 
               while ((amount <= 0 || board[pile - 1].size() < amount));
            }
         }
         
         // Lastly set the 'inputs' variable.
         inputs = {input1, input2, input3};
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

         // This just goes through the inputs and calls the correct function.
         if (input1 == "free")
         {
            if (input3 == "stack" && !free.empty())
            {
               size_t index = Util::CStrToInt(input2);
               MoveFreeToStack(free.back()->GetSuit(), index, false);
            }
            else if (!free.empty())
            {
               size_t index = Util::CStrToInt(input2);
               size_t pile = Util::CStrToInt(input3);
               MoveFreeToBoard(pile, index);
            }
            else
            {
               std::cout << "Make sure there are free cards to move!\n";
               std::cin.get();
            }
         }
         else if (input1 == "stack")
         {
            size_t stack_suit = GetStackSuit(input2);
            if (!stacks[stack_suit].empty())
            {
               size_t pile = Util::CStrToInt(input3);
               MoveStackToBoard(pile,input2);
            }
            else
            {
               std::cout << "Make sure there are stack cards to move!\n";
               std::cin.get();
            }
         }
         else
         {
            size_t pile1 = Util::CStrToInt(input1);
            if (input2 == "stack" && !board[pile1 - 1].empty())
            {
               MoveBoardToStack(pile1, 
                                board[pile1 - 1].back()->GetSuit(), 
                                false);
            }
            else if (input2 == "free" && !board[pile1 - 1].empty())
            {
               MoveBoardToFree(pile1);
            }
            else if (!board[pile1 - 1].empty())
            {
               size_t pile2 = Util::CStrToInt(input2);
               size_t amount = Util::CStrToInt(input3);
               MoveBoardToBoard(pile1, pile2, amount);
            }
            else 
            {
               std::cout << "Make sure there are board cards to move!\n";
               std::cin.get();
            }
         }
      } // ExecuteAction

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
      // Graphics!!
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

      /// @brief This function gets the top row which is particular to the
      ///        game being played. 
      /// @return Returns the string vector of the graphics all from the top
      ///         row of cards.
      std::vector<std::string> TopRowGraphic()
      {
         // Initialize output.
         std::vector<std::string> out;

         // Add the free card graphics.
         for (size_t i = 0; i < free.size(); i++)
         {
            // The first one we can just set 'out'.
            // Else we will append some space and then the card graphic.
            if (i == 0)
            {
               out = free[i]->GetGraphic();
            }
            else
            {
               for (size_t j = 0; j < out.size(); j++)
               {
                  out[j].append("  ");
                  out[j].append(free[i]->GetGraphic().at(j));
               }
            }
         }

         // If at this point 'out' is still empty we need
         // to initialize it.
         if (out.empty())
         {
            out = {"","","","","","","","",""};
         }

         // For the empty free slots we need to print the empty slot.
         for (size_t i = 4; i > free.size(); i--)
         {
            // Now start inputting the rows.
            for (size_t j = 0; j < out.size(); j++)
            {
               // First append a space.
               if (out[j] != "")
               {
                  out[j].append("  ");
               }

               // Then if it is the first or last the add the border.
               // Else do the internal border.
               if (j == 0 || j == out.size() - 1)
               { 
                  out[j].append("+-----------+");
               }
               else
               {
                  out[j].append("|           |");
               }
            }
         }

         // At the end we need to add another space.
         for (size_t j = 0; j < out.size(); j++)
         {
            out[j].append("  ");
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
         std::cout << "This is Freecell Solitaire!!\n";

         // Whether to start the game or not.
         std::string start;
         do
         {
            std::cout << "Type \"start\" to start. ";
            std::cin >> start;
         } 
         while (start != "start" && 
                start != "s");

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
}; // Freecell
// ----------------------------------------------------------------------------
} // Solitaire
// ----------------------------------------------------------------------------

#endif
