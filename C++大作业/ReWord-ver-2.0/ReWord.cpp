# include "./lib/LocalAccount.h"
# include "./lib/LocalAccount.cpp"
# include "./lib/Game.h"
# include "./lib/Game.cpp"
# include "./lib/Dict.h"
# include "./lib/Dict.cpp"

int main(){
    int flag;
    string str;
    Game* new_game = new Game();
    new_game->MainMenu(&flag);
    delete new_game;
    return 0;
}
/*
目前bug:
修改昵称/密码时输入缓冲区清空问题
*/