/********************************************************************************
** Form generated from reading UI file 'ControlPanel.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLPANEL_H
#define UI_CONTROLPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ControlPanel
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *controlGroup;
    QHBoxLayout *horizontalLayout;
    QPushButton *runBtn;
    QPushButton *pauseBtn;
    QPushButton *resetBtn;
    QPushButton *stepBtn;
    QGroupBox *paramsGroup;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *populationLabel;
    QSpinBox *populationSpin;
    QHBoxLayout *horizontalLayout_3;
    QLabel *mutationLabel;
    QDoubleSpinBox *mutationSpin;
    QHBoxLayout *horizontalLayout_4;
    QLabel *crossoverLabel;
    QDoubleSpinBox *crossoverSpin;
    QHBoxLayout *horizontalLayout_5;
    QLabel *elitismLabel;
    QSpinBox *elitismSpin;
    QHBoxLayout *horizontalLayout_6;
    // QLabel *maxComponentsLabel;
    // QSpinBox *maxComponentsSpin;
    QGroupBox *strategiesGroup;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_7;
    QLabel *selectionLabel;
    QComboBox *selectionCombo;
    QHBoxLayout *horizontalLayout_8;
    QLabel *crossoverLabel2;
    QComboBox *crossoverCombo;
    QHBoxLayout *horizontalLayout_9;
    QLabel *mutationLabel2;
    QComboBox *mutationCombo;
    QGroupBox *statsGroup;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_10;
    QLabel *generationLabel;
    QLabel *generationValue;
    QHBoxLayout *horizontalLayout_11;
    QLabel *bestFitnessLabel;
    QLabel *bestFitnessValue;
    QHBoxLayout *horizontalLayout_12;
    QLabel *avgFitnessLabel;
    QLabel *avgFitnessValue;

    void setupUi(QWidget *ControlPanel)
    {
        if (ControlPanel->objectName().isEmpty())
            ControlPanel->setObjectName("ControlPanel");
        ControlPanel->resize(300, 700);
        verticalLayout = new QVBoxLayout(ControlPanel);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(10, 10, 10, 10);
        verticalLayout->setSpacing(10);
        controlGroup = new QGroupBox(ControlPanel);
        controlGroup->setObjectName("controlGroup");
        horizontalLayout = new QHBoxLayout(controlGroup);
        horizontalLayout->setObjectName("horizontalLayout");
        runBtn = new QPushButton(controlGroup);
        runBtn->setObjectName("runBtn");

        horizontalLayout->addWidget(runBtn);

        pauseBtn = new QPushButton(controlGroup);
        pauseBtn->setObjectName("pauseBtn");

        horizontalLayout->addWidget(pauseBtn);

        resetBtn = new QPushButton(controlGroup);
        resetBtn->setObjectName("resetBtn");

        horizontalLayout->addWidget(resetBtn);

        stepBtn = new QPushButton(controlGroup);
        stepBtn->setObjectName("stepBtn");

        horizontalLayout->addWidget(stepBtn);


        verticalLayout->addWidget(controlGroup);

        paramsGroup = new QGroupBox(ControlPanel);
        paramsGroup->setObjectName("paramsGroup");
        verticalLayout_2 = new QVBoxLayout(paramsGroup);
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        populationLabel = new QLabel(paramsGroup);
        populationLabel->setObjectName("populationLabel");

        horizontalLayout_2->addWidget(populationLabel);

        populationSpin = new QSpinBox(paramsGroup);
        populationSpin->setObjectName("populationSpin");
        populationSpin->setRange(10, 1000);
        populationSpin->setValue(100);

        horizontalLayout_2->addWidget(populationSpin);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        mutationLabel = new QLabel(paramsGroup);
        mutationLabel->setObjectName("mutationLabel");

        horizontalLayout_3->addWidget(mutationLabel);

        mutationSpin = new QDoubleSpinBox(paramsGroup);
        mutationSpin->setObjectName("mutationSpin");
        mutationSpin->setRange(0.0, 1.0);
        mutationSpin->setSingleStep(0.01);
        mutationSpin->setValue(0.1);

        horizontalLayout_3->addWidget(mutationSpin);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        crossoverLabel = new QLabel(paramsGroup);
        crossoverLabel->setObjectName("crossoverLabel");

        horizontalLayout_4->addWidget(crossoverLabel);

        crossoverSpin = new QDoubleSpinBox(paramsGroup);
        crossoverSpin->setObjectName("crossoverSpin");
        crossoverSpin->setRange(0.0, 1.0);
        crossoverSpin->setSingleStep(0.01);
        crossoverSpin->setValue(0.8);

        horizontalLayout_4->addWidget(crossoverSpin);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        elitismLabel = new QLabel(paramsGroup);
        elitismLabel->setObjectName("elitismLabel");

        horizontalLayout_5->addWidget(elitismLabel);

        elitismSpin = new QSpinBox(paramsGroup);
        elitismSpin->setObjectName("elitismSpin");
        elitismSpin->setRange(0, 20);
        elitismSpin->setValue(2);

        horizontalLayout_5->addWidget(elitismSpin);


        verticalLayout_2->addLayout(horizontalLayout_5);

        // horizontalLayout_6 = new QHBoxLayout();
        // horizontalLayout_6->setObjectName("horizontalLayout_6");
        // maxComponentsLabel = new QLabel(paramsGroup);
        // maxComponentsLabel->setObjectName("maxComponentsLabel");

        // horizontalLayout_6->addWidget(maxComponentsLabel);

        // maxComponentsSpin = new QSpinBox(paramsGroup);
        // maxComponentsSpin->setObjectName("maxComponentsSpin");
        // maxComponentsSpin->setRange(1, 20);
        // maxComponentsSpin->setValue(8);

        // horizontalLayout_6->addWidget(maxComponentsSpin);


        // verticalLayout_2->addLayout(horizontalLayout_6);


        verticalLayout->addWidget(paramsGroup);

        strategiesGroup = new QGroupBox(ControlPanel);
        strategiesGroup->setObjectName("strategiesGroup");
        verticalLayout_3 = new QVBoxLayout(strategiesGroup);
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        selectionLabel = new QLabel(strategiesGroup);
        selectionLabel->setObjectName("selectionLabel");

        horizontalLayout_7->addWidget(selectionLabel);

        selectionCombo = new QComboBox(strategiesGroup);
        selectionCombo->setObjectName("selectionCombo");
        selectionCombo->addItems({"Tournament", "Roulette Wheel"});

        horizontalLayout_7->addWidget(selectionCombo);


        verticalLayout_3->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        crossoverLabel2 = new QLabel(strategiesGroup);
        crossoverLabel2->setObjectName("crossoverLabel2");

        horizontalLayout_8->addWidget(crossoverLabel2);

        crossoverCombo = new QComboBox(strategiesGroup);
        crossoverCombo->setObjectName("crossoverCombo");
        crossoverCombo->addItems({"One Point", "Uniform"});

        horizontalLayout_8->addWidget(crossoverCombo);


        verticalLayout_3->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        mutationLabel2 = new QLabel(strategiesGroup);
        mutationLabel2->setObjectName("mutationLabel2");

        horizontalLayout_9->addWidget(mutationLabel2);

        mutationCombo = new QComboBox(strategiesGroup);
        mutationCombo->setObjectName("mutationCombo");
        mutationCombo->addItems({"Swap", "Parameter", "AddRemove"});

        horizontalLayout_9->addWidget(mutationCombo);


        verticalLayout_3->addLayout(horizontalLayout_9);


        verticalLayout->addWidget(strategiesGroup);

        statsGroup = new QGroupBox(ControlPanel);
        statsGroup->setObjectName("statsGroup");
        verticalLayout_4 = new QVBoxLayout(statsGroup);
        verticalLayout_4->setObjectName("verticalLayout_4");
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName("horizontalLayout_10");
        generationLabel = new QLabel(statsGroup);
        generationLabel->setObjectName("generationLabel");

        horizontalLayout_10->addWidget(generationLabel);

        generationValue = new QLabel(statsGroup);
        generationValue->setObjectName("generationValue");
        generationValue->setText("0");

        horizontalLayout_10->addWidget(generationValue);


        verticalLayout_4->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName("horizontalLayout_11");
        bestFitnessLabel = new QLabel(statsGroup);
        bestFitnessLabel->setObjectName("bestFitnessLabel");

        horizontalLayout_11->addWidget(bestFitnessLabel);

        bestFitnessValue = new QLabel(statsGroup);
        bestFitnessValue->setObjectName("bestFitnessValue");
        bestFitnessValue->setText("0.0");

        horizontalLayout_11->addWidget(bestFitnessValue);


        verticalLayout_4->addLayout(horizontalLayout_11);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName("horizontalLayout_12");
        avgFitnessLabel = new QLabel(statsGroup);
        avgFitnessLabel->setObjectName("avgFitnessLabel");

        horizontalLayout_12->addWidget(avgFitnessLabel);

        avgFitnessValue = new QLabel(statsGroup);
        avgFitnessValue->setObjectName("avgFitnessValue");
        avgFitnessValue->setText("0.0");

        horizontalLayout_12->addWidget(avgFitnessValue);


        verticalLayout_4->addLayout(horizontalLayout_12);


        verticalLayout->addWidget(statsGroup);

        verticalLayout->addStretch();

        retranslateUi(ControlPanel);

        QMetaObject::connectSlotsByName(ControlPanel);
    } // setupUi

    void retranslateUi(QWidget *ControlPanel)
    {
        ControlPanel->setWindowTitle(QCoreApplication::translate("ControlPanel", "Form", nullptr));
        controlGroup->setTitle(QCoreApplication::translate("ControlPanel", "Simulation Control", nullptr));
        runBtn->setText(QCoreApplication::translate("ControlPanel", "Run", nullptr));
        pauseBtn->setText(QCoreApplication::translate("ControlPanel", "Pause", nullptr));
        resetBtn->setText(QCoreApplication::translate("ControlPanel", "Reset", nullptr));
        stepBtn->setText(QCoreApplication::translate("ControlPanel", "Step", nullptr));
        paramsGroup->setTitle(QCoreApplication::translate("ControlPanel", "Algorithm Parameters", nullptr));
        populationLabel->setText(QCoreApplication::translate("ControlPanel", "Population Size:", nullptr));
        mutationLabel->setText(QCoreApplication::translate("ControlPanel", "Mutation Rate:", nullptr));
        crossoverLabel->setText(QCoreApplication::translate("ControlPanel", "Crossover Rate:", nullptr));
        elitismLabel->setText(QCoreApplication::translate("ControlPanel", "Elitism Count:", nullptr));
        // maxComponentsLabel->setText(QCoreApplication::translate("ControlPanel", "Max Components:", nullptr));
        strategiesGroup->setTitle(QCoreApplication::translate("ControlPanel", "Algorithm Strategies", nullptr));
        selectionLabel->setText(QCoreApplication::translate("ControlPanel", "Selection:", nullptr));
        crossoverLabel2->setText(QCoreApplication::translate("ControlPanel", "Crossover:", nullptr));
        mutationLabel2->setText(QCoreApplication::translate("ControlPanel", "Mutation:", nullptr));
        statsGroup->setTitle(QCoreApplication::translate("ControlPanel", "Statistics", nullptr));
        generationLabel->setText(QCoreApplication::translate("ControlPanel", "Generation:", nullptr));
        bestFitnessLabel->setText(QCoreApplication::translate("ControlPanel", "Best Fitness:", nullptr));
        avgFitnessLabel->setText(QCoreApplication::translate("ControlPanel", "Avg Fitness:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ControlPanel: public Ui_ControlPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLPANEL_H