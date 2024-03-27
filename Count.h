//
//  Count.h
//  21pointGG
//
//  Created by 杰珉 蔡 on 2024/3/27.
//
#include <vector>
#include <random>
#include <cmath>
#include <string>
#include <regex>
#ifndef countbet_h
#define countbet_h
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
        std::uniform_int_distribution<int> dis(0, deck.size() - 1);
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
        auto display = [this](const std::vector<int> &deck, const std::string &name) {
            std::cout << name << " deck: ";
            showDeck(deck);
            std::cout << ", " << countpoint(deck) << std::endl;
        };
        
        display(A, "A");
        display(B, "B");
        display(C, "C");
        display(D, "D");
        display(Player, "Player");
        display(Banker, "Banker");
        
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
    void plusmoney(const int *betmultiple) {
        poolmoney += *betmultiple*betmoney;
    }
//    輸了要扣多少錢
    void minusmoney(const int *betmultiple) {
        poolmoney -= *betmultiple*betmoney;
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

#endif /* countbet_h */
