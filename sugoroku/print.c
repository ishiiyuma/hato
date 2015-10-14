void print_player(struct player *player){//プレイヤのステータスを表示する．
}

void print_koma(struct player *player){//プレイヤのコマを表示する．
}

void print_mas(){//各マスを表示する．
	if (1/*マスにプレイヤが乗っていたら*/) print_koma();
}

void print_board(struct mas *origin,int fore,int back){//前後fore,backマスのボードを表示する．
	while(1/*表示するマスのループ*/){
		print_mas();
	}
}

void print_game(struct game *game){
	print_player(game->player[game->player_index]);
	print_board(NULL,7,6);
}

