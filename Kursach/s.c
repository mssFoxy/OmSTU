#include <windows.h>
#include <stdio.h>

HANDLE hstdout;//для вывода
DWORD actlen;//
CRITICAL_SECTION csec;//для синхронизации процесса


void constructWindow(int x, int y){//создание окна
    COORD size;
    size.X = x;
    size.Y = y;
    SMALL_RECT windowSize= {0, 0, x-1, y-1};//SMALL_RECT-координаты прямоугольной области
    system("CLS");
    SetConsoleWindowInfo(hstdout, TRUE, &windowSize);//задаётся размер окна
    SetConsoleScreenBufferSize(hstdout, size);//задаётся размер буфера символов
}
void addVine(int x, int y){//добавление вина

}
void addPie(int x, int y){//добавление пирога

}
void phyloThread(){//функция работы философов

}
void draw(){//рисование

}
void start(int phyloCount){//сохдание философов

}
void main(){
    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);//получение ссылки на окно
    constructWindow(100, 40);
    sleep(10000000);
    //start(6);
    
}