#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define TRUE 1

enum ivent {NONE = 0,GOAL = 1,TRAP = 2,STEP = 3,WARP = 4,BRANCH = 5};
enum color {BLACK = 0,RED = 1,GREEN = 2,YELLOW = 3,BLUE = 4,PARPLE = 5,EMELALD = 6,WHITE = 7,DEFAULT = 8};

/*データ構造*/
typedef struct mas MAS;

typedef unsigned char IVENT_ID;
typedef unsigned char BOOL;

typedef struct koma{
    MAS *mas;
    enum color forecolor;
    /*enum color backcolor;*/
    char character[2];
}KOMA;

typedef struct player{
    BOOL goal_flag;
    BOOL trap_flag;
    char name[32/*?*/];
    int/*?*/ rank;
    KOMA *koma;
    int step;
    struct player *next;
}PLAYER;

typedef struct komalist KOMALIST;

typedef struct komalist{
    KOMA *koma;
    KOMALIST *next;
}KOMALIST;

typedef struct mas{
    IVENT_ID ivent_id;
    void *ivent_param;
    KOMALIST komalist;
    int index;
    struct mas *next;
    struct mas *prev;
}MAS;

typedef struct game{
    MAS *start_mas;
    PLAYER *first_player;
    int rank_cnt;
    int player_cnt;
}GAME;

/*typedef struct ivent_none_param{
}IVENT_NONE_PARAM;*/

/*typedef struct ivent_goal_param{
}IVENT_GOAL_PARAM;*/

/*typedef struct ivent_trap_param{
}IVENT_TRAP_PARAM;*/

typedef struct ivent_step_param{
    int step;
}IVENT_STEP_PARAM;

typedef struct ivent_warp_param{
    MAS *to;
}IVENT_WARP_PARAM;

/*関数*/
void KOMA_print(KOMA *koma);

#define esc_forecolor(color) printf("\33[3%dm",color)
/*#define esc_backcolor(color) printf("\33[4%dm",color)*/

/*KOMALIST*/

KOMALIST *KOMALIST_make(KOMA *koma){
    KOMALIST *komalist;

    komalist = (KOMALIST *)malloc(sizeof(KOMALIST));
    komalist->koma = koma;
    komalist->next = NULL;
    return komalist;
}

/*end KOMALIST*/

/*MAS*/
void MAS_delete_koma(MAS *mas,KOMA *koma){
    KOMALIST *list,*prev_list;

    list = prev_list = &mas->komalist;
    while( (list = list->next)!=NULL ){
        if (list->koma == koma){
            prev_list->next = list->next;
            free(list);
            break;
        }
        prev_list = list;
    }
}

void MAS_insert_koma(MAS *mas,KOMA *koma){
    KOMALIST *list;

    list = &mas->komalist;
    while(list->next != NULL){
        list = list->next;
    }

    list->next = KOMALIST_make(koma);
}

void MAS_print_ivent(enum ivent ivent_id,void *ivent_param){
    switch(ivent_id){
        case NONE:
        printf("");
        break;

        case GOAL:
        printf("ゴール");
        break;

        case TRAP:
        printf("1回休み");
        break;

        case STEP:
        printf("%d",abs(((IVENT_STEP_PARAM *)ivent_param)->step));
        if (((IVENT_STEP_PARAM *)ivent_param)->step >= 0){
            printf("進む");
        }else{
            printf("戻る");
        }
        break;

        case WARP:
        printf("%dへワープ",((IVENT_WARP_PARAM *)ivent_param)->to->index);
        break;

    }
}

MAS *MAS_make(int index){
    MAS *mas;
    mas = (MAS *)malloc(sizeof(MAS));
    mas->ivent_id = NONE;
    mas->ivent_param = NULL;
    mas->index = index;
    mas->next = NULL;
    mas->prev = NULL;

    return mas;
}

void MAS_print_koma(MAS *mas){
    KOMALIST *list;

    list = &mas->komalist;
    while( (list = list->next)!=NULL ){
        KOMA_print(list->koma);
    }
}

void MAS_print(MAS *mas){
    /*if (mas->prev != NULL){
        printf("↓\n");
    }*/
    //IVENT_ID ivent_id;
    //void *ivent_param;
    //POSITION position;
    printf("%d",mas->index);
    printf("[");
    MAS_print_koma(mas);
    MAS_print_ivent(mas->ivent_id,mas->ivent_param);
    printf("]\n");

    if (mas->next != NULL){
        printf("↓\n");
    }
}
/*end MAS*/

/*KOMA*/
KOMA *KOMA_make(enum color forecolor){
    KOMA *koma;

    koma = (KOMA *)malloc(sizeof(KOMA));

    koma->mas = NULL;
    koma->forecolor = forecolor;
    strcpy(koma->character,"★");

    return koma;
}

void KOMA_print(KOMA *koma){
    esc_forecolor(koma->forecolor);
    printf("%c%c",koma->character[0],koma->character[1]);
    esc_forecolor(BLACK);
}
/*end KOMA*/

/*PLAYER*/
PLAYER *PLAYER_make(enum color forecolor){
    PLAYER *player;
    char name[32],dummy;

    player = (PLAYER *)malloc(sizeof(PLAYER));

    player->goal_flag = FALSE;
    player->trap_flag = FALSE;
    player->koma = KOMA_make(forecolor);
    printf("名前:");
    scanf("%s%c",name,&dummy);
    strcpy(player->name,name);
    player->rank = 0;
    player->step = 0;
    player->next = NULL;

    return player;
}

void PLAYER_print(PLAYER *player){
    KOMA_print(player->koma);
    printf("%s\n",player->name);
    if (player->goal_flag){
        printf("ゴール\n");
        printf("順位:%d\n",player->rank);
    }
    if (player->trap_flag){
        printf("1回休み\n");
    }
    printf("\n");
}
/*end PLAYER*/

BOOL ask_make_player(){
    printf("プレイヤを登録しますか?(y/n)\n");
    while(1){
        char ch,dummy;
        scanf("%c%c",&ch,&dummy);
        switch(ch){
            case 'y':
                return TRUE;
            case 'n':
                return FALSE;
        }
    }
}

/*void print_players(PLAYER *player){
    while(player != NULL){
        PLAYER_print(player);
        player = player->next;
    }
}*/

void print_mases(MAS *mas,int fore_n,int back_n){
    int n;
    n = fore_n + back_n;

    while(back_n && mas->prev != NULL){
        mas = mas->prev;
        back_n--;
    }

    n -= back_n;

    while(n && mas != NULL){
        MAS_print(mas);
        mas = mas->next;
        n--;
    }
}

void koma_move(KOMA *koma,MAS *mas){
    if (koma->mas != NULL) MAS_delete_koma(koma->mas,koma);
    MAS_insert_koma(mas,koma);
    koma->mas = mas;
    /*koma->next_masはイベントで格納*/
}

void wait_push_enter(){
    char dummy;
    scanf("%c",&dummy);
}

/*GAME*/
GAME *GAME_make(){
    GAME *game;

    game = (GAME *)malloc(sizeof(GAME));
    game->start_mas = NULL;
    game->first_player = NULL;
    game->rank_cnt = 0;
    game->player_cnt = 0;

    return game;
}

MAS *GAME_find_mas_from_index(GAME *game,int index){
    MAS *mas;

    mas = game->start_mas;
    while(mas != NULL){
        if (mas->index == index) return mas;
        mas = mas->next;
    }
    return NULL;
}

void GAME_make_mases(GAME *game){
    MAS *mas;
    int index;

    index = 0;
    mas = game->start_mas = MAS_make(index);
    while(rand()%32){
        int rand_num;

        index++;
        mas->next = MAS_make(index);
        rand_num = rand()%10;
        if (rand_num < 5){
            /*50%*/
            mas->next->ivent_id = NONE;
            mas->next->ivent_param = NULL;
        }else if (rand_num < 7){
            /*20%*/
            mas->next->ivent_id = TRAP;
            mas->next->ivent_param = NULL;
        }else if (rand_num < 9){
            /*20%*/
            mas->next->ivent_id = STEP;
            mas->next->ivent_param = (IVENT_STEP_PARAM *)malloc(sizeof(IVENT_STEP_PARAM));
            ((IVENT_STEP_PARAM *)mas->next->ivent_param)->step = rand()%13-6;
        }else{
            /*10%*/
            mas->next->ivent_id = WARP;
            mas->next->ivent_param = (IVENT_WARP_PARAM *)malloc(sizeof(IVENT_WARP_PARAM));
            ((IVENT_WARP_PARAM *)mas->next->ivent_param)->to = NULL;/*あとで決定*/
        }

        mas->next->prev = mas;
        mas = mas->next;
    }

    index++;
    mas->next = MAS_make(index);
    mas->next->ivent_id = GOAL;
    mas->next->ivent_param = NULL;
    mas->next->prev = mas;
    mas = mas->next;

    mas = game->start_mas;
    index++;
    while(mas != NULL){
        if (mas->ivent_id == WARP){
            ((IVENT_WARP_PARAM *)mas->ivent_param)->to = GAME_find_mas_from_index(game,rand()%index);
        }
        mas = mas->next;
    }

}

void GAME_put_player_start(GAME *game){
    PLAYER *player;

    player = game->first_player;
    while(player != NULL){
        koma_move(player->koma,game->start_mas);
        player = player->next;
    }
}

void GAME_run_ivent(GAME *game,PLAYER *player){
    switch(player->koma->mas->ivent_id){
        case NONE:
        break;
       
        case GOAL:
            if(player->step == 0){
                /*ゴール処理*/
                player->goal_flag = TRUE;
                player->rank = game->rank_cnt;
                game->rank_cnt++;
            }else{
                /*戻る処理*/
                player->step *= -1;
            }
        break;
       
        case TRAP:
            if(player->step == 0){
                /*1回休み処理*/
                player->trap_flag = TRUE;
            }
        break;
       
        case STEP:
            if(player->step == 0){
                /*n進む/戻る処理*/
                player->step = ((IVENT_STEP_PARAM *)player->koma->mas->ivent_param)->step;
            }
        break;
       
        case WARP:
            if(player->step == 0){
                /*ワープ処理*/
                koma_move(player->koma,((IVENT_WARP_PARAM *)player->koma->mas->ivent_param)->to);
            }
        break;
       
    }
}

void GAME_move_player(GAME *game,PLAYER *player){
    while(player->step != 0){
        printf("step:%d\n",player->step);
        sleep(1);
        if (player->step >= 0){
            if (player->koma->mas->next != NULL) koma_move(player->koma,player->koma->mas->next);
            player->step--;
        }else{
            if (player->koma->mas->prev != NULL) koma_move(player->koma,player->koma->mas->prev);
            player->step++;
        }
        GAME_run_ivent(game,player);

        PLAYER_print(player);
        print_mases(player->koma->mas,7,6);
    }
}

