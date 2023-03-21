
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "tempmodel.h"
#include "timer.h"
#include <QDebug>
#include <cmath>

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QSettings>

#define DSBXHIDE(X) ui->lbl_##X, ui->dsbx_##X
#define CBXHIDE(X) ui->lbl_##X, ui->cbx_##X

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug(__PRETTY_FUNCTION__);

    ui->setupUi(this);

    // tuple
    using key = std::tuple<int, int>;
    using val = std::tuple<
        std::vector<QWidget*>,
        QStringList,
        QStringList
        // QStringList,
        // QStringList,
        // QStringList //
        >;

    std::initializer_list<QWidget*> qweNotParamGas { DSBXHIDE(viscocity), CBXHIDE(viscocityType) };
    std::initializer_list<QWidget*> qweNotParamWaterSteam { DSBXHIDE(viscocity), CBXHIDE(viscocityType), CBXHIDE(measGas) };
    std::initializer_list<QWidget*> qweNotParamFluid { CBXHIDE(measGas), DSBXHIDE(pressure), CBXHIDE(typeOfPressure), CBXHIDE(temperature) };
    std::initializer_list<QString> lstDnMortiseGas { "25", "32", "40", "50 с сужением на 19", "50 с сужением на 25", "50", "65", "80 с сужением на 50", "80", "100", "150", "200", "250", "300" };
    std::initializer_list<QString> lstProbe { "100", "150", "200", "300", "400", "500", "600", "700", "800", "900", "1000", "1100", "1200", "1300", "1400", "1500", "1600", "1700", "1800", "1900", "2000" };
    std::initializer_list<QString> lstDnMortise { "25", "32", "40", "50", "65", "80", "100", "150", "200", "250", "300" };
    std::initializer_list<QString> lstParamGasWater { "Объёмный расход", "Массовый расход", "Скорость потока" };
    std::initializer_list<QString> lstParamSteam { "Объёмный расход", "Массовый расход", "Скорость потока", "Тепловая мощность" };
    std::initializer_list<QString> lstParamFluid { "Объёмный расход", "Скорость потока" };
    // std::initializer_list<QString> lstTypeMeasGas { "Воздух", "Кислород", "Азот", "Метан", "Природный газ", "Попутный нефтяной газ" };
    // std::initializer_list<QString> lstTypeOfPressure { "Избыточное", "Абсолютное" };
    // std::initializer_list<QString> lstViscocityType { "Кинематическая вязкость", "Динамическая вязкость и плотность" };

    ui->cbx_measGas->addItem("Воздух", .0);
    ui->cbx_measGas->addItem("Кислород", .0);
    ui->cbx_measGas->addItem("Азот", .0);
    ui->cbx_measGas->addItem("Метан", .0);
    ui->cbx_measGas->addItem("Природный газ", .0);
    ui->cbx_measGas->addItem("Попутный нефтяной газ", .0);

    ui->cbx_typeOfPressure->addItems({ "Избыточное", "Абсолютное" });
    ui->cbx_viscocityType->addItems({ "Кинематическая вязкость", "Динамическая вязкость и плотность" });

    std::map<key, val> ma {
        { { 0, 0 }, {
                        { qweNotParamGas }, { lstDnMortiseGas }, { lstParamGasWater },
                        // { lstTypeMeasGas },
                        // { lstTypeOfPressure },
                        // {},
                    } },
        { { 0, 1 }, {
                        { qweNotParamGas }, { lstProbe }, { lstParamGasWater },
                        // { lstTypeMeasGas },
                        // { lstTypeOfPressure },
                        // {},
                    } },
        { { 1, 0 }, {
                        { qweNotParamWaterSteam }, { lstDnMortise }, { lstParamGasWater },
                        // {},
                        // { lstTypeOfPressure },
                        // {},
                    } },
        { { 1, 1 }, {
                        { qweNotParamWaterSteam }, { lstProbe }, { lstParamGasWater },
                        // {},
                        // { lstTypeOfPressure },
                        // {},
                    } },
        { { 2, 0 }, {
                        { qweNotParamWaterSteam }, { lstDnMortise }, { lstParamSteam },
                        // {},
                        // { lstTypeOfPressure },
                        // {},
                    } },
        { { 2, 1 }, {
                        { qweNotParamWaterSteam }, { lstProbe }, { lstParamSteam },
                        // {},
                        // { lstTypeOfPressure },
                        // {},
                    } },
        { { 3, 0 }, {
                        { qweNotParamFluid }, { lstDnMortise }, { lstParamFluid },
                        // {},
                        // {},
                        // { lstViscocityType },
                    } },
        { { 3, 1 }, {
                        { qweNotParamFluid }, { lstProbe }, { lstParamFluid },
                        // {},
                        // {},
                        // { lstViscocityType },
                    } },
    };

    auto hw = [ma = std::move(ma), this] {
        for (auto* w : ui->widget->findChildren<QWidget*>())
            w->setVisible(true);

        auto& [wgt, diam, paramCalc /*, TypeMeasGas, TypeOfPressure, ViscocityType*/] = ma.at({ ui->cbx_MeasuredMedium->currentIndex(), ui->cbx_TypeFlowmeter->currentIndex() });
        for (auto hideWidget : wgt)
            hideWidget->setVisible(false);
        ui->cbx_Dn->clear();
        ui->cbx_Dn->addItems(diam);
        ui->cbx_paramCalc->clear();
        ui->cbx_paramCalc->addItems(paramCalc);
        // ui->cbx_measGas->clear();
        // ui->cbx_measGas->addItems(TypeMeasGas);
        //        ui->cbx_typeOfPressure->clear();
        //        ui->cbx_typeOfPressure->addItems(TypeOfPressure);
        //        ui->cbx_viscocityType->clear();
        //        ui->cbx_viscocityType->addItems(ViscocityType);

        // for (int i {}; i < ui->cbx_Dn->count(); ++i) {
        // static QRegularExpression re(R"((.+\s)?(\d+))");
        // bool ok {};
        // double val_Dn { re.match(diam[i]).captured(2).toDouble(&ok) };
        // ui->cbx_Dn->setItemData(i, val_Dn);
        // }
    };
    hw();

    connect(ui->cbx_MeasuredMedium, &QComboBox::currentIndexChanged, hw);
    connect(ui->cbx_TypeFlowmeter, &QComboBox::currentIndexChanged, hw);

    // connect(ui->cbx_measGas, &QComboBox::currentIndexChanged,loadJson());
    //  double S;
    //  double res_Dn = ui->cbx_Dn->currentData().toDouble();
    //  S = acos(-1.0)*pow(res_Dn/1000,2)/4;
    //  qDebug()<<S;
    //  qDebug() << __FUNCTION__ << ui->cbx_Dn->currentData().toDouble();

    connect(ui->cbx_Dn, &QComboBox::currentIndexChanged, this, &MainWindow::calc);
    connect(ui->cbx_MeasuredMedium, &QComboBox::currentIndexChanged, this, &MainWindow::calc);
    connect(ui->cbx_TypeFlowmeter, &QComboBox::currentIndexChanged, this, &MainWindow::calc);
    // connect(ui->cbx_measGas, &QComboBox::currentIndexChanged, this, &MainWindow::calc);
    connect(ui->cbx_paramCalc, &QComboBox::currentIndexChanged, this, &MainWindow::calc);
    connect(ui->cbx_temperature, &QComboBox::currentIndexChanged, this, &MainWindow::calc);
    connect(ui->cbx_typeOfPressure, &QComboBox::currentIndexChanged, this, &MainWindow::calc);
    connect(ui->cbx_viscocityType, &QComboBox::currentIndexChanged, this, &MainWindow::calc);
    connect(ui->dsbx_OfParamCalcDouble, &QDoubleSpinBox::valueChanged, this, &MainWindow::calc);
    connect(ui->dsbx_pressure, &QDoubleSpinBox::valueChanged, this, &MainWindow::calc);
    connect(ui->dsbx_viscocity, &QDoubleSpinBox::valueChanged, this, &MainWindow::calc);

    connect(ui->cbx_measGas, &QComboBox::currentTextChanged, this, &MainWindow::loadJson);
    // loadJson(ui->cbx_measGas->currentText());
    // connect(ui->cbx_measGas, &QComboBox::currentIndexChanged,&MainWindow::
    loadJson(ui->cbx_measGas->currentText());
    // loadJson();

    load();
}

MainWindow::~MainWindow()
{
    save();
    qDebug(__PRETTY_FUNCTION__);
    delete ui;
}

void MainWindow::calc()
{
    ui->lbl_S->setNum(S());

    //        Гидравлический диаметр м	Dг	0,05
    //        Динамическая вязкость Па*с	µ	0,0000181
    //        Кинематическая вязкость м2/с	ν	0,0000156
    //        Площадь трубы м2	S	0,001963495
    //        Re=(Q/3600*Dг)/(ν*S)

    ui->lbl_Re->setNum(Re());
    ui->lbl_kinematic_viscocity->setNum(kinematic_viscosity());
    ui->lbl_dynamic_viscocity->setNum(dynamic_viscosity());
    ui->lbl_density->setNum(density());
    ui->lbl_value->setNum(1 / density());
    ui->lbl_mass_flow->setNum(density() * flow());

    ui->dsbx_OfParamCalcDouble->setValue((this->*map[ui->cbx_paramCalc->currentText()])());
}

void MainWindow::loadJson(const QString& str_cbx)
{
    QFile file("../CalcViscocity.json");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        auto obj { QJsonDocument::fromJson(file.readAll()).object() };
        delete ui->cbx_temperature->model();
        ui->cbx_temperature->setModel(new TempModel(obj[str_cbx].toArray(), ui->cbx_temperature));
        ui->cbx_temperature->setModelColumn(3);
    } else {
        qDebug() << file.errorString();
    }
}

void MainWindow::load()
{
    QSettings settings;
    settings.beginGroup("MainWindow");

    restoreGeometry(settings.value("Geometry").toByteArray());
    restoreState(settings.value("State").toByteArray());

    ui->cbx_Dn->setCurrentIndex(settings.value("cbx_Dn").toInt());
    ui->cbx_MeasuredMedium->setCurrentIndex(settings.value("cbx_MeasuredMedium").toInt());
    ui->cbx_TypeFlowmeter->setCurrentIndex(settings.value("cbx_TypeFlowmeter").toInt());
    ui->cbx_measGas->setCurrentIndex(settings.value("cbx_measGas").toInt());
    ui->cbx_paramCalc->setCurrentIndex(settings.value("cbx_paramCalc").toInt());
    ui->cbx_temperature->setCurrentIndex(settings.value("cbx_temperature").toInt());
    ui->cbx_typeOfPressure->setCurrentIndex(settings.value("cbx_typeOfPressure").toInt());
    ui->cbx_viscocityType->setCurrentIndex(settings.value("cbx_viscocityType").toInt());
    ui->dsbx_OfParamCalcDouble->setValue(settings.value("dsbx_OfParamCalcDouble").toDouble());
    ui->dsbx_pressure->setValue(settings.value("dsbx_pressure").toDouble());
    ui->dsbx_viscocity->setValue(settings.value("dsbx_viscocity").toDouble());
    settings.endGroup();
}

void MainWindow::save()
{
    QSettings settings;
    settings.beginGroup("MainWindow");

    settings.setValue("Geometry", saveGeometry());
    settings.setValue("State", saveState());

    settings.setValue("cbx_Dn", ui->cbx_Dn->currentIndex());
    settings.setValue("cbx_MeasuredMedium", ui->cbx_MeasuredMedium->currentIndex());
    settings.setValue("cbx_TypeFlowmeter", ui->cbx_TypeFlowmeter->currentIndex());
    settings.setValue("cbx_measGas", ui->cbx_measGas->currentIndex());
    settings.setValue("cbx_paramCalc", ui->cbx_paramCalc->currentIndex());
    settings.setValue("cbx_temperature", ui->cbx_temperature->currentIndex());
    settings.setValue("cbx_typeOfPressure", ui->cbx_typeOfPressure->currentIndex());
    settings.setValue("cbx_viscocityType", ui->cbx_viscocityType->currentIndex());
    settings.setValue("dsbx_OfParamCalcDouble", ui->dsbx_OfParamCalcDouble->value());
    settings.setValue("dsbx_pressure", ui->dsbx_pressure->value());
    settings.setValue("dsbx_viscocity", ui->dsbx_viscocity->value());
    settings.endGroup();
}

double MainWindow::calc_speed()
{
    double tmpSpeed = flow() / S() / 3600;
    ui->lbl_flow_speed->setNum(tmpSpeed);
    return tmpSpeed;
}

double MainWindow::calc_mass_flow() { return density() * flow(); }

double MainWindow::calc_heat_power() { return flow(); }

double MainWindow::calc_volume_flow() { return flow(); }

double MainWindow::kinematic_viscosity() { return ui->cbx_temperature->currentData(Qt::UserRole + 1).toDouble(); }

double MainWindow::dynamic_viscosity() { return ui->cbx_temperature->currentData(Qt::UserRole + 0).toDouble(); }

double MainWindow::density() { return ui->cbx_temperature->currentData(Qt::UserRole + 2).toDouble(); }

double MainWindow::flow() { return ui->dsbx_OfParamCalcDouble->value(); }

double MainWindow::Re() { return ((flow() / 3600) * (val_Dn() / 1000)) / (S() * kinematic_viscosity()); }

double MainWindow::val_Dn()
{
    static QRegularExpression re(R"((.+\s)?(\d+))");
    bool ok {};
    return re.match(ui->cbx_Dn->currentText()).captured(2).toDouble(&ok);
}

double MainWindow::S() { return acos(-1.0) * pow(val_Dn() / 1000, 2) / 4; }

void MainWindow::testj()
{
    if (0) {
        qDebug() << ui->cbx_temperature->currentData().toJsonObject()["Плотность"];
        qDebug() << ui->cbx_temperature->currentData().toJsonObject()["ДинамическаяВязкость"];
        qDebug() << ui->cbx_temperature->currentData().toJsonObject()["КинематическаяВязкость"];
        qDebug() << ui->cbx_temperature->currentData().toJsonObject();
    } else if (0) {
        qDebug() << ui->cbx_temperature->currentData(Qt::UserRole + 0).toDouble();
        qDebug() << ui->cbx_temperature->currentData(Qt::UserRole + 1).toDouble();
        qDebug() << ui->cbx_temperature->currentData(Qt::UserRole + 2).toDouble();
        qDebug() << ui->cbx_temperature->currentData(Qt::UserRole + 3).toDouble();
        // ui->cbx_temperature->currentData(Qt::UserRole + 4).value<name>();
        // TempModel ("ДинамическаяВязкость", "КинематическаяВязкость", "Плотность", "Температура")
    }
}
// void MainWindow::loadJson()
//{
//     QFile file("../CalcViscocity.json");
//     if (file.open(QFile::ReadOnly | QFile::Text)) {
//         auto obj { QJsonDocument::fromJson(file.readAll()).object() };
//         if (0) {
//             obj = obj["Воздух_для_сортировки_в_массиве"].toObject();
//             auto keys { obj.keys().toVector() };
//             std::ranges::sort(keys, {}, [](const QString& str) { return str.toInt(); });
//             for (auto&& key : keys) {
//                 ui->cbx_temperature->addItem(key, { obj[key] });
//             }
//         } else if (0) {
//             auto arr = obj["Воздух"].toArray();
//             for (auto&& ref : arr) {
//                 obj = ref.toObject();
//                 ui->cbx_temperature->addItem(QString::number(obj["Температура"].toInt()), { obj });
//             }
//         } else {
//             delete ui->cbx_temperature->model();
//             ui->cbx_temperature->setModel(new TempModel(obj["Воздух"].toArray(), ui->cbx_temperature));
//             ui->cbx_temperature->setModelColumn(3);
//         }
//     } else {
//         qDebug() << file.errorString();
//     }
// }