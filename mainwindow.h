
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLineEdit>
#include <QMainWindow>
#include <map>

class MyLe : public QLineEdit {
    QString suffix_ { "%1" };

public:
    MyLe(QWidget* parent = nullptr)
        : QLineEdit { parent }
    {
        setReadOnly(true);
    }
    virtual ~MyLe() { }
    void setSuffix(QString suffix) { suffix_ = "%1 " + suffix; }
    void setValue(double val) { setText(suffix_.arg(val)); }
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

    MyLe* dsbx_val_S;
    MyLe* dsbx_val_dencity;
    MyLe* dsbx_val_dynamic_viscocity;
    MyLe* dsbx_val_entalpy;
    MyLe* dsbx_val_flow_speed;
    MyLe* dsbx_val_kinematic_viscocity;
    MyLe* dsbx_val_mass_flow;
    MyLe* dsbx_val_value_flow;
    MyLe* dsbx_val_value;
    MyLe* dsbx_val_value_re;

    /*


<widget class="QLabel" name="lbl_S_|||Площадь</string>
<widget class="QLabel" name="lbl_dencity_|||Плотность среды</string>
<widget class="QLabel" name="lbl_dynamic_viscocity_|||Динамическая вязкость среды</string>
<widget class="QLabel" name="lbl_entalpy_|||Энтальпия среды</string>
<widget class="QLabel" name="lbl_flow_speed_|||Скорость потока</string>
<widget class="QLabel" name="lbl_kinematic_viscocity_|||Кинематическая вязкость среды</string>
<widget class="QLabel" name="lbl_mass_flow_|||Массовый расход среды</string>
<widget class="QLabel" name="lbl_name_Re_|||Число Рейнольдса</string>
<widget class="QLabel" name="lbl_value_flow_|||Объёмный расход среды</string>
<widget class="QLabel" name="lbl_value_|||Удельный объём среды</string>

*/

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
