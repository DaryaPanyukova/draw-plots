#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QDebug>
#include <QTableWidgetItem>


#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>


#include <mygrahicsview.h>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_lineEdit_editingFinished();

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    void Redraw();


    void on_clearButton_clicked();

private:
    Ui::MainWindow *ui;
  //  betterGraphicsScene *scene;
    QGraphicsScene *scene;
    void DrawPlot(QString);
    void ClearScene();
};
#endif // MAINWINDOW_H
