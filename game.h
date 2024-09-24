#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "type.h"
zhuangtai GAME;
void shuffle_players(int*);
void send(int socket_, zhuangtai &game){// 发送消息
    char BUF[BUF_SIZE];
    memset(BUF, 0, sizeof(BUF));
    memcpy(BUF, &game, sizeof(zhuangtai));
    send(socket_, BUF, sizeof(zhuangtai), 0);
}
bool check_flying(){// 检查是否有人被飞
    for(int i = 0 ; i < 4 ; i++)
        if(GAME.p[i].point < 0)
            return 1;
    return 0;
}
void game(int *sockets){
    shuffle_players(sockets);
    int qi_shi_dian_shu = 25000, yi_wei_bi_yao_dian_shu = 30000;
    int chang_shu = 0;
    cout << "请输入起始点数：";
    cin >> qi_shi_dian_shu;
    cout << "请输入一位必要点数：";
    cin >> yi_wei_bi_yao_dian_shu;
    cout << "请输入轮数：";
    cin >> chang_shu;
    for(int i = 0 ; i < 4 ; i++)//初始化
        GAME.p[i].point = qi_shi_dian_shu;
    int now_zhuang_jia = 0;
    int chang_feng = 0;
    bool not_enough_point = 1;
    for(int i = 1 ; i <= chang_shu || not_enough_point; ){
        GAME.init(now_zhuang_jia, chang_feng);//初始化
        int now_player = 0;
        static char BUF[BUF_SIZE];
        memset(BUF, 0, sizeof(BUF));
        memcpy(BUF, &GAME, sizeof(zhuangtai));
        for(int j = 0 ; j < 4 ; j++)
            send(sockets[j], GAME);
        //传输局面
        while(GAME.rht < 122){//发牌
            GAME.p[now_player].inhand.push_back(GAME.paishan[GAME.lft++]);//拿牌
            got_card:;
            for(int j = 0 ; j < 4 ; j++) 
                send(sockets[j], GAME);// 更新所有人牌面
            send(sockets[now_player], "check_an_gang", 13, 0);
            int siz = recv(sockets[now_player], BUF, BUF_SIZE, 0);// 是否有暗杠
            BUF[siz] = '\0';
            if(strcmp(BUF, "no_an_gang") != 0){// 有暗杠，BUF为牌
                int x = BUF[0];
                GAME.gang++;
                if(GAME.gang > 4 || (GAME.gang == 4 && GAME.p[now_player].angang.size() + GAME.p[now_player].fulu_gang.size() < 4)){
                    for(int j = 0 ; j < 4 ; j++)
                        send(sockets[j], "liu_ju", 6, 0);// 四杠散了
                    break;
                }
                GAME.p[now_player].angang.push_back(x);
                GAME.open_dora();
                GAME.p[now_player].inhand.push_back(GAME.paishan[GAME.rht--]);
                goto got_card; // 拿新牌
            }
            send(sockets[now_player], "check_zi_mo", 11, 0);// 询问是否自摸
            int siz = recv(sockets[now_player], BUF, sizeof(BUF), 0);
            BUF[siz] = '\0';
            if(strcmp(BUF, "bu_zi_mo") != 0){// 自摸 buf 返回点数
                int got_point = atoi(BUF);
                int x = got_point / 100;
                if(now_player == now_zhuang_jia){
                    x = ceil(x * 1.0 / 3);
                    x *= 100;
                    for(int j = 0 ; j < 4 ; j++){
                        GAME.p[j].point -= x;
                    }
                    GAME.p[now_player].point += 4 * x;
                }
                else{
                    x = ceil(x * 1.0 / 4);
                    int y = ceil(x * 1.0 / 2);
                    for(int j = 0 ; j < 4 ; j++)
                        GAME.p[j].point -= x;
                    GAME.p[now_player].point += 3 * x + y;
                    GAME.p[now_zhuang_jia].point += x - y;
                }
                GAME.p[now_player].point += GAME.leiji_point;
                GAME.leiji_point = 0;
                if(check_flying())
                    goto end;
                break;
            }
            //舍张
            send(sockets[now_player], "she_zhang", 9, 0);
            recv(sockets[now_player], BUF, sizeof(BUF), 0);
            int she_zhang = BUF[0];
            for(int j = 0 ; j < 4 ; j++)
                send(sockets[j], GAME);
            // 是否有人荣
            vector<pair<int, int> >v;
            for(int i = 0 ; i < 4 ; i++){
                if(i == now_player)
                    continue;
                send(sockets[now_player], "check_rong", 10, 0);
                int siz = recv(sockets[now_player], BUF, sizeof(BUF), 0);
                BUF[siz] = '\0';
                if(strcmp(BUF, "not_rong") != 0){
                    int x = atoi(BUF);
                    v.push_back(make_pair(i, x));
                }
            }
            if(v.size()){
                int x = GAME.leiji_point;
                GAME.leiji_point = 0;
                int idx = 0;
                int minn = 10;
                for(int i = 0 ; i < v.size() ; i++){
                    if((v[i].first - now_player + 4) % 4 < minn){
                        minn = (v[i].first - now_player + 4) % 4;
                        idx = v[i].first;
                    }
                }
                GAME.p[idx].point += x;
                for(int i = 0 ; i < v.size() ; i++){
                    GAME.p[now_player].point -= v[i].second;
                    GAME.p[v[i].first].point += v[i].second;
                }
                if(check_flying())
                    goto end;
                break;
            }
            // 是否有人杠
        }
    }
    end:;
}
void shuffle_players(int *players){
    mt19937_64 rd((unsigned)time(NULL));
    shuffle(players, players + 4, rd);
}
#endif