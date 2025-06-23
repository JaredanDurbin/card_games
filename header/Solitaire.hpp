#ifndef SOLITAIRE_HPP
#define SOLITAIRE_HPP

// ----------------------------------------------------------------------------
// Includes block.
// ----------------------------------------------------------------------------

// Some ANSI utility functions like clear the screen and go to the corner.
#include "Ansi.h"

// Regular File includes.
// This is the header file for the implementation of a standard deck
// of cards class.
#include "StandardDeck.hpp"
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Namespace block.
// ----------------------------------------------------------------------------

/// @brief This is the base or start of the Solitaire namespace.
namespace Solitaire
{
// ----------------------------------------------------------------------------
// Class block.
// ----------------------------------------------------------------------------

/// @brief This will be the start and base class to all solitaire games. This
///        is mostly a virtual class so things will probably need
//         to be rewritten.
/// @tparam T Some solitaire games need only standard playing cards and not
///           polar ones. So when classes inherit this one you need to
///           specify which one it uses.
template<class T> 
class Solitaire
{
   protected:
      // ----------------------------------------------------------------------
      // Protected Variables block.
      // ----------------------------------------------------------------------

      // The deck for the game. They should all use this besides maybe spider.
      mutable StandardDeck<T>      deck;
      // The free or drawn cards.
      mutable Pile<T>              free;
      // The board, where the games are played.
      mutable std::vector<Pile<T>> board;
      // The place where the cards are stacked by suit.
      mutable std::vector<Pile<T>> stacks;
      // ----------------------------------------------------------------------

      // ----------------------------------------------------------------------
      // Protected Functions block.
      // ----------------------------------------------------------------------

      /// @brief This function checks if the pile is in descending order.
      /// @param pile_from The pile we are checking.
      /// @param amount The amount of cards we are checking.
      /// @return Whether the pile and amount passes or not. True passes.
      ///         False otherwise.
      virtual bool CheckDescendingPile(const size_t pile_from,
                                       const size_t amount = 1)
      {
         // If the amount is 1 then just return true; nothing to check.
         if (amount == 1)
         {
            return true; 
         }

         // Get the size of the pile.
         size_t board_pile_size = board[pile_from - 1].size();
         // Get the last card in the amounts rank.
         size_t prev_rank = board[pile_from - 1][board_pile_size - amount]
                               ->GetRank();
         // Get the last cards color.
         std::string prev_color 
            = board[pile_from - 1][board_pile_size - amount]
            ->GetColor();
         // Now for the amount go through each card.
         for (size_t i = 1; i < amount; i++)
         {
            // Get the current rank and color.
            size_t curn_rank = 
               board[pile_from - 1][board_pile_size - amount + i]->GetRank();
            std::string curnColor = 
               board[pile_from - 1][board_pile_size - amount + i]->GetColor();
            
            // Then if the previous and current colors match or the ranks are
            // not descending then return false.
            if (curnColor == prev_color ||
                !(prev_rank == curn_rank + 1))
            {
               return false;
            }

            // Make the current rank and color the previous ones. 
            prev_rank = board[pile_from - 1][board_pile_size - amount + i]
                           ->GetRank();
            prev_color = board[pile_from - 1][board_pile_size - amount + i]
                           ->GetColor();
         }

         // If we go through all that return true.
         return true;
      } // CheckDescendingPile

      /// @brief This function will give the index of the stack as
      ///        defined by the variable "stack".
      /// @param stack The suit name.
      /// @return The index of the stack which matches the entered suit.
      size_t GetStackSuit(const std::string stack)
      {
         // Pretty self explanatory just a else if chain.
         if (stack == "spade")
         {
            return 0;
         }
         else if (stack == "heart")
         {
            return 1;
         }
         else if (stack == "club")
         {
            return 2;
         }
         else if (stack == "diamond")
         {
            return 3;
         }
         else
         {
            std::cerr << "Invalid stack input!\n";
         }

         return -1;
      }

      /// @brief An entirely virtual class this will need to be defined in
      ///        child classes. This function should deal all the cards.
      virtual void Deal() {}

      /// @brief This class will move automatically cards to the stacks from
      ///        the free cards. This should be defined on child classes.
      /// @return Return whether cards were moved or not. True if we did.
      ///         False otherwise.
      virtual bool AutoMoveFree() { return false; }

      /// @brief This class will move automatically cards to the stacks from
      ///        the board. This does not need to be defined on child classes.
      /// @return Return whether cards were moved or not. True if we did.
      ///         False otherwise.
      virtual bool AutoMoveBoard() 
      {
         // Start by defining the "out" variable and set it to 'false'.
         // Basically if we move a card we will trip true and then return that.
         // Otherwise it just stays false.
         bool out = false;
         // Go though each board slot.
         for (size_t i = 0; i < board.size(); i++)
         {
            // If it is not empty check it.
            if (!board[i].empty())
            {
               size_t size = board[i].size();
               // Check the bottom card.
               MoveBoardToStack(i + 1, 
                                board[i].back()->GetSuit(),
                                true);
               // Set the "out" variable.
               out = board[i].size() != size || out;
            }
         }
         // Return it here.
         return out;
      }

      /// @brief This is the automove function which will use the two
      ///        previous functions until no cards were moved by either.
      virtual void AutoMove()
      {
         do { } while (AutoMoveFree() || AutoMoveBoard());
         
      }

      /// @brief This function will move cards from a board slot
      ///        to another board slot. This should be defined on 
      ///        child classes.
      /// @param pile_from The board index the cards will be moved from.
      /// @param pile_to The board index the cards will be moved to.
      /// @param amount The amount of cards to be moved.
      virtual void MoveBoardToBoard(const size_t pile_from, 
                                    const size_t pile_to,
                                    const size_t amount = 1)
      {}


      /// @brief This function will move cards from a board slot
      ///        to the stacks. This should be defined on 
      ///        child classes.
      /// @param pile_from The board index the cards will be moved from.
      /// @param stack The stack suit to move the card to.
      /// @param aut Whether this is called by the automove function.
      virtual void MoveBoardToStack(const size_t      pile_from,
                                    const std::string stack,
                                    bool              aut)
      {}

      /// @brief This function will move cards from a free slot
      ///        to a board slot. This should be defined on 
      ///        child classes.
      /// @param pile_to The board index to move the card to.
      virtual void MoveFreeToBoard(const size_t pile_to) {}

      /// @brief This function will move cards from a free slot
      ///        to the stacks. This should be defined on 
      ///        child classes.
      /// @param stack The stack suit to move the card to.
      /// @param aut Whether this is called by the automove function.
      virtual void MoveFreeToStack(const std::string stack,
                                   bool              aut) 
      {}

      /// @brief This function will move cards from stack slot
      ///        to the board. This should be defined on 
      ///        child classes.
      /// @param pile_to The board index to move the card to.
      /// @param stack The stack suit to move the card to.
      virtual void MoveStackToBoard(const size_t      pile_to, 
                                    const std::string stack)
      {}

      /// @brief This function will get the users input and put them in
      ///        the inputs variable. That is why it is passed by reference.
      ///        This function is entirely virtual so it should be defined
      ///        on the child class.
      /// @param inputs The variable that the inputs will be put in.
      virtual void GetInput(std::vector<std::string>& inputs)
      {}

      /// @brief This function will take the inputs from the previous function
      ///        and will then execute the actions.
      ///        This function is entirely virtual so it should be defined
      ///        on the child class.
      /// @param inputs The inputs are here so use these to execute everything.
      virtual void ExecuteAction(const std::vector<std::string> inputs)
      {}

      /// @brief Get the players input and excute the input.
      /// @return Return whether the game should continue. True means it should
      ///         continue. False means stop.
      bool GetInputAndExecuteAction()
      {
         // Input string vector.
         std::vector<std::string> inputs;

         // Get the inputs. Passed by reference.
         GetInput(inputs);
         
         // If we have an 'e' or 'exit' stop the game and end.
         if (std::find(inputs.begin(),
                       inputs.end(), "e") != inputs.end() ||
             std::find(inputs.begin(),
                       inputs.end(), "exit") != inputs.end())
         {
            return false;
         }
         else if (std::find(inputs.begin(),
                            inputs.end(), "s") != inputs.end() ||
                  std::find(inputs.begin(),
                            inputs.end(), "stop") != inputs.end())
         {
            // If 'stop' or 's' then restart getting the inputs.
            return true;
         }
         else
         {
            // Execute actions if we made it here and return true to 
            // continue the game. (Unless the win condition is tripped.)
            ExecuteAction(inputs);
            return true;
         }

         // False return not to throw errors.
         return false;
      } // GetInputAndExecuteAction

      /// @brief This function gets the top row graphic. 
      ///        This function is entirely virtual so it should be defined
      ///        on the child class.
      /// @return Returns the string vector of the top row graphic.
      virtual std::vector<std::string> TopRowGraphic() { return {}; }

      /// @brief This function tracks if the game has been won.
      /// @return Returns whether the game is won. True if won. False
      ///         otherwise.
      bool WinCondition()
      {
         // Search all stacks.
         for (size_t i = 0; i < stacks.size(); i++)
         {
            // If the stacks have less than 13 cards then they have
            // not won so return false.
            if (stacks[i].size() < 13)
            {
               return false;
            }
         }

         // If we get here they have won so return true.
         return true;
      } // WinCondition

      /// @brief This function will add the stack graphics to the string
      ///        vector.
      /// @param original The original string vector with the graphics. 
      /// @param stack The stack suit.
      void AddStackGraphics(std::vector<std::string>& original,
                            size_t                    stack)
      {
         // Go through each row.
         for (size_t i = 0; i < original.size(); i++)
         {
            // If the stack is empty then put the empty graphic.
            if (stacks[stack].empty())
            {
               original[i].append(CardGraphicsAndInfo::empty[i]);
            }
            else
            {
               // Else just add the cards graphics.
               original[i].append(stacks[stack].back()->GetGraphic().at(i));
            }

            // Make sure to add a space.
            if (stack != stacks.size() - 1)
            {
               original[i].append("  ");
            }
         }
      } // AddStackGraphics

      
      /// @brief Get the string for graphics from the board at the row 
      ///        in the slot specified. 
      ///        If something is out of range it should return "".
      /// @param row The row of the pile to get the graphic from.
      /// @param pile The pile to get the graphic from.
      /// @return Return the string for that row.
      std::string GetBoardRowPile(const size_t row,
                                  const size_t pile)
      {
         // Initialize the "out" variable where we will put the string.
         std::string out;

         // The amount of cards in the pile.
         size_t cards = board[pile].size();

         // If the pile is not empty and the row is not of the last card...
         if (!board[pile].empty() && row < 3 * (cards - 1))
         {
            // Then show the top three rows of the card graphic.
            // Get the back if the card is face down.
            if (row % 3 == 0)
            {
               return CardGraphicsAndInfo::back[0];
            }
            else if (row % 3 == 1)
            {
               if constexpr(std::is_same_v<T, PolarStandardPlayingCard>)
               {
                  if (!board[pile][row / 3]->GetFaceUp())
                  {
                     return CardGraphicsAndInfo::back[1];
                  }
                  else 
                  {
                     return board[pile][row / 3]->GetGraphic().at(1);
                  }
               }
               else 
               {
                  return board[pile][row / 3]->GetGraphic().at(1);
               }
            }
            else if (row % 3 == 2)
            {
               if constexpr(std::is_same_v<T, PolarStandardPlayingCard>)
               {
                  if (!board[pile][row / 3]->GetFaceUp())
                  {
                     return CardGraphicsAndInfo::back[2];
                  }
                  else 
                  {
                     return board[pile][row / 3]->GetGraphic().at(2);
                  }
               }
               else 
               {
                  return board[pile][row / 3]->GetGraphic().at(2);
               }
            }
         }
         else if ((row >= (3 * (cards - 1))) &&
                  (row < ((3 * (cards - 1)) + 9)) &&
                  (!board[pile].empty()))
         {
            // Otherwise we just get the card graphic.
            size_t index = row - (3 * (board[pile].size() - 1));
            return board[pile].back()->GetGraphic().at(index);
         }

         // If we failed miserably return nothing.
         return "";
      } // GetBoardRowPile

      /// @brief This function will display the screen given to it.
      /// @param screen The vector of strings which is the string.
      void DisplayScreen(const std::vector<std::string> screen)
      {
         // Clear the screen.
         std::cout << Ansi_t::clrscr();
         // Go to the corner.
         std::cout << Ansi_t::gotoRC(0,0);

         // Go through each row and and display the screen string.
         for (size_t i = 0; i < screen.size(); i++)
         {
            std::cout << screen[i] << "\n";
         }
      }

      /// @brief This function prints the screen.
      void PrintScreen()
      {
         // The screen to be printed.
         std::vector<std::string> screen = TopRowGraphic();

         // DEBUG
         //DisplayScreen(screen);

         // Make labels for the different board piles.
         std::string labels("");
         for (size_t i = 0; i < board.size(); i++)
         {
            labels.append("      " + std::to_string(i + 1) + "      ");
            if (i != board.size() - 1)
            {
               labels.append("  ");
            }
         }
         screen.push_back(labels);
         
         // Create the lines for the board graphics.
         // First get the max cards in the stacks.
         size_t max_cards = 0;
         for (size_t i = 0; i < board.size(); i++)
         {
            if (max_cards < board[i].size())
            {
               max_cards = board[i].size();
            }
         }

         // How many lines we will need to output.
         size_t lines = (3 * max_cards) + 9;
         // Get the graphics for the board.
         for (size_t i = 0; i < lines; i++)
         {
            std::string temp1;

            for (size_t j = 0; j < board.size(); j++)
            {
               std::string temp2 = GetBoardRowPile(i, j);

               if ((board[j].empty() && i < 9) &&
                  (temp2 == ""))
               {
                  temp1.append(CardGraphicsAndInfo::empty[i]);
               }
               else if ((board[j].empty() && i >= 9) ||
                        (temp2 == ""))
               {
                  temp1.append("             ");
               }
               else
               {
                  temp1.append(temp2);
               }

               if (j < board.size() - 1)
               {
                  temp1.append("  ");
               }
            }

            screen.push_back(temp1);
            
            // DEBUG
            //DisplayScreen(screen);
         }

         DisplayScreen(screen);
      } // PrintScreen
      // ----------------------------------------------------------------------

   public:
      // ----------------------------------------------------------------------
      // Public Function block.
      // ----------------------------------------------------------------------
      
      /// @brief This function actually plays the game. This is an entirely
      ///        virtual function and should be defined by it's child classes.
      virtual void PlayGame() {};
      // ----------------------------------------------------------------------
}; // Solitaire
// ----------------------------------------------------------------------------
} // Solitaire
// ----------------------------------------------------------------------------

#endif
