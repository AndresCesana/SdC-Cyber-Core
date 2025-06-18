El código `boot.asm` muestra cómo pasar de modo real a modo protegido en x86.


##  Cómo compilar y ejecutar

Compilar el archivo ensamblador `boot.asm` y generar la imagen binaria:

``` bash
nasm -f bin boot.asm -o boot.img

```
Ejecutar en QEMU:

``` bash
qemu-system-i386 -fda boot.img`
```

##  Cómo debuggear con QEMU y GDB

1.  Iniciar QEMU con un puerto de depuración habilitado:
   
```
qemu-system-i386 -fda boot.img -boot a -s -S -monitor stdio

```

-   `-fda boot.img`: Especifica la imagen de arranque a usar.

-   `-boot a`: Indica que el arranque debe hacerse desde la disquetera (A:), que es donde se encuentra boot.img.

-   `-s`: Activa el servidor GDB en el puerto 1234.

-   `-S`: Hace que QEMU se inicie pausado, esperando a que GDB se conecte.

-   `monitor stdio`: Muestra la consola de QEMU en la terminal estándar, para ver los mensajes y controlar el emulador.
    

2.  En otra terminal, iniciar GDB
    
```
gdb
```

3.  En GDB, configurar la arquitectura y conectar al QEMU:

``` 
> set architecture i8086
> target remote localhost:1234
```

4.  A partir de ahí, se utilizan comandos como `break`, `stepi`, `info registers`, etc.
    



##  Descripción de archivos del directorio

`boot.asm` : Código ensamblador en NASM que implementa el cambio de modo real a modo protegido.

`boot.img` : Imagen binaria generada, lista para ejecutarse como sector de arranque en QEMU.
