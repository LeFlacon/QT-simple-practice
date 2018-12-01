#include "webview.h"
#include "mainwindow.h"

WebView::WebView(QWidget *parent):QWebEngineView (parent)
{

}

//新建窗口
QWebEngineView *WebView::createWindow(QWebEnginePage::WebWindowType type)
{
    mainWindow=new MainWindow(this);
    mainWindow->show();
    return mainWindow->createView();
}
