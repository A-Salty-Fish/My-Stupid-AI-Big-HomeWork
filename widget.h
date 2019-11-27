#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_PushButton_clicked();

    void on_ResetButton_clicked();


    void on_ContinueButton_clicked();

    void on_AutoButton_clicked();

    void on_AntMinButton_clicked();

    void on_GenMinButton_2_clicked();

    void on_ContinueButton_2_clicked();

    void on_AutoButton_2_clicked();

    void on_DPButton_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
