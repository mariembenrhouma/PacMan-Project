#ifndef ITEM_H
#define ITEM_H

#include <QGraphicsItem> /* Tous les éléments affichés dans une scène sont des QGraphicsItem, et si tu gardes leur pointeur,
                          tu peux les déplacer, les modifier, ou les effacer...*/
#include <QPixmap>
#include <QDebug>
#include <QLabel>
class Item: public QGraphicsPixmapItem {
public:
    virtual void move_up()=0; //virtuelle pure
    virtual void move_down()=0;
    virtual void moveleft()=0;
    virtual void moveright()=0;
    virtual int get_dir()=0;
    virtual int get_px()=0;
    virtual int get_py()=0;
    virtual int get_posx()=0;
    virtual int get_posy()=0;
    virtual int get_changept()=0;
    virtual void set_px(int)=0;
    virtual void set_py(int)=0;
    virtual void set_posx(int)=0;
    virtual void set_posy(int)=0;
    virtual void set_dir(int)=0;
    virtual void set_nextdir(int)=0;
    virtual void set_changept(int)=0;
    virtual void check_dirchange()=0;
    virtual bool isinbox()=0;
    virtual void setinbox(bool)=0;
    virtual int get_lastdir()=0;

    int state, time_gate,counter;
    bool out_gate;

    struct etat {
        QPixmap img;
        etat *next;
    };
};

class Pacman: public Item {
public:
    Pacman(int,int);
    void move_up();
    void move_down();
    void moveleft();
    void moveright();
    int get_px(){return px;}
    int get_py(){return py;}
    int get_posx(){return posx;}
    int get_posy(){return posy;}
    int get_dir(){return dir;}
    int get_nextdir(){return nextdir;}
    int get_changept(){return changept;}
    void set_px(int n){px=n;}
    void set_py(int n){py=n;}
    void set_posx(int n){posx=n;}
    void set_posy(int n){posy=n;}
    void set_dir(int d){dir = d;}
    void set_nextdir(int nd){nextdir = nd;}
    void set_changept(int npt){changept=npt;}
    void check_dirchange();
    bool isinbox(){return false;}
    void setinbox(bool b){inbox = b;}
    int get_lastdir(){return 0;}
private:
    int addscore();
    int px,py,posx,posy,dir,nextdir,changept;
    bool inbox;
    etat *etat_item[4][10], *anindex[4];
};

class Ghost: public Item {
public:
    Ghost(int,int,int);
    void move_up();
    void move_down();
    void moveleft();
    void moveright();
    int get_px(){return px;}
    int get_py(){return py;}
    int get_posx(){return posx;}
    int get_posy(){return posy;}
    int get_dir(){return dir;}
    int get_nextdir(){return nextdir;}
    int get_changept(){return changept;}
    void set_px(int n){px=n;}
    void set_py(int n){py=n;}
    void set_posx(int n){posx=n;}
    void set_posy(int n){posy=n;}
    void set_dir(int d){lastdir = dir = d;}
    void set_nextdir(int nd){nextdir = nd;}
    void set_changept(int npt){changept=npt;}
    bool isinbox(){return inbox;}
    void setinbox(bool b){inbox = b;}
    int get_lastdir(){return lastdir;}
    void check_dirchange();
private:
    int px,py,posx,posy,dir,nextdir;
    int changept,color,lastdir;
    bool inbox;
    QPixmap etat_item[5][2];
};

#endif // ITEM_H
