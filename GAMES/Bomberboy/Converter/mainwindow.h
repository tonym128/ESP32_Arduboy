#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void ConvertBitmap(std::stringstream& stream, QString file_name, const char* name);
    void ConvertSpriteSet(std::stringstream& stream, QString file_name, const char* name);
    void ConvertSprite(std::stringstream& stream, QString file_name, const char* name);
};

#endif // MAINWINDOW_H
