#include "main.h"
#include <QApplication>

static map_main *main_ptr = NULL;

map_main * Win::GetMainWin(){
    return main_ptr;
}

void Win::SetMainWin(map_main* ptr){
    main_ptr = ptr;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    map_main w;

    Win::SetMainWin(&w);
    w.show();

    return a.exec();
}
