# 🌐 Proyecto paralela - Screen saver

This project simulate a screen saver in which we have N number of squares moving with circular uniform movement. This also detect the collision of the squares and when that happends it changes the symbols of the angular speed of the squares in question. User is able to write the number of squares he want to render in screen throught the cli arguments, its worth mentioning that if the number of squares is too big it will no render all of those screens because the figures can not start with overlapping between them. The program also shows the FPS in the console to know the performance of the program.

## 🛠 Installation requirements

1. The program needs the computer to have installed successfully the `SDL` library for C++.

## 🚀 Running the proyect

1. To run the proyect we just need to run the command

```shell
  g++ <file_name>.cpp -o <executable_file_name> -lmingw32 -lSDL2main -lSDL2 -fopenmp
``○`
```

In which `file_name` is the name of the file user wants to compile, It can be `mainSeq` or `mainPar` respectible for secuential or parallel programs.
And `executable_file_name` is the name user want to save the executable file.

## Contributors

| Raul Angel J. | Donaldo Garcia | Bryann Alfaro |
| :---: |:---:| :---:|
| [![Raul Angel](https://avatars0.githubusercontent.com/u/46568595?s=200&u=c1481289dc10f8babb1bdd0853e0bcf82a213d26&v=4)](https://github.com/raulangelj)    | [![Donaldo Garcia](https://avatars1.githubusercontent.com/u/54748964?s=200&u=5e617360d13f87fa6d62022e81bab94ebf50c4e3&v=4)](https://github.com/donaldosebas) | [![Bryann Alfaro](https://avatars0.githubusercontent.com/u/46506166?s=200&u=c1481289dc10f8babb1bdd0853e0bcf82a213d26&v=4)](https://github.com/bryannalfaro) |
| <a href="https://github.com/raulangelj" target="_blank">`@raulangelj`</a> | <a href="https://github.com/donaldosebas" target="_blank">`@donaldosebas`</a> | <a href="https://github.com/bryannalfaro" target="_blank">`@bryannalfaro`</a> |