#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "item.h"
#include <QFile> //fournit une interface pour lire et écrire dans des fichiers.
#include<QRandomGenerator>
#include<QDateTime> //fournit des fonctions de date et d'heure

//create shared resource
char map[22][31];
QGraphicsPixmapItem *block[22][31];
QGraphicsScene *scene;
int nb_balls, eat_ball, totalscore, x_gate, y_gate;
QLabel *score_title, *score, *winning, *losing ,*us;
QString eat_str;
Item *pac,*ghost[4];
QVector<QGraphicsPixmapItem*> pball;
bool islose=false, iswin=false;

void MainWindow::set_board() {
    //qsrand(QDateTime::currentDateTime().toTime_t());
    int ghost_ind=0;
    memset(map,'0',sizeof(map));// REMPLIR le map par '0'
    nb_balls = eat_ball = 0;
    totalscore = altern=0;
    QPixmap blockpix(":/rsc/block.png"); //objet
    QPixmap dotpix(":/rsc/dot.png");
    QPixmap powerball(":/rsc/power_ball.png");
    QFile mapfile(":/rsc/map.txt");
    QString mapstr;
    //QByteArray mapline;
    mapfile.open(QIODevice::ReadOnly|QIODevice::Text);
    for(int i=1;i<21;++i) {
        QByteArray mapline = mapfile.readLine();
        mapstr = QString(mapline);
        for(int j=1;j<30;++j) {
            map[i][j] = mapline.at(j-1);
            switch(map[i][j]) {
            case '1':
                block[i][j] = new QGraphicsPixmapItem(blockpix);
                scene->addItem(block[i][j]);
                block[i][j]->setPos(x_lim+j*_width, y_lim+i*_width);
                break;
            case '0':
                block[i][j] = new QGraphicsPixmapItem(dotpix);
                scene->addItem(block[i][j]);
                block[i][j]->setPos(x_lim+j*_width, y_lim+i*_width);
                nb_balls++;
                break;
            case '4':
                map[i][j] = '0';
                block[i][j] = new QGraphicsPixmapItem(powerball);
                scene->addItem(block[i][j]);
                block[i][j]->setPos(x_lim+j*_width, y_lim+i*_width);
                pball.push_back(block[i][j]);
                nb_balls++;
                break;
            case 'g':   //l'interieur du cage
                map[i][j] = '0';
                block[i][j] = nullptr;
                ghost[ghost_ind] = new Ghost(j,i,ghost_ind);
                ghost[ghost_ind]->setPos(x_lim+j*_width, y_lim+i*_width);
                scene->addItem(ghost[ghost_ind]);
                ghost[ghost_ind]->set_posx(static_cast<int>(ghost[ghost_ind]->x()));
                ghost[ghost_ind]->set_posy(static_cast<int>(ghost[ghost_ind]->y()));
                ghost[ghost_ind]->state = _normal;
                ghost[ghost_ind]->out_gate = false;
                ghost_ind++;
                break;
            case '3':
                map[i][j] = '0';
                block[i][j]=nullptr;
                break;
            case 'p':
                map[i][j] = '0';
                block[i][j] = nullptr;
                pac = new Pacman(j,i);
                pac->setPos(x_lim+j*_width, y_lim+i*_width);
                scene->addItem(pac);
                break;
            case '2':
                block[i][j] = new QGraphicsPixmapItem(QPixmap(":/rsc/gate.png"));
                scene->addItem(block[i][j]);
                block[i][j]->setPos(x_lim+j*_width, y_lim+i*_width);
                x_gate=j;
                y_gate=i;
            }
        }
    }
    pac->setZValue(2);
    score_title = new QLabel(this);
    score_title->setText("score :");
    score_title->setStyleSheet("QLabel {font-family: Fixedsys;color: white;font-size: 16px;}");
    score_title->setGeometry(50,12,60,26);
    score = new QLabel(this);
    score->setIndent(-80);
    score->setText("0");
    score->setStyleSheet("QLabel {font-family: Fixedsys;color: white;font-size: 16px;}");
    score->setGeometry(110,12,150,26);

    winning = new QLabel(this);
    winning->hide();
    winning->setText("You win!");
    winning->setStyleSheet("QLabel {font-family: Fixedsys;color: yellow;font-size: 16px;}");
    winning->setGeometry(310,12,150,26);

    losing = new QLabel(this);
    losing->hide();
    losing->setText("You lose");
    losing->setStyleSheet("QLabel {font-family: Fixedsys;color: red;font-size: 16px;}");
    losing->setGeometry(310,12,150,26);
    ghost[0]->counter = 1;
    ghost[1]->counter = 50;
    ghost[2]->counter = 100;
    ghost[3]->counter = 150;
    us = new QLabel(this);
    us->setText("©Mariem & Aya & Wassim");
    us->setStyleSheet("QLabel {font-family: Fixedsys;color: white;font-size: 16px;}");
    us->setGeometry(270,470,180,30);

    mapfile.close();
}
