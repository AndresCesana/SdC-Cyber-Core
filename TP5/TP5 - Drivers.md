

# Sistemas de Computación


## Práctico 5: *Device drivers*

<!-- Imágenes de portada (sin títulos) -->

**Alumnos:** Cesana Andrés Agustín, Felipe Pillichody, Sol Agustina Nou

**Profesores:** Jorge Javier Alejandro, Solinas Miguel Ángel

  

---

# Introducción

  

Un **driver** es aquel que conduce, administra, controla, dirige o monitorea la entidad bajo su mando.

Un **bus driver** hace eso con un *bus* y, de forma análoga, un **device driver** lo hace con un dispositivo.

Un dispositivo puede ser cualquier periférico conectado a una computadora (mouse, teclado, pantalla, disco duro, cámara, reloj, etc.).

  

Un *driver* puede ser una persona o un sistema automático. Del mismo modo, el *device driver* podría ser una pieza de software u otro periférico/dispositivo, normalmente controlado por software.

Si ese segundo elemento es otro hardware, se lo llama **device controller**; por “driver”, en cambio, nos referimos solo al *software driver*.

Un device controller es un dispositivo en sí y, por lo tanto, muchas veces también necesita su propio *driver*, comúnmente denominado **bus driver**.

  

Ejemplos típicos de *device controller*:

  

* Controladores de disco duro

* Controladores de pantalla

* Controladores de audio

* Controladores de protocolos de hardware (IDE, PCI, USB, SPI, I²C, …)

  

---

  

# Consigna

  

Para superar este TP deben **diseñar y construir un CDD** que:

  

1. Sense **dos señales externas** con un período de **1 s**.

2. Permita que una aplicación de usuario lea **una sola señal** a la vez y la grafique en función del tiempo.

3. Permita indicar al CDD **cuál de las dos señales** leer.

4. Realice eventuales correcciones de escala **a nivel de usuario**.

5. Muestre gráficos con:

* Tipo de señal sensada

* Unidades en abscisas

* Tiempo en ordenadas

6. **Reinicie** el gráfico y ajuste la escala al cambiar de señal.

  



  

---

  

# Marco teórico

  

### ¿Qué es un driver?
 

Un **driver** (controlador) es un programa de software que permite que el sistema operativo se comunique y controle un hardware.

Actúa como intérprete entre el SO y el dispositivo físico. Sin el driver adecuado, el sistema no sabría cómo manejar ese hardware.

  

#### Características clave

  

- Cada tipo de hardware necesita su propio driver.

- Sin el driver correcto, un dispositivo puede no funcionar o hacerlo de forma incorrecta.

- Los drivers pueden venir preinstalados, descargarse automáticamente o instalarse manualmente.

  

### ¿Cuántos tipos hay?

  

Un *device driver* puede ser:

  

* **Software puro** que controla otro software (simplemente “driver”).

* **Device driver** propiamente dicho, que controla hardware.

  

A su vez, los device drivers se clasifican en:

  

* **Paquetes (packet drivers)**

* **Bloques (block drivers)**

* **Character drivers**

  

> Todos los *device drivers* son *drivers*, pero no todos los *drivers* son *device drivers*.

  

### ¿Quién los construye?

  

Habitualmente los crea el **fabricante del hardware**, pero también existen controladores libres mantenidos por la comunidad, sobre todo en sistemas operativos de código abierto.

  

---

  

## CDD — *Character Device Driver*

  

Un **Character Device Driver** permite leer o escribir datos **byte a byte**, como una secuencia.

En Linux se representan como archivos especiales bajo el directorio `/dev`.

  
![Flujo de llamadas al kernel](https://raw.githubusercontent.com/solnou/SdC-Cyber-Core/main/TP5/Imagenes/image3.png)
---

  

# Desarrollo

  
## Primera parte: Prueba de los drivers ejemplos.
En Linux, todo driver se compone de **constructor** y **destructor**:

  

* El **constructor** se ejecuta cada vez que `insmod` carga el módulo en el kernel.

* El **destructor** se ejecuta cada vez que `rmmod` descarga el módulo del kernel.

  

Este comportamiento se prueba mediante el **Driver 1**.


### Driver 1
---
#### Código fuente
```c
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TP5 – CDD");
MODULE_DESCRIPTION("drv1: módulo mínimo");
/* Constructor */
static int __init drv1_init(void)
{
printk(KERN_INFO "SdeC: drv1 cargado\n");
return 0;
}
/* Destructor */
static void __exit drv1_exit(void)
{
printk(KERN_INFO "SdeC: drv1 descargado\n");
}
module_init(drv1_init);
module_exit(drv1_exit);
```

El fin de correr este primer módulo es poder enteneder como funciona la carga y descarga del driver.
A continuacion se evidencia este comportamiento.
```c
sudo insmod drv1.ko
sudo dmesg | tail -n 5
[sudo] contraseña para andresces:
[ 2237.762936] SdeC: drv1 cargado
rmmod drv1
sudo dmesg | tail -n 5
[ 2250.139672] SdeC: drv1 descargado
```
Para este trabajo era necesario firmar los drivers para poder utilizarlos, como no eramos los auores se decidio desactivar el SecureBoot para poder cargarlos sin dificultad.
### Driver 2
---
#### Codigo fuente 
```c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
static dev_t first;
static int __init drv2_init(void)
{
    int ret;
    pr_info("SdeC_drv2: Registrado!\n");
    ret = alloc_chrdev_region(&first, 0, 3, "SdeC_Driver2");
    if (ret) return ret;
    pr_info("<Major, Minor inicial>: <%d,%d>\n", MAJOR(first), MINOR(first));
    return 0;
}
static void __exit drv2_exit(void)
{
    unregister_chrdev_region(first, 3);
    pr_info("SdeC_drv2: Desregistrado\n");
}
module_init(drv2_init);
module_exit(drv2_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cátedra SdC");
MODULE_DESCRIPTION("Driver 2 – ejemplo MAJOR/MINOR");
```
Como se puede notar la forma de cargarlo y descargarlo del kernel es igual, pero la función del driver es demostrar asignación dinámica de números major/minor, por lo que obtendremos la salida de consola que se muestra a continuacion.
```c
[sudo] contraseña para andresces:
[ 5087.788884] SdeC_drv2: Registrado!
[ 5087.788890] <Major, Minor inicial>: <511,0>
# Crear nodos de dispositivo (suponiendo major 511)
sudo mknod /dev/SdeC_drv2_0 c 511 0
sudo mknod /dev/SdeC_drv2_1 c 511 1
sudo mknod /dev/SdeC_drv2_2 c 511 2
ls -l /dev/SdeC_drv2_*
# Descargar el módulo y limpiar
sudo rmmod drv2
sudo rm /dev/SdeC_drv2_{0,1,2}
sudo dmesg | tail -n 5
[ 5937.712737] SdeC_drv2: Desregistrado
```
Si ahora se busca el major, podemos ver la lista de todos los nodos del dispositivo.
```c
cpi_thermal_rel
autofs
block
btrfs-control
[...]
zfs
```
Como no aparecen se crean tres nodos de prueba.
```c
sudo mknod /dev/SdeC_drv2_0 c $major 0
sudo mknod /dev/SdeC_drv2_1 c $major 1
sudo mknod /dev/SdeC_drv2_2 c $major 2
```
y la nueva lista de nodos es.
```c
sudo mknod /dev/SdeC_drv2_0 c $major 0
sudo mknod /dev/SdeC_drv2_1 c $major 1
sudo mknod /dev/SdeC_drv2_2 c $major 2
```
Podemos verlo en la lista.
![Nodos asignados](https://raw.githubusercontent.com/solnou/SdC-Cyber-Core/main/TP5/Imagenes/Image4.png)
### Driver 3
---
#### Codigo fuente 
```c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>

static dev_t first;
static struct cdev c_dev;
static struct class *cl;

/* operaciones */
static int my_open (struct inode *i, struct file *f){ pr_info("drv3: open\n");  return 0; }
static int my_close(struct inode *i, struct file *f){ pr_info("drv3: close\n"); return 0; }
static ssize_t my_read (struct file *f, char __user *b, size_t l, loff_t *o){
    pr_info("drv3: read\n"); return 0;
}
static ssize_t my_write(struct file *f, const char __user *b, size_t l, loff_t *o){
    pr_info("drv3: write – %zu bytes\n", l); return l;
}

static const struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = my_open,
    .release = my_close,
    .read    = my_read,
    .write   = my_write,
};

/* init / exit */
static int __init drv3_init(void)
{
    int ret;
    struct device *dev_ret;

    pr_info("drv3: loading\n");

    if ((ret = alloc_chrdev_region(&first, 0, 1, "SdeC_drv3")))
        return ret;

    cl = class_create("chardrv");          /* sin THIS_MODULE en kernels ≥ 5.17 */
    if (IS_ERR(cl)){
        unregister_chrdev_region(first, 1);
        return PTR_ERR(cl);
    }
    dev_ret = device_create(cl, NULL, first, NULL, "SdeC_drv3");
    if (IS_ERR(dev_ret)){
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return PTR_ERR(dev_ret);
    }
    cdev_init(&c_dev, &fops);
    if ((ret = cdev_add(&c_dev, first, 1))){
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return ret;
    }
    pr_info("drv3: <major,minor>=<%d,%d>\n", MAJOR(first), MINOR(first));
    return 0;
}
static void __exit drv3_exit(void)
{
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    pr_info("drv3: unloaded\n");
}
module_init(drv3_init);
module_exit(drv3_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cátedra SdC");
MODULE_DESCRIPTION("Driver 3 – char device con file_operations");
```
La función del driver es aceptar llamadas open / read / write / close, pero no almacena datos ni controla hardware. Se realiza una prueba de escitura y lectura
```c
echo "Hola driver..." > /dev/SdeC_drv3
cat  /dev/SdeC_drv3
```
De lo que se obtiene
```c
drv3: open
drv3: write – 15 bytes
drv3: close
drv3: open
drv3: read
drv3: close
```
`open()` Imprime “open” y retorna 0.
`write()` Informa tamaño recibido (15 bytes).
`read()` Imprime “read” y devuelve 0.
`close()` Fin de ambas operaciones.
Por ultimo debemos descargarlo
```c
sudo rmmod drv3
```
```c
drv3: unloaded
```
### Driver 4
---
#### Codigo fuente 
```c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>

static dev_t first;
static struct cdev  c_dev;
static struct class *cl;
static char c;

/* operaciones */
static int my_open (struct inode *i, struct file *f){ pr_info("drv4: open\n");  return 0; }
static int my_close(struct inode *i, struct file *f){ pr_info("drv4: close\n"); return 0; }

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
    pr_info("drv4: read\n");
    if (*off == 0){
        if (copy_to_user(buf, &c, 1)) return -EFAULT;
        (*off)++;                     /* EOF tras primer byte */
        return 1;
    }
    return 0;                         /* fin de archivo */
}

static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    pr_info("drv4: write\n");
    if (copy_from_user(&c, buf + len - 1, 1)) return -EFAULT;
    return len;
}

static const struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = my_open,
    .release = my_close,
    .read    = my_read,
    .write   = my_write,
};

/* init / exit */
static int __init drv4_init(void)
{
    int ret;
    struct device *dev_ret;

    pr_info("drv4: loading\n");

    if ((ret = alloc_chrdev_region(&first, 0, 1, "SdeC_drv4"))) return ret;

    cl = class_create("SdeC_drive");          /* ← SIN THIS_MODULE */
    if (IS_ERR(cl)){ unregister_chrdev_region(first,1); return PTR_ERR(cl); }

    dev_ret = device_create(cl, NULL, first, NULL, "SdeC_drv4");
    if (IS_ERR(dev_ret)){
        class_destroy(cl); unregister_chrdev_region(first,1); return PTR_ERR(dev_ret);
    }

    cdev_init(&c_dev, &fops);
    if ((ret = cdev_add(&c_dev, first, 1))){
        device_destroy(cl, first); class_destroy(cl);
        unregister_chrdev_region(first,1); return ret;
    }
    pr_info("drv4: <major,minor>=<%d,%d>\n", MAJOR(first), MINOR(first));
    return 0;
}

static void __exit drv4_exit(void)
{
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first,1);
    pr_info("drv4: unloaded\n");
}
module_init(drv4_init);
module_exit(drv4_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cátedra SdC");
MODULE_DESCRIPTION("Driver 4 – char device con buffer de 1 byte");
```
El driver mantiene un único byte de estado dentro del kernel. Permite que aplicaciones de espacio de usuario escriban un carácter y luego lo lean.
```c
sudo insmod drv4.ko
SdeC_drv4: Registrado exitosamente..!!
grep SdeC_drv4 /proc/devices         
ls -l /dev/SdeC_drv4               
sudo chmod 666 /dev/SdeC_drv4        
echo -n "H" > /dev/SdeC_drv4         
cat /dev/SdeC_drv4                  
sudo dmesg | tail -4
drv4: open
drv4: write
drv4: close
drv4: open
drv4: read
drv4: close
sudo rmmod drv4
```
Se carga con insmod drv4.ko
opne crea el descriptor;  write copia sólo el carácter ‘H’ en c; close cierra el descriptor.
Primera llamada a read() devuelve el byte ‘H’; cat lo muestra. Hace una segunda llamada, read() devuelve 0, termina y luego close.
Por ultimo se descarga con rmmod drv4


## Segunda parte: Controlador de Dispositivo de Caracteres (CDD)

En este apartado se busca realizar la construcción de un Controlador de Dispositivo de Caracteres (CDD) que permita sensar dos señales digitales externas con un período de un segundo.

A través de una aplicación de usuario, se debe poder:
-   Elegir cuál de las dos señales leer.      
-   Leer dicha señal y graficarla en función del tiempo. El gráfico debe incluir:  
-   El tipo de señal (nombre/etiqueta).      
-   Unidades en el eje X (abcisas).      
-   Tiempo en el eje Y (ordenadas).

Al cambiar de señal:
-   El gráfico debe reiniciarse y adaptarse a la nueva medición.

Las correcciones de escala, si fueran necesarias, deben aplicarse en la aplicación de usuario, no en el driver.
Esta práctica busca comprender el rol del device driver como pieza clave en la comunicación entre el sistema operativo y los dispositivos periféricos, enfocándose en el caso particular de señales digitales.  
Además, se explora la diferencia entre software drivers y hardware controllers, un concepto fundamental en sistemas operativos y arquitectura de computadoras.


### Raspberry Pi 3B emulada con QEMU
---
Para el desarrollo, se utiliza una Raspberry Pi 3B emulada con QEMU, lo que permite simular tanto el hardware como el entorno operativo, sin requerir hardware físico.

Primero, comencemos por instalar QEMU y las herramientas necesarias, como el proyecto qemu-rpi-gpio o el binario qemu-system-arm necesario para emular arquitectura ARM (como la Raspberry Pi).

```c
sudo apt update
sudo apt install qemu-system qemu-user qemu-utils qemu-system-arm socat build-essential libfdt-dev device-tree-compiler python3-pip
pip install qemu-rpi-gpio
```
Para el proyecto qemu-rpi-gpio se utilizó un entorno virtual.

A continuación, debemos descargar y preparar la imagen de Raspbian (OS para Raspberry Pi).

wget https://downloads.raspberrypi.com/raspios_lite_arm64/images/raspios_lite_arm64-2024-03-15/2024-03-15-raspios-bookworm-arm64-lite.img.xz
unxz 2024-03-15-raspios-bookworm-arm64-lite.img.xz
qemu-img resize 2024-03-15-raspios-bookworm-arm64-lite.img 8G

NOTA sobre los 8 GB: se aumenta el tamaño del archivo de imagen del sistema operativo a 8 gigabytes porque, por defecto, muchas imágenes de sistemas embebidos vienen con un tamaño mínimo justo para arrancar y contener sólo lo esencial del sistema operativo, lo cual no nos sirve si buscamos construir nuevos drivers.

![](https://raw.githubusercontent.com/solnou/SdC-Cyber-Core/main/TP5/Imagenes/image6.png)

Ahora, aun desde Linux nativo debemos montar la imagen para extraer los archivos necesaria para QEMU pueda emular. Se van a extraer dos archivos clave:
-   kernel8.img (el kernel ARM para Raspberry Pi 64 bits)     
-   El archivo .dtb (device tree binary para Raspberry Pi 3B+)  
 
Creamos carpetas de montaje en /mnt:
```bash
sudo mkdir -p /mnt/rpi-boot /mnt/rpi-root
```
El directorio /mnt en sistemas tipo Unix, como Linux, se utiliza tradicionalmente como un punto de montaje temporal para sistemas de archivos externos como discos duros, unidades USB o CD-ROMs.


Creamos carpeta destino:
```bash
mkdir -p ~/qemu-rpi/rootfs
```
Asociar la imagen a un dispositivo de loop con detección de particiones
```bash
sudo losetup --find --partscan --show 2024-03-15-raspios-bookworm-arm64-lite.img
```
Esto nos devuelve /dev/loop1
Montar las particiones (boot y root)
```bash
sudo mount /dev/loop1p1 /mnt/rpi-boot
sudo mount /dev/loop1p2 /mnt/rpi-root
```
Copiamos los archivos necesarios en la carpeta destino
```bash
cp /mnt/rpi-boot/kernel8.img ~/rootfs/
cp /mnt/rpi-boot/*.dtb ~/rootfs/
```
Veamos el resultado:

![](https://raw.githubusercontent.com/solnou/SdC-Cyber-Core/main/TP5/Imagenes/image7.png)


De los archivos que observamos que se copiaron en la carpeta /rootfs, nosotros necesitamos el device tree blob bcm2710-rpi-3-b-plus.dtb, porque se está emulando una Raspberry Pi 3B+, que usa el SoC Broadcom BCM2710. Tambien necesitamos la imagen del núcleo de Linux compilada para arquitectura ARM de 64 bits (AArch64) . Esto es porque QEMU no arranca la imagen de Raspbian como una PC (con BIOS o GRUB), sino como una Raspberry real: necesita que le des el kernel (kernel8.img) y el device tree (.dtb) explícitamente.


A continuación, se debe construir un script de shell basado en el script run.sh del proyecto qemu-rpi-gpio. Este script implementa un gestor virtual de pines GPIO que se comunica con QEMU a través de un socket UNIX (/tmp/tmp-gpio.sock) usando socat. Utiliza las direcciones de memoria mapeadas de los GPIO de la Raspberry Pi 3B para simular lecturas y escrituras sobre los registros de control de pines.

De esta forma, desde una terminal simlulamos las entradas GPIO
```bash
cd ~/qemu-rpi
source venv/bin/activate
python qemu_rpi_gpio.py
```

Y desde otra ventan levantamos la Raspberry Pi emulada:    
```bash
cd ~/qemu-rpi
./run.sh
```bash

Esto lanza la VM de Raspbian.La primera vez que se ingresa solicita una configuración inicial.

![](https://raw.githubusercontent.com/solnou/SdC-Cyber-Core/main/TP5/Imagenes/image5.png)

![](https://raw.githubusercontent.com/solnou/SdC-Cyber-Core/main/TP5/Imagenes/qemu_adentro.png)

Una vez terminada la configuración inicial y ya vemos la línea de comandos en la ventana de QEMU, ahora podemos activar y usar SSH, lo que nos va a permitir un manejo más ágil que usar la ventana emulada.

Dentro de la Raspberry, hacemos:

```bash
sudo systemctl enable ssh
sudo systemctl start ssh
```

Verificamos que esté corriendo:
```bash
sudo systemctl status ssh
```
Desde la máquina host (Linux Mint), abrimoss una terminal nueva y ejecutamos:
```bash
ssh felipe@127.0.0.1 -p 50022
```

![](https://raw.githubusercontent.com/solnou/SdC-Cyber-Core/main/TP5/Imagenes/ssh_established.png)

Una vez configurado el acceso SSH y antes de comenzar con la construccion del CDD propiamente dicha, el utimo es hacer efectivo el aumento del tamaño de la imagen que hicimos anteriormente con:
```bash
sudo raspi-config
```

Luego desde la interfaz que se abre vamos a Advanced Options → Expand Filesystem. Esto expande el sistema de archivos al total disponible en la imagen, haciendo efectivos los 8 G luego de un reboot.

![](https://raw.githubusercontent.com/solnou/SdC-Cyber-Core/main/TP5/Imagenes/resized_succesfull.png)

### Construccion del CDD

El código fuente se encuentra bajo el nombre de signal_driver.c. 


Ahora debemos copiar y compilar el driver en la Raspberry. En la máquina host (Linux Mint) copiamos el folder /drivers y sus respectivos archivos al entorno emulado así:
```bash
scp -P 50022 -r driver/ felipe@127.0.0.1:/home/felipe/driver
```
Ahora, para compilar, dentro de la Raspberry via SSH hacemos
```bash
cd ~/driver
make # compila el módulo
dtc -@ -I dts -O dtb -o signal_driver.dtbo signal_driver.dts # compila el overlay
```

![](https://raw.githubusercontent.com/solnou/SdC-Cyber-Core/main/TP5/Imagenes/driver_copiado_compilado.png)

Para copiar el overlay al lugar correcto:

```bash
sudo cp signal_driver.dtbo /boot/overlays/
```
![](https://raw.githubusercontent.com/solnou/SdC-Cyber-Core/main/TP5/Imagenes/image10.png)

Estos comandos que vemos en la imagen se utilizan para instalar y activar el Device Tree Overlay.


Ahora, cargamos el driver en el kernel de la Raspberry y preparamos su funcionamiento.

Tal como vimos en la Parte 1 con los drivers de ejemplo, al cargar el módulo con *insmod*, se crea automáticamente un archivo especial en el sistema, /dev/signal_dev_TP5, que actúa como interfaz entre el usuario y el driver. A través de este archivo, se pueden realizar lecturas (cat) y escrituras (echo) que son gestionadas por las funciones definidas en el driver
Con 
```bash
echo 0 > /dev/signal_dev_TP5
```
se selecciona el pin GPIO17; con 1, el GPIO27.
Luego, con 
```bash
cat /dev/signal_dev_TP5
```
se obtiene el valor actual del pin seleccionado (0 o 1).

![](https://raw.githubusercontent.com/solnou/SdC-Cyber-Core/main/TP5/Imagenes/image11.png)

Verificamos que el archivo de dispositivo existe, lo cual confirma que el módulo fue cargado correctamente y que el dispositivo está disponible para su uso.

![](https://raw.githubusercontent.com/solnou/SdC-Cyber-Core/main/TP5/Imagenes/image12.png)

Tenemos entonces ahora nuestra interfaz entre el espacio de kernel y el espacio de usuario.

![](https://raw.githubusercontent.com/solnou/SdC-Cyber-Core/main/TP5/Imagenes/image13.png)

Es decir. el dispositivo /dev/signal_dev_TP5 está accesible. Con el comando cat podemos ver que la lectura devuelve valores 0 y 1, alternando correctamente, dado que las señales externas están siendo sensadas a través del CDD.





### Interfaz de usuario
Se implementa una interfaz de usuario a través de una aplicación desarrollada con Streamlit, que permite seleccionar qué señal graficar, ya sea la señal 0 (correspondiente al pin GPIO17) o la señal 1 (pin GPIO27). La aplicación genera en tiempo real un gráfico que muestra la evolución del valor leído en el pin seleccionado. Además, incluye una opción para reiniciar el gráfico, permitiendo al usuario comenzar una nueva visualización desde cero en cualquier momento.

![](https://raw.githubusercontent.com/solnou/SdC-Cyber-Core/main/TP5/Imagenes/interfaz.png)

