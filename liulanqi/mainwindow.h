#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QModelIndex>

class WebView;
class QLineEdit;
class QNetworkAccessManager;
class QListWidget;
class QWebSettings;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    WebView *createView();

private:
    Ui::MainWindow *ui;
    WebView *view;
    QLineEdit *dizhilan;
    int progress;
    QNetworkAccessManager *manager;
    QListWidget *historyList;
    QLineEdit *findEdit;

protected slots:
    void changedizhi();
    void jindu(int);
    void biaoti();
    void jiazaiok(bool);
    void handleIconUrlChanged(const QUrl &url);
    void handleIconLoaded();
    void lishijilu();
    void zhuandao(const QModelIndex &index);
    void chazhao();

private slots:
    void on_actionyincang_triggered();
    void on_actionxianshi_triggered();
    void on_actionabout_triggered();
    void on_actionfusion_triggered();
    void on_actionmacintosh_triggered();
    void on_actionWindows_triggered();

    void on_action_triggered();

    void on_actionfuzhi_triggered();

    void on_actionsave_triggered();

    void on_actionyoujian_triggered();

public slots:
    void slotTableMenu(const QPoint &pos);      //表格右键菜单
    void slotActionCopy(bool checked);          //复制
    void slotActionSave(bool checked);          //另存为

};



#endif // MAINWINDOW_H
