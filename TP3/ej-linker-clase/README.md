## Código de autoría

Esta carpeta es un ejemplo utilizado en clase. Se basa en el código original de:

> Santilli, C. (2015). _x86-bare-metal-examples_ [Código fuente]. GitHub. [https://github.com/cirosantilli/x86-bare-metal-examples/blob/a2ca298afe704b0b9e94cd39edb924094034cc54/bios_hello_world.S](https://github.com/cirosantilli/x86-bare-metal-examples/blob/a2ca298afe704b0b9e94cd39edb924094034cc54/bios_hello_world.S)

## Cómo compilar y ejecutar

Compilar y enlazar con los siguientes comandos:
```
    as -g -o main.o main.S
    ld --oformat binary -o main.img -T link.ld main.o
    qemu-system-x86_64 -hda main.img
```

## Descripción de archivos del directorio

`main.S` : Código fuente en ensamblador que contiene el programa principal a ejecutar.

`main.o` : Archivo objeto generado a partir de `main.S` mediante el ensamblador.

`link.ld` : Script de linker que define la disposición de la imagen binaria en memoria.

`main.img` : Imagen binaria final lista para ser ejecutada con QEMU.

