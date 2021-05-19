#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow> //La classe QMainWindow fournit une fenêtre d'application principale
#include <QGraphicsView> //La classe QGraphicsView fournit un widget pour afficher le contenu d'un QGraphicsScene.
#include <QGraphicsScene> /*QGraphicsScene implémente un gestionnaire de graphe de scène 2d,
                           et chaque QGraphicsItem représente un noeud*/
#include <QGraphicsItem> //La classe QGraphicsItem est la classe de base pour tous les éléments graphiques dans un QGraphicsScene.
#include <QPixmap>     //La classe QPixmap est une représentation d'image hors écran qui peut être utilisée comme dispositif de peinture

#include <QKeyEvent>
#include <QString>
#include <QObject>  //La classe QObject est la classe de base de tous les objets Qt
#include <QCoreApplication> //La classe QCoreApplication fournit une boucle d'événements pour les applications Qt sans interface utilisateur.
#include <QDebug> //La classe QDebug fournit un flux de sortie pour les informations de débogage.
#include <QTimer> //La classe QTimer fournit des minuteries répétitives et uniques.
#include "QThread" //La classe QThread fournit un moyen indépendant de la plate-forme de gérer les threads.(thread=message)
#include "item.h"

#define _width 20       //width of items (largeur)
#define x_lim 30     //(x of qgraphicsview) - 20
#define y_lim 30     //(y of qgraphicsview) - 20
#define _slptime 5000   //moving sleep time

/*item move direction*/
#define _up 0
#define _down 1
#define _left 2
#define _right 3
#define _stop 4

/*ghost color*/
#define _red 0
#define _yellow 1
#define _pink 2
#define _green 3

/*ghost state*/
#define _normal 0
#define _blue 1
#define _go_togate 2

/*declare shared resource*/
extern char map[22][31];
extern QGraphicsPixmapItem *block[22][31];
extern QGraphicsScene *scene;
extern int nb_balls, eat_ball ,totalscore, x_gate, y_gate;
extern QLabel *score_title, *score, *winning, *losing;    //QLabel est l'un des widgets standard de Qt et peut afficher un texte ou un QPixmap, avec ou sans fenêtre.
extern QString eat_str;
extern Item *pac, *ghost[4];
extern QVector<QGraphicsPixmapItem*> pball;
extern bool islose, iswin;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void set_board(); // tableau
    void chase(int);
    void escape(int);
    void go_togate(int);
    ~MainWindow() override;//Lorsque la classe de base et la classe dérivée ont des fonctions membres avec exactement le même nom, le même type de retour et la même liste d'arguments, on dit alors qu'il s'agit d'un remplacement de fonction

private slots:   //slots permettent d'interconnecter des objets Qt entre eux
    void ghost_move();
    void ball_show();

private:
    Ui::MainWindow *ui;  
    QTimer *timer, *show_time;
    bool altern;
protected:
    void keyPressEvent(QKeyEvent *event) override;
};


#endif // MAINWINDOW_H
