
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QDoubleSpinBox;

class MainWindow : public QMainWindow {
    Q_OBJECT



    QDoubleSpinBox* dsbx_val_S;
    QDoubleSpinBox* dsbx_val_dencity;
    QDoubleSpinBox* dsbx_val_dynamic_viscocity;
    QDoubleSpinBox* dsbx_val_entalpy;
    QDoubleSpinBox* dsbx_val_flow_speed;
    QDoubleSpinBox* dsbx_val_kinematic_viscocity;
    QDoubleSpinBox* dsbx_val_mass_flow;
    QDoubleSpinBox* dsbx_val_value_flow;
    QDoubleSpinBox* dsbx_val_value;

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

    double calc_flow_speed();
    double calc_heat_power();
    double calc_mass_flow();
    double calc_volume_flow();

    /////////////////////

    double kinematic_viscosity_t();
    double dynamic_viscosity_t();
    double density_t();

    double val_Dn_t();

    double flow_t();
    double Re();

    //    double val_dencity {};
    //    double val_dynamic_viscocity {};
    //    double val_kinematic_viscocity {};

    double val_flow_speed {};
    double val_heat_power {};
    double val_mass_flow {};
    double val_volume_flow {};

    double val_Re;
    double val_S {};
    double val_entalpy {};
    double val_value {};
    double val_value_flow {};

    double S();

    void testj();

    using CalcFunc = decltype(&MainWindow::calc_volume_flow);

    std::map<QStringView, double*, std::less<>> map {
        { L"Массовый расход", &val_mass_flow },
        { L"Объёмный расход", &val_volume_flow },
        { L"Скорость потока", &val_flow_speed },
        { L"Тепловая мощность", &val_heat_power },
    };
};

#endif // MAINWINDOW_H
