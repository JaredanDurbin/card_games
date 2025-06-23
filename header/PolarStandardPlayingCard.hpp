#ifndef POLARSTANDARDPLAYINGCARD_HPP
#define POLARSTANDARDPLAYINGCARD_HPP

// ----------------------------------------------------------------------------
// Includes block.
// ----------------------------------------------------------------------------

// Regular File includes.
// This is the header file for the standard playing card class.
#include "StandardPlayingCard.hpp"
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Class block.
// ----------------------------------------------------------------------------

/// @brief This implements the standard playing card class but the cards are
///        polar: meaning they can be flipped face-up and face-down.
class PolarStandardPlayingCard: public StandardPlayingCard
{
   private:
      // ----------------------------------------------------------------------
      // Private Variables block.
      // ----------------------------------------------------------------------

      // If this is true the card is face-up if it is false it is
      // face-down.
      mutable bool face_up;
      // ----------------------------------------------------------------------

   public:
      // ----------------------------------------------------------------------
      // Constructors block.
      // ----------------------------------------------------------------------

      /// @brief Cards cannot be constructed normally so we delete the 
      ///        standard constructor.
      PolarStandardPlayingCard() = delete;

      /// @brief This is the constructor that should be used.
      /// @param rank_input The rank of the card to be constructed.
      /// @param suit_input The suit of the card to be constructed.
      /// @param face_up_input If the card is to be constructed face up or 
      ///                      face down. Face down by default.
      PolarStandardPlayingCard(const int         rank_input, 
                               const std::string suit_input,
                               const bool        face_up_input = false): 
         StandardPlayingCard(rank_input,suit_input),
         face_up(face_up_input)
      {};

      /// @brief The copy constructor.
      /// @param copy The instance that we would like to copy.
      PolarStandardPlayingCard(const PolarStandardPlayingCard &copy): 
         StandardPlayingCard(copy),
         face_up(copy.face_up)
      {};

      /// @brief The move constructor.
      /// @param move The instance we would like to move.
      PolarStandardPlayingCard(PolarStandardPlayingCard &&move): 
         StandardPlayingCard(std::move(move)),
         face_up(std::move(move.face_up))
      {};

      /// @brief The standard destructor.
      ~PolarStandardPlayingCard() {};
      // ----------------------------------------------------------------------

      // ----------------------------------------------------------------------
      // Operator Overloads block.
      // ----------------------------------------------------------------------

      /// @brief This is an oveload for the equals operator.
      /// @param obj The object we are trying to see if it is the same as ours.
      /// @return Returns whether the values are equal or not.
      bool operator==(const PolarStandardPlayingCard &obj)
      {
         return StandardPlayingCard::operator==(obj) && face_up == obj.face_up;
      }

      /// @brief This is an overload for the copy set operator.
      ///        Nothing special.
      /// @param other The variable we would like to set this to instance to.
      /// @return Standard return for the set operator.
      ///         A pointer to this class.
      PolarStandardPlayingCard& operator=(PolarStandardPlayingCard &other)
      {
         if (this != &other)
         {
            StandardPlayingCard::operator=(other);
            face_up = other.face_up;
         }
         return *this;
      }

      /// @brief This is an overload for the move set operator.
      ///        Nothing special.
      /// @param other The variable we would like to set this to instance to.
      /// @return Standard return for the set operator.
      ///         A pointer to this class.
      PolarStandardPlayingCard& operator=(PolarStandardPlayingCard &&other)
      {
         if (this != &other)
         {
            StandardPlayingCard::operator=(std::move(other));
            face_up = std::move(other.face_up);
         }
         return *this;
      }
      // ----------------------------------------------------------------------

      // ----------------------------------------------------------------------
      // Get Functions block.
      // ----------------------------------------------------------------------

      /// @brief Standard get function for the "face_up" variable.
      /// @return Returns the "face_up" variable.
      const bool GetFaceUp() const { return face_up; }
      // ----------------------------------------------------------------------

      // ----------------------------------------------------------------------
      // Public Functions block.
      // ----------------------------------------------------------------------
      
      /// @brief This function will flip a card face up or flip it face down;
      ///        Whatever the card is not this function will flip it the other
      ///        way.
      void FlipCard()
      {
         if (face_up)
         {
            face_up = false;
         }
         else 
         {
            face_up = true;
         }
      }
      // ----------------------------------------------------------------------
}; // PolarStandardPlayingCard
// ----------------------------------------------------------------------------

#endif