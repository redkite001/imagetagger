#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("RedKite");
    QApplication::setOrganizationDomain("redkite.be");
    QApplication::setApplicationName("Image Tagger");
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
 * - Add a version box and automatically increment build number (and by hand major and minor)
 * - Add the possibility to drop an image
 */


/*****
 * NOTES
 * - For allowing to load jpeg files on Windows, I needed to copy next to my .exe the plugin from  "C:\Qt\4.8.0\plugins\imageformats\qjpeg4.dll" to "Image Tagger\imageformats\qjpeg4.dll"
 */
