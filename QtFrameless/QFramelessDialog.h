#ifndef QFRAMELESSDIALOG_H
#define QFRAMELESSDIALOG_H

#include <QDialog>
#include <QPushButton>

class QFramelessDialog : public QDialog
{
	Q_OBJECT

public:
	QFramelessDialog(QWidget *parent);
	~QFramelessDialog();

	void InitFrameless(QWidget *widget_title, 
			  QPushButton *menuButton_Close, 
			  QPushButton *menuButton_Max,
			  QPushButton *menuButton_Min,
			  bool bResize,
			  bool bMinHide);
private:
	bool m_bMaximized;
	bool m_bMoveable;
	QPoint dragPosition;
	QRect m_rcNormal;

protected:
	void mouseMoveEvent(QMouseEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseDoubleClickEvent(QMouseEvent *event);
	bool nativeEvent(const QByteArray & eventType, void * message, long * result);

	public slots:
		void sltClickMin();
		void sltClickMaxRestore();

public:
	bool m_bResize;
	QPushButton *m_menuButton_Close;
	QPushButton *m_menuButton_Max;
	QPushButton *m_menuButton_Min;
	QWidget *m_widget_title;
	bool m_bMinHide;
};

#define FRAMELESS_DIALOG_INIT() InitFrameless(ui.widget_title, ui.menuButton_Close, nullptr, nullptr, false, false)
#define FRAMELESS_MAIN_DIALOG_INIT() InitFrameless(ui.widget_title, ui.menuButton_Close, ui.menuButton_Max, ui.menuButton_Min, true, true)

#endif // QFRAMELESSDIALOG_H
