#ifndef DATABASE
#define DATABASE
#include<QMessageBox>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QDebug>
#include<QSqlError>


static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("bookMangement.db");
    if (!db.open()) {
        qDebug()<<"unable to create connection";
        QMessageBox::critical(0,"Error",
                              "Fail to connect to database",QMessageBox::Ok);
        return false;
    }
    qDebug()<<"success to create connection";
    QSqlQuery query;
    query.exec("create table Book (bookID int primary key,bookName varchar(20),author varchar(20),year int,sellAll int,price real)");
    query.exec("create table Sells (bookID int ,sells int,sellDate date,customer int,primary key(bookID,sellDate))");
    query.exec("insert into sells values(5,10,'2016-5-13')");
    query.exec("insert into Book values(5,'NewYear','yzz',1995,10,10.0)");
    //qDebug()<<query.lastError();
    query.exec("select * from Book");
    while(query.next())
    {
        int bookId=query.value(0).toInt();
        //qDebug()<<bookId;
    }
    //qDebug()<<query.lastError();
    return true;
}
#endif // DATABASE

