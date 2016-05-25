#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QString>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include<QMessageBox>
#include<QDebug>
#include<QSqlError>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   enableAll(false);
   //searchOK=false;
   /* static QSqlQueryModel *model = new QSqlQueryModel(ui->tableView);
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("bookID"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("sells"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("date"));
    ui->tableView->setModel(model);*/
   model = new QSqlTableModel (this);
   model->setTable(QObject::tr("Book"));
   model->setSort(0,Qt::AscendingOrder);
   //model->removeColumn(0);
   model->select();
   ui->tableView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearAll()//清除Edit中的所有内容
{
    ui->bookIDEdit->clear();
    ui->bookNameEdit->clear();
    ui->authorEdit->clear();
    ui->PriceDoubleSpinBox->clear();
    ui->sellsAllSpinBox->clear();
    ui->yearSpinBox->clear();
}

void MainWindow::enableAll(bool enable)
{
    ui->AddButton->setEnabled(enable);
    ui->DeleteButton->setEnabled(enable);
    ui->UpdateButton->setEnabled(enable);
}

void MainWindow::on_SearchButton_clicked()//Search功能要在tableView里面显示所有的Sell记录这部分没写，表格里面加触发器也没加呢
{
    QString bookId=ui->bookIDEdit->toPlainText();
    QSqlQuery query;
    query.exec(QString("select * from Book where bookID = '%1'").arg(bookId));
    int ok = 0;
    while(query.next())
        ok++;
    if(ok == 1)
    {
        query.first();
        QString bookName = query.value(1).toString();
        QString author = query.value(2).toString();
        int year = query.value(3).toInt();
        int sellAll = query.value(4).toInt();
        double price = query.value(5).toDouble();
        ui->bookNameEdit->setText(bookName);
        ui->yearSpinBox->setValue(year);
        ui->authorEdit->setText(author);
        ui->sellsAllSpinBox->setValue(sellAll);
        ui->PriceDoubleSpinBox->setValue(price);
        nowBook=bookId.toInt();
       // searchOK=true;
        enableAll(true);
        //QSqlTableModel *mod = new QSqlTableModel(this);
        model->setTable("sellsNoCus");;
        model->setFilter(QString("bookID = %1").arg(bookId));
        model->select();
        ui->tableView->setModel(model);
    }
    else
    {
        QMessageBox::critical(0,"Error",
                              "Wrong BookId,Please input again",QMessageBox::Ok);
        clearAll();
        enableAll(false);
        //searchOK=false;
    }
}

void MainWindow::on_DeleteButton_clicked()//delete需要把tableView里面的内容清除
{
    if(nowBook==ui->bookIDEdit->toPlainText().toInt())
    {
        QSqlQuery query;
        query.exec(QString("delete from book where bookID='%1'").arg(nowBook));
        query.exec(QString("delete from sells where bookID='%1'").arg(nowBook));
        clearAll();
    }
    else
    {
        QMessageBox::critical(0,"Error",
                              "Please search first",QMessageBox::Ok);
    }
}

void MainWindow::on_AddButton_clicked()
{
    QString date=ui->dateEdit->date().toString("yyyy-M-d");
    int sell=ui->sellsSpinBox->text().toInt();
    if(nowBook==ui->bookIDEdit->toPlainText().toInt())
    {
        QSqlQuery query;
        //query.prepare("insert into sells (bookID,sells,selldate)"
                     // "Values(?,?,?)");
        //query.addBindValue(nowBook);
       // query.addBindValue(sell);
        //query.addBindValue(date);
        bool ok = query.exec(QString("insert into sells values(%1,%2,'%3',1)")
                   .arg(nowBook)
                   .arg(sell)
                   .arg(date));
        qDebug()<<query.lastError();
        if(!ok)
        {
            QMessageBox::critical(0,"Error",
                                  "Record has existed",QMessageBox::Ok);
        }
        qDebug()<<query.lastError();
    }
    else
    {
        QMessageBox::critical(0,"Error",
                              "Please search first",QMessageBox::Ok);
    }
}

void MainWindow::on_UpdateButton_clicked()
{
    QSqlQuery query;
    QString date=ui->dateEdit->date().toString("yyyy-M-d");
    query.exec(QString("select * from sells where bookId=%1 and selldate ='%2'")
                         .arg(nowBook)
                         .arg(date));
    int size=0;
    while(query.next())
        size++;
    int sell=ui->sellsSpinBox->text().toInt();
    if(nowBook==ui->bookIDEdit->toPlainText().toInt()&&size != 0)
    {

        query.exec(QString("select sells from sells where bookId = %1 and selldate = '%2'")
                               .arg(nowBook)
                               .arg(date));
        while(query.next());
        query.first();
        int increase = sell - query.value(0).toInt();
        query.exec(QString("select sellAll from book where  bookId = %1").arg(nowBook));
        query.first();
        int sellAll = query.value(0).toInt()+increase;
        query.exec(QString("update sells set sells = %1 where bookID = %2 and selldate ='%3'")
                             .arg(sell)
                             .arg(nowBook)
                             .arg(date));
        qDebug()<<query.lastError();
        query.exec(QString("update book set sellAll = %1 where bookID = %2")
                   .arg(sellAll)
                   .arg(nowBook));
        qDebug()<<query.lastError();

    }
    else
    {
        QMessageBox::critical(0,"Error",
                              "Wrong BookID or date!",QMessageBox::Ok);
    }
}
