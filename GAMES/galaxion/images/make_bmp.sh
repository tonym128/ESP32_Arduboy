#/bin/sh

./img2bmp.py title.png > ../bmp_title.h

./img2bmp.py -x 5 -y 7 -2 ship.png > ../bmp_chara.h
./img2bmp.py -x 9 -y 9 -2 shipexp.png >> ../bmp_chara.h
./img2bmp.py -x 5 -y 5 -2 enemyzako.png >> ../bmp_chara.h
./img2bmp.py -x 5 -y 5 -2 enemyboss.png >> ../bmp_chara.h
./img2bmp.py -x 7 -y 7 -2 enemyexp.png >> ../bmp_chara.h
./img2bmp.py -x 9 -y 5 -2 score.png >> ../bmp_chara.h
./img2bmp.py -x 18 -y 8 -2 label.png >> ../bmp_chara.h
./img2bmp.py -x 2 -y 5 -2 number.png >> ../bmp_chara.h
