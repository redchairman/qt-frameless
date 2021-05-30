#include "myframelessiconctrl.h"
#include "iconhelper.h"

MyFramelessIconCtrl::MyFramelessIconCtrl(QObject *parent)
    : QObject(parent)
{

}

void MyFramelessIconCtrl::setIcon(FramelessIconCtrl::ButtonType type, QAbstractButton *button)
{
    switch (type) {
    case FramelessIconCtrl::Button_MinSize:
    {
        IconHelper::Instance()->setIcon(button, IconHelper::IconMin);
        break;
    }
    case FramelessIconCtrl::Button_MaxSize:
    {
        IconHelper::Instance()->setIcon(button, IconHelper::IconMax);
        break;
    }
    case FramelessIconCtrl::Button_Restore:
    {
        IconHelper::Instance()->setIcon(button, IconHelper::IconNormal);
        break;
    }
    case FramelessIconCtrl::Button_Close:
    {
        IconHelper::Instance()->setIcon(button, IconHelper::IconClose);
        break;
    }

    }
}
