#define MAX_PLAYER 16

struct player{
	unsigned char goalflag;
	unsigned char trapflag;
	char name[32];
	int rank;
	int step;
	int money;
	struct item item[MAX_ITEM];
	int item_index;
	
	/*koma*/
	struct mas *mas;
	int forecolor;
	char character[2];
}

void player_init(struct player *player){
}

void make_players(struct player player[/*MAX_PLAYER*/]){
	/*プレイヤを登録していく*/
	while(1/*プレイヤ登録ループ*/){
		player_init(NULL);
	}
}

