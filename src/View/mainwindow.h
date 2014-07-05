#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <QGraphicsScene>
#include "resizableitem.h"
#include "iView.h"
#include <QWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public iView
{
    Q_OBJECT
public:
    enum Mode{DialogMode,GUIMode};

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void test_inputed(QVector<qreal> features) override;
    void train_sample_inputed(QVector<QPair<QVector<qreal>, QVector<qreal> > >& sample) override;
public slots:
    void show_results(const QVector<qreal>& results) override;
    void hide_message() override;
    void show_message(QString msg) override;
    void show_dlg_message(QString msg) override;
private:
    QVector<qreal> extract_features();
    void setMode(Mode);
    bool open_rec_img(QString image_path);
    bool save_feature_sample(QString file_name);
private slots:
//

    void on_pushButton_file_path_clicked();
    void on_pushButton_ok_clicked();
    void on_pushButton_skip_clicked();
    void on_pushButton_cancel_clicked();
    void on_pushButton_stop_clicked();
    void on_pushButton_image_open_clicked();
    void on_pushButton_test_clicked();
    void sample_inputed(QVector<QPair<QVector<qreal>, QVector<qreal> > >);
private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
    ResizableItem       left_brow,
                        right_brow,
                        left_eye,
                        right_eye,
                        nose,
                        mouse;
    QGraphicsPixmapItem pic_item;

    QString dir_path;
//for forming sample
    QVector<QPair<QVector<qreal>, QVector<qreal> > > sample;
    QVector<QPair<QVector<qreal>, QString> > result_imageName;
    int cur_index;
    QVector<QWidget*> toolPages;
};

#endif // MAINWINDOW_H
