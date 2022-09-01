## Аналог скринсейвера Windows 3D Pipes
(3 курс 2019 год)

Установка библиотек:
```
sudo apt-get install libglfw3-dev
sudo apt-get install libglm-dev
sudo apt-get install libassimp-dev
```

Сборка проекта (в корневой папке):
```
mkdir build
cd build
cmake ..
make
```

Управление камерой:

Камера повораивается вокруг центра сцены\
W/S - поворот вокруг горизонтальной оси\
A/D - поворот вокруг вертикальной оси\
R/F - поворот вокруг своей оси

Esc - Выход

![image](https://user-images.githubusercontent.com/68297855/187933455-33147437-1b1f-408b-abeb-27c134af35dd.png)
