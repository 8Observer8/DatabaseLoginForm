#include "dialog.h"
#include "ui_dialog.h"

#define Path_to_DB "d:/Documents/Qt/QtExamples/0011_Database/DatabaseLoginForm/Accounts.sqlite"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    myDB = QSqlDatabase::addDatabase("QSQLITE");

    QString pathToDB = QDir::currentPath()+QString("/Accounts.sqlite");
    myDB.setDatabaseName(pathToDB);

    QFileInfo checkFile(pathToDB);

    if (checkFile.isFile()) {
        if (myDB.open()) {
            ui->lblResult->setText("[+] Connected to Database File");
        }
        else {
            ui->lblResult->setText("[!] Database File was not opened");
        }
    }
    else {
        ui->lblResult->setText("[!] Database File does not exist");
    }
}

Dialog::~Dialog()
{
    delete ui;
    qDebug() << "Closing the connection to Database file on exist";
    myDB.close();
}

void Dialog::on_btnClear_clicked()
{
    ui->txtUser->setText("");
    ui->txtPass->setText("");
}

void Dialog::on_btnLogin_clicked()
{
    QString Username, Password;

    Username = ui->txtUser->text();
    Password = ui->txtPass->text();

    if (!myDB.isOpen()) {
        qDebug() << "No connection to Database";
        return;
    }

    QSqlQuery qry;

    if (qry.exec("SELECT Username, Password, Role FROM Users WHERE Username=\'" + Username +
                 "\' AND Password=\'" + Password + "\'"))
    {
        if (qry.next()) {
            ui->lblResult->setText("[+] Valid Username and Password");
            QString msg = "Username = " + qry.value(0).toString() + "\n" +
                    "Password = " + qry.value(1).toString() + "\n" +
                    "Role = " + qry.value(2).toString() + "\n";
            QMessageBox::warning(this, "Login was successful", msg);
        }
        else {
            ui->lblResult->setText("[-] Wrong Username or Password");
        }
    }
    else {
        ui->lblResult->setText("[-] Wrong query");
    }
}
