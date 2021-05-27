#include "shadowbasedialog.h"
#include "shadowwidget.h"
#include <QVBoxLayout>

ShadowBaseDialog::ShadowBaseDialog(QWidget *parent) : QDialog(parent)
{
   setAttribute(Qt::WA_TranslucentBackground);
   setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
   setContentsMargins(0, 0, 0, 0);

   QVBoxLayout *pLayout = new QVBoxLayout(this);
   pLayout->setContentsMargins(0, 0, 0, 0);
   pLayout->setSpacing(0);
   int shadowSize = 10;
   ShadowWidget *pShadowWidget = new ShadowWidget(this);
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
