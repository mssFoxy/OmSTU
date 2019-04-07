#include <windows.h>
#include <process.h>
#include <stdio.h>

HANDLE hstdout;//для вывода
DWORD actlen;//вспомогательная переменная
CRITICAL_SECTION csec;//для синхронизации процесса
HDC hdc;//дискриптер рисовалки
HWND hwnd;//дискриптер окна
RECT clientRect;//размер окна
HANDLE hstdin;//перменная для ввода
DWORD cNumRead;//DWORD = int
INPUT_RECORD inBuf[128];
CRITICAL_SECTION csec;//средство синхронизации

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

int knifeX = 330, knifeY = 300;//координаты ножа
int knifeIsTaken = 0;


struct phylo{
    int sleep;//спит
    int moveToPie;//тянется к пирогу
    int moveToVine;//тянется к вину
    int takingPie;//отрезает пирог
    int takingVine;//берёт вино
    int moveToKnife;//тянется к ножу
    int returning;//возвращение руки
    int sleepTime;//время сна
    int moveSpeed;//скорость перемещения руки
    int armX;//координаты начала руки
    int armY;//
    int armEndX;//координаты конца руки
    int armEndY;//
    int armNormX;
    int armNormY;
    int posX;//координаты философа
    int posY;//
};

struct phylo phyloList[6];

void phyloThread(void *_num){//функция работы философов
    //берём нож
    int num = _num;
    while(1){
        //printf("\n%d", num);
        if(phyloList[num].moveToKnife == 1 && knifeIsTaken == 0 && pieCount > 0)
        {//проверка координат конца руки
            if(phyloList[num].armEndX > knifeX){
                phyloList[num].armEndX--;
            }
            if(phyloList[num].armEndY > knifeY){
                phyloList[num].armEndY--;
            }
            if(phyloList[num].armEndX < knifeX){
                phyloList[num].armEndX++;
            }
            if(phyloList[num].armEndY < knifeY){
                phyloList[num].armEndY++;
            }

            if(phyloList[num].armEndX == knifeX && phyloList[num].armEndY == knifeY){//если добрались до ножа то он занят
                knifeIsTaken = 1;
                phyloList[num].moveToKnife = 0;// не двигает к ножу
                phyloList[num].moveToPie = 1;// двигает ругу к пирогу
            }
        }
        //берём пирог
        if(phyloList[num].moveToPie == 1 && pieCount > 0){//проверка координат конца руки
            if(phyloList[num].armEndX > 400){
                phyloList[num].armEndX--;
            }
            if(phyloList[num].armEndY > 350){
                phyloList[num].armEndY--;
            }
            if(phyloList[num].armEndX < 400){
                phyloList[num].armEndX++;
            }
            if(phyloList[num].armEndY < 350){
                phyloList[num].armEndY++;
            }
            knifeX = phyloList[num].armEndX;
            knifeY = phyloList[num].armEndY;

            if(phyloList[num].armEndX == 400 && phyloList[num].armEndY == 350){//если добрались до пирога
            Sleep(phyloList[num].sleepTime/2);
            pieCount--;
            knifeX = 330, knifeY = 300;
            knifeIsTaken = 0;
            phyloList[num].moveToPie = 0;// не двигает руку к пирогу
            phyloList[num].moveToVine = 1;// двигает руку к вину
            
            }
        }
        //берём вино
        if(phyloList[num].moveToVine == 1 && vineCount > 0){//проверка координат конца руки
            if(phyloList[num].armEndX > 485){
                phyloList[num].armEndX--;
            }
            if(phyloList[num].armEndY > 400){
                phyloList[num].armEndY--;
            }
            if(phyloList[num].armEndX < 485){
                phyloList[num].armEndX++;
            }
            if(phyloList[num].armEndY < 400){
                phyloList[num].armEndY++;
            }

            if(phyloList[num].armEndX == 485 && phyloList[num].armEndY == 400){//если добрались до вина
            Sleep(phyloList[num].sleepTime/2);
            vineCount--;
            phyloList[num].returning = 1;// двигает руку обратно
            phyloList[num].moveToVine = 0;
            
            }
        }

        //дивагает руку обратно
        if(phyloList[num].returning == 1){//проверка координат конца руки
            if(phyloList[num].armEndX > phyloList[num].armNormX){
                phyloList[num].armEndX--;
            }
            if(phyloList[num].armEndY > phyloList[num].armNormY){
                phyloList[num].armEndY--;
            }
            if(phyloList[num].armEndX < phyloList[num].armNormX){
                phyloList[num].armEndX++;
            }
            if(phyloList[num].armEndY < phyloList[num].armNormY){
                phyloList[num].armEndY++;
            }

            if(phyloList[num].armEndX == phyloList[num].armNormX && phyloList[num].armEndY == phyloList[num].armNormY){//если добрались в нормальное положение руки
            
            Sleep(phyloList[num].sleepTime);
            phyloList[num].returning = 0;// двигает руку обратно
            phyloList[num].moveToKnife = 1;
            }
        }
        Sleep(1);
       /* int h = 512;
        int c = 0;
        for(int i = 1; i < h; i++)
        {
            for(int n = 1; n < h; n++)
            {
                    c = i+n/i*n+n%i-i%n;
            }
        }*/
    }
}
HBRUSH grayBrush;
HBRUSH redBrush;
HBRUSH greenBrush;
void draw(){//рисование
    FillRect (hdc, &clientRect, grayBrush);//заливка фона
    Ellipse (hdc, 200, 150, 600, 550);//отрисовка стола

    if(pieCount > 0){//отрисовка первой части пирога
        RECT rct ={350, 300, 400, 350};
        FillRect (hdc, &rct, greenBrush);
    }
    if(pieCount > 1){//отрисовка второй части пирога
        RECT rct ={400, 300, 450, 350};
        FillRect (hdc, &rct, greenBrush);
    }
    if(pieCount > 2){//отрисовка третьей части пирога
        RECT rct ={350, 350, 400, 400};
        FillRect (hdc, &rct, greenBrush);
    }
    if(pieCount > 3){//отрисовка четвёртой части пирога
        RECT rct ={400, 350, 450, 400};
        FillRect (hdc, &rct, greenBrush);
    }

    RECT rct ={ knifeX, knifeY, knifeX + 10, knifeY + 50};//отрисовка ножа
        FillRect (hdc, &rct, grayBrush);

    if(vineCount > 0){//отрисовка вина
        Rectangle (hdc, 460, 400, 510, 500);
        Rectangle (hdc, 475, 360, 495, 400);
        RECT rct ={460, 500 - vineCount*5, 510, 500};
        FillRect (hdc, &rct, redBrush);
    }

    for(int i = 0; i < 6; i++){//отрисовка философов
        int x, y;
        x = phyloList[i].posX;
        y = phyloList[i].posY;
        Ellipse (hdc, x-40, y-40, x+40, y+40);
        MoveToEx(hdc, phyloList[i].armX, phyloList[i].armY, NULL);
        LineTo(hdc, phyloList[i].armEndX, phyloList[i].armEndY);
    }
}
void start(int phyloCount){//создание философов

}
void main(){
    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);//получение ссылки на окно вывода
    hstdin = GetStdHandle(STD_INPUT_HANDLE);//ссылка на стандартный ввод
    constructWindow(100, 40);
    hwnd = GetConsoleWindow();//получение информации об окне
    hdc = GetDC(hwnd);//сохранение дискриптора контекста окна (то как рисуется)
    GetClientRect(hwnd, &clientRect);//получение размера окна
    SetConsoleMode(hstdin, ENABLE_MOUSE_INPUT | ENABLE_PROCESSED_INPUT);//включение режима консоли с вводом мыши 
                                                                // и управляющих комбинаций клавиш (ctrl +c)

    struct phylo phylo1;
    phylo1.moveToKnife = 1;
    phylo1.posX = 400;
    phylo1.posY = 100;
    phylo1.armX = 360;
    phylo1.armY = 100;
    phylo1.armEndX = 330;
    phylo1.armEndY = 150;
    phylo1.armNormX = 330;
    phylo1.armNormY = 150;
    phylo1.sleepTime = 2000;
    phyloList[0] = phylo1;

     struct phylo phylo2;
    phylo2.moveToKnife = 1;
    phylo2.posX = 200;
    phylo2.posY = 200;
    phylo2.armX = 160;
    phylo2.armY = 200;
    phylo2.armEndX = 130;
    phylo2.armEndY = 250;
    phylo2.armNormX = 130;
    phylo2.armNormY = 250;
    phylo2.sleepTime = 3000;
    phyloList[1] = phylo2;

     struct phylo phylo3;
    phylo3.moveToKnife = 1;
    phylo3.posX = 150;
    phylo3.posY = 400;
    phylo3.armX = 110;
    phylo3.armY = 400;
    phylo3.armEndX = 90;
    phylo3.armEndY = 450;
    phylo3.armNormX = 90;
    phylo3.armNormY = 450;
    phylo3.sleepTime = 4000;
    phyloList[2] = phylo3;

     struct phylo phylo4;
    phylo4.moveToKnife = 1;
    phylo4.posX = 600;
    phylo4.posY = 200;
    phylo4.armX = 560;
    phylo4.armY = 200;
    phylo4.armEndX = 530;
    phylo4.armEndY = 250;
    phylo4.armNormX = 530;
    phylo4.armNormY = 250;
    phylo4.sleepTime = 5000;
    phyloList[3] = phylo4;

     struct phylo phylo5;
    phylo5.moveToKnife = 1;
    phylo5.posX = 650;
    phylo5.posY = 400;
    phylo5.armX = 610;
    phylo5.armY = 400;
    phylo5.armEndX = 590;
    phylo5.armEndY = 450;
    phylo5.armNormX = 590;
    phylo5.armNormY = 450;
    phylo5.sleepTime = 6000;
    phyloList[4] = phylo5;

     struct phylo phylo6;
    phylo6.moveToKnife = 1;
    phylo6.posX = 400;
    phylo6.posY = 600;
    phylo6.armX = 360;
    phylo6.armY = 600;
    phylo6.armEndX = 330;
    phylo6.armEndY = 650;
    phylo6.armNormX = 330;
    phylo6.armNormY = 650;
    phylo6.sleepTime = 7000;
    phyloList[5] = phylo6;
    int x = 0;
    int y = 0;

    for(int i = 0; i< 6; i++){
        HANDLE hthr;
        unsigned long lthr;
        hthr = (HANDLE)_beginthread(phyloThread, 0,(void*)i);
    }



     SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0, 0, 0)));//создание кисти для рисования линий 
     grayBrush = CreateSolidBrush(RGB(128,128,128));
     redBrush = CreateSolidBrush(RGB(255,0,0));
     greenBrush = CreateSolidBrush(RGB(128,255,128));

    while(1){
        
        GetNumberOfConsoleInputEvents(hstdin, &cNumRead);//получить число событий ввода
        if(cNumRead > 0){
            ReadConsoleInput(hstdin, inBuf, 128, &cNumRead);
            if(inBuf[0].EventType == MOUSE_EVENT)
            {//Ессли событие ввода поступило отмыши
                if(inBuf[0].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)//если это нажатие кнопки мыши
                {
                    COORD mp = inBuf[0].Event.MouseEvent.dwMousePosition;
                    x = mp.X*8;
                    y = mp.Y*16;
                    //получаем координаты указателя
                    addPie(mp.X*8,mp.Y*16);//добавляем пирог
                    addVine(mp.X*8,mp.Y*16);//добавляем вино
                }
            }
            //printf("\nnum: %d", cNumRead);
        }
        

       /* phyloThread(0);
        phyloThread(1);
        phyloThread(2);
        phyloThread(3);
        phyloThread(4);
        phyloThread(5);*/

        draw();
        Sleep(1);
    }
}