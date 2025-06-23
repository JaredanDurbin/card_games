#ifndef STANDARDDECK_HPP
#define STANDARDDECK_HPP

// ----------------------------------------------------------------------------
// Includes block.
// ----------------------------------------------------------------------------

// Standard Library includes.
#include <algorithm>
#include <memory>
#include <random>
#include <vector>

// File includes.
// Includes the actual cards.
#include "StandardPlayingCard.hpp"
#include "PolarStandardPlayingCard.hpp"
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Type Declarations block.
// ----------------------------------------------------------------------------

/// @brief A "pile" of cards.
/// @tparam T This should be either PolarStandardPlayingCard or
///           StandardPlayingCard. May expand later.
template<typename T>
using Pile = std::vector<std::unique_ptr<T>>;
/// @brief A split deck is a deck that is split into two halves.
/// @tparam T This should be either PolarStandardPlayingCard or
///           StandardPlayingCard. May expand later.
template<typename T>
using SplitDeck = std::pair<Pile<T>, Pile<T>>;
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Class block.
// ----------------------------------------------------------------------------

/// @brief This is a class for a standard playing card deck.
/// @tparam T This should be either PolarStandardPlayingCard or
///           StandardPlayingCard. May expand later.
template<typename T>
class StandardDeck
{
   private:
      // ----------------------------------------------------------------------
      // Private Variables block.
      // ----------------------------------------------------------------------

      // The actual deck itself.
      // We will use pointers for this because the card probably shouldn't
      // really move at all just the references.
      //
      // NOTE: The top of the deck is the back.
      mutable Pile<T> deck;
      // ----------------------------------------------------------------------
   
   public:
      // ----------------------------------------------------------------------
      // Constructors block.
      // ----------------------------------------------------------------------

      /// @brief Standard constructor.
      StandardDeck()
      {
         // We should only be making "StandardDecks" with standard cards.
         static_assert(std::is_same<T, StandardPlayingCard>::value || 
                       std::is_same<T, PolarStandardPlayingCard>::value,
                       "StandardDeck can only be instantiated with"
                       "StandardPlayingCard or\n"
                       "PolarStandardPlayingCard.");

         // Deck of 52 cards can maybe be expanded later.
         deck.reserve(52);

         // Create all the cards. This is basically like openning a fresh
         // pack of cards.
         for (const auto& suit : CardGraphicsAndInfo::suits)
         {
            for (const auto& rank : CardGraphicsAndInfo::ranks)
            {
               deck.push_back(std::make_unique<T>(rank,suit));
            }
         }
      }

      /// @brief The copy constructor. Basically magic. Making cards from thin
      ///        air.
      /// @param copy The instance we want to copy.
      StandardDeck(const StandardDeck<T> &copy)
      {
         // It's a copy so only need the same amount.
         deck.reserve(copy.deck.size());

         // Push back uniques pointers copying the cards.
         for (const auto& ptr : copy.deck) 
         {
            if (ptr)
            {
               deck.push_back(std::make_unique<T>(*ptr));
            }
            else
            {
               deck.push_back(nullptr);
            }
         }
      }
      
      /// @brief Custom constructor to make a deck from a vector.
      /// @param pile The pile of cards we want to make our deck.
      StandardDeck(const Pile<T> pile): deck(std::move(pile)) {};

      /// @brief The move constructor.
      /// @param move The instance we would like to move.
      StandardDeck(StandardDeck<T> &&move) noexcept: 
         deck(std::move(move.deck)) 
      {};

      /// @brief The default destructor.
      ~StandardDeck() = default;
      // ----------------------------------------------------------------------

      // ----------------------------------------------------------------------
      // Operator Overloads block.
      // ----------------------------------------------------------------------

      /// @brief Overload the move operator.
      /// @param other The instance we want to move.
      /// @return Standard return for the set operator.
      ///         A pointer to this class.
      StandardDeck& operator=(StandardDeck &&other) noexcept
      {
         if (this != &other)
         {
            deck = std::move(other.deck);
         }
         return *this;
      }
      // ----------------------------------------------------------------------

      // ----------------------------------------------------------------------
      // Get block.
      // ----------------------------------------------------------------------

      /// @brief Get the deck that this class represents.
      /// @return Return the reference to the deck.
      const Pile<T>& GetDeck() const { return deck; };
      // ----------------------------------------------------------------------

      // ----------------------------------------------------------------------
      // Set block.
      // ----------------------------------------------------------------------

      /// @brief The set deck function to set the deck to whaat we need if we
      ///        need it.
      /// @param pile The pile of cards we want to set this deck to.
      void SetDeck(Pile<T> pile)
      {
         // We should only be making "StandardDecks" with standard cards.
         static_assert(std::is_same<T, StandardPlayingCard>::value || 
                       std::is_same<T, PolarStandardPlayingCard>::value,
                       "StandardDeck can only be instantiated with"
                       "StandardPlayingCard or\n"
                       "PolarStandardPlayingCard.");

         // Clear our deck for now.
         deck.clear();
         // Get the piles size.
         size_t size = pile.size();
         // Reserve the memory for faster processing.
         deck.reserve(size);

         // Move the pile.
         deck = std::move(pile);

         // Clear the memory.
         pile.clear();
      }
      // ----------------------------------------------------------------------

      // ----------------------------------------------------------------------
      // Public Functions block.
      // ----------------------------------------------------------------------

      /// @brief Draw one card from the top of the deck.
      /// @return Returns the card.
      std::unique_ptr<T> DrawOne()
      {
         if (deck.size() == 0)
         {
            return nullptr;
         }

         std::unique_ptr<T> out = std::move(deck.back());
         deck.pop_back();
         return out;
      }

      /// @brief Draws more than one card
      /// @param amount The amount of cards that are to be drawn from the top
      ///               of the deck.
      /// @return Returns the pile of cards drawn.
      Pile<T> DrawMultiple(const size_t amount)
      {
         // If the amount is bigger than the deck just draw all the cards.
         if (amount > deck.size())
         {
            amount = deck.size();
         }

         // Initialize the return pile.
         Pile<T> out;
         // Reserve the amount of cards.
         out.reserve(amount);
         // Take cards off the top.
         for (size_t i = 0; i < amount; i++)
         {
            out.push_back(std::move(deck.back()));
            // Make sure to pop the memory slot.
            deck.pop_back();
         }
         // Return the piles.
         return out;
      }

      /// @brief Basically split the deck with one half having the amount of
      ///        card specified in "mid".
      /// @param mid The amount of cards in the first half. Which is from the
      ///            bottom of the deck.
      /// @return Returns the split deck.
      SplitDeck<T> Split(const size_t mid)
      {
         // Initialize the piles.
         Pile<T> pile1;
         Pile<T> pile2;

         // Reserve the memory.
         pile1.reserve(mid);
         pile2.reserve(deck.size() - mid);

         // Get the middle iter because we will do this move fast.
         auto mid_iter = deck.begin();
         std::advance(mid_iter,mid);

         // This is the quick way to move a lot of cards from the deck.
         // (Or just vectors in general.)
         std::move(std::make_move_iterator(deck.begin()),
                   std::make_move_iterator(mid_iter),
                   std::back_inserter(pile1));
         std::move(std::make_move_iterator(mid_iter),
                   std::make_move_iterator(deck.end()),
                   std::back_inserter(pile2));

         // Clear the deck's memory.
         deck.clear();
         // Return the pair.
         return std::make_pair(std::move(pile1), std::move(pile2));
      }

      /// @brief This function will take a split deck and put it together.
      /// @param halves This is the actual split deck we would like to put
      ///                together.
      /// @param type_input 
      /// @param down_first_input 
      /// @return Returns the piles put together into one.
      Pile<T> PutHalvesTogether(SplitDeck<T>      halves,
                                const std::string type_input,
                                const std::string down_first_input = "random")
      {
         // Get the total number of cards.
         size_t total = halves.first.size() + halves.second.size();
         // Initialize the return variable.
         Pile<T> out;
         // Reserve the memory for faster processing.
         out.reserve(total);

         // If the type is just a cut then put the top half (second) on the
         // bottom and the bottom half (first) on top.
         if (type_input == "cut")
         {
            std::move(std::make_move_iterator(halves.second.begin()),
                     std::make_move_iterator(halves.second.end()),
                     std::back_inserter(out));
            std::move(std::make_move_iterator(halves.first.begin()),
                     std::make_move_iterator(halves.first.end()),
                     std::back_inserter(out));
         }
         else 
         {
            // DEBUG
            //std::cout << "Inside correct if statement.\n";

            // Get our random numbers set up.
            std::random_device rd;
            std::mt19937 rng(rd());
            std::uniform_int_distribution<std::mt19937::result_type> dist1(0,1);

            // This is for which half of the deck should go down first.
            int add = 0;
            if (down_first_input == "top" ||
                (down_first_input == "random" && dist1(rng) == 1))
            {
               add = 1;
            }
            
            // This does all the figuring of which one goes in first if 
            // it is not a cut.
            for (size_t i = add; i < total + add; i++)
            {
               // DEBUG
               //std::cout << type_input << " " << dist1(rng) << std::endl;

               // If the type input is perfect and the bottom should go down
               // first or it is random and we got the random 0 from the rng
               // or bottom was specified or the top half is just empty.
               // Also make sure it has cards too.
               if (((type_input == "perfect" && i % 2 == 0) ||
                    (type_input == "random" && dist1(rng) == 0) ||
                    (down_first_input == "bottom" && i == 0) ||
                    (halves.second.size() == 0)) &&
                   halves.first.size() > 0)
               {
                  // DEBUG
                  //std::cout << "Bottom.\n";
                  //std::cout << i << ": " <<  halves.first[0]->GetRank() << " of " 
                  //          << halves.first[0]->GetSuit() << "s.\n";

                  out.push_back(std::move(halves.first[0]));
                  halves.first.erase(halves.first.begin());
               }
               else if (halves.second.size() > 0)
               {
                  // DEBUG
                  //std::cout << "Top.\n";
                  //std::cout << i << ": " << halves.second[0]->GetRank() << " of " 
                  //          << halves.second[0]->GetSuit() << "s.\n";

                  out.push_back(std::move(halves.second[0]));
                  halves.second.erase(halves.second.begin());
               }
            }
         }

         // Clear the memory.
         halves.first.clear();
         halves.second.clear();
         // Return the pile.
         return out;
      } // PutHalvesTogether

      /// @brief Cut the deck.
      /// @param mid This is a perfect cut which is default, but otherwise
      ///            You can specify the amount of cards to cut.
      ///            If "mid" = '-1' then the cut will be random.
      ///            "mid" is also the amount of cards that will be cut from
      ///            the bottom.
      void Cut(size_t mid = 26)
      {
         // Determine the amount of cards to cut.
         if (mid >= deck.size())
         {
            mid = deck.size() / 2;
         }

         if (mid < 0)
         {
            mid = std::rand() % 52;
         }

         // Split the deck, and put it back together.
         SetDeck(PutHalvesTogether(Split(mid), "cut"));
      }

      /// @brief This represents a riffle shuffle.
      /// @param mid This is a perfect cut which is default, but otherwise
      ///            You can specify the amount of cards to cut.
      ///            If "mid" = '-1' then the cut will be random.
      ///            "mid" is also the amount of cards that will be cut from
      ///            the bottom.
      /// @param down_first_input Which card will fall down first.
      /// @param type_input This will be either 'perfect' or 'random' to 
      ///                   simulate a perfect riffle shuffle or just one done
      ///                   Leisurely.
      void Riffle(size_t            mid = 26,
                  const std::string down_first_input = "bottom",
                  const std::string type_input = "perfect")
      {
         // Get the random numbers.
         std::random_device rd;
         std::mt19937 rng(rd());
         std::uniform_int_distribution<std::mt19937::result_type> dist1(0,1);
         std::uniform_int_distribution<std::mt19937::result_type> dist51(0,51);
         // Get the mid point.
         if (mid < 0)
         {
            mid = dist51(rng);
         }

         // Get which card should go down first.
         // Could be specified or random.
         if (down_first_input == "bottom" ||  
             (down_first_input == "random" &&
              dist1(rng) == 0))
         {
            SetDeck(PutHalvesTogether(Split(mid), type_input, "bottom"));
         }
         else
         {
            SetDeck(PutHalvesTogether(Split(mid), type_input, "top"));
         }
      } // Riffle

      /// @brief This function just entirely randomizes the deck.
      void RandomizeDeck()
      {
         // Random setup.
         std::random_device rd;
         std::mt19937 rng(rd());

         // Shuffle.
         std::shuffle(deck.begin(), deck.end(), rng);
      }
      // ----------------------------------------------------------------------
}; // StandardDeck
// ----------------------------------------------------------------------------

#endif
