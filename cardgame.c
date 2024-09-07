#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

#define SUIT 4	                // カードの種類の数
#define SUIT_LEN 13		    // 手札の種類の文字列長
#define RANK 13		        // 種類ごとのカードの枚数
#define INDEX 8		        // 手札の最大枚数

typedef struct {              // 手札の構造体
	char suit[SUIT_LEN];       // 種類文字列
	int cardNum;               // 札番号
} Card;

typedef struct {             // カードセットの構造体
	Card card [SUIT * RANK];  // カード配列
	int max;                  // カード枚数
} CardSet;

typedef struct {             // プレイヤーの構造体
	Card card [INDEX];        // 手札配列（最大８枚）
	int number;               // 手札枚数
} Player;

//--- （２）カードセットを作成 ---//
void initDeck(CardSet *trump)
{
	char *mark[] = {"クラブ", "ダイヤ", "ハート", "スペード"};
	int n[] = {1,2,3,4,5,6,7,8,9,10,11,12,13};
	int mai = 0;

	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 13; j++){
			strcpy(trump->card[mai].suit , mark[i]);
			trump->card[mai].cardNum = n[j];
//			printf("%s<%d>\n",trump->card[mai].suit,trump->card[mai].cardNum);
			mai++;
		}
	}
	trump->max = 0;
}

//--- （３）カードをシャッフルする ---//
void shuffle(CardSet *trump)
{
	initDeck(trump);
	int mai = 52;
	Card temp;

	do{
		int r = rand() % mai--;
//		printf("%s<%d>\n",trump->card[r].suit,trump->card[r].cardNum);
		temp = trump->card[mai];
		trump->card[mai] = trump->card[r];
		trump->card[r] = temp;

	}while(mai > 0);

	trump->max = SUIT * RANK;
}

//--- （４）カードを1枚引く ---//
Card nextCard(CardSet *trump)
{
	Card ret;

	ret = trump->card[trump->max - 1];
	trump->max -= 1;

	return ret;
}

//--- （５）プレイヤーに手札を2枚配る ---//
void deal(Player *p, CardSet *trump)
{
	int kaisu = 2;

	p->number = 0;

	while(kaisu > 0){
		p->card[p->number] = nextCard(trump);
		p->number += 1;
		kaisu--;
	}
}

//--- （２－１）ポイントの定義 ---//
int toPoints(int cardNum)
{
	if(cardNum <= 10){
		return cardNum;
	}else if(cardNum > 10){
		return 10;
	}else
		return 0;
}

//--- （２－２）ポイントの合計値を返す ---//
int sum(Player p)
{
	int num = 0;
	int num2 = 0;

	while(p.number > 0){
		p.number--;
		num = toPoints(p.card[p.number].cardNum);
		num2 += num;
	}
	return num2;
}

//--- （７）手札を表示する ---//
void show(Player ko, Player oya)
{
	printf("□Computer");
	printf("(%dP)", sum(oya));

	int j = 0;
	while(j < oya.number){
		printf("   %4s<%2d>  ",oya.card[j].suit,oya.card[j].cardNum);
		j++;
	}
	putchar('\n');

	printf("■Player  ");
	printf("(%dP)", sum(ko));

	int i = 0;
	while(i < ko.number){
		printf("  %4s<%2d>  ",ko.card[i].suit,ko.card[i].cardNum);
		i++;
	}
}

//--- （７ー１） ---//
void disp(Player ko, Player oya)
{
	printf("□Computer");
	printf("(--P)");

	printf("   %4s<%2d>  ",oya.card[0].suit,oya.card[0].cardNum);
	printf("********<-->  ");
	putchar('\n');


	printf("■Player  ");
	printf("(%dP)", sum(ko));

	int i = 0;
	while(i < ko.number){
		printf("  %4s<%2d>  ",ko.card[i].suit,ko.card[i].cardNum);
		i++;
	}
}

//--- （５－１） ---//
void hit(Player *ko, CardSet *trump, Player oya)
{
	int no;

	while(sum(*ko) < 21){
		printf("\n");
		printf("さらにカードを引きますか　【Yes…1 / No…0】：");
		fflush(0);
		scanf("%d", &no);
		putchar('\n');

		if(no == 0){
			break;
		}

		ko->card[ko->number] = nextCard(trump);
		ko->number += 1;

		disp(*ko, oya);
	}
}

//--- （６－１） ---//
void dealer(Player *oya, CardSet *trump)
{
//	putchar('\n');

	while(sum(*oya) <= 16){
		oya->card[oya->number] = nextCard(trump);
		oya->number += 1;
//		putchar('\n');
	}
}

//--- （７－３） ---//
void judge(Player ko, Player oya)
{
	int c = sum(oya);
	int p = sum(ko);

	printf("\n");

	if(( p == c ) || ( p > 21 && c > 21 )){
		printf("引き分け\n");
	}else if(( p <= 21 ) && ( c > 21 || c < p )){
		printf("子(Player)の勝ち\n");
	}else{
		printf("親(Computer)の勝ち\n");
	}
}

//--- （７－３） ---//
int cip_sum(Player ko, Player oya, int cip, int kake)
{
	int c = sum(oya);
	int p = sum(ko);
	int sum = 0;

	if(( p == c ) || ( p > 21 && c > 21 )){
		sum = cip - kake;
	}else if(p == 21){
		sum = cip + ( kake * 2 );
	}else if(( p <= 21 ) && ( c > 21 || c < p )){
		sum = cip + kake;
	}else{
		sum = cip - kake;
	}
	return sum;
}

//--- （７－４）---//
void animation()
{
    putchar('\n');
    putchar('\n');
    for (int i = 0; i < 10; i++) {
        printf("☆☆");
        fflush(0);
        Sleep(100);
    }
   putchar('\n');
   putchar('\n');
}

int main(void)
{

	srand(time(NULL));		// 最初に乱数を初期化(main関数の中で一度だけ記述)

	CardSet deck;    // カードデッキ
	Player pHand;    // 子（プレーヤー）
	Player cHand;    // 親（コンピュータ)
	int no, cip, kake;

	printf("■□■□■□■□ Blackjack Start ■□■□■□■□■□\n");
	printf("\n");

	initDeck(&deck);

	printf("手持ちのチップの枚数は？：");
	fflush(0);
	scanf("%d",&cip);
	printf("\n");

	do{
		if(deck.max <= (INDEX * 2)){
			shuffle(&deck);
		}

		deal(&pHand, &deck);
		deal(&cHand, &deck);

		disp(pHand, cHand);

		do{
			printf("\n賭けるチップの枚数は？：");
			fflush(0);
			scanf("%d",&kake);
			if(kake > cip){
				printf("手持ちのチップを超えています。再入力してください。\n");
			}
		}while(kake > cip);

		hit(&pHand, &deck, cHand);

		dealer(&cHand, &deck);
		animation();
		show(pHand, cHand);
		judge(pHand, cHand);

		cip = cip_sum(pHand, cHand, cip, kake);
		printf("\n[[現在のチップの枚数：%d]]\n",cip);


		if(cip > 0){
			printf("\nゲームを続けますか【Yes…1 / No…0】：");
			fflush(0);
			scanf("%d", &no);
		}else{
			printf("GameOver\n");
			break;
		}

	}while(no > 0);

	printf("\n");
	printf("■□■□■□■□ Blackjack End ■□■□■□■□■□\n");

	return 0;

}



