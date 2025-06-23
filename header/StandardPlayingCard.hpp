#ifndef STANDARDPLAYINGCARD_HPP
#define STANDARDPLAYINGCARD_HPP

// ----------------------------------------------------------------------------
// Includes block.
// ----------------------------------------------------------------------------

// Standard Library includes.
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

// File includes.
// This adds a class which maked the variables so they can only be set once.
#include "SetOnce.h"

// This adds the static const variables used for the cards.
#include "StandardPlayingCardStaticVariables.h"
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Class block.
// ----------------------------------------------------------------------------

/// @brief This class implements everything that a standard playing card 
///        should need or want.
class StandardPlayingCard
{
   protected:
      // ----------------------------------------------------------------------
      // Protected Variables block.
      // ----------------------------------------------------------------------

      // These variables should only be set once. 
      // This is all the cosmetic and descriptive information.
      //
      // The graphic of the card.
      SetOnce<std::vector<std::string>> graphic;
      // The rank 1 through 13.
      SetOnce<int>         rank;
      // The suit spade, heart, club, or diamond.
      SetOnce<std::string> suit;
      // ----------------------------------------------------------------------

      // ----------------------------------------------------------------------
      // Protected Functions block.
      // ----------------------------------------------------------------------

      /// @brief Take the rank and suit and find the correct graphic for them.
      /// @param rank_input The rank of the card to be found.
      /// @param suit_input The suit of the card to be found.
      /// @return Return the string vector of the graphic.
      std::vector<std::string> FindGraphic(const int         rank_input, 
                                           const std::string suit_input)
      {
         // Initialize the indices.
         size_t index1 = -1;
         size_t index2 = -1;
         // Find the rank.
         for (size_t i = 0; i < CardGraphicsAndInfo::ranks.size(); i++)
         {
            if (rank_input == CardGraphicsAndInfo::ranks[i])
            {
               index1 = i;
               break;
            }
         }
         // find the suit.
         for (size_t i = 0; i < CardGraphicsAndInfo::ranks.size(); i++)
         {
            if (suit_input == CardGraphicsAndInfo::suits[i])
            {
               index2 = i;
               break;
            }
         }

         // We failed bad.
         if (index1 == -1 || index2 == -1) 
         {
            std::cerr << "Error: Invalid rank or suit input." << std::endl;
            return {"Unknown"}; // Or handle error as appropriate
         }
         
         // Return the graphic.
         return CardGraphicsAndInfo::graphics[index2][index1];
      } // FindGraphic
      // ----------------------------------------------------------------------

   public:
      // ----------------------------------------------------------------------
      // Constructors block.
      // ----------------------------------------------------------------------

      /// @brief Cards cannot be constructed normally so we delete the 
      ///        standard constructor.
      StandardPlayingCard() = delete;

      /// @brief This is the standard constructor.
      ///        Set the rank and suit then find and set the graphic.
      /// @param rank_input The rank of the card we want to make.
      /// @param suit_input The suit of the card we want to make.
      StandardPlayingCard(const int         rank_input, 
                          const std::string suit_input): 
         rank(rank_input),
         suit(suit_input)
      {
         // DEBUG
         //std::cout << "StandardPlayingCard Constructor." << std::endl;
         graphic = FindGraphic(rank_input, suit_input);
         // DEBUG
         //std::cout << graphic.Get()[1] << std::endl;
      }

      /// @brief The copy constructor.
      /// @param copy The instance we want to copy.
      StandardPlayingCard(const StandardPlayingCard &copy): 
         graphic(copy.graphic.Get()),
         rank(copy.rank.Get()),
         suit(copy.suit.Get())
      {};

      /// @brief The move constructor.
      /// @param copy The instance we want to move.
      StandardPlayingCard(StandardPlayingCard &&move): 
         graphic(std::move(move.graphic)),
         rank(std::move(move.rank)),
         suit(std::move(move.suit))
      {};

      /// @brief The standard destructor.
      ~StandardPlayingCard() {};
      // ----------------------------------------------------------------------

      // ----------------------------------------------------------------------
      // Operator overloads block.
      // ----------------------------------------------------------------------

      /// @brief Overloaded equals operator to see if two cards are the same.
      /// @param obj The object we want to test.
      /// @return Whether they are equal or not. True if they are. False
      ///         otherwise.
      bool operator==(const StandardPlayingCard &obj)
      {
         return rank.Get() == obj.rank.Get() && suit.Get() == obj.suit.Get();
      }

      /// @brief The copy operator overload.
      /// @param other The instance we want to copy.
      /// @return The pointer to this class.
      StandardPlayingCard& operator=(StandardPlayingCard &other)
      {
         if (this != &other)
         {
            graphic = other.graphic;
            rank = other.rank;
            suit = other.suit;
         }
         return *this;
      }

      /// @brief The move operator overload.
      /// @param other The instance we want to move.
      /// @return The pointer to this class.
      StandardPlayingCard& operator=(StandardPlayingCard &&other)
      {
         if (this != &other)
         {
            graphic = std::move(other.graphic);
            rank = std::move(other.rank);
            suit = std::move(other.suit);
         }
         return *this;
      }
      // ----------------------------------------------------------------------

      // ----------------------------------------------------------------------
      // Get block.
      // ----------------------------------------------------------------------

      /// @brief Get the graphic for displaying the card.
      /// @return Returns the graphic string vector.
      const std::vector<std::string> GetGraphic() const 
      { 
         return graphic.Get(); 
      }

      /// @brief Get the rank of the card.
      /// @return Return the rank.
      const int GetRank() const { return rank.Get(); }

      /// @brief Get the suit of the card.
      /// @return Return the suit.
      const std::string GetSuit() const { return suit.Get(); }

      /// @brief Get the color of the suit.
      /// @return Returns the color of the suit.
      const std::string GetColor() const 
      {
         if (suit.Get() == "heart" || suit.Get() == "diamond")
         {
            return "red";
         }
         
         return "black";
      }
      // ----------------------------------------------------------------------
}; // StandardPlayingCard
// ----------------------------------------------------------------------------

#endif
