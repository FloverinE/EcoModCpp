#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>

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
    void on_setup_button_clicked();
    void on_go_button_clicked();
    void setup_map();
    void setup_trees();
    void setup_patches();
    void perform_dispersal();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QImage image;  // Declare image as a member variable


};
#endif // MAINWINDOW_H
