#include "shadowbasewidget.h"
#include "shadowwidget.h"
#include <QVBoxLayout>

ShadowBaseWidget::ShadowBaseWidget(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setContentsMargins(0, 0, 0, 0);
    pLayout->setSpacing(0);
    ShadowWidget *pShadowWidget = new ShadowWidget(this);
    int shadowSize = 10;
    pShadowWidget->setShadowSize(shadowSize);
    pShadowWidget->setContentsMargins(shadowSize, shadowSize, shadowSize, shadowSize);
    pLayout->addWidget(pShadowWidget);

    QLayout* rootLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    pShadowWidget->setLayout(rootLayout);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    shadowClientWidget = new QWidget(pShadowWidget);
    shadowClientWidget->setAutoFillBackground(true);
    rootLayout->addWidget(shadowClientWidget);
}
