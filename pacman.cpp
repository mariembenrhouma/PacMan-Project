#include "mainwindow.h"
 #include "item.h"

void Item::move_up(){}; //out of line definition

Pacman::Pacman(int j, int i):px(j),py(i),dir(0) {
    dir = _stop;
    nextdir = _stop;
    changept = -1;
    setPixmap(QPixmap(":/rsc/a1.png"));
    for(int i=0;i<4;++i) {
        for(int j=0;j<10;++j)
            etat_item[i][j] = new etat;
    }

    etat_item[_right][0]->img.load(":/rsc/a1.png");
    etat_item[_right][1]->img.load(":/rsc/a2.png");
    etat_item[_right][2]->img.load(":/rsc/a3.png");
    etat_item[_right][3]->img.load(":/rsc/a4.png");
    etat_item[_right][4]->img.load(":/rsc/a5.png");
    etat_item[_right][5]->img.load(":/rsc/a6.png");
    etat_item[_right][6]->img.load(":/rsc/a5.png");
    etat_item[_right][7]->img.load(":/rsc/a4.png");
    etat_item[_right][8]->img.load(":/rsc/a3.png");
    etat_item[_right][9]->img.load(":/rsc/a2.png");
    anindex[_right] = etat_item[_right][0];

    etat_item[_up][0]->img.load(":/rsc/a1.png");
    etat_item[_up][1]->img.load(":/rsc/b2.png");
    etat_item[_up][2]->img.load(":/rsc/b3.png");
    etat_item[_up][3]->img.load(":/rsc/b4.png");
    etat_item[_up][4]->img.load(":/rsc/b5.png");
    etat_item[_up][5]->img.load(":/rsc/b6.png");
    etat_item[_up][6]->img.load(":/rsc/b5.png");
    etat_item[_up][7]->img.load(":/rsc/b4.png");
    etat_item[_up][8]->img.load(":/rsc/b3.png");
    etat_item[_up][9]->img.load(":/rsc/b2.png");
    anindex[_up] = etat_item[_up][0];

    etat_item[_left][0]->img.load(":/rsc/a1.png");
    etat_item[_left][1]->img.load(":/rsc/c2.png");
    etat_item[_left][2]->img.load(":/rsc/c3.png");
    etat_item[_left][3]->img.load(":/rsc/c4.png");
    etat_item[_left][4]->img.load(":/rsc/c5.png");
    etat_item[_left][5]->img.load(":/rsc/c6.png");
    etat_item[_left][6]->img.load(":/rsc/c5.png");
    etat_item[_left][7]->img.load(":/rsc/c4.png");
    etat_item[_left][8]->img.load(":/rsc/c3.png");
    etat_item[_left][9]->img.load(":/rsc/c2.png");
    anindex[_left] = etat_item[_left][0];

    etat_item[_down][0]->img.load(":/rsc/a1.png");
    etat_item[_down][1]->img.load(":/rsc/d2.png");
    etat_item[_down][2]->img.load(":/rsc/d3.png");
    etat_item[_down][3]->img.load(":/rsc/d4.png");
    etat_item[_down][4]->img.load(":/rsc/d5.png");
    etat_item[_down][5]->img.load(":/rsc/d6.png");
    etat_item[_down][6]->img.load(":/rsc/d5.png");
    etat_item[_down][7]->img.load(":/rsc/d4.png");
    etat_item[_down][8]->img.load(":/rsc/d3.png");
    etat_item[_down][9]->img.load(":/rsc/d2.png");
    anindex[_down] = etat_item[_down][0];

    for(int i=0;i<9;++i) {
        etat_item[_right][i]->next = etat_item[_right][i+1];
        etat_item[_up][i]->next = etat_item[_up][i+1];
        etat_item[_left][i]->next = etat_item[_left][i+1];
        etat_item[_down][i]->next = etat_item[_down][i+1];
    }
    etat_item[_right][9]->next = etat_item[_right][0];
    etat_item[_up][9]->next = etat_item[_up][0];
    etat_item[_left][9]->next = etat_item[_left][0];
    etat_item[_down][9]->next = etat_item[_down][0];
}

void Pacman::move_up(){
    int posy = static_cast<int>(this->y()); //Returns a value of type int
    int remender = (posy-y_lim)%_width; //definir dans quelle ligne se trouve le pacman
    py = (posy-y_lim)/_width;
    dir = _up;
    while(remender){            //adjust coordinate
        this->setPixmap(anindex[_up]->img);
        anindex[_up]=anindex[_up]->next;
        this->setY(--posy);
        --remender;
        QCoreApplication::processEvents();
        QThread::usleep(_slptime);
        if(dir!=_up)
            return;
        if(islose) {
            return;
        }
    }

    if(block[py][px]) {
        if(addscore())
            return;
    }
    if(py == changept) {        //changedir
        check_dirchange();
        return;
    }

    while(py>0 && map[py-1][px] == '0'){
        for(int i=0; i<_width; ++i) {
            this->setPixmap(anindex[_up]->img);
            anindex[_up]=anindex[_up]->next;
            this->setY(--posy);
            QCoreApplication::processEvents();
            QThread::usleep(_slptime);
            if(dir != _up)
                return;
            if(islose) {
                return;
            }
        }
        --py;

        if(block[py][px]) {
            if(addscore())
                return;
        }
        if(py == changept) {        //changedir
            check_dirchange();
            return;
        }
    }
    dir = _stop;
}

void Pacman::move_down() {
    posy = static_cast<int>(this->y());
    int remender = _width - (posy-y_lim)%_width;
    py = (posy-y_lim)/_width;
    if(remender == _width && map[py+1][px] == '1')
        return;
    dir = _down;
    while(remender){           //adjust coordinate
        this->setPixmap(anindex[_down]->img);
        anindex[_down]=anindex[_down]->next;
        this->setY(++posy);
        --remender;
        QCoreApplication::processEvents();
        QThread::usleep(_slptime);
        if(dir!=_down)
            return;
        if(islose) {
            return;
        }
    }
    ++py;

    if(block[py][px]) {
        if(addscore())
            return;
    }
    if(py == changept) {        //changedir
        check_dirchange();
        return;
    }

    while(py<21 && map[py+1][px] == '0'){
        for(int i=0; i<_width; ++i) {
            this->setPixmap(anindex[_down]->img);
            anindex[_down]=anindex[_down]->next;
            this->setY(++posy);
            QCoreApplication::processEvents();
            QThread::usleep(_slptime);
            if(dir!=_down)
                return;
            if(islose) {
                return;
            }
        }
        ++py;

        if(block[py][px]) {
            if(addscore())
                return;
        }
        if(py == changept) {        //changedir
            check_dirchange();
            return;
        }
    }
    dir = _stop;
}

void Pacman::moveleft() {
    posx = static_cast<int>(this->x());
    int remender = (posx-x_lim)%_width;
    dir = _left;
    while(remender){            //adjust coordinate
        this->setPixmap(anindex[_left]->img);
        anindex[_left]=anindex[_left]->next;
        this->setX(--posx);
        --remender;
        QCoreApplication::processEvents();
        QThread::usleep(_slptime);
        if(dir!=_left)
            return;
        if(islose) {
            return;
        }
    }

    if(block[py][px]) {
        if(addscore())
            return;
    }
    if(px == changept) {        //changedir
        check_dirchange();
        return;
    }

    while(py>0 && map[py][px-1] == '0'){
        for(int i=0; i<_width; ++i) {
            this->setPixmap(anindex[_left]->img);
            anindex[_left]=anindex[_left]->next;
            this->setX(--posx);
            QCoreApplication::processEvents();
            QThread::usleep(_slptime);
            if(dir != _left)
                return;
            if(islose) {
                return;
            }
        }
        --px;
        if(block[py][px]) {
            if(addscore())
                return;
        }
        if(px == changept) {        //changedir
            check_dirchange();
            return;
        }
    }
    dir = _stop;
}

void Pacman::moveright() {
    posx = static_cast<int>(this->x());
    int remender = _width - (posx-x_lim)%_width;
    px = (posx-x_lim)/_width;
    if(remender == _width && map[py][px+1] == '1')
        return;
    dir = _right;
    while(remender){
        this->setPixmap(anindex[_right]->img);
        anindex[_right]=anindex[_right]->next;
        this->setX(++posx);
        --remender;
        QCoreApplication::processEvents();
        QThread::usleep(_slptime);
        if(dir!=_right)
            return;
        if(islose) {
            return;
        }
    }
    ++px;
    if(block[py][px]) {
        if(addscore())
            return;
    }
    if(px == changept) {        //changedir
        check_dirchange();
        return;
    }

    while(px<30 && map[py][px+1] == '0'){
        for(int i=0; i<_width; ++i) {
            this->setPixmap(anindex[_right]->img);
            anindex[_right]=anindex[_right]->next;
            this->setX(++posx);
            QCoreApplication::processEvents();
            QThread::usleep(_slptime);
            if(dir != _right)
                return;
            if(islose) {
                return;
            }
        }
        ++px;
        if(block[py][px]) {
            if(addscore())
                return;
        }
        if(px == changept) {        //changedir
            check_dirchange();
            return;
        }
    }
    dir = _stop;
}

void Pacman::check_dirchange() {
    changept = -1;
    int tmp = nextdir;
    nextdir = _stop;
    switch(tmp) {
    case _up:
        move_up();
        break;
    case _down:
        move_down();
        break;
    case _left:
        moveleft();
        break;
    case _right:
        moveright();
        break;
    }
}

int Pacman::addscore() {
    if(py == y_gate && px == x_gate) {
        return 0;
    }
    eat_ball++;
    totalscore+=10;
    score->setText(QString::number(totalscore));
    for(int i=0;i<pball.size();++i) {
        if(block[py][px] == pball.at(i)) {
            pball.remove(i);
            for(int j=0; j<4 ;++j) {
                if(ghost[j]->state == _normal) {
                     ghost[j]->state = _blue;
                }
                ghost[j]->time_gate = 300;
            }
        }
    }
    delete block[py][px];
    block[py][px] = nullptr;
    if(eat_ball == nb_balls) {
        winning->show();
        iswin=true;
        dir = _stop;
        return 1;
    }
    else {
        return 0;
    }
}
