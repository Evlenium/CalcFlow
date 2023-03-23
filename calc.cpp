
#include "calc.h"

#include "timer.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QSpinBox>

#include <cmath>

Calc::Calc(QObject* parent)
    : QObject { parent }
{
}

void Calc::calc()
{
    emit dsbx_S_setValue(S());

    //        Гидравлический диаметр м	Dг	0,05
    //        Динамическая вязкость Па*с	µ	0,0000181
    //        Кинематическая вязкость м2/с	ν	0,0000156
    //        Площадь трубы м2	S	0,001963495
    //        Re=(Q/3600*Dг)/(ν*S)

    //   emit dsbx_value_re_setValue(Re());
    // emit dsbx_value_setValue(1 / density_t());
    //   emit dsbx_mass_flow_setValue(density_t() * flow());
    if (map.contains(cbx_paramCalc))
        *map[cbx_paramCalc] = dsbx_OfParamCalcDouble->value();

    static const std::map<QStringView, std::function<void()>, std::less<>> map {
        { L"Массовый расход", [this] {
             calc_volume_flow_();
             // calc_volume_flow();
             calc_flow_speed();
             calc_heat_power();
             specific_volume();
         } },
        { L"Объёмный расход", [this] {
             calc_flow_speed();
             calc_mass_flow();
             calc_heat_power();
             specific_volume();
         } },
        { L"Скорость потока", [this] {
             calc_mass_flow();
             calc_volume_flow();
             calc_heat_power();
             specific_volume();
         } },
        { L"Тепловая мощность", [this] {
             calc_flow_speed();
             calc_mass_flow();
             calc_volume_flow();
             specific_volume();
         } },
    };

    if (map.contains(cbx_paramCalc))
        map.at(cbx_paramCalc)();

    S();
    Re();

    emit dsbx_dencity_setValue(density_t());
    emit dsbx_dynamic_viscocity_setValue(dynamic_viscosity_t());
    emit dsbx_kinematic_viscocity_setValue(kinematic_viscosity_t());

    emit dsbx_value_re_setValue(Re_);
    emit dsbx_S_setValue(S_);
    emit dsbx_value_flow_setValue(volume_flow_);
    emit dsbx_entalpy_setValue(entalpy_);
    emit dsbx_flow_speed_setValue(flow_speed_);
    emit dsbx_mass_flow_setValue(mass_flow_);
    emit dsbx_value_setValue(value_);

    //    ui->emit dsbx_OfParamCalcDouble_setValue((this->*map[cbx_paramCalc])());
}

void Calc::updateMap()
{
    if (map.contains(cbx_paramCalc)) {
        dsbx_OfParamCalcDouble->setValue(*map[cbx_paramCalc]);
        dsbx_OfParamCalcDouble->setSuffix(map_suff[cbx_paramCalc]);
    }
}

double Calc::calc_flow_speed() { return flow_speed_ = volume_flow_ / S_ / 3600; }

double Calc::calc_mass_flow() { return mass_flow_ = density_t() * volume_flow_; }

double Calc::calc_heat_power() { return heat_power_ = flow_t(); }

double Calc::calc_volume_flow() { return volume_flow_ = flow_speed_ * S_ * 3600; }

double Calc::calc_volume_flow_() { return volume_flow_ = mass_flow_ / density_t(); }

double Calc::specific_volume() { return value_ = 1 / density_t(); }

double Calc::Dn_t_(const QString& str)
{
    static QRegularExpression re(R"((.+\s)?(\d+))");
    if (auto match { re.match(str) }; match.hasMatch()) {
        bool ok {};
        Dn_ = match.captured(2).toDouble(&ok);
    }
    S();
    return Dn_;
}

double Calc::flow_t() { return (map.contains(cbx_paramCalc)) ? *map[cbx_paramCalc] : 0.0; }

double Calc::Re() { return Re_ = ((volume_flow_ / 3600) * (Dn_ / 1000)) / (S_ * kinematic_viscosity_t()); }

void Calc::setCbx_paramCalc(const QString& newCbx_paramCalc)
{
    cbx_paramCalc = newCbx_paramCalc;
    updateMap();
}

double Calc::S() { return S_ = acos(-1.0) * pow(Dn_ / 1000, 2) / 4; }
