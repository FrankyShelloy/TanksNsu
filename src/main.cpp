#include <QApplication>
#include <QGraphicsView>
#include "GameScane.h"
#include "GlobalConstants.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  GameScane* scene = new GameScane();
  QGraphicsView* view = new QGraphicsView(scene);

  view->setFixedSize(kSceneWidth, kSceneHeight);
  view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view->setWindowTitle("ТанкиNSU");
  view->show();

  return app.exec();
}
