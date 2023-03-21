
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow* ui;
    // double S;

    void calc();
    void loadJson(const QString& str_cbx);

    void load();
    void save();

    double calc_speed();
    double calc_mass_flow();
    double calc_heat_power();
    double calc_volume_flow();

    /////////////////////

    double kinematic_viscosity();
    double dynamic_viscosity();
    double density();
    double flow();
    double Re();
    double val_Dn();

    double S();

    void testj();

    using CalcFunc = decltype(&MainWindow::calc_volume_flow);

    std::map<QStringView, CalcFunc, std::less<>> map {
        { L"Объёмный расход", &MainWindow::calc_volume_flow },
        { L"Массовый расход", &MainWindow::calc_mass_flow },
        { L"Скорость потока", &MainWindow::calc_speed },
        { L"Тепловая мощность", &MainWindow::calc_heat_power },
    };
};

#endif // MAINWINDOW_H
