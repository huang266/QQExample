#ifndef REGISTR_H
#define REGISTR_H
#include <QWidget>
namespace Ui {
class Registr;
}

class Registr : public QWidget
{
    Q_OBJECT

public:
    explicit Registr(QWidget *parent = nullptr);
    ~Registr();
    void paintEvent(QPaintEvent *enent);

private:
    Ui::Registr *ui;
};

#endif // REGISTR_H
