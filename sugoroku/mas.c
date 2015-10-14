#define EVENT_NONE 0
#define EVENT_TRAP 1
/*まだある*/

struct mas{
	int index;
	int event_id;
	void *event_param;
	unsigned char player_exist_flag[MAX_PLAYER];
	struct mas *next_left
	struct mas *next_right;
	struct mas *prev;
};

struct mas *mas_make_board(){
	return NULL/*スタートのマスのポインタを返す*/;
}

