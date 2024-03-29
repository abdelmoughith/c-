#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

        // Set the stylesheet for the entire application
    qApp->setStyleSheet(
            "QMainWindow { background-color: #2C3E50; color: #ECF0F1; }"
            "QDialog { background-color: #34495E; border: 2px solid #2C3E50; }"
            "QPushButton { background-color: #3498DB; color: #ECF0F1; border: 1px solid #3498DB; padding: 8px; border-radius: 5px; }"
            "QPushButton:hover { background-color: #2980B9; }"
            "QComboBox { background-color: #2C3E50; color: #ECF0F1; border: 1px solid #2C3E50; padding: 5px; border-radius: 3px; }"
            "QComboBox::drop-down { border: none; }"
            "QComboBox::down-arrow { image: url(:/assets/img/down-arrow.png); }"
            "QLabel { font-size: 12px; color: #ECF0F1; }"
            "QLabel#lblInfoImage { border: 2px solid #3498DB; border-radius: 5px; }"
        );




    QPixmap pm(":/assets/img/infoImage.jpg");
    ui->lblInfoImage->setPixmap(pm);

    ptrBook  = new book(this);
    ptrCancel = new cancel(this);

    connect(ptrBook, &book::mysignal, this, &MainWindow::updateUI);
    connect(ptrCancel, &cancel::mysignal1, this, &MainWindow::updateUI);

    updateUI();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ptrBook;
    delete ptrCancel;
}

void MainWindow::updateUI()
{
    qDebug() << "in updateUI()";

    QSqlQuery query(MyDB::getInstance()->getDBInstance());
    query.prepare("select Seat, Available from cppbuzz_movie_ticket_booking");

    if(!query.exec())
    {
        qDebug() << query.lastError().text() << query.lastQuery();
        //ui->lblInfo->setText(query.lastError().text());
    }
    else{
        qDebug() << "read was successful "<< query.lastQuery();
    }

    int no = 1;
    while(query.next())
    {
        QString sLabel = "lbl" + query.value(0).toString();
        qDebug() <<"label is : "<< sLabel;
        QLabel * ptr = this->findChild<QLabel*>(sLabel);
        if(ptr!=nullptr)
        {   qDebug() << "label found "<<sLabel;
            if(query.value(1).toString().compare("N") == 0)
            {
                //int no = (qrand() % ((6 + 1) - 1) + 1);
                QPixmap pm(":/assets/img/booked.jpg");
                ptr->setPixmap(pm);
                ptr->setScaledContents(true);
                qDebug()<<query.value(0).toString() << " : is booked";
            }
            else
            {
                QPixmap pm(":/assets/img/available.jpg");
                ptr->setPixmap(pm);
                ptr->setScaledContents(true);
                qDebug()<<query.value(0).toString() << " : is Available";
            }


        }
    }

}

void MainWindow::on_btnBook_clicked()
{
    ptrBook->updateUI();
    ptrBook->show();
}

void MainWindow::on_btnCancel_clicked()
{
    ptrCancel->updateUI();
    ptrCancel->show();
}

void MainWindow::on_btnReset_clicked()
{

    QSqlQuery query(MyDB::getInstance()->getDBInstance());
    query.prepare("update cppbuzz_movie_ticket_booking set Available = 'Y'");

    if(!query.exec())
    {
        qDebug() << query.lastError().text() << query.lastQuery();
        //ui->lblInfo->setText(query.lastError().text());
    }
    else{
        if(query.numRowsAffected() > 0)
        {

            qDebug() << "Update was successful. "<< query.lastQuery();
        }
        else
        {
            qDebug() << "Update Failed. "<< query.lastQuery();
        }

    }

    updateUI();

}
