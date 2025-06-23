// ----------------------------------------------------------------------------
// Includes block.
// ----------------------------------------------------------------------------

// Standard Library includes.
#include <iostream>

// Regular File includes.
// This is the header file for the "StandardDeck" class.
#include "../header/StandardDeck.hpp"
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Functions block.
// ----------------------------------------------------------------------------

/// @brief This function will take a deck of cards and output all of the cards
///        to the console for viewing and testing.
/// @param deckInput This will be the deck of cards that is output to
///                  console.
void OutputStandardDeck(const StandardDeck<PolarStandardPlayingCard> deckInput)
{
   for (size_t i = 0; i < deckInput.GetDeck().size(); i++)
   {
      std::cout << "Card " << i + 1 << ": " 
                << deckInput.GetDeck()[i]->GetRank();
      std::cout << " of " << deckInput.GetDeck()[i]->GetSuit() << "s.\n";
   }
   std::cout << "\n";
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Main block.
// ----------------------------------------------------------------------------

/// @brief This program is for testing the "StandardDeck" class.
/// @return The basic return for a successfully run program.
int main()
{
   // Use the standard constructor with the "PolarStandardPlayingCard".
   StandardDeck<PolarStandardPlayingCard> deck1;
   // Call the function to output the deck we just constructed.
   OutputStandardDeck(deck1);

   // Now test the copy constructor on the first deck.
   StandardDeck<PolarStandardPlayingCard> deck2(deck1);
   // Output the deck to console to check it.
   OutputStandardDeck(deck2);

   // Now do a "cut" shuffle so now the bottom should be on top.
   deck1.Cut(21);
   // Output the deck to console to check it.
   OutputStandardDeck(deck1);

   // Now do a riffle shuffle. Split the deck by 34 cards let the top fall
   // first, and let a random amount fall each time.
   deck1.Riffle(34,"top","random");
   // Output the deck to console to check it.
   OutputStandardDeck(deck1);

   // This function should prefectly randomize the deck of cards.
   deck2.RandomizeDeck();
   // Output the deck to console to check it.
   OutputStandardDeck(deck2);

   // Here we will test the "SetDeck" function.
   // Create a temp deck.
   Pile<PolarStandardPlayingCard> temp;
   // Use the draw one function repeatedly to draw 5 cards.
   temp.push_back(deck1.DrawOne());
   temp.push_back(deck1.DrawOne());
   temp.push_back(deck1.DrawOne());
   temp.push_back(deck1.DrawOne());
   temp.push_back(deck1.DrawOne());
   // Use the "SetDeck" function.
   deck2.SetDeck(std::move(temp));
   // Output the deck to console to check it.
   OutputStandardDeck(deck2);

   return 0;
}
// ----------------------------------------------------------------------------
