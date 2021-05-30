#ifndef ICONHELPER_H
#define ICONHELPER_H

#include <QObject>
#include <QLabel>
#include <QAbstractButton>
//图形字体处理类
class IconHelper : public QObject
{
    Q_OBJECT

public:
    static IconHelper *Instance();
    explicit IconHelper(QObject *parent = 0);

    static QChar IconMain;          //标题栏左上角图标
    static QChar IconMenu;          //下拉菜单图标
    static QChar IconMin;           //最小化图标
    static QChar IconMax;           //最大化图标
    static QChar IconNormal;        //还原图标
    static QChar IconClose;         //关闭图标


    //获取图形字体
    QFont getIconFont();

    //设置图形字体到标签
    void setIcon(QLabel *lab, const QChar &str, quint32 size = 12);
    //设置图形字体到按钮
    void setIcon(QAbstractButton *btn, const QChar &str, quint32 size = 12);

    //获取指定图形字体,可以指定文字大小,图片宽高,文字对齐
    QPixmap getPixmap(const QColor &color, const QChar &str, quint32 size = 12,
                      quint32 pixWidth = 15, quint32 pixHeight = 15,
                      int flags = Qt::AlignCenter);
private:
    static QScopedPointer<IconHelper> self;

    QFont iconFont;             //图形字体

};


#endif // ICONHELPER_H
