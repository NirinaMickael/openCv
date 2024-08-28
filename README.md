Documentation du Code body 

Mode d'emploie
==============

### Window user

#### Conditions préalables

1.  Système Windows (10/8/7)
2.  Mingw-w64 , vous pouvez le télécharger [ici](https://www.mingw-w64.org/doku.php/download)
3.  OpenCv 3+ , repo officiel sur github
4.  Cmake , à télécharger ici
5.  Éditeur et terminal, peu importe ce que

[Click ici pour voir la configuration](https://medium.com/csmadeeasy/opencv-c-installation-on-windows-with-mingw-c0fc1499f39)

### Linux user

#### Conditions préalables

##### #Installer libopencv-dev

`sudo apt install libopencv-dev`

##### #Change dans le répertoire du projet, Puis exécuter ce command

`` g++ main.cpp -o ImageProcess `pkg-config --cflags --libs opencv4` ``

##### #Puis

`./ImgeProcess`
