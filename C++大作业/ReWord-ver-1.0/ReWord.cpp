# include "LocalAccount.h"
# include "LocalAccount.cpp"
# include "Game.h"
# include "Game.cpp"
# include "Dict.h"
# include "Dict.cpp"

int main(){
    int flag;
    string str;
    cout<<&flag<<endl;
    Game* new_game = new Game();
    new_game->MainMenu(&flag);
    delete new_game;
    cout<<"Ending..."<<endl;
    cin>>flag;
    return 0;
}
/*
目前bug:
挑战者挑战功能未实现
*/