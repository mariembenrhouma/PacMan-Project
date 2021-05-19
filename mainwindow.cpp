#include "mainwindow.h"
#include "item.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) // definir namespace de l'interface graphique
{
    ui->setupUi(this); /*crée les instances réelles de widgets pour vous.
                         Donc pour pouvoir construire la "fenêtre" réelle avec
                        tous les éléments que vous y mettez dans QtDesigner et
                         l'afficher dans votre application*/
    setWindowTitle(tr("PacMan"));
    ui->graphicsView->setStyleSheet("QGraphicsView {border: none;}");
    ui->graphicsView->setBackgroundBrush(Qt::black);
    scene = new QGraphicsScene(50, 50, ui->graphicsView->width() - 2, ui->graphicsView->height() - 2);
    ui->graphicsView->setFocusPolicy(Qt::NoFocus);
    ui->graphicsView->setScene(scene);
    set_board();
    timer = new QTimer(this);
    show_time = new QTimer(this);
    timer->start(6);
    show_time->start(200);
    connect(timer,SIGNAL(timeout()),this,SLOT(ghost_move()));
    connect(show_time,SIGNAL(timeout()),this,SLOT(ball_show()));
}

void MainWindow::ghost_move() {
    if(islose) {
        timer->stop();
        show_time->stop();
        QCoreApplication::processEvents();
        QThread::sleep(1);
        losing->show();
        return;
    }
    if(iswin) {
        timer->stop();
        for(int i=0;i<4;++i) {
            delete ghost[i];
        }
        return;
    }
    for(int i=0;i<4;++i) {
        switch(ghost[i]->get_dir()) {
        case _left:
            ghost[i]->moveleft();
            break;
        case _right:
            ghost[i]->moveright();
            break;
        case _up:
            ghost[i]->move_up();
            break;
        case _down:
            ghost[i]->move_down();
            break;
        case _stop:
            if(ghost[i]->isinbox()) {
                if(ghost[i]->state == _go_togate) {
                    go_togate(i);
                    break;
                }
                int disx, disy;
                disx = x_gate - ghost[i]->get_px();
                disy = y_gate - ghost[i]->get_py();
                if(disx==0 && disy==0) {
                    ghost[i]->setinbox(false);
                    ghost[i]->set_dir(ghost[i]->get_lastdir());
                    switch(ghost[i]->get_lastdir()) {
                    case _left:
                        ghost[i]->moveleft();
                        break;
                    case _right:
                        ghost[i]->moveright();
                        break;
                    case _up:
                        ghost[i]->move_up();
                        break;
                    case _down:
                        ghost[i]->move_down();
                        break;
                    }
                    return;
                }

                if(disx>0) { //ghosts > faw9 al  gate
                    ghost[i]->set_dir(_right);
                } else if(disx<0) {
                    ghost[i]->set_dir(_left);
                } else if(disx == 0) {
                    if(disy > 0)
                        ghost[i]->set_dir(_down);
                    else
                        ghost[i]->set_dir(_up);
                }
            }
           else {
                if(ghost[i]->counter) {
                    ghost[i]->counter--;
                    if(ghost[i]->counter==0)
                        ghost[i]->setinbox(true);
                }
                switch(ghost[i]->state) {
                case _normal:
                    chase(i);
                    break;
                case _blue:
                    ghost[i]->time_gate--;
                        if(ghost[i]->time_gate == 0) {
                            ghost[i]->state = _normal;
                        }
                        escape(i);

                    break;
                }
            }
            break;
        }
    }
}

void MainWindow::ball_show() {
    if(pball.empty()) {
        show_time->stop();
        return;
    }
    if(altern) {
        for(int i=0;i<pball.size();++i)
            pball.at(i)->hide();//pball.index
        altern=0;
    } else {
        for(int i=0;i<pball.size();++i)
            pball.at(i)->show();
        altern=1;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if(islose){
        return;
    }
    if(event->key() == Qt::Key_Up){
        switch(pac->get_dir()) {
        case _down: case _stop:
            pac->set_nextdir(_stop);
            pac->set_changept(-1);
            pac->move_up();
            break;
        case _left:
            if(pac->get_py()>0) {
                int nowx = pac->get_px() - 1, nowy = pac->get_py();
                while(map[nowy][nowx] == '0') {
                    if(map[nowy-1][nowx] == '0') {
                        pac->set_nextdir(_up);
                        pac->set_changept(nowx);
                        break;                 
                    }
                    --nowx;
                }
            }
            break;
        case _right:
            if(pac->get_py()>0) {
                int nowx = pac->get_px() + 1, nowy = pac->get_py();
                while(map[nowy][nowx] == '0') {
                    if(map[nowy-1][nowx] == '0') {
                        pac->set_nextdir(_up);
                        pac->set_changept(nowx);
                        break;
                    }
                    ++nowx;
                }
            }
            break;
        }
    }

    else if(event->key() == Qt::Key_Left){
        switch(pac->get_dir()) {
        case _right: case _stop:
            pac->set_nextdir(_stop);
            pac->set_changept(-1);
            pac->moveleft();
            break;
        case _up:
            if(pac->get_px()>0) {
                int nowx = pac->get_px(), nowy = pac->get_py() - 1;
                while(map[nowy][nowx] == '0') {
                    if(map[nowy][nowx-1] == '0') {
                        pac->set_nextdir(_left);
                        pac->set_changept(nowy);
                        break;
                    }
                    --nowy;
                }
            }
            break;
        case _down:
            if(pac->get_px()>0) {
                int nowx = pac->get_px(), nowy = pac->get_py() + 1;
                while(map[nowy][nowx] == '0') {
                    if(map[nowy][nowx-1] == '0') {
                        pac->set_nextdir(_left);
                        pac->set_changept(nowy);
                        break;
                    }
                    ++nowy;
                }
            }
            break;
        }
    }
    else if(event->key() == Qt::Key_Down){
        switch(pac->get_dir()) {
        case _up: case _stop:
            pac->set_nextdir(_stop);
            pac->set_changept(-1);
            pac->move_down();
            break;
        case _left:
            if(pac->get_py()<21) {
                int nowx = pac->get_px() - 1, nowy = pac->get_py();
                while(map[nowy][nowx] == '0') {
                    if(map[nowy+1][nowx] == '0') {
                        pac->set_nextdir(_down);
                        pac->set_changept(nowx);
                        break;
                    }
                    --nowx;
                }
            }
            break;
        case _right:
            if(pac->get_py()<21) {
                int nowx = pac->get_px() + 1, nowy = pac->get_py();
                while(map[nowy][nowx] == '0') {
                    if(map[nowy+1][nowx] == '0') {
                        pac->set_nextdir(_down);
                        pac->set_changept(nowx);
                        break;
                    }
                    ++nowx;
                }
            }
            break;
        }
    }
    else if(event->key() == Qt::Key_Right){         //d: right
        switch(pac->get_dir()) {
        case _left: case _stop:
            pac->moveright();
            break;
        case _up:
            if(pac->get_px()<30) {
                int nowx = pac->get_px(), nowy = pac->get_py() - 1;
                while(map[nowy][nowx] == '0') {
                    if(map[nowy][nowx+1] == '0') {
                        pac->set_nextdir(_right);
                        pac->set_changept(nowy);
                        break;
                    }
                    --nowy;
                }
            }
            break;
        case _down:
            if(pac->get_px()<30) {
                int nowx = pac->get_px(), nowy = pac->get_py() + 1;
                while(map[nowy][nowx] == '0') {
                    if(map[nowy][nowx+1] == '0') {
                        pac->set_nextdir(_right);
                        pac->set_changept(nowy);
                        break;
                    }
                    ++nowy;
                }
            }
            break;
        }       
    }
}

MainWindow::~MainWindow() {
    delete ui;
}
void MainWindow::chase(int ind) {
    int disx=0, disy=0, gpx, gpy,permis_num=0,lastdir;
    bool dir_permise[4];
    QVector<int> way;
    gpx = ghost[ind]->get_px();
    gpy = ghost[ind]->get_py();
    dir_permise[_right] = map[gpy][gpx+1] == '0' ? true:false;
    dir_permise[_left] = map[gpy][gpx-1] == '0' ? true:false;
    dir_permise[_up] = map[gpy-1][gpx] == '0' ? true:false;
    dir_permise[_down] = map[gpy+1][gpx] == '0' ? true:false;
    for(int i=0; i<4; ++i) {
        if(dir_permise[i]) { // si dir_permise [up] = True
            permis_num++;
            way.push_back(i);
        }
    }
    lastdir = ghost[ind]->get_lastdir();
    if(permis_num==2) {      //dans un coin
        if(dir_permise[lastdir] == false) {
            switch(ghost[ind]->get_lastdir()) {
            case _left:
                if(way.at(0)==_right)
                    ghost[ind]->set_dir(way.at(1));
                else
                    ghost[ind]->set_dir(way.at(0));
                break;
            case _right:
                if(way.at(0)==_left)
                    ghost[ind]->set_dir(way.at(1));
                else
                    ghost[ind]->set_dir(way.at(0));
                break;
            case _up:
                if(way.at(0)==_down)
                    ghost[ind]->set_dir(way.at(1));
                else
                    ghost[ind]->set_dir(way.at(0));
                break;
            case _down:
                if(way.at(0)==_up)
                    ghost[ind]->set_dir(way.at(1));
                else
                    ghost[ind]->set_dir(way.at(0));
                break;
            }
        } else {
            ghost[ind]->set_dir(ghost[ind]->get_lastdir());
        }
        return;
    }
    switch(ind) {
    case 0:
        disx = pac->get_px() - gpx;
        disy = pac->get_py() - gpy;
        break;
    case 1:
        disx = pac->get_px() - gpx;
        disy = pac->get_py() - gpy;
        switch(pac->get_dir()) {
        case _up:
            disy-=4;
            break;
        case _down:
            disy+=4;
            break;
        case _left:
            disx-=4;
            break;
        case _right:
            disx+=4;
            break;
        }
        break;
    case 2:
        disx = 2*(pac->get_px()) - ghost[0]->get_px() - gpx;
        disy = 2*(pac->get_py()) - ghost[0]->get_py() - gpy;
        break;
    case 3:
        disx = pac->get_px() - gpx;
        disy = pac->get_py() - gpy;
        if(disx*disx + disy*disy < 64) {
            disx = 2 - gpx;
            disy = 2 - gpy;
        }
        break;
    }

    if(disy > 0) {
        if(dir_permise[_down] && lastdir != _up)
            ghost[ind]->set_dir(_down);
        else {
            if(disx >= 0) {
                if(dir_permise[_right])
                    ghost[ind]->set_dir(_right);
                else if(dir_permise[_left])
                    ghost[ind]->set_dir(_left);
            } else if(disx < 0){
                if(dir_permise[_left])
                    ghost[ind]->set_dir(_left);
                else if(dir_permise[_right])
                    ghost[ind]->set_dir(_right);
            }
        }
    } else if(disy < 0) {
        if(dir_permise[_up] && lastdir != _down)
            ghost[ind]->set_dir(_up);
        else {
            if(disx >= 0) {
                if(dir_permise[_right])
                    ghost[ind]->set_dir(_right);
                else if(dir_permise[_left])
                    ghost[ind]->set_dir(_left);
            } else if(disx < 0){
                if(dir_permise[_left])
                    ghost[ind]->set_dir(_left);
                else if(dir_permise[_right])
                    ghost[ind]->set_dir(_right);
            }
        }
    } else if(disx > 0) {
        if(dir_permise[_right] && lastdir != _left)
            ghost[ind]->set_dir(_right);
        else {
            if(dir_permise[_up])
                ghost[ind]->set_dir(_up);
            else if(dir_permise[_down])
                ghost[ind]->set_dir(_down);
        }
    } else if(disx < 0) {
        if(dir_permise[_left] && lastdir != _right)
            ghost[ind]->set_dir(_left);
        else {
            if(dir_permise[_up])
                ghost[ind]->set_dir(_up);
            else if(dir_permise[_down])
                ghost[ind]->set_dir(_down);
        }
    }
}

void MainWindow::escape(int ind) {
    int disx=0, disy=0, gpx, gpy,permis_num=0,lastdir;
    bool dir_permise[4];
    QVector<int> way;
    gpx = ghost[ind]->get_px();
    gpy = ghost[ind]->get_py();
    dir_permise[_right] = map[gpy][gpx+1] == '0' ? true:false;
    dir_permise[_left] = map[gpy][gpx-1] == '0' ? true:false;
    dir_permise[_up] = map[gpy-1][gpx] == '0' ? true:false;
    dir_permise[_down] = map[gpy+1][gpx] == '0' ? true:false;
    for(int i=0; i<4; ++i) {
        if(dir_permise[i]) {
            permis_num++;
            way.push_back(i);
        }
    }
    lastdir = ghost[ind]->get_lastdir();
    if(permis_num==2) {      //not intersection
        if(dir_permise[lastdir] == false) {
            switch(ghost[ind]->get_lastdir()) {
            case _left:
                if(way.at(0)==_right)
                    ghost[ind]->set_dir(way.at(1));
                else
                    ghost[ind]->set_dir(way.at(0));
                break;
            case _right:
                if(way.at(0)==_left)
                    ghost[ind]->set_dir(way.at(1));
                else
                    ghost[ind]->set_dir(way.at(0));
                break;
            case _up:
                if(way.at(0)==_down)
                    ghost[ind]->set_dir(way.at(1));
                else
                    ghost[ind]->set_dir(way.at(0));
                break;
            case _down:
                if(way.at(0)==_up)
                    ghost[ind]->set_dir(way.at(1));
                else
                    ghost[ind]->set_dir(way.at(0));
                break;
            }
        } else {
            ghost[ind]->set_dir(ghost[ind]->get_lastdir());
        }
        return;
    }

    disx = pac->get_px() - gpx;
    disy = pac->get_py() - gpy;

    if(disy > 0) {
        if(dir_permise[_up])
            ghost[ind]->set_dir(_up);
        else {
            if(qrand()&1) {
            if(disx >= 0) {
                if(dir_permise[_left])
                    ghost[ind]->set_dir(_left);
                else if(dir_permise[_right])
                    ghost[ind]->set_dir(_right);
            }else if(disx < 0){
                if(dir_permise[_right])
                    ghost[ind]->set_dir(_right);
                else if(dir_permise[_left])
                    ghost[ind]->set_dir(_left);
            }
            }
            else {
                if(disx >= 0) {
                    if(dir_permise[_right])
                        ghost[ind]->set_dir(_right);
                    else if(dir_permise[_left])
                        ghost[ind]->set_dir(_left);
                } else if(disx < 0){
                    if(dir_permise[_left])
                        ghost[ind]->set_dir(_left);
                    else if(dir_permise[_right])
                        ghost[ind]->set_dir(_right);
                }
            }
        }
    } else if(disy < 0) {
        if(dir_permise[_down])
            ghost[ind]->set_dir(_down);
        else {
            if(qrand()&1) {
            if(disx >= 0) {
                if(dir_permise[_left])
                    ghost[ind]->set_dir(_left);
                else if(dir_permise[_right])
                    ghost[ind]->set_dir(_right);
            } else if(disx < 0){
                if(dir_permise[_right])
                    ghost[ind]->set_dir(_right);
                else if(dir_permise[_left])
                    ghost[ind]->set_dir(_left);
            }
            }
            else {
                if(disx >= 0) {
                    if(dir_permise[_right])
                        ghost[ind]->set_dir(_right);
                    else if(dir_permise[_left])
                        ghost[ind]->set_dir(_left);
                } else if(disx < 0){
                    if(dir_permise[_left])
                        ghost[ind]->set_dir(_left);
                    else if(dir_permise[_right])
                        ghost[ind]->set_dir(_right);
                }
            }
        }
    } else if(disy == 0) {
        if(dir_permise[_up])
            ghost[ind]->set_dir(_up);
        else if(dir_permise[_down])
            ghost[ind]->set_dir(_down);
        else {
            if(disx >= 0) {
                if(dir_permise[_left])
                    ghost[ind]->set_dir(_left);
                else if(dir_permise[_right])
                    ghost[ind]->set_dir(_right);
            } else if(disx < 0){
                if(dir_permise[_right])
                    ghost[ind]->set_dir(_right);
                else if(dir_permise[_left])
                    ghost[ind]->set_dir(_left);
            }
        }
    }
}

void MainWindow::go_togate(int ind) {
    int disx=0, disy=0, gpx, gpy,permis_num=0,lastdir;
    bool dir_permise[4];
    QVector<int> way;
    gpx = ghost[ind]->get_px();
    gpy = ghost[ind]->get_py();
    dir_permise[_right] = (map[gpy][gpx+1] == '0' || map[gpy][gpx+1] =='2') ? true:false;
    dir_permise[_left] = (map[gpy][gpx-1] == '0' || map[gpy][gpx-1] == '2' )? true:false;
    dir_permise[_up] = (map[gpy-1][gpx] == '0' || map[gpy-1][gpx] == '2') ? true:false;
    dir_permise[_down] = (map[gpy+1][gpx] == '0' || map[gpy+1][gpx] == '2') ? true:false;
    if(gpx == x_gate && gpy == y_gate)
        ghost[ind]->out_gate = true;
    for(int i=0; i<4; ++i) {
        if(dir_permise[i]) {
            permis_num++;
            way.push_back(i);
        }
    }
    lastdir = ghost[ind]->get_lastdir();
    if(permis_num==2) {      //not intersection
        if(dir_permise[lastdir] == false) {
            switch(ghost[ind]->get_lastdir()) {
            case _left:
                if(way.at(0)==_right)
                    ghost[ind]->set_dir(way.at(1));
                else
                    ghost[ind]->set_dir(way.at(0));
                break;
            case _right:
                if(way.at(0)==_left)
                    ghost[ind]->set_dir(way.at(1));
                else
                    ghost[ind]->set_dir(way.at(0));
                break;
            case _up:
                if(way.at(0)==_down)
                    ghost[ind]->set_dir(way.at(1));
                else
                    ghost[ind]->set_dir(way.at(0));
                break;
            case _down:
                if(way.at(0)==_up)
                    ghost[ind]->set_dir(way.at(1));
                else
                    ghost[ind]->set_dir(way.at(0));
                break;
            }
        } else {
            ghost[ind]->set_dir(ghost[ind]->get_lastdir());
        }
        return;
    }

    if(ghost[ind]->out_gate) {
        ghost[ind]->out_gate = false;
        ghost[ind]->state = _normal;
        return;
    }

    disx = x_gate - gpx;
    disy = y_gate - gpy;

    if(disx > 0) {
        if(dir_permise[_right] && lastdir != _left)
            ghost[ind]->set_dir(_right);
        else {
            if(disy >= 0) {
                if(dir_permise[_down])
                    ghost[ind]->set_dir(_down);
                else if(dir_permise[_up])
                    ghost[ind]->set_dir(_up);
            } else if(disy < 0){
                if(dir_permise[_up])
                    ghost[ind]->set_dir(_up);
                else if(dir_permise[_down])
                    ghost[ind]->set_dir(_down);
            }
        }
    } else if(disx < 0) {
        if(dir_permise[_left] && lastdir != _right)
            ghost[ind]->set_dir(_left);
        else {
            if(disy >= 0) {
                if(dir_permise[_down])
                    ghost[ind]->set_dir(_down);
                else if(dir_permise[_up])
                    ghost[ind]->set_dir(_up);
            } else if(disy < 0){
                if(dir_permise[_up])
                    ghost[ind]->set_dir(_up);
                else if(dir_permise[_down])
                    ghost[ind]->set_dir(_down);
            }
        }
    } else if(disy > 0) {
        if(dir_permise[_down] && lastdir != _up)
            ghost[ind]->set_dir(_down);
        else {
            if(dir_permise[_left])
                ghost[ind]->set_dir(_left);
            else if(dir_permise[_right])
                ghost[ind]->set_dir(_right);
        }
    } else if(disy < 0) {
        if(dir_permise[_up] && lastdir != _down)
            ghost[ind]->set_dir(_up);
        else {
            if(dir_permise[_left])
                ghost[ind]->set_dir(_left);
            else if(dir_permise[_right])
                ghost[ind]->set_dir(_right);
        }
    }
}


