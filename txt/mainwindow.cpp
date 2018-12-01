#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QMessageBox>
#include <QLineEdit>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextCursor>
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>
#include <QTextEdit>
#include <QTextListFormat>
#include <QTextCharFormat>
#include <QColor>
#include <QColorDialog>
#include <QDateTime>
#include <QDate>
#include <QString>
#include <QDragEnterEvent>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <QMimeData>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isSaved=false;
    num=0;
    linenum=0;
    curFile=tr("newfile.txt");
    setWindowTitle(tr("curFile"));
    ui->textEdit->setCurrentFont(QFont(tr("黑体"),36));
    QFile file("/Users/mac/Desktop/newfile.txt");
    file.open(QFile::ReadOnly|QFile::Text);
    QTextStream in(&file);
    Position=in.readAll();
    zhuangtaibar();
    some_action();
    search_init();

    ui->textEdit->setAcceptDrops(false);
    setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//选取
void MainWindow::mergeFormat(QTextCharFormat format)
{
    QTextCursor cursor=ui->textEdit->textCursor();
    if(!cursor.hasSelection()){
        cursor.mergeCharFormat(format);
    }
    cursor.mergeCharFormat(format);
}

//新建文件
void MainWindow::file_New()
{
    file_SaveOrNot();
    isSaved=false;
    curFile=tr("newfile.txt");
    setWindowTitle(tr("curFile"));
    ui->textEdit->clear();
    ui->textEdit->setVisible(true);
}

//是否保存
void MainWindow::file_SaveOrNot()
{
    if(ui->textEdit->document()->isModified()){
        QMessageBox box;
        box.setWindowTitle(tr("warning!"));
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("当前文件尚未保存，是否保存？"));
        box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        if(box.exec()==QMessageBox::Yes)file_Save();
    }
}

//保存
void MainWindow::file_Save()
{
    if(isSaved)saveFile(curFile);
    else file_SaveAs();
    saveFile(curFile);
}

//另存为
void MainWindow::file_SaveAs()
{
    QString filename=QFileDialog::getSaveFileName(this,tr("另存为"),curFile);
    if(!filename.isEmpty())saveFile(filename);
}

//保存文件窗口
bool MainWindow::saveFile(const QString &filename)
{
    QFile file(filename);
    if(!file.open(QFile::WriteOnly|QFile::Text)){
        QMessageBox::warning(this,tr("保存文件"),tr("无法保存文件 %1:\n%2").arg(filename).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    out<<ui->textEdit->toPlainText();
    isSaved=true;
    curFile=QFileInfo(filename).canonicalFilePath();
    setWindowTitle(curFile);
    return true;
}

//帮助-关于
void MainWindow::aboutMyEditor()
{
    QMessageBox::information(nullptr,"About",tr("这是一个简易的文本编辑器\n"
                                          "菜单栏和工具栏的功能都可实现\n"
                                          "更多功能等待开发(^-^)\n"
                                          "---to be continued---\n"
                                          "---from nku1711447---"));
}

//打开文件
void MainWindow::file_open()
{
    file_SaveOrNot();
    QString filename=QFileDialog::getOpenFileName(this);
    if(!filename.isEmpty())file_load(filename);
    isSaved=true;
}

//载入文件
bool MainWindow::file_load(const QString &filename)
{
    QFile file(filename);
    if(!file.open(QFile::ReadOnly|QFile::Text)){
        QMessageBox::warning(this,tr("读取文件"),tr("无法读取文件 %1:\n%2.").arg(filename).arg(file.errorString()));
        return false;
    }
    QTextStream in(&file);
    ui->textEdit->setText(in.readAll());
    curFile=QFileInfo(filename).canonicalFilePath();
    setWindowTitle(curFile);
    ui->textEdit->setVisible(true);
    return true;
}

//转到槽
void MainWindow::on_actionxinjian_triggered()
{
    file_New();
}

void MainWindow::on_actiondakai_triggered()
{
    file_open();
}

void MainWindow::on_actionbaocun_triggered()
{
    file_Save();
}

void MainWindow::on_actionlincunwei_triggered()
{
    file_SaveAs();
}

//关闭窗体事件处理
void MainWindow::closeEvent(QCloseEvent *event)
{
    QString st=ui->textEdit->toPlainText().toUtf8();
    num=st.length();
    if(ui->textEdit->document()->isModified()&&num!=0){
        int btn=QMessageBox::warning(this,tr("warning"),tr("当前文件尚未保存，是否保存？"),QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if(btn==QMessageBox::Yes){
            file_Save();
            event->accept();
        }
        else if(btn==QMessageBox::No)event->accept();
        else event->ignore();
    }
    else
        event->accept();
}

//设置颜色
void MainWindow::on_actionyanse_triggered()
{
    QPalette qp=ui->textEdit->palette();
    const QColor qc=QColorDialog::getColor(Qt::white,this,"Color dialog");
    if(qc.isValid()){
       qp.setColor(QPalette::Text,qc);
       ui->textEdit->setPalette(qp);
    }
}

//设置字体
void MainWindow::on_actionziti_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,this);
    if(ok)ui->textEdit->setCurrentFont(font);
}

//帮助-关于
void MainWindow::on_actionguanyu_triggered()
{
    aboutMyEditor();
}

//状态栏
void MainWindow::zhuangtaibar()
{
    ui->statusBar->showMessage(tr("欢迎使用简易文本编辑器"),5000);
    QLabel *permanent = new QLabel(this);
    permanent->setFrameStyle(QFrame::Box|QFrame::Sunken);
    permanent->setText("nku--1711447");
    ui->statusBar->addPermanentWidget(permanent);
}

//一些textEdit自带函数关联
void MainWindow::some_action()
{
    connect(ui->actionchexiao,SIGNAL(triggered()),ui->textEdit,SLOT(undo()));
    connect(ui->actionchongzuo,SIGNAL(triggered()),ui->textEdit,SLOT(redo()));
    connect(ui->actionjianqie,SIGNAL(triggered()),ui->textEdit,SLOT(cut()));
    connect(ui->actionfuzhi,SIGNAL(triggered()),ui->textEdit,SLOT(copy()));
    connect(ui->actionzhantie,SIGNAL(triggered()),ui->textEdit,SLOT(paste()));
    connect(ui->actionquanxuan,SIGNAL(triggered()),ui->textEdit,SLOT(selectAll()));
    connect(ui->actionshanchu,SIGNAL(triggered()),ui->textEdit,SLOT(cut()));//
}

//查找相关
void MainWindow::search_init()
{
    connect(ui->actionchazhao,&QAction::triggered,this,&MainWindow::textFind);
    findDialog=new QDialog(this);
    lineEdit=new QLineEdit(findDialog);
    QPushButton *btn=new QPushButton(findDialog);
    btn->setText(tr("查找下一个"));
    connect(btn,&QPushButton::clicked,this,&MainWindow::findNext);
    QVBoxLayout *layout=new QVBoxLayout;
    layout->addWidget(lineEdit);
    layout->addWidget(btn);
    findDialog->setLayout(layout);
}

void MainWindow::textFind()
{
    findDialog->show();
}

void MainWindow::findNext()
{
    QString st=lineEdit->text();
    bool isfind=ui->textEdit->find(st,QTextDocument::FindBackward);
    if(isfind){
        //qDebug()<<tr("行号：%1 列号%2").arg(ui->textEdit->textCursor().blockNumber()).arg(ui->textEdit->textCursor().columnNumber());
    }
    else{
        QMessageBox::warning(this,tr("查找"),tr("没有找到%1").arg(st));
    }
}

//txt文件拖入事件
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls()){
        event->acceptProposedAction();
    }
    else{
        event->ignore();
    }
}
void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimedata=event->mimeData();
    if(mimedata->hasUrls())
    {
        QList<QUrl>urllist=mimedata->urls();
        QString filename=urllist.at(0).toLocalFile();
        if(!filename.isEmpty()){
            QFile file(filename);
            if(!file.open(QIODevice::ReadOnly))return;
            QTextStream in(&file);
            ui->textEdit->setText(in.readAll());
        }
    }
}

//统计字数行数的按钮
void MainWindow::on_pushButton_clicked()
{
    QTextDocument *document = ui->textEdit->document();
    linenum = document->lineCount();
    QString st=ui->textEdit->toPlainText().toUtf8();
    num=st.length();
    QMessageBox box;
    box.setText(tr("行数：%1 字数：%2  ").arg(linenum).arg(num));
    box.exec();
}

//删除全部
void MainWindow::on_actionquanshan_triggered()
{
    ui->textEdit->selectAll();
    ui->textEdit->cut();
}
