#include <iostream>
#include <algorithm>    // std::shuffle
#include <array>        // std::array ; begin() and end() didn't work with regular arrays
#include <random>       // std::default_random_engine 
#include <chrono>       // std::chrono::system_clock
#include <queue>

using std::cout;
using std::endl;

void displayDeck(std::queue<int> deck);
bool cardDuel(std::queue<int> &p1deck, std::queue<int> &p2deck);    // return true if p1 won, false if p2 won
bool cardTie(std::queue<int> &p1deck, std::queue<int> &p2deck);     // return true if p1 won, false if p2 won

int const G_DECK_SIZE = 52;
bool G_winCondition = false;    // game driver will change to true when win condition is met
bool G_winner;                  // true is player 1, false is player 2

int main()
{
    // --------------------------------------------
    // INITIALIZATION STUFF
    // --------------------------------------------

    std::array<int,G_DECK_SIZE> deck {14,14,14,14, 2,2,2,2, 3,3,3,3, 4,4,4,4, 5,5,5,5, 6,6,6,6, 7,7,7,7, 8,8,8,8, 9,9,9,9, 10,10,10,10, 11,11,11,11, 12,12,12,12, 13,13,13,13};
    //std::array<int,G_DECK_SIZE> deck {5,1,1,1,7,1,5,2,2,2,7,2};

    // displays the default deck
    /*cout << "Sorted Deck:\t";
    for (int& x: deck) cout << ' ' << x;
    cout << endl;*/

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); // initialize seed for randomness
    shuffle( deck.begin(), deck.end(), std::default_random_engine(seed) ); // randomizes the deck based on the seed

    // displays the shuffled deck
    /*cout << "Shuffled Deck:\t";
    for (int& x: deck) cout << ' ' << x;
    cout << endl;*/

    std::queue<int> p1deck; // keeps track of player 1's cards
    std::queue<int> p2deck; // keeps track of player 2's cards

    // gives each player half of the deck
    // instead of doing one loop for each player, i can just use a single loop and add a card to 
    //     player 2 based on an offset (half the size of the deck) for each card added to player 1
    for(int i = 0; i < (G_DECK_SIZE / 2); ++i)
    {
        p1deck.push( deck[i] );
        p2deck.push( deck[i + (G_DECK_SIZE / 2) ] );
    }

    cout << "p1deck:"; displayDeck(p1deck);
    cout << "p2deck:"; displayDeck(p2deck);
    cout << endl;

    // --------------------------------------------
    // PLAYING THE GAME
    // --------------------------------------------

    while(!G_winCondition) // while both players still have cards...
    {
        // compares top card of each player's deck.
        // if one card is greater than the other, the player with the greater value card
        //     puts both cards at the bottom of their deck. if the cards are the same value, 
        //     each player must put three cards from the top of their deck into their pot;
        //     the players then duel again. whoever wins this duel wins all the cards in the pot. 
        //     if a player cannot put three cards in the pot, they lose.
        cardDuel(p1deck, p2deck); 

        cout << "Player 1's current deck:";
        displayDeck(p1deck);
        cout << "Player 2's current deck:";
        displayDeck(p2deck);
        cout << endl;

        if(p1deck.size() <= 0) // if player 1 has run out of cards...
        {
            G_winCondition = true;
            G_winner = false; // player 2 is the winner
        }
        if(p2deck.size() <= 0) // if player 2 has run out of cards...
        {
            G_winCondition = true;
            G_winner = true; // player 1 is the winner
        }
    }

    if(G_winner) // if p1 won the game...
    {
        cout << "Player 1 is the winner!" << endl;
    }
    else // if p2 won the game...
    {
        cout << "Player 2 is the winner!" << endl;
    }

    return 0;
}

void displayDeck(std::queue<int> deck)
{
    for(int i = 0; i < deck.size(); ++i) // for each card in the deck...
    {
        int temp = deck.front(); // copy the front value into temp...
        deck.pop(); // remove it from the front...
        deck.push(temp); // add it to the back...

        cout << " " << temp; // display it...
        // and move on to the next value in the queue.
    }
    cout << endl;
}

bool cardDuel(std::queue<int> &p1deck, std::queue<int> &p2deck)
{
    int p1card = p1deck.front(); p1deck.pop(); // pl removes the card from the top of their deck and flips it over
    int p2card = p2deck.front(); p2deck.pop(); // p2 removes the card from the top of their deck and flips it over

    cout << "\tp1: " << p1card << endl;
    cout << "\tp2: " << p2card << endl;

    if(p1card > p2card) // if p1 wins the duel...
    {
        p1deck.push(p1card); // p1 adds their card to the bottom of their deck...
        p1deck.push(p2card); // and then p2's card to the bottom of their deck.

        cout << "\tPlayer 1 won the duel!" << endl;

        return true;
    }
    else if(p1card < p2card) // if p2 wins the duel...
    {
        p2deck.push(p2card); // p2 adds their card to the bottom of their deck...
        p2deck.push(p1card); // and then p1's card to the bottom of their deck.

        cout << "\tPlayer 2 won the duel!" << endl;

        return false;
    }
    else // if the cards are equal value...
    {
        cout << "\tIt's a Tie! War!" << endl;

        return cardTie(p1deck, p2deck);
    }
}

bool cardTie(std::queue<int> &p1deck, std::queue<int> &p2deck)
{
    if(p1deck.size() <= 3) // if p1's deck currently has three cards or less...
    {
        G_winCondition = true;
        G_winner = false; // player 2 is the winner

        cout << "\tPlayer 2 wins by forfiet!" << endl;

        return false;
    }

    if(p2deck.size() <= 3) // if p2's deck currently has three cards or less...
    {
        G_winCondition = true;
        G_winner = true; // player 1 is the winner

        cout << "\tPlayer 1 wins by forfiet!" << endl;

        return true;
    }



    std::array<int,3> p1pot; // keeps track of the cards in player 1's pot
    for(int i = 0; i < 3; ++i) // p1 does this for 3 cards:
    {
        p1pot[i] = p1deck.front(); p1deck.pop(); // p1 removes card from top of their deck and places it in their pot
    }

    std::array<int,3> p2pot; // keeps track of the cards in player 2's pot
    for(int i = 0; i < 3; ++i) // p2 does this for 3 cards:
    {
        p2pot[i] = p2deck.front(); p2deck.pop(); // p2 removes card from top of their deck and places it in their pot
    }

    if( cardDuel(p1deck, p2deck) ) // if p1 wins the duel...
    {
        //cout << "\tPlayer 1 won the duel!" << endl;

        for(int i = 0; i < 3; ++i) // add the cards in both pots to the bottom of your deck
        {
            p1deck.push(p1pot[i]);
            p1deck.push(p2pot[i]);
        }

        return true; // return value stating that p1 won the tie
    }
    else // if p2 wins the duel...
    {
        //cout << "\tPlayer 2 won the duel!" << endl;

        for(int i = 0; i < 3; ++i) // add the cards in both pots to the bottom of your deck
        {
            p2deck.push(p2pot[i]);
            p2deck.push(p1pot[i]);
        }

        return false; // return value stating that p2 won the tie
    }
}
