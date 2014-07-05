#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&scene);
    scene.setSceneRect(ui->graphicsView->contentsRect());

    scene.addItem(&left_brow);
    scene.addItem(&left_eye);
    scene.addItem(&right_brow);
    scene.addItem(&right_eye);
    scene.addItem(&nose);
    scene.addItem(&mouse);
    scene.addItem(&pic_item);

    left_brow.setRect(100,10,90,30);
    right_brow.setRect(400,10,90,30);
    left_eye.setRect(130,70,60,30);
    right_eye.setRect(400,70,60,30);
    nose.setRect(260,200,70,70);
    mouse.setRect(245,300,100,50);

    left_brow.setText("left_brow");
    right_brow.setText("right_brow");
    left_eye.setText("left_eye");
    right_eye.setText("right_eye");
    nose.setText("nose");
    mouse.setText("mouthe");

    left_brow.setPen(QPen(QColor(183,101,40)));
    right_brow.setPen(QPen(QColor(183,101,40)));

    left_eye.setPen(QPen(QColor(40,10,215)));
    right_eye.setPen(QPen(QColor(40,10,215)));

    nose.setPen(QPen(QColor(244,186,11)));
    mouse.setPen(QPen(QColor(250,30,5)));

    setMode(MainWindow::GUIMode); 

    //connect(this, SIGNAL(train_sample_inputed(QVector<QPair<QVector<qreal>,QVector<qreal> > >)), this, SLOT(sample_inputed(QVector<QPair<QVector<qreal>,QVector<qreal> > >)));
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::sample_inputed(QVector<QPair<QVector<qreal>, QVector<qreal> > > sample)
{
    if(sample.size())
        save_feature_sample("sources/feature_sample.txt");
}

void MainWindow::hide_message()
{
    statusBar()->clearMessage();
}

void MainWindow::show_dlg_message(QString msg)
{
    QMessageBox::information(this, "Вчитель", msg);
}

bool MainWindow::save_feature_sample(QString file_name)
{
    QFile file;
    QTextStream out(&file);
    file.setFileName(file_name);

    if(!QFile::exists(file.fileName()) || !file.open(QIODevice::WriteOnly))
        return false;

    out << "su ha sa di an || F1 - F21\n";
    out << "----------------------------";
    for(int i=0; i < sample.size(); i++)
    {
        out << "\n";
        for(int j = 0; j < sample[i].second.size(); j++)
            out << QString::number(sample[i].second[j],'f',4)<< " ";
        out << "|| ";
        for(int j = 0; j < sample[i].first.size(); j++)
            out << sample[i].first[j] << " ";
    }
    file.close();
    return true;
}


QVector<qreal> MainWindow::extract_features()
{
    QVector<qreal> result;
    result.resize(21);

    result[0] = left_brow.rect().height();
    result[1] = left_brow.rect().width();
    result[2] = right_brow.rect().height();
    result[3] = right_brow.rect().width();
    result[4] = left_eye.rect().height();
    result[5] = left_eye.rect().width();
    result[6] = right_eye.rect().height();
    result[7] = right_eye.rect().width();
    result[8] = nose.rect().height();
    result[9] = nose.rect().width();
    result[10]= mouse.rect().height();
    result[11]= mouse.rect().width();

    result[12]= right_brow.rect().center().x() - left_brow.rect().center().x();
    result[13]= right_eye.rect().center().x() - left_eye.rect().center().x();
    result[14]= mouse.rect().center().y() - nose.rect().center().y();

    result[15]= left_eye.rect().center().y() - left_brow.rect().center().y();
    result[16]= right_eye.rect().center().y() - right_brow.rect().center().y();

    result[17]= nose.rect().center().y() - (left_brow.rect().center().y() + right_brow.rect().center().y())/2;
    result[18]= mouse.rect().center().y() - (left_brow.rect().center().y() + right_brow.rect().center().y())/2;

    result[19]= nose.rect().center().y() - (left_eye.rect().center().y() + right_eye.rect().center().y())/2;
    result[20]= mouse.rect().center().y() - (left_eye.rect().center().y() + right_eye.rect().center().y())/2;

    return result;
}

void MainWindow::setMode(MainWindow::Mode mode)
{
    bool trigger;
    if(pic_item.scene())
        scene.removeItem(&pic_item);
    switch(mode){
    case MainWindow::GUIMode:{
        trigger = true;
        ui->statusBar->clearMessage();
        QVector<qreal> tmp;
        tmp.resize(5);
        show_results(tmp);
        pic_item.pixmap().mask().clear();
        break;
    }
    case MainWindow::DialogMode:{
        trigger = false;
        ui->statusBar->showMessage(QString("Підготовка навчальної вибірки, зображення %1/%2, об'єм вибірки %3").arg(cur_index+1).arg(result_imageName.size()).arg(sample.size()));
        break;
    }
    default:
        return;
        break;
    }
    ui->toolBox->widget(1)->setEnabled(trigger);
    ui->pushButton_file_path->setEnabled(trigger);
    ui->radioButton_by_features->setEnabled(trigger);
    ui->radioButton_by_images->setEnabled(trigger);

    ui->pushButton_cancel->setEnabled(!trigger);
    ui->pushButton_ok->setEnabled(!trigger);
    ui->pushButton_skip->setEnabled(!trigger);
    ui->pushButton_stop->setEnabled(!trigger);
}

void MainWindow::on_pushButton_file_path_clicked()
{

    QFile file;
    QTextStream stream(&file);
    if(ui->radioButton_by_images->isChecked())
        file.setFileName("sources/image_sample.txt");
    else
        file.setFileName("sources/feature_sample.txt");

    if(!QFile::exists(file.fileName()) || !stream.device()->open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "Помилка", QString("Неможливо відкрити файл \"%1\". Перевірте правильність вводу файла.").arg(file.fileName()));
        return;
    }
//
    result_imageName.clear();
    sample.clear();
    cur_index = 0;
//
    QString str;
    QTextStream s_stream(&str,QIODevice::ReadOnly);
    try{
        if(ui->radioButton_by_images->isChecked()){
            dir_path = stream.readLine();
            stream.readLine();
            stream.readLine();
            for(str = stream.readLine(); str.size(); str = stream.readLine(),s_stream.seek(0))
            {
                QPair<QVector<qreal>, QString> pair;
                pair.first.resize(5);
                s_stream >> pair.first[0];//first column
                for(int i = 0; i < 5; i++){
                    s_stream >> pair.first[i];
                    pair.first[i]=(pair.first[i]-1)/4;
                }
                s_stream >> pair.second;

                result_imageName.append(pair);
                str.clear();

                setMode(MainWindow::DialogMode);
                if(open_rec_img(dir_path + result_imageName[cur_index].second))
                    show_results(result_imageName[cur_index].first);
                else
                    on_pushButton_skip_clicked();
            }
        }else
        {
            stream.readLine();
            stream.readLine();
            for(str = stream.readLine(); str.size(); str = stream.readLine(),s_stream.seek(0))
            {
                QPair<QVector<qreal>, QVector<qreal> > pair;

                pair.second.resize(5);
                for(int i = 0; i < pair.second.size(); i++)
                    s_stream >> pair.second[i];

                QString tmp;
                s_stream >> tmp;

                pair.first.resize(21);
                for(int i = 0; i < pair.first.size(); i++)
                    s_stream >> pair.first[i];

                sample.append(pair);
                str.clear();
            }

            emit train_sample_inputed(sample);
        }
    }
    catch(...)
    {
        QMessageBox::warning(this, "Помилка", QString("Неправильний формат файлу \"%1\".").arg(file.fileName()));
        return;
    }
    file.close();
}

bool MainWindow::open_rec_img(QString image_path)
{
    if(pic_item.scene())
        scene.removeItem(&pic_item);
    if(image_path.isNull()) return false;
    if(!QFile::exists(image_path)) {
        QMessageBox::warning(this, "Помилка", QString("Неможливо відкрити файл \"%1\". Перевірте правильність вводу файла.").arg(image_path));
        return false;
    }
    QPixmap pic(image_path);

    if(pic.isNull()){
        QMessageBox::warning(this, "Помилка", QString("Неправильний формат файлу \"%1\".").arg(image_path));
        return false;
    }

    pic_item.setPixmap(pic.scaled(QSize(scene.width(),scene.height()),Qt::KeepAspectRatio));
    scene.addItem(&pic_item);
    return true;
}


void MainWindow::on_pushButton_ok_clicked()
{
    QPair<QVector<qreal>,QVector<qreal> >  tmp;
    tmp.first = extract_features();
    tmp.second = result_imageName[cur_index].first;

    sample.append(tmp);

    on_pushButton_skip_clicked();
}

void MainWindow::on_pushButton_skip_clicked()
{
    cur_index++;
    while(cur_index < result_imageName.size()){
        ui->statusBar->showMessage(QString("Підготовка навчальної вибірки, зображення %1/%2, об'єм вибірки %3").arg(cur_index+1).arg(result_imageName.size()).arg(sample.size()));
        if(open_rec_img(dir_path + result_imageName[cur_index].second))
            break;
        cur_index++;
    }

    if(cur_index < result_imageName.size())
    {
        show_results(result_imageName[cur_index].first);
    }else
    {
        setMode(MainWindow::GUIMode);
        emit train_sample_inputed(sample);
    }
}

void MainWindow::on_pushButton_cancel_clicked()
{
    setMode(MainWindow::GUIMode);
}

void MainWindow::on_pushButton_stop_clicked()
{
    setMode(MainWindow::GUIMode);
    emit train_sample_inputed(sample);

}

void MainWindow::show_results(const QVector<qreal> &results)
{
    ui->progressBar_surprized->setValue(int(results[0]*100));
    ui->progressBar_happiness->setValue(int(results[1]*100));
    ui->progressBar_sad->setValue(int(results[2]*100));
    ui->progressBar_disgusted->setValue(int(results[3]*100));
    ui->progressBar_anger->setValue(int(results[4]*100));

//    QString status_line;//
//    for(auto& r: results)//
//        status_line.append(QString::number(r,'f') + " ");//
//    statusBar()->showMessage(status_line);//
}

void MainWindow::show_message(QString msg)
{
    statusBar()->showMessage("Вчитель: " + msg);
}

void MainWindow::on_pushButton_image_open_clicked()
{
    QString image_path = QFileDialog::getOpenFileName(this,tr("Відкрити зображення"),tr("./"));
    open_rec_img(image_path);
}

void MainWindow::on_pushButton_test_clicked()
{
    statusBar()->clearMessage();
    emit test_inputed(extract_features());
}
