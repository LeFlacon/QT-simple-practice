#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "webview.h"
#include<QDebug>
#include<QWebEngineView>
#include<QLineEdit>
#include<QNetworkRequest>
#include<QNetworkReply>
#include<QNetworkAccessManager>
#include<QListWidget>
#include<QWebEngineHistory>
#include<QPalette>
#include<QPixmap>
#include<QMessageBox>
#include<QStyleFactory>
#include<QDir>
#include<QDateTime>
#include<QClipboard>
#include<QFileDialog>
#include<QWebEngineSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    view=new WebView(this);//创建webview视图部件用于显示网页内容
    view->load(QUrl("http://www.baidu.com/"));//load（）加载网站
    setCentralWidget(view);
    resize(1024,680);

    progress=0;
    connect(view,&QWebEngineView::loadProgress,this,&MainWindow::jindu);
    connect(view,&QWebEngineView::titleChanged,this,&MainWindow::biaoti);
    connect(view,&QWebEngineView::loadFinished,this,&MainWindow::jiazaiok);
    dizhilan=new QLineEdit(this);

    dizhilan->setText("http://www.baidu.com/");
    connect(dizhilan,&QLineEdit::returnPressed,this,&MainWindow::changedizhi);

    //工具栏--返回，前进，重新加载，停止
    ui->mainToolBar->addAction(view->pageAction(QWebEnginePage::Back));
    ui->mainToolBar->addAction(view->pageAction(QWebEnginePage::Forward));
    ui->mainToolBar->addAction(view->pageAction(QWebEnginePage::Reload));
    ui->mainToolBar->addAction(view->pageAction(QWebEnginePage::Stop));
    ui->mainToolBar->addWidget(dizhilan);

    connect(view,&QWebEngineView::iconUrlChanged,this,&MainWindow::handleIconUrlChanged);
    manager=new QNetworkAccessManager(this);

    //缩放设置,factor范围0.25-5.0
    QMenu *viewMenu=new QMenu(tr("缩放设置"));
    QAction *zoomIn=viewMenu->addAction(tr("放大"));
    zoomIn->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_Plus));
    connect(zoomIn,&QAction::triggered,[this](){
        view->setZoomFactor(view->zoomFactor()+0.1);
        ui->statusBar->showMessage(tr("缩放%1%").arg(view->zoomFactor()*100));
    });
    QAction *zoomOut=viewMenu->addAction(tr("缩小"));
    zoomOut->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_Minus));
    connect(zoomOut,&QAction::triggered,[this](){
        view->setZoomFactor(view->zoomFactor()-0.1);
        ui->statusBar->showMessage(tr("缩放%1%").arg(view->zoomFactor()*100));
    });
    QAction *resetZoom=viewMenu->addAction(tr("重置"));
    resetZoom->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_0));
    connect(resetZoom,&QAction::triggered,[this](){
        view->setZoomFactor(1.0);
        ui->statusBar->showMessage(tr("缩放%1%").arg(view->zoomFactor()*100));
    });
    menuBar()->addMenu(viewMenu);

    //查找页面中内容
    findEdit=new QLineEdit(this);
    findEdit->setMaximumWidth(150);
    ui->mainToolBar->addWidget(findEdit);
    ui->mainToolBar->addAction(tr("查找"),[this](){
        view->findText(findEdit->text());
    });
    connect(findEdit,&QLineEdit::returnPressed,this,&MainWindow::chazhao);

    //历史记录
    ui->mainToolBar->addAction(tr("历史"),this,SLOT(lishijilu()));
    historyList=new QListWidget;
    historyList->setWindowTitle(tr("历史记录"));
    historyList->setMinimumWidth(300);
    connect(historyList,&QListWidget::clicked,this,&MainWindow::zhuandao);

    //设置背景
    QPixmap pixmap(":/new/imageee/1.jpg");
    QPalette palette=this->palette();
    palette.setBrush(QPalette::Background,QBrush(pixmap));
    this->setPalette(palette);

    //播放视频设置
    view->page()->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled,true);
    view->page()->settings()->setAttribute(QWebEngineSettings::PluginsEnabled,true);
    view->page()->settings()->setAttribute(QWebEngineSettings::AutoLoadImages,true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//查找
void MainWindow::chazhao()
{
    view->findText(findEdit->text());
}

//改变路径
void MainWindow::changedizhi()
{
    QUrl url=QUrl(dizhilan->text());
    //qDebug()<<dizhilan->text();
    view->load(url);
    view->setFocus();
}

//更新进度
void MainWindow::jindu(int p)
{
    progress=p;
    biaoti();
}

//更新标题和地址栏
void MainWindow::biaoti()
{
    dizhilan->setText(view->url().toString());//更新新窗口的地址
    if(progress<=0||progress>=100){
        setWindowTitle(view->title());       
    }
    else{
        setWindowTitle(QString("%1(%2%)").arg(view->title()).arg(progress));
    }
}

//加载完成和失败
void MainWindow::jiazaiok(bool fns)
{
    if(fns){
        progress=100;
        setWindowTitle(view->title());
    }
    else{
        setWindowTitle("web page loading error!");
    }
}

//获取新的网站图标???
void MainWindow::handleIconUrlChanged(const QUrl &url)
{
    QNetworkRequest iconRequest(url);
    QNetworkReply *iconReply=manager->get(iconRequest);
    iconReply->setParent(this);
    connect(iconReply,&QNetworkReply::finished,this,&MainWindow::handleIconLoaded);
}
void MainWindow::handleIconLoaded()
{
    QIcon icon;
    QNetworkReply *iconReply=qobject_cast<QNetworkReply*>(sender());
    if(iconReply&&iconReply->error()==QNetworkReply::NoError){
        QByteArray data=iconReply->readAll();
        QPixmap pmp;
        pmp.loadFromData(data);
        icon.addPixmap(pmp);
        iconReply->deleteLater();
    }
    setWindowIcon(icon);
}

//显示历史记录
void MainWindow::lishijilu()
{
    historyList->clear();
    foreach(QWebEngineHistoryItem item,view->history()->items()){
         QListWidgetItem *history=new QListWidgetItem(item.title());
         historyList->addItem(history);
    }
    historyList->show();
}

//转到历史记录
void MainWindow::zhuandao(const QModelIndex &index)
{
    QWebEngineHistoryItem item=view->history()->itemAt(index.row());
    view->history()->goToItem(item);
}

//多窗口显示
WebView *MainWindow::createView()
{
    return view;
}

//显示隐藏工具栏
void MainWindow::on_actionyincang_triggered()
{
    ui->mainToolBar->hide();
}
void MainWindow::on_actionxianshi_triggered()
{
    ui->mainToolBar->show();
}

//关于
void MainWindow::on_actionabout_triggered()
{
    QMessageBox::information(nullptr,"About",tr("这是一个简易浏览器\n\n"
                                                "目前可以实现基本的多窗口网页浏览功能\n\n"
                                                "还有后退/前进/刷新/暂停/历史记录/查找/缩放/显示隐藏工具栏/全网页截图功能\n\n"
                                                "更多功能等待开发 (´･ω･`)\n\n"
                                                "                                        ——nku1711447\n"));
}

//工具栏风格
void MainWindow::on_actionfusion_triggered()
{
    ui->mainToolBar->setStyle(QStyleFactory::create("fusion"));
}

void MainWindow::on_actionmacintosh_triggered()
{
    ui->mainToolBar->setStyle(QStyleFactory::create("macintosh"));
}

void MainWindow::on_actionWindows_triggered()
{
    ui->mainToolBar->setStyle(QStyleFactory::create("Windows"));
}

//右键截图菜单设置
void MainWindow::slotTableMenu(const QPoint &pos)
{
    QMenu menu;
    menu.addAction(QStringLiteral("复制"), this, SLOT(slotActionCopy(bool)));
    menu.addAction(QStringLiteral("另存为"), this, SLOT(slotActionSave(bool)));
    menu.exec(QCursor::pos());
}

//复制截图到剪切板
void MainWindow::slotActionCopy(bool checked)
{
    QString strFile = QCoreApplication::applicationDirPath() + "\\ScreenShot\\Data_";
    strFile = strFile + QDateTime::currentDateTime().toString("yyyyMMddHHmmss") + ".png";
    QPixmap pix = QPixmap::grabWidget(view);
    QApplication::clipboard()->setPixmap(pix);
}

//截图另存为文件
void MainWindow::slotActionSave(bool checked)
{
    QString strDir;
    QDir dir(strDir);
    if(!dir.exists()){
        dir.mkdir(strDir);
    }
    QString strFile = strDir + "\\数据快照" + QDateTime::currentDateTime().toString("yyyyMMddHHmmss") + ".png";
    QPixmap pix = QPixmap::grabWidget(view);
    QString fileName = QFileDialog::getSaveFileName(this,"保存图片",strFile,"PNG (*.png);;BMP (*.bmp);;JPEG (*.jpg *.jpeg)");
    if (!fileName.isNull()){
        pix.save(fileName);
    }
}
void MainWindow::on_action_triggered()
{
    bool ok=true;
    slotActionSave(ok);
}
void MainWindow::on_actionfuzhi_triggered()
{
    bool ok=true;
    slotActionCopy(ok);
}
void MainWindow::on_actionsave_triggered()
{
    bool ok=true;
    slotActionSave(ok);
}

//右键截图设置
void MainWindow::on_actionyoujian_triggered()
{
    view->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(view, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(slotTableMenu(const QPoint&)));
}
