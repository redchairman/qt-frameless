#include "QFramelessDialog.h"
#include "iconhelper.h"
#include <QMouseEvent>

QFramelessDialog::QFramelessDialog(QWidget *parent)
	: QDialog(parent)
	, m_bMaximized(false)
	, m_bMoveable(false)
	, m_bResize(false)
	, m_menuButton_Close(nullptr)
	, m_menuButton_Max(nullptr)
	, m_menuButton_Min(nullptr)
	, m_widget_title(nullptr)
	, m_bMinHide(false)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog | Qt::WindowMinimizeButtonHint);
	setWindowModality(Qt::WindowModal);
}

QFramelessDialog::~QFramelessDialog()
{

}

void QFramelessDialog::InitFrameless(QWidget *widget_title, 
							QPushButton *menuButton_Close, 
							QPushButton *menuButton_Max,
							QPushButton *menuButton_Min,
							bool bResize,
							bool bMinHide)
{
	m_bResize = bResize;
	m_bMinHide = bMinHide;
	m_menuButton_Min = menuButton_Min;
	m_menuButton_Max = menuButton_Max;
	m_menuButton_Close = menuButton_Close;
	m_widget_title = widget_title;

	if (m_menuButton_Close)
	{
        IconHelper::Instance()->setIcon(m_menuButton_Close, IconHelper::IconClose);
		if (m_bMinHide)
		{
			connect(m_menuButton_Close, SIGNAL(clicked()), this, SLOT(hide()));
		}
		else
		{
			connect(m_menuButton_Close, SIGNAL(clicked()), this, SLOT(close()));
		}
	}
	if (m_menuButton_Max)
	{
        IconHelper::Instance()->setIcon(m_menuButton_Max, IconHelper::IconMax);
		connect(m_menuButton_Max, SIGNAL(clicked()), this, SLOT(sltClickMaxRestore()));
	}
	if (m_menuButton_Min)
	{
        IconHelper::Instance()->setIcon(m_menuButton_Min, IconHelper::IconMin);
		connect(m_menuButton_Min, SIGNAL(clicked()), this, SLOT(sltClickMin()));
	}
	return;
}

void QFramelessDialog::mousePressEvent(QMouseEvent *event)
{
	if (isFullScreen())
	{
		return;
	}

	if (event->button() == Qt::LeftButton)
	{
		dragPosition = event->globalPos() - frameGeometry().topLeft();

		QRect rect = m_widget_title->rect();
		if (rect.contains(event->pos()))
		{
			m_bMoveable = true;
		}
	}
	event->accept();
}

void QFramelessDialog::mouseMoveEvent(QMouseEvent *event)
{
	if (isFullScreen())
	{
		return;
	}

	if (event->buttons() & Qt::LeftButton && m_bMoveable)
	{
		if (isMaximized())
		{
//			int nWidth = m_rcNormal.width();
//			int nHeight = m_rcNormal.height();

			float fx = (float)event->pos().x() / (float)rect().width();
			//屏幕大小
			int old_x = m_rcNormal.width() * fx + m_rcNormal.left();
			int old_y = m_rcNormal.top() + event->pos().y();

			QPoint pt_new(m_rcNormal.left() + event->globalPos().x() - old_x, m_rcNormal.top() + event->globalPos().y() - old_y);
			m_rcNormal.moveTopLeft(pt_new);
			sltClickMaxRestore();
			//m_rcNormal
			dragPosition = event->globalPos() - frameGeometry().topLeft();
		}
		else
		{
			move(event->globalPos() - dragPosition);
		}
	}
	event->accept();

}

void QFramelessDialog::mouseReleaseEvent(QMouseEvent *event)
{
	if (isFullScreen())
	{
		return;
	}
	if (m_bMoveable)
	{
		m_bMoveable = false;
	}
	event->accept();
}

void QFramelessDialog::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (m_bResize == false)
	{
		return;
	}
	if (isFullScreen())
	{
		return;
	}
	if (event->buttons() & Qt::LeftButton)
	{
		QRect rect = m_widget_title->rect();
		if (rect.contains(event->pos()))
		{
			sltClickMaxRestore();
		}
	}
	event->accept();
}

void QFramelessDialog::sltClickMin()
{
	showMinimized();
}

void QFramelessDialog::sltClickMaxRestore()
{
	if (isMaximized())
	{
		showNormal();
		setGeometry(m_rcNormal);
        //qDebug() << m_rcNormal;

        IconHelper::Instance()->setIcon(m_menuButton_Max, IconHelper::IconMax);
		m_menuButton_Max->setToolTip(QStringLiteral("最大化"));
	}
	else
	{
		m_rcNormal = geometry();
		showMaximized();

        IconHelper::Instance()->setIcon(m_menuButton_Max, IconHelper::IconNormal);
		m_menuButton_Max->setToolTip(QStringLiteral("还原"));
	}
}

#include <windows.h>

bool QFramelessDialog::nativeEvent(const QByteArray & eventType, void * message, long * result)
{
	Q_UNUSED(eventType);
	const int HIT_BORDER = 4;
	const MSG *msg = static_cast<MSG*>(message);
	if (m_bResize == false || m_widget_title == nullptr)
	{
		return QDialog::nativeEvent(eventType, message, result);
	}

	if (msg->message == WM_NCHITTEST)
	{
		if (isMaximized())
		{
			return false;
		}
		int xPos = ((int)(short)LOWORD(msg->lParam)) - this->frameGeometry().x();
		int yPos = ((int)(short)HIWORD(msg->lParam)) - this->frameGeometry().y();

		if (xPos >= 0 && xPos < HIT_BORDER && yPos >= 0 && yPos < HIT_BORDER) {
			*result = HTTOPLEFT;
			return true;
		}
		if (xPos >(this->width() - HIT_BORDER) && xPos < (this->width() - 0) && yPos > 0 && yPos < HIT_BORDER) {
			*result = HTTOPRIGHT;
			return true;
		}
		if (xPos > 0 && xPos < HIT_BORDER && yPos >(this->height() - HIT_BORDER) && yPos < (this->height() - 0)) {
			*result = HTBOTTOMLEFT;
			return true;
		}
		if (xPos >(this->width() - HIT_BORDER) && xPos < (this->width() - 0) && yPos >(this->height() - HIT_BORDER) && yPos < (this->height() - 0)) {
			*result = HTBOTTOMRIGHT;
			return true;
		}
		if (xPos >= 0 && xPos < HIT_BORDER) {
			*result = HTLEFT;
			return true;
		}
		if (xPos >(this->width() - HIT_BORDER) && xPos < (this->width() - 0)) {
			*result = HTRIGHT;
			return true;
		}
		if (yPos >= 0 && yPos < HIT_BORDER) {
			*result = HTTOP;
			return true;
		}
		if (yPos >(this->height() - HIT_BORDER) && yPos < (this->height() - 0)) {
			*result = HTBOTTOM;
			return true;
		}
		if (m_widget_title->geometry().contains(QPoint(xPos, yPos)))
		{
			*result = HTCAPTION;
			return false;
		}
		return false;
	}
	return false;
}
