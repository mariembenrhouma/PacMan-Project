#include "item.h"
#include "mainwindow.h"

Ghost::Ghost(int j, int i, int c):px(j),py(i),color(c){
    /*setZValue(3); Affecte z à la valeur Z de l'élément. La valeur Z décide de l'ordre d'empilement
                  des éléments voisins de même niveau (même parent ou sans parent). Un élément
                   de valeur Z élevée sera dessiné au dessus d'un élément de même niveau
                   ayant une valeur Z plus basse.*/
    dir = _stop;
    nextdir = _stop;
    lastdir = _down;
    changept = -1;
    inbox = false; //les fantomes dans la case
    etat_item[4][0].load(":/rsc2/bl1.png");
    etat_item[4][1].load(":/rsc2/bl2.png");
    switch(color) {
        case _red:
        setPixmap(QPixmap(":/rsc2/redright1.png")); //place l'élement dans la map
        etat_item[_right][0].load(":/rsc2/redright1.png");
        etat_item[_right][1].load(":/rsc2/redright2.png");
        etat_item[_up][0].load(":/rsc2/redup1.png");
        etat_item[_up][1].load(":/rsc2/redup2.png");
        etat_item[_left][0].load(":/rsc2/redleft1.png");
        etat_item[_left][1].load(":/rsc2/redleft2.png");
        etat_item[_down][0].load(":/rsc2/reddown1.png");
        etat_item[_down][1].load(":/rsc2/reddown2.png");
        break;
    case _yellow:
        setPixmap(QPixmap(":/rsc2/yellowright1.png"));
        etat_item[_right][0].load(":/rsc2/yellowright1.png");
        etat_item[_right][1].load(":/rsc2/yellowright2.png");
        etat_item[_up][0].load(":/rsc2/yellowup1.png");
        etat_item[_up][1].load(":/rsc2/yellowup2.png");
        etat_item[_left][0].load(":/rsc2/yellowleft1.png");
        etat_item[_left][1].load(":/rsc2/yellowleft2.png");
        etat_item[_down][0].load(":/rsc2/yellowdown1.png");
        etat_item[_down][1].load(":/rsc2/yellowdown2.png");
        break;
    case _green:
        setPixmap(QPixmap(":/rsc2/greenright1.png"));
        etat_item[_right][0].load(":/rsc2/greenright1.png");
        etat_item[_right][1].load(":/rsc2/greenright2.png");
        etat_item[_up][0].load(":/rsc2/greenup1.png");
        etat_item[_up][1].load(":/rsc2/greenup2.png");
        etat_item[_left][0].load(":/rsc2/greenleft1.png");
        etat_item[_left][1].load(":/rsc2/greenleft2.png");
        etat_item[_down][0].load(":/rsc2/greendown1.png");
        etat_item[_down][1].load(":/rsc2/greendown2.png");
        break;
    case _pink:
        setPixmap(QPixmap(":/rsc2/pinkright1.png"));
        etat_item[_right][0].load(":/rsc2/pinkright1.png");
        etat_item[_right][1].load(":/rsc2/pinkright2.png");
        etat_item[_up][0].load(":/rsc2/pinkup1.png");
        etat_item[_up][1].load(":/rsc2/pinkup2.png");
        etat_item[_left][0].load(":/rsc2/pinkleft1.png");
        etat_item[_left][1].load(":/rsc2/pinkleft2.png");
        etat_item[_down][0].load(":/rsc2/pinkdown1.png");
        etat_item[_down][1].load(":/rsc2/pinkdown2.png");
        break;
    }

}

void Ghost::move_up() {
    if(map[py-1][px] == '0' || (inbox == true && map[py-1][px]=='2')) {
        if(state == _normal)
            setPixmap(etat_item[_up][posy&1]);// posy elle prend 0 ou 1
        else if(state == _blue)
            setPixmap(etat_item[4][posy&1]);
        setY(--posy);
        if((static_cast<int>(y())-y_lim)%_width==0) { //ne pas depasser la limite (les murs)
            py--;
            dir = _stop;
        }
    } else {
        dir = _stop;
    }
    if(collidesWithItem(pac,Qt::IntersectsItemShape)){ //intersection entre pacman et fantome
        if(state == _normal)
            islose = true;
        else if(state == _blue) {
            setPixmap(QPixmap(":/rsc2/go_togate.png"));
            totalscore +=200;
            score->setText(QString::number(totalscore));
            inbox = true;
            state = _go_togate; //les yeux s'enfuirent
        }
    }
}

void Ghost::move_down() {
    if(map[py+1][px] == '0' || (inbox == true && map[py+1][px]=='2')) {
        if(state == _normal)
            setPixmap(etat_item[_down][posy&1]);
        else if(state == _blue)
            setPixmap(etat_item[4][posy&1]);
        setY(++posy);
        if((static_cast<int>(y())- y_lim)%_width==0) {
            py++;
            dir = _stop;
        }
    } else {
        dir = _stop;
    }
    if(collidesWithItem(pac,Qt::IntersectsItemShape)){
        if(state == _normal)
            islose = true;
        else if(state == _blue) {
            setPixmap(QPixmap(":/rsc2/go_togate.png"));
            totalscore +=200;
            score->setText(QString::number(totalscore));
            inbox = true;
            state = _go_togate;
        }
    }
}

void Ghost::moveleft() {
    if(map[py][px-1] == '0' || (inbox == true && map[py][px-1]=='2')) {
        if(state == _normal)
            setPixmap(etat_item[_left][posx&1]);
        else if(state == _blue)
            setPixmap(etat_item[4][posx&1]);
        setX(--posx);
        if((static_cast<int>(x())-x_lim)%_width==0) {
            px--;
            dir = _stop;
        }
    } else {
        dir = _stop;
    }
    if(collidesWithItem(pac,Qt::IntersectsItemShape)){
        if(state == _normal)
            islose = true;
        else if(state == _blue) {
            setPixmap(QPixmap(":/rsc2/go_togate.png"));
            totalscore +=200;
            score->setText(QString::number(totalscore));
            inbox = true;
            state = _go_togate;
        }
    }
}

void Ghost::moveright() {
    if(map[py][px+1] == '0' || (inbox == true && map[py][px+1]=='2')) {
        if(state == _normal)
            setPixmap(etat_item[_right][posx&1]);
        else if(state == _blue)
            setPixmap(etat_item[4][posx&1]);
        setX(++posx);
        if((static_cast<int>(x())-x_lim)%_width==0) {
            px++;
            dir = _stop;
        }
    } else {
        dir = _stop;
    }
    if(collidesWithItem(pac,Qt::IntersectsItemShape)){
        if(state == _normal)
            islose = true;
        else if(state == _blue) {
            setPixmap(QPixmap(":/rsc2/go_togate.png"));
            totalscore +=200;
            score->setText(QString::number(totalscore));
            inbox = true;
            state = _go_togate;
        }
    }
}

void Ghost::check_dirchange() {

}
