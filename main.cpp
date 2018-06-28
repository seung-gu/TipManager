#include <QApplication>
#include "tipmanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TipManager tipDialog;
    tipDialog.show();

    return app.exec();
}
