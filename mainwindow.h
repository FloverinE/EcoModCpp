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
    void perform_pop_dynamics();
    void setup_min_distance_to_tree();
    void count_populations();

    void update_map();
    void clear_charts();
    void draw_charts();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QImage image;  // Declare image as a member variable
    // needed for plotting
    QChart *N_seeds_chart;
    QChart *N_height_class_chart;
    QChart *N_birch_pop_chart;
    QChart *N_oak_pop_chart;


};
#endif // MAINWINDOW_H
