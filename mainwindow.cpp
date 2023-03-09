#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLibrary>
#include "windows.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QLibrary cardsLib("cards.dll");
    QLibrary user32Lib("user32.dll");   //find dll in the c:/windows/system32 this is 32 bit build app
//    QLibrary user32Lib("user32.dll");


    cardsLib.load();
    user32Lib.load();

    if(!cardsLib.isLoaded() || !user32Lib.isLoaded()) return;

    int* a = new int;
    int* b = new int;

    typedef int (WINAPI *cdtInitFun)(int *, int *);
    typedef int (WINAPI *GetDCFun)(int);
    typedef int (WINAPI *cdtDrawFun)(int,int,int,int,int,int);
    typedef int (*MessageBoxAFun)(HWND, LPCSTR, LPCSTR, UINT);

    cdtInitFun cdtInit = reinterpret_cast<cdtInitFun>(cardsLib.resolve("cdtInit"));
    GetDCFun GetDC = reinterpret_cast<GetDCFun>(user32Lib.resolve("GetDC"));
    cdtDrawFun cdtDraw = reinterpret_cast<cdtDrawFun>(cardsLib.resolve("cdtDraw"));
    MessageBoxAFun messageBoxA = reinterpret_cast<MessageBoxAFun>(user32Lib.resolve("MessageBoxA"));

    if(!cdtInit || !GetDC || !cdtDraw || !messageBoxA) return;

    messageBoxA(NULL, "Hello from user32.dll!", "Message", MB_OK);

    cdtInit(a, b);

    cdtDraw(GetDC(0), rand() % 1000, rand() % 1000, rand() % 52, 0, 0);

//    QTimer* pTimer = new QTimer(this);
//    pTimer->start(500);

//    connect(pTimer, &QTimer::timeout, [=]()
//    {
//        cdtDraw(GetDC(0), rand() % 1000, rand() % 1000, rand() % 52, 0, 0);
//    });

    delete a; // Free allocated memory
    delete b; // Free allocated memory

}

MainWindow::~MainWindow()
{
    delete ui;
}

