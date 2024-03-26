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

class Count21Point {
private:
    std::vector<int> deck;
    int RunningCount = 0;
    
public:
    int betmoney = 25;     //一次要下的賭金
    int poolmoney = 1000;  //總共有多少賭金
    std::vector<int> A, B, C, D, Player, Banker;
//             初始化牌堆
    Count21Point() {
        initializeDeck();
    }
//     製作牌庫 一共五副牌 13*4*5
    void initializeDeck() {
        deck.clear();
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 4; ++j) {
                for (int k = 1; k < 14; ++k) {
                    deck.push_back(k);
                }
            }
        }
    }
//    分牌進去想要的牌庫 並從總牌庫裡面刪除
    void drawCard(std::vector<int> &playerDeck) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, deck.size() - 1);
        int index = dis(gen);
        playerDeck.push_back(deck[index]);
        deck.erase(deck.begin() + index);
    }
//    計算你有多少點數
    int countpoint(const std::vector<int> &decknumber) const{
        int allpoints = 0;
        for(int point : decknumber) {
            if(point >= 2 && point <= 9) {
                allpoints += point;
            } else if(point >= 10) {
                allpoints += 10;
            }
        }
//     如果總數不超過21點則將Ａ認為11點 ， 其餘都將Ａ認為 1 點
        for (int point : decknumber) {
            if (point == 1) {
                allpoints += (allpoints + 11 <= 21) ? 11 : 1;
            }
        }
        return allpoints;
    }
    //顯示牌庫剩下多少張牌
    void showDeck(const std::vector<int> &decknumber) const {
        for (int num : decknumber) {
            std::cout << num << " ";
        }
    }
//     顯示所有玩家還有莊家的牌是多少
    void SHOWALL() {
        std::cout << "A deck: ";
        showDeck(A);
        std::cout << ", " << countpoint(A) << std::endl;
        std::cout << "B deck: ";
        showDeck(B);
        std::cout << ", " << countpoint(B) << std::endl;
        std::cout << "C deck: ";
        showDeck(C);
        std::cout << ", " << countpoint(C) << std::endl;
        std::cout << "D deck: ";
        showDeck(D);
        std::cout << ", " << countpoint(D) << std::endl;
        std::cout << "Player deck: ";
        showDeck(Player);
        std::cout << ", " << countpoint(Player) << std::endl;
        std::cout << "Bankers deck: ";
        showDeck(Banker);
        std::cout << ", " << countpoint(Banker) << std::endl;
        std::cout << std::endl;
    }
//    你要放進去的牌 必且從總牌庫裡面刪除
    void inputCardNumber(int yourcardnumber) {
        Player.push_back(yourcardnumber);
        auto it = std::find(deck.begin(), deck.end(), yourcardnumber);
        if (it != deck.end()) {
            deck.erase(it);
        }
    }
    
    // 展示牌庫  引用private
    const std::vector<int>& getdeckDeck() const {
        return deck;
    }
//    計算你有多少點數  （使用Hi-Lo算牌法  2-6 總體真數-1  10 J Q K A 總體真數+1
    int countRunningCount() const {
        int localRunningCount = 0;
        for (int card : deck) {
            if (card >= 2 && card <= 6) {
                localRunningCount--;
            } else if (card >= 10 || card == 1) {
                localRunningCount++;
            }
        }
        return localRunningCount;
    }
//     用四捨五入取總牌庫裡面剩下幾副牌
    float deckRemaining() const {
        return static_cast<float>(round(deck.size() / 52.0 * 10) / 10);
    }
//    清理牌庫
    void clearDecks() {
        A.clear();
        B.clear();
        C.clear();
        D.clear();
        Player.clear();
        Banker.clear();
    }
//    贏了要加多少錢
    void plusmoney(int& betmultiple) {
        poolmoney += betmultiple*betmoney;
    }
//    輸了要扣多少錢
    void minusmoney(int& betmultiple) {
        poolmoney -= betmultiple*betmoney;
    }
//    判斷輸入值是否為數字
    bool isNumber(const std::string &s) const {
        std::regex pattern("-?[0-9]+");
        return std::regex_match(s, pattern);
    }
//    Hi-Lo計算真數的方程式
    int trueCount() const {
        return countRunningCount() / static_cast<int>(deckRemaining());
    }
//    Hi-Lo算牌法去計算你應該下多少倍的賭金
    int bet() const {
        if(trueCount() != 0) {
            return trueCount() - 1;
        } else {
            return trueCount();
        }
    }
};

int main() {
    Count21Point c21p;
    std::string inputcommand;
    bool newgame = true;
    bool addstop = true;
    int betmultiple;
    while (newgame) {
        std::cout << "Recommend Bet* : " << c21p.bet() << std::endl;
        std::cout << "How many bet: ";
        std::cin >> betmultiple;
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

