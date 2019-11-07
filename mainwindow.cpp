#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QFileDialog>

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);
    ui->slider_blue_yellow->setEnabled(false);
    ui->slider_green_red->setEnabled(false);
    ui->spinBox_blue_yellow->setEnabled(false);
    ui->spinBox_green_red->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    ui->slider_blue_yellow->setEnabled(true);
    ui->slider_green_red->setEnabled(true);
    ui->spinBox_blue_yellow->setEnabled(true);
    ui->spinBox_green_red->setEnabled(true);
    ui->labelWithInitText->setVisible(false);

    string pathToFile = (QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png)"))).toStdString();

    if(!pathToFile.empty())
        inputImg = imread(pathToFile, IMREAD_COLOR);
    if(!inputImg.empty())
    {
        cvtColor(inputImg, inputImg, CV_BGR2RGB);

        generate_oRGB_img();

        generateShift();

        generate_RGB_img();

        setImages();
    }
}

void MainWindow::on_actionSave_triggered()
{
    QFileDialog *fileDialog = new QFileDialog;
    fileDialog->setDefaultSuffix(".png");
    string path_to_file = (fileDialog->getSaveFileName(this, "Save file", ".",tr(" PNG(*.png)"))).toStdString();

    if(!inputImg.empty() && !path_to_file.empty())
    {
        int width = inputImg.cols;
        int height = inputImg.rows;
        Mat resultImg(3*height, 3*width, inputImg.type());

        warm_cool.copyTo(resultImg(Rect(0, 0, width, height)));
        warm_neutral.copyTo(resultImg(Rect(width, 0, width, height)));
        warm_warm.copyTo(resultImg(Rect(2*width, 0, width, height)));

        neutral_cool.copyTo(resultImg(Rect(0, height, width, height)));
        inputImg.copyTo(resultImg(Rect(width, height, width, height)));
        neutral_warm.copyTo(resultImg(Rect(2*width, height, width, height)));

        cool_cool.copyTo(resultImg(Rect(0, 2*height, width, height)));
        cool_neutral.copyTo(resultImg(Rect(width, 2*height, width, height)));
        cool_warm.copyTo(resultImg(Rect(2*width, 2*height, width, height)));

        cvtColor(resultImg, resultImg, CV_RGB2BGR);

        imwrite(path_to_file.c_str(), resultImg);
    }
}

void MainWindow::generate_oRGB_img()
{
    oRGBImg = this->conv.convert_img_to_oRGB(inputImg);
}

void MainWindow::generateShift()
{
    warm_cool =  this->conv.cool_warm_shift(oRGBImg,delta_green_red, -delta_blue_yellow);
    warm_neutral = this->conv.cool_warm_shift(oRGBImg,delta_green_red, 0);
    warm_warm = this->conv.cool_warm_shift(oRGBImg,delta_green_red, delta_blue_yellow);
    neutral_cool = this->conv.cool_warm_shift(oRGBImg,0, -delta_blue_yellow);
    neutral_warm = this->conv.cool_warm_shift(oRGBImg,0, delta_blue_yellow);
    cool_cool = this->conv.cool_warm_shift(oRGBImg,-delta_green_red, -delta_blue_yellow);
    cool_neutral = this->conv.cool_warm_shift(oRGBImg,-delta_green_red, 0);
    cool_warm = this->conv.cool_warm_shift(oRGBImg,-delta_green_red, delta_blue_yellow);
}

void MainWindow::generate_RGB_img()
{
    warm_cool = this->conv.convert_img_to_RGB(warm_cool);
    warm_neutral = this->conv.convert_img_to_RGB(warm_neutral);
    warm_warm = this->conv.convert_img_to_RGB(warm_warm);
    neutral_cool = this->conv.convert_img_to_RGB(neutral_cool);
    neutral_warm = this->conv.convert_img_to_RGB(neutral_warm);
    cool_cool = this->conv.convert_img_to_RGB(cool_cool);
    cool_neutral = this->conv.convert_img_to_RGB(cool_neutral);
    cool_warm = this->conv.convert_img_to_RGB(cool_warm);
}

void MainWindow::setImages()
{
    QImage qimg = QImage((unsigned char*) inputImg.data, inputImg.cols, inputImg.rows,static_cast<int>(inputImg.step), QImage::Format_RGB888);
    qimg.rgbSwapped();
    QPixmap qpixmapImg = QPixmap::fromImage(qimg);
    this->ui->origin->setPixmap(qpixmapImg);

    QImage qimg1 = QImage((unsigned char*) warm_cool.data, warm_cool.cols, warm_cool.rows, static_cast<int>(warm_cool.step),QImage::Format_RGB888);
    qimg1.rgbSwapped();
    QPixmap qpixmapImg1 = QPixmap::fromImage(qimg1);
    this->ui->left_up->setPixmap(qpixmapImg1);

    QImage qimg2 = QImage((unsigned char*) warm_neutral.data, warm_neutral.cols, warm_neutral.rows, static_cast<int>(warm_neutral.step), QImage::Format_RGB888);
    qimg2.rgbSwapped();
    QPixmap qpixmapImg2 = QPixmap::fromImage(qimg2);
    this->ui->up->setPixmap(qpixmapImg2);

    QImage qimg3 = QImage((unsigned char*) warm_warm.data, warm_warm.cols, warm_warm.rows,static_cast<int>(warm_warm.step), QImage::Format_RGB888);
    qimg3.rgbSwapped();
    QPixmap qpixmapImg3 = QPixmap::fromImage(qimg3);
    this->ui->right_up->setPixmap(qpixmapImg3);

    QImage qimg4 = QImage((unsigned char*) neutral_cool.data, neutral_cool.cols, neutral_cool.rows, static_cast<int>(neutral_cool.step), QImage::Format_RGB888);
    qimg4.rgbSwapped();
    QPixmap qpixmapImg4 = QPixmap::fromImage(qimg4);
    this->ui->left->setPixmap(qpixmapImg4);

    QImage qimg5 = QImage((unsigned char*) neutral_warm.data, neutral_warm.cols, neutral_warm.rows, static_cast<int>(neutral_warm.step), QImage::Format_RGB888);
    qimg5.rgbSwapped();
    QPixmap qpixmapImg5 = QPixmap::fromImage(qimg5);
    this->ui->right->setPixmap(qpixmapImg5);

    QImage qimg6 = QImage((unsigned char*) cool_cool.data, cool_cool.cols, cool_cool.rows, static_cast<int>(cool_cool.step), QImage::Format_RGB888);
    qimg6.rgbSwapped();
    QPixmap qpixmapImg6 = QPixmap::fromImage(qimg6);
    this->ui->left_down->setPixmap(qpixmapImg6);

    QImage qimg7 = QImage((unsigned char*) cool_neutral.data, cool_neutral.cols, cool_neutral.rows, static_cast<int>(cool_neutral.step), QImage::Format_RGB888);
    qimg7.rgbSwapped();
    QPixmap qpixmapImg7 = QPixmap::fromImage(qimg7);
    this->ui->down->setPixmap(qpixmapImg7);

    QImage qimg8 = QImage((unsigned char*) cool_warm.data, cool_warm.cols, cool_warm.rows, static_cast<int>(cool_warm.step), QImage::Format_RGB888);
    qimg8.rgbSwapped();
    QPixmap qpixmapImg8 = QPixmap::fromImage(qimg8);
    this->ui->right_down->setPixmap(qpixmapImg8);
}

void MainWindow::updateImgs()
{
    if(!inputImg.empty())
    {
        generateShift();

        generate_RGB_img();

        setImages();
    }
}

void MainWindow::on_spinBox_green_red_valueChanged(double arg1)
{
    delta_green_red = arg1;
    this->ui->slider_green_red->setValue((int)(delta_green_red*100));    
}

void MainWindow::on_spinBox_blue_yellow_valueChanged(double arg1)
{
    delta_blue_yellow = arg1;
    this->ui->slider_blue_yellow->setValue((int)(delta_blue_yellow*100));    
}

void MainWindow::on_slider_blue_yellow_sliderReleased()
{
    slider_blue_yellow_pressed = false;
    updateImgs();
}

void MainWindow::on_slider_green_red_sliderReleased()
{
    slider_green_red_pressed = false;
    updateImgs();
}

void MainWindow::on_spinBox_blue_yellow_editingFinished()
{
    updateImgs();
}

void MainWindow::on_slider_blue_yellow_sliderPressed()
{
    slider_blue_yellow_pressed = true;
}

void MainWindow::on_slider_green_red_sliderPressed()
{
    slider_green_red_pressed = true;
}

void MainWindow::on_slider_blue_yellow_valueChanged(int position)
{
    delta_blue_yellow = position/100.;
    this->ui->spinBox_blue_yellow->setValue(delta_blue_yellow);
    if(!slider_blue_yellow_pressed)
        updateImgs();
}

void MainWindow::on_slider_green_red_valueChanged(int position)
{
    delta_green_red = position/100.;
    this->ui->spinBox_green_red->setValue(delta_green_red);
    if(!slider_green_red_pressed)
        updateImgs();
}
