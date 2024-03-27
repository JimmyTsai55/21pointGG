//
//  main.cpp
//  21pointGG
//
//  Created by 杰珉 蔡 on 2024/3/21.
//

#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <ctime>
#include <cmath>
#include <string>
#include <regex>
#include "Count.h"
int main() {
    Count21Point c21p;
    std::string inputcommand;
    bool newgame = true;
    bool addstop = true;
    int *betmultiple = new int;
    while (newgame) {
        std::cout << "Recommend Bet* : " << c21p.bet() << std::endl;
        std::cout << "How many bet: ";
        std::cin >> *betmultiple;
//        if (c21p.isNumber(betmultiple)) {
//            int betmultiple = std::stoi(betmultiple);
//        }
        do {
            for(int i = 0 ; i < 2; i++) {
                c21p.drawCard(c21p.A);
                c21p.drawCard(c21p.B);
                c21p.drawCard(c21p.C);
                c21p.drawCard(c21p.D);
                c21p.drawCard(c21p.Player);
                c21p.drawCard(c21p.Banker);
            }
        } while(c21p.A.size() > 2);
        std::vector<std::vector<int>*> allDecks =
        {&c21p.A, &c21p.B, &c21p.C, &c21p.D};
        //幫其他玩家自動補牌
        for (auto* deck : allDecks) {
            if (c21p.countpoint(*deck) < 17) {        c21p.drawCard(*deck);
            }
        }
        while (addstop) {
            int playerPoints = c21p.countpoint(c21p.Player);
            int bankerPoints = c21p.countpoint(c21p.Banker);
            c21p.SHOWALL();
            std::cout << "Enter your command(add, next, quit, go): ";
            std::cin >> inputcommand;
            if (inputcommand == "add") {
                c21p.drawCard(c21p.Player);
                if(playerPoints > 21) {
                    std::cout << "!!!!!!!LOSE!!!!!!" << std::endl;
                }
//                進行下一回合遊戲  並同時結算遊戲
            } else if(inputcommand == "next") {
                if (bankerPoints <= 17) {
                    c21p.drawCard(c21p.Banker);
                }
                c21p.SHOWALL();
                if (playerPoints > 21) {
                    c21p.minusmoney(betmultiple);
                    std::cout << "!!!!!!!LOSE!!!!!!" << std::endl;
                }
                else if (bankerPoints > 21 || playerPoints > bankerPoints) {
                    c21p.plusmoney(betmultiple);
                    std::cout << "!!!Winner Winner Chicken Dinner!!!" << std::endl;
                } else if (bankerPoints == playerPoints) {
                    std::cout << "!!!equal!!!" << std::endl;
                }
                else {
                    c21p.minusmoney(betmultiple);
                    std::cout << "!!!!!!!LOSE!!!!!!" << std::endl;
                }
                std::cout << "====NEW_GAME====" << std::endl;
                c21p.clearDecks();
                break;
//                結束遊戲
            } else if (inputcommand == "quit") {
                std::cout << "=========quit==========" << std::endl;
                newgame = false;
                break;
//                幫莊家自動補牌
            } else if (inputcommand == "go") {
                while (bankerPoints <= 17) {
                    c21p.drawCard(c21p.Banker);
                    bankerPoints = c21p.countpoint(c21p.Banker);
                }
            }
        }
        delete betmultiple;
    }
    std::cout << "RunningCount : " << c21p.countRunningCount() << std::endl;
    std::cout <<
    "Deck Remaining: " << c21p.getdeckDeck().size() <<
    ", " << c21p.deckRemaining() <<
    ", Used : " << 260 - c21p.getdeckDeck().size() << std::endl;
    std::cout << "TrueCount: " << c21p.trueCount() << std::endl;
    std::cout << "Bet* : " << c21p.bet() << std::endl;
    std::cout << "MoneyPool: " << c21p.poolmoney << std::endl;
    
    return 0;
}


