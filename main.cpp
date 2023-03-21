
#include "mainwindow.h"
#include "menu.h"

#include <QApplication>
void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QByteArray localMsg = msg.toUtf8();
    const char* file = context.file ? context.file : "";
    //    const char* function = context.function ? context.function : "";

    const char* file_ { file };
    while (*file > 0) {
        if (*file == '\\')
            file_ = file + 1;
        ++file;
    }
    file = file_;

    switch (type) {
    case QtDebugMsg:
        fprintf(stdout, "Debug: %s\n\t%s : %u\n", localMsg.constData(), file, context.line /*, function*/);
        break;
    case QtInfoMsg:
        fprintf(stdout, "Info: %s\n\t%s : %u\n", localMsg.constData(), file, context.line /*, function*/);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning:%s\n\t%s : %u\n", localMsg.constData(), file, context.line /*, function*/);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical:%s\n\t%s : %u\n", localMsg.constData(), file, context.line /*, function*/);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal:%s\n\t%s : %u\n", localMsg.constData(), file, context.line /*, function*/);
        break;
    }
}

int main(int argc, char* argv[])
{
    //qInstallMessageHandler(myMessageOutput);
    QApplication a(argc, argv);

    QApplication::setOrganizationName("Elemer");
    QApplication::setApplicationName("CalcFlow");

    QFont f;
    f.setPixelSize(16);
    QApplication::setFont(f);

    MainWindow w;
    w.show();
    return a.exec();
}
