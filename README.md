# 3DRenderer
1. git clone --recurse-submodules https://github.com/TrueAstralpirate/3DRenderer.git
2. apt-get install libsdl2-dev или yum install SDL2-devel
3. cd 3DRenderer
4. cd build
5. cmake ..
6. make
 
Пример работы:
1. ./run 0 0 140 0 0 0 600 400
2. Нажать пробел
3. Ввести в консоль diamond.obj 0 0 0

Управление: стрелочки для движения и мышь/тачпад для поворота камеры
P.S. На Linux поворот может работать некорректно из-за бага в SDL2, неправильно считывающего изменения курсора.
