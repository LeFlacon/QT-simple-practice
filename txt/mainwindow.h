#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextCharFormat>
#include"QEvent"
#include<QDialog>
#include<QLineEdit>
#include<QPushButton>
#include<QRadioButton>
#include<QGroupBox>
#include<QCheckBox>
#include<QLabel>
#include<QCloseEvent>

class QLineEdit;
class QDialog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionxinjian_triggered();
    void on_actiondakai_triggered();
    void on_actionbaocun_triggered();
    void on_actionlincunwei_triggered();
    void closeEvent(QCloseEvent *);
    void on_actionyanse_triggered();
    void on_actionziti_triggered();
    void on_actionguanyu_triggered();
    void textFind();
    void findNext();
    void on_pushButton_clicked();
    void on_actionquanshan_triggered();

private:
    Ui::MainWindow *ui;
    QString curFile;
    QLineEdit *lineEdit;
    QDialog *findDialog;
    bool isSaved;
    int num;
    int linenum;
    QLabel* findtext_label;
    QLineEdit* text_edit;
    QCheckBox* matchCase_checkbox;
    QGroupBox* turns_groupbox;
    QRadioButton* goFornt_button;
    QRadioButton* goBack_button;
    QPushButton* find_button;
    QPushButton* cancel_button;

    void file_New();
    void file_SaveOrNot();
    void file_Save();
    void file_SaveAs();
    bool saveFile(const QString& fileName);
    void file_open();
    bool file_load(const QString& fileName);
    void aboutMyEditor();
    void mergeFormat(QTextCharFormat fmt);
    void change_fonts();
    void Finding();
    void zhuangtaibar();
    void some_action();
    void search_init();
    QString Position;

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // MAINWINDOW_H
