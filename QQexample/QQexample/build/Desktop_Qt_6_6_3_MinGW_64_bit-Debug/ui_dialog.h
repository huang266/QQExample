/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QLineEdit *passwordLine;
    QLineEdit *accountLine;
    QPushButton *photoBtn;
    QPushButton *logBtn;
    QPushButton *registBtn;
    QCheckBox *checkBox;
    QPushButton *forgetBtn;
    QComboBox *comboBox;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->resize(747, 1360);
        passwordLine = new QLineEdit(Dialog);
        passwordLine->setObjectName("passwordLine");
        passwordLine->setGeometry(QRect(60, 530, 610, 98));
        accountLine = new QLineEdit(Dialog);
        accountLine->setObjectName("accountLine");
        accountLine->setGeometry(QRect(60, 390, 610, 98));
        photoBtn = new QPushButton(Dialog);
        photoBtn->setObjectName("photoBtn");
        photoBtn->setGeometry(QRect(240, 120, 181, 201));
        QFont font;
        font.setPointSize(10);
        photoBtn->setFont(font);
        photoBtn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"  border-radius: 50px;\n"
"  background-size:cover;\n"
"\n"
"}"));
        logBtn = new QPushButton(Dialog);
        logBtn->setObjectName("logBtn");
        logBtn->setEnabled(true);
        logBtn->setGeometry(QRect(100, 680, 471, 51));
        QFont font1;
        font1.setPointSize(11);
        logBtn->setFont(font1);
        registBtn = new QPushButton(Dialog);
        registBtn->setObjectName("registBtn");
        registBtn->setGeometry(QRect(130, 810, 141, 51));
        registBtn->setFont(font1);
        checkBox = new QCheckBox(Dialog);
        checkBox->setObjectName("checkBox");
        checkBox->setGeometry(QRect(110, 750, 531, 41));
        forgetBtn = new QPushButton(Dialog);
        forgetBtn->setObjectName("forgetBtn");
        forgetBtn->setGeometry(QRect(400, 810, 141, 51));
        forgetBtn->setFont(font1);
        comboBox = new QComboBox(Dialog);
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(460, 390, 211, 98));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
#if QT_CONFIG(statustip)
        passwordLine->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        photoBtn->setText(QString());
        logBtn->setText(QCoreApplication::translate("Dialog", "\347\231\273\345\275\225", nullptr));
        registBtn->setText(QCoreApplication::translate("Dialog", "\346\263\250\345\206\214", nullptr));
        checkBox->setText(QCoreApplication::translate("Dialog", "\345\267\262\351\230\205\350\257\273\345\271\266\345\220\214\346\204\217\346\234\215\345\212\241\345\215\217\350\256\256\345\222\214QQ\351\232\220\347\247\201\344\277\235\346\212\244\346\214\207\345\215\227", nullptr));
        forgetBtn->setText(QCoreApplication::translate("Dialog", "\345\277\230\350\256\260\345\257\206\347\240\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
