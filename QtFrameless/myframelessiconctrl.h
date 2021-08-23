#ifndef MYFRAMELESSICONCTRL_H
#define MYFRAMELESSICONCTRL_H

#include "framelesshelper.h"
#include <QObject>

class MyFramelessIconCtrl : public QObject, public FramelessIconCtrl
{
    Q_OBJECT
public:
    MyFramelessIconCtrl(QObject *parent = 0);

    virtual void setIcon(FramelessIconCtrl::ButtonType type, QAbstractButton* button);
};





#endif // MYFRAMELESSICONCTRL_H
