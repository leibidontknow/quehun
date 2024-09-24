#ifndef _TYPE_H_
#define _TYPE_H_

#define HAND_COUNT 13
#define WHOLE_COUNT 136
#define PORT 11451
#define BUF_SIZE 65536

#include <bits/stdc++.h>
using namespace std;


const vector<int> stdpaishan = {
	1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3,
	4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6,
	7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9,
	10, 10, 10, 10, 11, 11, 11, 11, 12, 12,
	12, 12, 13, 13, 13, 13, 14, 14, 14, 14,
	15, 15, 15, 15, 16, 16, 16, 16, 17, 17,
	17, 17, 18, 18, 18, 18, 19, 19, 19, 19,
	20, 20, 20, 20, 21, 21, 21, 21, 22, 22,
	22, 22, 23, 23, 23, 23, 24, 24, 24, 24,
	25, 25, 25, 25, 26, 26, 26, 26, 27, 27,
	27, 27, 28, 28, 28, 28, 29, 29, 29, 29,
	30, 30, 30, 30, 31, 31, 31, 31, 32, 32,
	32, 32, 33, 33, 33, 33, 34, 34, 34, 34
};

struct geren{
	vector<int> inhand, fulu_peng, fulu_gang, fulu_chi, angang, shezhang, shezhang_notfulu;//副露状态与舍张
	bool wlizhi, lizhi, mengqianqing, yifa, liuman;//立直状态与门前清状态
	int lizhixun, point;//立直巡数与分数
}p[4];
struct zhuangtai{
	geren p[4];
	int zhuangjia, changfeng, xunshu, gang, lft, rht, leiji_point;//庄家 场风 当前巡数
	vector<int> paishan, baopai;//牌山
	void init(int zj, int cf){
		for(int i = 0 ; i <= 3 ; i ++){
			p[i].lizhi = p[i].wlizhi = p[i].yifa = 0;
			p[i].lizhixun = -1, p[i].mengqianqing = p[i].liuman = 1;
			p[i].fulu_peng.clear();
			p[i].fulu_gang.clear();
			p[i].fulu_chi.clear();
			p[i].angang.clear();
			p[i].shezhang.clear();
			p[i].inhand.clear();	
			p[i].shezhang_notfulu.clear();		
		}
		paishan = stdpaishan;
		mt19937 rng(time(0));
		shuffle(paishan.begin(), paishan.end(), rng);
		lft = 0, rht = paishan.size() - 1;
		baopai.push_back(paishan[rht --]);
		gang = 0, xunshu = 0;
		changfeng = cf, zhuangjia = zj;
		for(int i = 1 ; i <= 13 ; i ++){
			for(int j = 0 ; j < 4 ; j ++)
				p[j].inhand.push_back(paishan[lft ++]);
		}
		for(int i = 0 ; i < 4 ; i ++)
			sort(p[i].inhand.begin(), p[i].inhand.end());
	}
	void open_dora(){
		baopai.push_back(paishan[rht --]);
	}
};
void network_flow(const char*, int siz);
#endif
