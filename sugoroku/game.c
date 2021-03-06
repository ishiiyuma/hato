/*関数の引数に1やNULLがあるところは，未定義な部分*/
#define true 1
#define false 0

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "player.c"
#include "mas.c"
#include "print.c"

struct game{
	int player_index;
	int player_num;
	struct player player[MAX_PLAYER];
	struct mas *start_mas;
};

void game_init(struct game *game){
	player_index = 0;
	player_num = 0;
	/*player[]の初期化は不要*/
	start_mas = NULL;
}

int saikoro(){
	return rand()%6+1;/*?*/
}

void game_move_player_main(struct game *game,struct player *player){
	while(player->step != 0){
		printf("step:%d\n",player->step);
		sleep(1);
		if (player->step >= 0){
            if (player->koma->mas->next != NULL) game_move_player(player,player->koma->mas->next);
            player->step--;
        }else{
            if (player->koma->mas->prev != NULL) koma_move_player(player->koma,player->koma->mas->prev);
            player->step++;
        }
        game_run_event(game);

        player_game(&game);
    }
}

void game_run(struct game *game){
	player_make_players(game->player);
	game->start_mas = mas_make_board();
	
	game_put_player_start();

	while(1/*各プレイヤのループ*/){
		print_game(game);

		printf("Push Enter\n");
		wait_push_enter();

		saikoro();
		game_move_player_main(game,NULL);
        	}
}

int main(){
	struct game game;
	
	srand((unsigned)time(NULL));
	
	game_init(&game);
	game_run(&game);
	
	return 1;
}

