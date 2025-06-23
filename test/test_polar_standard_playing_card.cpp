// ----------------------------------------------------------------------------
// Includes block.
// ----------------------------------------------------------------------------

// Standard Library includes.
#include <iostream>

// Utility File includes.
// This is my own home grown utilities for this specific project.
#include "../header/Util.h"

// Regular File includes.
// This is the header file for the "PolarStandardPlayingCard" class.
#include "../header/PolarStandardPlayingCard.hpp"
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Functions block.
// ----------------------------------------------------------------------------

/// @brief This function takes a graphic string and then outputs the 
///        graphic to the console.
/// @param graphic The graphic string from a card.
void OutputCardGraphic(std::vector<std::string> graphic)
{
   for (size_t i = 0; i < graphic.size(); i++)
   {
      std::cout << Util::UTF8SubStr(graphic[i], 4) << "\n";
   }
   std::cout << "\n";
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Main block.
// ----------------------------------------------------------------------------

/// @brief This program is for testing the "PolarStandardPlayingCard" class.
/// @return The basic return for a successfully run program.
int main()
{
   // Printing to console what this program is doing.
   // We will test the constructors first.
   std::cout << "Testing constructors.\n";

   // Test the standard constructor.
   std::cout << "Card default constructor.\n";
   // Constructing a Jack of Spades.
   PolarStandardPlayingCard card1(11,"spade");
   // Print the card data: rank and suit.
   std::cout << "Card1: " << card1.GetRank() << " of " 
             << card1.GetSuit() << "s.\n";
   // Call the function to output the graphic.
   OutputCardGraphic(card1.GetGraphic());

   // Test the copy constructor.
   std::cout << "Card copy constructor.\n";
   // Use this constructor to copy the last card: the Jack of Spades.
   PolarStandardPlayingCard card2(card1);
   // Print the card data: rank and suit.
   std::cout << "Card2: " << card2.GetRank() << " of " 
             << card2.GetSuit() << "s.\n";
   // Call the function to output the graphic.
   OutputCardGraphic(card2.GetGraphic());

   // Printing out the test data.
   std::cout << "Card default constructor.\n";
   std::cout << "This should fail and be unknown.\n";
   // As mentioned above this will fail (or should).
   PolarStandardPlayingCard card3(1324,"dsaf");
   std::cout << "Card3: " << card3.GetRank() << " of " << card3.GetSuit() << "s.\n";
   OutputCardGraphic(card3.GetGraphic());

   // Print out some test data for the first two cards.
   // Pass the test if the cards are the same.
   if (card1 == card2)
   {
      std::cout << "PASS: cards are equal.\n";
   }
   else
   { 
      std::cout << "FAIL: cards are not equal.\n";
   }

   // This will test the flipping mechanic.
   std::cout << "Flipped? " << card1.GetFaceUp() << "\n";
   card1.FlipCard();
   std::cout << "Flipped? " << card1.GetFaceUp() << "\n";
   card1.FlipCard();
   std::cout << "Flipped? " << card1.GetFaceUp() << "\n";
   card1.FlipCard();
   std::cout << "Flipped? " << card1.GetFaceUp() << "\n";

   return 0;
}
// ----------------------------------------------------------------------------
