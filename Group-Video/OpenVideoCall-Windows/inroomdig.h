#ifndef INROOMDIG_H
#define INROOMDIG_H

#include <QDialog>
#include <QLabel>

namespace Ui {
class InRoomDig;
}

class InRoomDig : public QDialog
{
    Q_OBJECT

public:
    explicit InRoomDig(QWidget *parent = 0);
    ~InRoomDig();

    void initWindow();

private:
    Ui::InRoomDig *ui;
    QLabel* m_labText;
};

#endif // INROOMDIG_H
