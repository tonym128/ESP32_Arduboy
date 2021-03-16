#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdint.h>
#include <QImage>
#include <QMessageBox>
#include <sstream>
#include <iomanip>

void MainWindow::ConvertBitmap(std::stringstream& stream, QString file_name, const char* name)
{
    QImage img;
    if (!img.load(file_name))
    {
        QMessageBox::critical(this, "Converter", "Can't load assets");
        return;
    }
    img = img.convertToFormat(QImage::Format_ARGB32);

    stream << std::endl;
    stream << "const uint8_t " << name << "[] PROGMEM =" << std::endl;
    stream << "{" << std::endl;
    int index = 0;
    for (int yi = 0; yi < img.height()/8; ++yi)
    {
        for (int xi = 0; xi < img.width()/8; ++xi)
        {
            for (int x = 0; x < 8; ++x)
            {
                uint8_t value = 0;
                for (int y = 0; y < 8; ++y)
                {
                    uint32_t color = img.pixel(x + xi*8, y + yi*8);
                    if ( (color & 0xFF) > 128)
                        value |= 1 << y;
                }
                if (index % 32 == 0)
                    stream << "  ";
                stream << "0x" << std::hex << std::setw(2) << std::setfill('0') << (((int)value)&0xFF) << ",";
                if (index % 32 == 31)
                    stream << std::endl;
                index ++;
            }
        }
    }
    stream << std::endl <<"};" << std::endl;
}

void MainWindow::ConvertSpriteSet(std::stringstream& stream, QString file_name, const char* name)
{
    QImage sprites;
    if (!sprites.load(file_name))
    {
        QMessageBox::critical(this, "Converter", "Can't load spriteset");
        return;
    }
    sprites = sprites.convertToFormat(QImage::Format_ARGB32);

    stream << std::endl;
    stream << "const uint8_t " << name << "[] PROGMEM =" << std::endl;
    stream << "{" << std::endl;
    int index = 0;
    for (int yi = 0; yi < sprites.height()/8; ++yi)
    {
        for (int xi = 0; xi < sprites.width()/8; ++xi)
        {
            if (index % 32 == 0)
                stream << "  ";
            stream << "0x" << std::hex << std::setw(2) << std::setfill('0') << 8 << ",";
            if (index % 32 == 31)
                stream << std::endl;
            index ++;

            if (index % 32 == 0)
                stream << "  ";
            stream << "0x" << std::hex << std::setw(2) << std::setfill('0') << 8 << ",";
            if (index % 32 == 31)
                stream << std::endl;
            index ++;

            for (int x = 0; x < 8; ++x)
            {
                uint8_t value = 0;
                uint8_t mask = 0;
                for (int y = 0; y < 8; ++y)
                {
                    uint32_t color = sprites.pixel(x + xi*8, y + yi*8);
                    uint8_t r = color & 0xFF;
                    uint8_t g = (color >> 8) & 0xFF;
                    uint8_t b = (color >> 16) & 0xFF;

                    if ( r > 200 && g > 200 && b > 200)
                    {
                        value |= 1 << y;
                        mask |= 1 << y;
                    }
                    if ( r < 32 && g < 32 && b < 32)
                        mask |= 1 << y;
                }
                if (index % 32 == 0)
                    stream << "  ";
                stream << "0x" << std::hex << std::setw(2) << std::setfill('0') << (((int)value)&0xFF) << ",";
                if (index % 32 == 31)
                    stream << std::endl;
                index ++;

                if (index % 32 == 0)
                    stream << "  ";
                stream << "0x" << std::hex << std::setw(2) << std::setfill('0') << (((int)mask)&0xFF) << ",";
                if (index % 32 == 31)
                    stream << std::endl;
                index ++;
            }
        }
    }
    stream << std::endl <<"};" << std::endl;
}

void MainWindow::ConvertSprite(std::stringstream& stream, QString file_name, const char* name)
{
    QImage sprites;
    if (!sprites.load(file_name))
    {
        QMessageBox::critical(this, "Converter", "Can't load image");
        return;
    }
    sprites = sprites.convertToFormat(QImage::Format_ARGB32);

    stream << std::endl;
    stream << "const uint8_t " << name << "[] PROGMEM =" << std::endl;
    stream << "{" << std::endl;

    int index = 0;
    if (index % 32 == 0)
        stream << "  ";
    stream << "0x" << std::hex << std::setw(2) << std::setfill('0') << sprites.width() << ",";
    if (index % 32 == 31)
        stream << std::endl;
    index ++;

    if (index % 32 == 0)
        stream << "  ";
    stream << "0x" << std::hex << std::setw(2) << std::setfill('0') << sprites.height() << ",";
    if (index % 32 == 31)
        stream << std::endl;
    index ++;

    for (int yi = 0; yi < sprites.height()/8; ++yi)
    {
        for (int x = 0; x < sprites.width(); ++x)
        {
            uint8_t value = 0;
            uint8_t mask = 0;
            for (int y = 0; y < 8; ++y)
            {
                uint32_t color = sprites.pixel(x, y + yi*8);
                uint8_t r = color & 0xFF;
                uint8_t g = (color >> 8) & 0xFF;
                uint8_t b = (color >> 16) & 0xFF;

                if ( r > 200 && g > 200 && b > 200)
                {
                    value |= 1 << y;
                    mask |= 1 << y;
                }
                if ( r < 32 && g < 32 && b < 32)
                    mask |= 1 << y;
            }
            if (index % 32 == 0)
                stream << "  ";
            stream << "0x" << std::hex << std::setw(2) << std::setfill('0') << (((int)value)&0xFF) << ",";
            if (index % 32 == 31)
                stream << std::endl;
            index ++;

            if (index % 32 == 0)
                stream << "  ";
            stream << "0x" << std::hex << std::setw(2) << std::setfill('0') << (((int)mask)&0xFF) << ",";
            if (index % 32 == 31)
                stream << std::endl;
            index ++;
        }
    }
    stream << std::endl <<"};" << std::endl;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::stringstream stream_cpp;
    stream_cpp << "#include \"assets.h\"" << std::endl;
    stream_cpp << "#include <stdint.h>" << std::endl;
    stream_cpp << "#include <avr/pgmspace.h>" << std::endl;
    stream_cpp << std::endl;
    stream_cpp << "namespace Bomberboy" << std::endl;
    stream_cpp << "{" << std::endl;

    ConvertBitmap(stream_cpp, "../assets/tileset.png", "s_tiles");
    ConvertSpriteSet(stream_cpp, "../assets/spriteset.png", "s_sprites");
    ConvertBitmap(stream_cpp, "../assets/logo.png", "s_logo");
    ConvertSprite(stream_cpp, "../assets/bomb.png", "s_bomb");

    //Build particle animation
    {
        int particle_count = 16;
        int frames_per_particle = 32;
        stream_cpp << std::endl;
        stream_cpp << "const uint8_t " << "s_particle" << "[] PROGMEM =" << std::endl;
        stream_cpp << "{" << std::endl;
        int index = 0;
        for (int p = 0; p < particle_count; ++p)
        {
            int x = 0;
            int y = 0;
            int vx = rand() % (4*256) - (2*256);
            int vy = rand() % (4*256) - (2*256);
            for (int f = 0; f < frames_per_particle; ++f)
            {
                vy += 32;
                x += vx;
                y += vy;
                int px = x / 256;
                if (px < -128)
                    px = -128;
                if (px > 127)
                    px = 127;
                int py = y / 256;
                if (py < -128)
                    py = -128;
                if (py > 127)
                    py = 127;

                if (index % 32 == 0)
                    stream_cpp << "  ";
                stream_cpp << "0x" << std::hex << std::setw(2) << std::setfill('0') << (((int)px)&0xFF) << ",";
                if (index % 32 == 31)
                    stream_cpp << std::endl;
                index ++;

                if (index % 32 == 0)
                    stream_cpp << "  ";
                stream_cpp << "0x" << std::hex << std::setw(2) << std::setfill('0') << (((int)py)&0xFF) << ",";
                if (index % 32 == 31)
                    stream_cpp << std::endl;
                index ++;
            }
        }

        stream_cpp << std::endl <<"};" << std::endl;

    }
    stream_cpp << "}" << std::endl;
    FILE* file = fopen("../Bomberboy/assets.cpp", "wb");
    fwrite(stream_cpp.str().c_str(), 1, stream_cpp.str().length(), file);
    fclose(file);
}

MainWindow::~MainWindow()
{
    delete ui;
}
