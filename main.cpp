#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}









/*****
 * TODO
 *
 * - Grid function & detec forms?
 * - choose language of images (_EN _FR)
 * - choose different tags
 * - Add zoom percentage and allow scroll bar
 * -
 */
