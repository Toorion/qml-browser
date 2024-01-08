/****************************************************************************
*
* QmlBrowser - Web browser with QML page support
* Copyright (C) 2022 Denis Solomatin <toorion@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/
#include "mainapplication.h"
#include <QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QDialogButtonBox>

MainApplication::MainApplication(int &argc, char **argv) : QApplication(argc, argv)
{

}

MainApplication::~MainApplication()
{

}

bool MainApplication::authDialog(QString &username, QString &password, const QString title, const QString header, QWidget *parent)
{
    QDialog* dialog = new QDialog(parent);
    dialog->setWindowTitle(title);

    QFormLayout* formLa = new QFormLayout(dialog);

    QLabel* label = new QLabel(dialog);
    QLabel* userLab = new QLabel(dialog);
    QLabel* passLab = new QLabel(dialog);
    userLab->setText(tr("Username: "));
    passLab->setText(tr("Password: "));

    QLineEdit* user = new QLineEdit(dialog);
    QLineEdit* pass = new QLineEdit(dialog);
    pass->setEchoMode(QLineEdit::Password);
    //QCheckBox* save = new QCheckBox(dialog);
    //save->setText(tr("Save username and password for this site"));

    QDialogButtonBox* box = new QDialogButtonBox(dialog);
    box->addButton(QDialogButtonBox::Ok);
    box->addButton(QDialogButtonBox::Cancel);
    connect(box, &QDialogButtonBox::rejected, dialog, &QDialog::reject);
    connect(box, &QDialogButtonBox::accepted, dialog, &QDialog::accept);

    label->setText(header);

    formLa->addRow(label);
    formLa->addRow(userLab, user);
    formLa->addRow(passLab, pass);
    //formLa->addRow(save);
    formLa->addWidget(box);

//    AutoFill* fill = mApp->autoFill();
//    QString storedUser;
//    QString storedPassword;
//    bool shouldUpdateEntry = false;

//    if (fill->isStored(url)) {
//        const QVector<PasswordEntry> &data = fill->getFormData(url);
//        if (!data.isEmpty()) {
//            save->setChecked(true);
//            shouldUpdateEntry = true;
//            storedUser = data.at(0).username;
//            storedPassword = data.at(0).password;
//            user->setText(storedUser);
//            pass->setText(storedPassword);
//        }
//    }

//    // Do not save when private browsing is enabled
//    if (mApp->isPrivate()) {
//        save->setVisible(false);
//    }
    if(!username.isEmpty()) {
        user->setText(username);
    }


    if (dialog->exec() != QDialog::Accepted) {
        delete dialog;
        return false;
    }

    username = user->text();
    password = pass->text();

//    if (save->isChecked()) {
//        if (shouldUpdateEntry) {
//            if (storedUser != user->text() || storedPassword != pass->text()) {
//                fill->updateEntry(url, user->text(), pass->text());
//            }
//        }
//        else {
//            fill->addEntry(url, user->text(), pass->text());
//        }
//    }

    delete dialog;
    return true;
}


MainApplication* MainApplication::instance()
{
    return static_cast<MainApplication*>(QApplication::instance());
}
