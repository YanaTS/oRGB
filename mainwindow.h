#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "converter.h"

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
    void on_actionOpen_triggered();

    void on_spinBox_green_red_valueChanged(double arg1);

    void on_spinBox_blue_yellow_valueChanged(double arg1);

    void on_actionSave_triggered();

    void on_slider_blue_yellow_sliderReleased();

    void on_slider_green_red_sliderReleased();

    void on_spinBox_blue_yellow_editingFinished();

    void on_slider_blue_yellow_sliderPressed();

    void on_slider_green_red_sliderPressed();

    void on_slider_blue_yellow_valueChanged(int position);

    void on_slider_green_red_valueChanged(int position);

private:
    Ui::MainWindow *ui;
    Mat inputImg;
    Mat oRGBImg;
    bool sliderPressed;
    Converter conv;
    double delta_green_red = 0;
    double delta_blue_yellow = 0;
    Mat warm_cool,    warm_neutral, warm_warm;
    Mat neutral_cool, origin,       neutral_warm;
    Mat cool_cool,    cool_neutral, cool_warm;
    void generateShift();
    void generate_oRGB_img();
    void generate_RGB_img();
    void setImages();
    void updateImgs();
    QLabel labelWithInitText;
    bool slider_blue_yellow_pressed = false;
    bool slider_green_red_pressed = false;
};

#endif // MAINWINDOW_H
