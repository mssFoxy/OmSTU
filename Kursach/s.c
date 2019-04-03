#include <windows.h>
#include <stdio.h>

HANDLE hstdout;//для вывода
DWORD actlen;
CRITICAL_SECTION csec;//для синхронизации процесса
HDC hdc;
HWND hwnd;
RECT clientRect;

void constructWindow(int x, int y){//создание окна
    COORD size;
    size.X = x;
    size.Y = y;
    SMALL_RECT windowSize= {0, 0, x-1, y-1};//SMALL_RECT-координаты прямоугольной области
    system("CLS");
    SetConsoleWindowInfo(hstdout, TRUE, &windowSize);//задаётся размер окна
    SetConsoleScreenBufferSize(hstdout, size);//задаётся размер буфера символов
}

int vineCount = 20;

void addVine(int x, int y){//добавление вина
if(x > 460 && x < 510 && y > 400 && y < 500){
        if(vineCount == 0){
            vineCount = 20;
        }
    }
}

int pieCount = 4;

void addPie(int x, int y){//добавление пирога
    if(x > 350 && x < 450 && y > 300 && y < 400){
        if(pieCount == 0){
            pieCount = 4;
        }
    }
}
void phyloThread(){//функция работы философов

}
void draw(){//рисование
    FillRect (hdc, &clientRect, CreateSolidBrush(RGB(128,128,128)));//заливка фона
    SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0, 0, 0)));//создание кисти для рисования линий 
    Ellipse (hdc, 200, 150, 600, 550);//отрисовка стола
    
    if(pieCount > 0){
        RECT rct ={350, 300, 400, 350};
        FillRect (hdc, &rct, CreateSolidBrush(RGB(100, 255, 100)));
    }
    if(pieCount > 1){
        RECT rct ={400, 300, 450, 350};
        FillRect (hdc, &rct, CreateSolidBrush(RGB(100, 255, 100)));
    }
    if(pieCount > 2){
        RECT rct ={350, 350, 400, 400};
        FillRect (hdc, &rct, CreateSolidBrush(RGB(100, 255, 100)));
    }
    if(pieCount > 3){
        RECT rct ={400, 350, 450, 400};
        FillRect (hdc, &rct, CreateSolidBrush(RGB(100, 255, 100)));
    }
    
    if(vineCount > 0){
        Rectangle (hdc, 460, 400, 510, 500);
        Rectangle (hdc, 475, 360, 495, 400);
        RECT rct ={460, 500 - vineCount*5, 510, 500};
        FillRect (hdc, &rct, CreateSolidBrush(RGB(255, 0, 0)));
    }
}
void start(int phyloCount){//создание философов

}
void main(){
    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);//получение ссылки на окно вывода
    constructWindow(100, 40);
    hwnd = GetConsoleWindow();//получение информации об окне
    hdc = GetDC(hwnd);//сохранение дискриптора контекста окна (то как рисуется)
    GetClientRect(hwnd, &clientRect);//получение размера окна

    while(1){
        draw();
        Sleep(1000);
    }
    
    //sleep(1000000000);
    //start(6);
}