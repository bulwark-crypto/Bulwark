// Copyright (c) 2019 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SETTINGSMAINOPTIONSWIDGET_H
#define SETTINGSMAINOPTIONSWIDGET_H

#include "qt/bulwark/pwidget.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
class QDataWidgetMapper;
QT_END_NAMESPACE

namespace Ui {
class SettingsMainOptionsWidget;
}

class SettingsMainOptionsWidget : public PWidget
{
    Q_OBJECT

public:
    explicit SettingsMainOptionsWidget(BWKGUI* _window, QWidget *parent = nullptr);
    ~SettingsMainOptionsWidget();

    void setMapper(QDataWidgetMapper *mapper);

public slots:
    void onResetClicked();

private:
    Ui::SettingsMainOptionsWidget *ui;
};

#endif // SETTINGSMAINOPTIONSWIDGET_H
