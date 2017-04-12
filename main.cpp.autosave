#include "core.h"
#include <QApplication>
#include "logging.h"

//http://magtuxdev.blogspot.ru/2013/08/git-qt-creator-github-windows.html
//http://exlmoto.ru/git-and-qt-creator/

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

//    std::cout << "main(0). logging::getViewFunctions() == " << Logging::viewFunctions << std::endl;
//    loghelper::mdelay(5);

    logging::setFlagsString("all");
    logging::qt::logPrint("e", __FUNCTION__, __FILE__, "main()");

    QCoreApplication::setApplicationName("QtLogViewer");
    QCoreApplication::setOrganizationName("Alex V. Smith");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    Core w;
    w.show();

    return a.exec();
}
