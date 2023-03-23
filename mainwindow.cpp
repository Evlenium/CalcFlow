
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

#ifdef __GCC_ASM_FLAG_OUTPUTS__
#define __FUNCSIG__ __PRETTY_FUNCTION__
#endif

#define DSBXHIDE(X) ui->lbl_##X, ui->dsbx_##X
#define CBXHIDE(X) ui->lbl_##X, ui->cbx_##X

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug(/*__PRETTY_FUNCTION__*/ __FUNCSIG__);

    ui->setupUi(this);

    ui->formLayout_2->addRow(new QLabel("Площадь", this), dsbx_S_ = new MyLe(this));
    ui->formLayout_2->addRow(new QLabel("Плотность среды", this), dsbx_dencity_ = new MyLe(this));
    ui->formLayout_2->addRow(new QLabel("Динамическая вязкость среды", this), dsbx_dynamic_viscocity_ = new MyLe(this));
    ui->formLayout_2->addRow(new QLabel("Энтальпия среды", this), dsbx_entalpy_ = new MyLe(this));
    ui->formLayout_2->addRow(new QLabel("Скорость потока", this), dsbx_flow_speed_ = new MyLe(this));
    ui->formLayout_2->addRow(new QLabel("Кинематическая вязкость среды", this), dsbx_kinematic_viscocity_ = new MyLe(this));
    ui->formLayout_2->addRow(new QLabel("Массовый расход среды", this), dsbx_mass_flow_ = new MyLe(this));
    ui->formLayout_2->addRow(new QLabel("Объёмный расход среды", this), dsbx_value_flow_ = new MyLe(this));
    ui->formLayout_2->addRow(new QLabel("Удельный объём среды", this), dsbx_value_ = new MyLe(this));
    ui->formLayout_2->addRow(new QLabel("Число Рейнольдса", this), dsbx_value_re_ = new MyLe(this));
    ui->formLayout_2->setLabelAlignment(Qt::AlignRight);
    // ² ³
    dsbx_S_->setSuffix(" м²");
    dsbx_dencity_->setSuffix(" кг/м³");
    dsbx_dynamic_viscocity_->setSuffix(" ");
    dsbx_entalpy_->setSuffix(" ");
    dsbx_flow_speed_->setSuffix(" м/с");
    dsbx_kinematic_viscocity_->setSuffix(" ");
    dsbx_mass_flow_->setSuffix(" м³/ч*кг/м³");
    dsbx_value_flow_->setSuffix(" м³/ч");
    dsbx_value_->setSuffix(" м³");
    dsbx_value_re_->setSuffix(" ");

    //    for (auto* dsbx : ui->widget_2->findChildren<QDoubleSpinBox*>()) {
    //        dsbx->setRange(-std::numeric_limits<double>::max(), +std::numeric_limits<double>::max());
    //        dsbx->setReadOnly(true);
    //        dsbx->setButtonSymbols(QDoubleSpinBox::NoButtons);
    //        dsbx->setDecimals(6);
    //    }

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
        // double Dn_ { re.match(diam[i]).captured(2).toDouble(&ok) };
        // ui->cbx_Dn->setItemData(i, Dn_);
        // }
    };
    hw();

    calc.kinematic_viscosity_t = [this] { return ui->cbx_temperature->currentData(Qt::UserRole + 1).toDouble(); };
    calc.dynamic_viscosity_t = [this] { return ui->cbx_temperature->currentData(Qt::UserRole + 0).toDouble(); };
    calc.density_t = [this] { return ui->cbx_temperature->currentData(Qt::UserRole + 2).toDouble(); };
    calc.dsbx_OfParamCalcDouble = ui->dsbx_OfParamCalcDouble;

    connect(ui->cbx_MeasuredMedium, &QComboBox::currentIndexChanged, hw);
    connect(ui->cbx_TypeFlowmeter, &QComboBox::currentIndexChanged, hw);

    // connect(ui->cbx_measGas, &QComboBox::currentIndexChanged,loadJson());
    //  double S;
    //  double res_Dn = ui->cbx_Dn->currentData().toDouble();
    //  S = acos(-1.0)*pow(res_Dn/1000,2)/4;
    //  qDebug()<<S;
    //  qDebug() << __FUNCTION__ << ui->cbx_Dn->currentData().toDouble();

    connect(ui->cbx_paramCalc, &QComboBox::currentTextChanged, &calc, &Calc::setCbx_paramCalc);

    connect(ui->cbx_Dn, &QComboBox::currentTextChanged, &calc, &Calc::Dn_t_);

    connect(ui->cbx_MeasuredMedium, &QComboBox::currentIndexChanged, &calc, &Calc::calc);
    connect(ui->cbx_TypeFlowmeter, &QComboBox::currentIndexChanged, &calc, &Calc::calc);
    // connect(ui->cbx_measGas, &QComboBox::currentIndexChanged, &calc, &Calc::calc);
    //    connect(ui->cbx_paramCalc, &QComboBox::currentIndexChanged, &calc, &Calc::calc);
    connect(ui->cbx_temperature, &QComboBox::currentIndexChanged, &calc, &Calc::calc);
    connect(ui->cbx_typeOfPressure, &QComboBox::currentIndexChanged, &calc, &Calc::calc);
    connect(ui->cbx_viscocityType, &QComboBox::currentIndexChanged, &calc, &Calc::calc);
    connect(ui->dsbx_OfParamCalcDouble, &QDoubleSpinBox::valueChanged, &calc, &Calc::calc);
    connect(ui->dsbx_pressure, &QDoubleSpinBox::valueChanged, &calc, &Calc::calc);
    connect(ui->dsbx_viscocity, &QDoubleSpinBox::valueChanged, &calc, &Calc::calc);

    connect(ui->cbx_measGas, &QComboBox::currentTextChanged, this, &MainWindow::loadJson); ///
    // loadJson(ui->cbx_measGas->currentText());
    // connect(ui->cbx_measGas, &QComboBox::currentIndexChanged,&MainWindow::
    loadJson(ui->cbx_measGas->currentText());
    // loadJson();

    connect(&calc, &Calc::dsbx_S_setValue, dsbx_S_, &MyLe::setValue);
    connect(&calc, &Calc::dsbx_dencity_setValue, dsbx_dencity_, &MyLe::setValue);
    connect(&calc, &Calc::dsbx_dynamic_viscocity_setValue, dsbx_dynamic_viscocity_, &MyLe::setValue);
    connect(&calc, &Calc::dsbx_entalpy_setValue, dsbx_entalpy_, &MyLe::setValue);
    connect(&calc, &Calc::dsbx_flow_speed_setValue, dsbx_flow_speed_, &MyLe::setValue);
    connect(&calc, &Calc::dsbx_kinematic_viscocity_setValue, dsbx_kinematic_viscocity_, &MyLe::setValue);
    connect(&calc, &Calc::dsbx_mass_flow_setValue, dsbx_mass_flow_, &MyLe::setValue);
    connect(&calc, &Calc::dsbx_value_flow_setValue, dsbx_value_flow_, &MyLe::setValue);
    connect(&calc, &Calc::dsbx_value_re_setValue, dsbx_value_re_, &MyLe::setValue);
    connect(&calc, &Calc::dsbx_value_setValue, dsbx_value_, &MyLe::setValue);

    load();

    ui->cbx_paramCalc->currentIndexChanged(0);
}

MainWindow::~MainWindow()
{
    save();
#ifdef __GCC_ASM_FLAG_OUTPUTS__
    qDebug(__PRETTY_FUNCTION__);
#else
    qDebug(/*__PRETTY_FUNCTION__*/ __FUNCSIG__);
#endif
    delete ui;
}

void MainWindow::loadJson(const QString& str_cbx)
{
    QFile file("../CalcViscocity.json");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        auto obj { QJsonDocument::fromJson(file.readAll()).object() };
        delete ui->cbx_temperature->model();
        ui->cbx_temperature->setModel(new TempModel(obj[str_cbx].toArray(), ui->cbx_temperature));
        ui->cbx_temperature->setModelColumn(3);
        ui->cbx_temperature->update();
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
