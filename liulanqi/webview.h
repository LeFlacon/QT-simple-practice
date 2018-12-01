#ifndef WEBVIEW_H
#define WEBVIEW_H
#include<QWebEngineView>
class MainWindow;

class WebView : public QWebEngineView
{
    Q_OBJECT
public:
    WebView(QWidget *parent=nullptr);
protected:
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type);//子窗口继承父窗口函数
private:
    MainWindow *mainWindow;
};

#endif // WEBVIEW_H
