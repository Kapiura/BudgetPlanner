#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QComboBox>
#include <QDateEdit>
#include <QDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

class EditDialog : public QDialog
{
    Q_OBJECT

  public:
    enum Flag
    {
        Expenses,
        Incomes,
        Savings
    };
    EditDialog(Flag flaga);
    void setData(const QStringList &data);
    void setDataSav(const QStringList &data);
    QStringList getData() const;
    QStringList getDataSav() const;

  private:
    QLineEdit *amountEdit;
    QComboBox *currencyComboBox;
    QComboBox *categoryComboBox;
    QDateEdit *dateEdit;
    QPlainTextEdit *descriptionEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // EDITDIALOG_H
