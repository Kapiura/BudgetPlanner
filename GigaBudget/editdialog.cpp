#include "editdialog.h"
#include <QComboBox>
#include <QDateEdit>
#include <QDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

EditDialog::EditDialog(Flag flaga)
{
    QFormLayout *formLayout = new QFormLayout;
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    QVBoxLayout *mainLayout = new QVBoxLayout;

    switch (flaga)
    {
    case Expenses:
        amountEdit = new QLineEdit;
        currencyComboBox = new QComboBox;
        categoryComboBox = new QComboBox;
        dateEdit = new QDateEdit;
        descriptionEdit = new QPlainTextEdit;
        okButton = new QPushButton(tr("OK"));
        cancelButton = new QPushButton(tr("Cancel"));

        currencyComboBox->addItems({"PLN", "EUR"});

        categoryComboBox->addItems({"rent", "groceries", "utilities", "dining", "travel", "work", "toiletries",
                                    "household items", "medicines", "other"});
        formLayout->addRow(tr("Amount"), amountEdit);
        formLayout->addRow(tr("Currency"), currencyComboBox);
        formLayout->addRow(tr("Category"), categoryComboBox);
        formLayout->addRow(tr("Date"), dateEdit);
        formLayout->addRow(tr("Description"), descriptionEdit);
        break;
    case Incomes:
        amountEdit = new QLineEdit;
        currencyComboBox = new QComboBox;
        categoryComboBox = new QComboBox;
        dateEdit = new QDateEdit;
        descriptionEdit = new QPlainTextEdit;
        okButton = new QPushButton(tr("OK"));
        cancelButton = new QPushButton(tr("Cancel"));

        currencyComboBox->addItems({"PLN", "EUR"});

        categoryComboBox->addItems({"salary", "bonus", "investment", "savings", "other"});
        formLayout->addRow(tr("Amount"), amountEdit);
        formLayout->addRow(tr("Currency"), currencyComboBox);
        formLayout->addRow(tr("Category"), categoryComboBox);
        formLayout->addRow(tr("Date"), dateEdit);
        formLayout->addRow(tr("Description"), descriptionEdit);
        break;
    case Savings:
        amountEdit = new QLineEdit;
        currencyComboBox = new QComboBox;
        dateEdit = new QDateEdit;
        descriptionEdit = new QPlainTextEdit;
        okButton = new QPushButton(tr("OK"));
        cancelButton = new QPushButton(tr("Cancel"));

        currencyComboBox->addItems({"PLN", "EUR"});
        formLayout->addRow(tr("Amount"), amountEdit);
        formLayout->addRow(tr("Currency"), currencyComboBox);
        formLayout->addRow(tr("Date"), dateEdit);
        formLayout->addRow(tr("Description"), descriptionEdit);
        break;
    default:
        break;
    }

    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);

    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);

    QObject::connect(okButton, &QPushButton::clicked, this, &EditDialog::accept);
    QObject::connect(cancelButton, &QPushButton::clicked, this, &EditDialog::reject);
}

void EditDialog::setData(const QStringList &data)
{
    amountEdit->setText(data[0]);
    currencyComboBox->setCurrentText(data[1]);
    categoryComboBox->setCurrentText(data[2]);
    dateEdit->setDate(QDate::fromString(data[3], "yyyy-MM-dd"));
    descriptionEdit->setPlainText(data[4]);
}

void EditDialog::setDataSav(const QStringList &data)
{
    amountEdit->setText(data[0]);
    currencyComboBox->setCurrentText(data[1]);
    dateEdit->setDate(QDate::fromString(data[2], "yyyy-MM-dd"));
    descriptionEdit->setPlainText(data[3]);
}

QStringList EditDialog::getData() const
{
    QStringList data;
    data.append(amountEdit->text());
    data.append(currencyComboBox->currentText());
    data.append(categoryComboBox->currentText());
    data.append(dateEdit->date().toString("yyyy-MM-dd"));
    data.append(descriptionEdit->toPlainText());
    return data;
}

QStringList EditDialog::getDataSav() const
{
    QStringList data;
    data.append(amountEdit->text());
    data.append(currencyComboBox->currentText());
    data.append(dateEdit->date().toString("yyyy-MM-dd"));
    data.append(descriptionEdit->toPlainText());
    return data;
}
