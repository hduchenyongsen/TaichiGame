/********************************************************************************
** Form generated from reading UI file 'taichigame.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TAICHIGAME_H
#define UI_TAICHIGAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TaichiGameClass
{
public:

    void setupUi(QWidget *TaichiGameClass)
    {
        if (TaichiGameClass->objectName().isEmpty())
            TaichiGameClass->setObjectName(QStringLiteral("TaichiGameClass"));
        TaichiGameClass->resize(223, 94);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        TaichiGameClass->setPalette(palette);

        retranslateUi(TaichiGameClass);

        QMetaObject::connectSlotsByName(TaichiGameClass);
    } // setupUi

    void retranslateUi(QWidget *TaichiGameClass)
    {
        TaichiGameClass->setWindowTitle(QApplication::translate("TaichiGameClass", "TaichiGame", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class TaichiGameClass: public Ui_TaichiGameClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TAICHIGAME_H
