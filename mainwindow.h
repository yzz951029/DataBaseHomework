#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtSql>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_SearchButton_clicked();

    void on_DeleteButton_clicked();
    void on_AddButton_clicked();

    void on_UpdateButton_clicked();

private:
    Ui::MainWindow *ui;
    void clearAll();
    int nowBook;
    //bool searchOK;
    QSqlTableModel *model;
    void enableAll(bool enable);
};

#endif // MAINWINDOW_H
