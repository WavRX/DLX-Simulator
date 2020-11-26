// Made by WavRX

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

void PopulateRegistersTable();
void PopulateMemoryTable();

int PC;
//Instruction types
QList<QPair<QString, QString> > insDef;

private slots:
void on_LoadButton_clicked();



void on_StepButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
