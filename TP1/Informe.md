# Sistemas de Computación
## Práctico 1: Rendimiento

**Alumnos:** Cesana Andrés Agustín, Felipe Pillichody, Sol Agustina Nou  
**Profesores:** Jorge Javier Alejandro, Solinas Miguel Ángel  


## Marco teórico

### Rendimiento de un Computador

El rendimiento de un computador es la medida de su capacidad para ejecutar tareas de manera eficiente. Se evalúa en función de diversos factores, como la velocidad del procesador, la cantidad y velocidad de la memoria RAM, la capacidad del sistema de almacenamiento y la eficiencia del sistema operativo en la gestión de recursos. También depende de la arquitectura del hardware y la optimización del software. Una mayor capacidad de procesamiento permite ejecutar aplicaciones más complejas y en menor tiempo, lo que mejora la experiencia del usuario y la productividad del sistema.

### Benchmark

Los benchmarks son programas diseñados para monitorear y medir el rendimiento de un sistema de cómputo. Estos programas ejecutan pruebas específicas que evalúan el desempeño de distintos componentes del hardware y software, permitiendo comparaciones objetivas entre sistemas con diferentes configuraciones. Existen distintos tipos de benchmarks que analizan aspectos como la velocidad de procesamiento, el rendimiento gráfico, la velocidad de acceso a disco y el rendimiento en tareas específicas, como edición de video o procesamiento de datos. Los resultados obtenidos ayudan a los usuarios y profesionales a tomar decisiones informadas sobre mejoras y actualizaciones de hardware o software.

### Time Profiling

El time profiling es una técnica utilizada en la evaluación del rendimiento de un sistema o aplicación para determinar cuánto tiempo tarda en ejecutarse cada parte del código. Se emplea en el desarrollo de software para identificar cuellos de botella y optimizar la eficiencia del programa. Esta técnica mide el tiempo de ejecución de funciones específicas, lo que permite detectar procesos que consumen demasiados recursos y mejorar su implementación. Las herramientas de time profiling son fundamentales en la optimización de software, ya que permiten desarrollar aplicaciones más rápidas y eficientes, reduciendo el tiempo de respuesta del sistema y mejorando la experiencia del usuario.

## Desarrollo

A continuación, haremos una lista de las tareas que realizamos cada uno con su computadora diariamente y cuáles benchmark podrían llegar a medir mejor las tareas que ustedes realizan a diario.

| Tarea | Benchmark |
|--------|-------------|
| Tareas de oficina que requieren procesamiento de texto y hojas de cálculo (Microsoft Office, Google Workspace) | PCMark 10 |
| Jugar videojuegos | 3DMark, GFXBench |
| Reproducción de vídeos, películas | SpyderX, UL Procyon Video Playback |
| Realizar videollamadas | Zoom Test Meeting Tool, Teams Performance Test |
| Utilizar IDEs | Cinebench R23, CrystalDiskMark |
| Modelar usando programas de simulación (Proteus, MultiSIM) | PassMark (CPU y RAM) |
| Navegación web (Google Chrome, Mozilla Firefox) | Basemark Web 3.0 |
| Redes sociales | JetStream 2, Speedometer 2.0 |

En detalle, este es el funcionamiento de estos distintos benchmark:

- **PCMark 10**: Mide el rendimiento general del sistema, evaluando tareas cotidianas como navegación web, productividad (ofimática) y uso multimedia.
- **3DMark**: Ejecuta pruebas de gráficos 3D para medir la capacidad de la GPU.
- **GFXBench**: Realiza pruebas de rendimiento gráfico en 2D y 3D.
- **SpyderX**: Analiza la precisión de color de la pantalla.
- **UL Procyon Video Playback**: Mide el rendimiento de la reproducción de video.
- **Zoom Test Meeting Tool** y **Teams Performance Test**: Simulan videollamadas para evaluar el rendimiento del sistema.
- **Cinebench R23**: Test de renderizado 3D usando Cinema 4D.
- **CrystalDiskMark**: Evalúa la velocidad de lectura y escritura de discos duros y SSDs.
- **PassMark (CPU y RAM)**: Pruebas de cálculo y acceso a memoria.
- **Basemark Web 3.0**: Evalúa el rendimiento del navegador en aplicaciones web modernas.
- **JetStream 2**: Mide la eficiencia en la ejecución de aplicaciones JavaScript.
- **Speedometer 2.0**: Evalúa la rapidez del navegador en interfaces dinámicas.

A continuación, nos interesa conocer el rendimiento de los siguientes procesadores en la compilación del kernel de Linux:

- **Intel Core i5-13600K**
- **AMD Ryzen 9 5900X 12-Core**

La página OpenBenchmarking proporciona comparaciones detalladas del rendimiento de varios procesadores, incluidos los solicitados. Para evaluar el desempeño, se utilizan dos configuraciones de compilación: **allmodconfig** y **defconfig**.

- **allmodconfig**: Habilita todas las opciones y módulos del kernel, generando una carga de compilación más exigente.
- **defconfig**: Utiliza la configuración predeterminada, resultando en una compilación más liviana.

## Comparación de rendimiento en distintos escenarios:

![Gráfica de rendimiento de CPU](images/image_1.png)




El rendimiento se calcula como:

$$
\text{Rendimiento} = \frac{1}{\text{Tiempo de compilación}}
$$


| Configuración | Intel Core i5-13600K (Tiempo en s) | Intel Core i5-13600K (Comp/s) | AMD Ryzen 9 5900X (Tiempo en s) | AMD Ryzen 9 5900X (Comp/s) | Diferencia |
|--------------|------------------|------------------|------------------|------------------|------------|
| Kernel 5.18 (allmodconfig) | 700 | 0.001431 | 775 | 0.001291 | +10% |
| Kernel 6.1 (defconfig) | 72 | 0.013891 | 76 | 0.013161 | +5% |
| Kernel 6.1 (allmodconfig) | 846 | 0.001181 | 901 | 0.001111 | +6% |
| Kernel 6.8 (defconfig) | 83 | 0.012051 | 97 | 0.010311 | +14% |
| Kernel 6.8 (allmodconfig) | 1094 | 0.0009141 | 1309 | 0.0007641 | +16% |

En todas las pruebas de compilación del Kernel de Linux (5.18, 6.1 y 6.8), el **Intel Core i5-13600K** supera consistentemente al **AMD Ryzen 9 5900X**.

Las diferencias son menores en **defconfig**, que representa configuraciones estándar, pero se amplían en **allmodconfig**, donde el Intel demuestra mayor eficiencia en cargas de compilación pesadas.

---


Ahora nos interesa conocer cuál es el speedup cuando pasamos a usar un AMD Ryzen 9 7950X 16-Core. Además, nos interesa saber cuál de ellos hace un uso más eficiente de la cantidad de núcleos que tiene y cuál es más eficiente en términos de costo.

Para ello, se elige la compilación del kernel 6.1 (allmodconfig) para comparar con los resultados previos:

![Gráfica de rendimiento de CPU](images/image_2.png)

### 1. Cálculo del Speedup

El **speedup** se define como:

```math
\text{Speedup} = \frac{\text{Rendimiento del procesador más rápido}}{\text{Rendimiento del procesador de referencia}}
```

Dado que el rendimiento se mide en compilaciones por segundo, tenemos:

```math
\text{Rendimiento del AMD Ryzen 9 7950X} = \frac{1}{512} = 0.001953125
```

```math
\text{Rendimiento del AMD Ryzen 9 5900X} = \frac{1}{901} = 0.001111
```

```math
\text{Rendimiento del Intel Core i5-13600K} = \frac{1}{846} = 0.001181
```

Speedup respecto al Ryzen 9 5900X:

```math
\text{Speedup}_{7950X/5900X} = \frac{0.001953125}{0.001111} = 1.76
```

Speedup respecto al i5-13600K:

```math
\text{Speedup}_{7950X/13600K} = \frac{0.001953125}{0.001181} = 1.65
```

### 2. Cálculo de la Eficiencia en el Uso de Núcleos  
La eficiencia en el uso de los núcleos se calcula como:

```math
\text{Eficiencia} = \frac{\text{Rendimiento}}{\text{Número de núcleos}}
```

Dado que los procesadores tienen:

- **Intel Core i5-13600K** → 14 núcleos  
- **AMD Ryzen 9 5900X** → 12 núcleos  
- **AMD Ryzen 9 7950X** → 16 núcleos  

Calculamos la eficiencia:

```math
\text{Eficiencia}_{i5-13600K} = \frac{0.001181}{14} = 8.44 \times 10^{-5}
```

```math
\text{Eficiencia}_{5900X} = \frac{0.001111}{12} = 9.25 \times 10^{-5}
```

```math
\text{Eficiencia}_{7950X} = \frac{0.001953125}{16} = 1.22 \times 10^{-4}
```

El **Ryzen 9 7950X es el más eficiente en uso de núcleos**.



### 3. Eficiencia en términos de costo  
Se calcula como:

```math
\text{Eficiencia por costo} = \frac{\text{Rendimiento}}{\text{Precio en dólares}}
```

Dado que los precios en Amazon son:

- **Intel Core i5-13600K** → $230  
- **AMD Ryzen 9 5900X** → $350  
- **AMD Ryzen 9 7950X** → $462  

Calculamos:

```math
\text{Eficiencia}_{i5-13600K} = \frac{0.001181}{230} = 5.14 \times 10^{-6}
```

```math
\text{Eficiencia}_{5900X} = \frac{0.001111}{350} = 3.17 \times 10^{-6}
```

```math
\text{Eficiencia}_{7950X} = \frac{0.001953125}{462} = 4.23 \times 10^{-6}
```

El **Intel Core i5-13600K es el más eficiente en relación costo-rendimiento**.



### Conclusiones  
1. **El AMD Ryzen 9 7950X tiene el mejor rendimiento absoluto**, con un speedup de **1.76x sobre el Ryzen 9 5900X** y **1.65x sobre el i5-13600K**.  
2. **El AMD Ryzen 9 7950X usa mejor sus núcleos**, con la mayor eficiencia por núcleo.  
3. **El Intel Core i5-13600K es la opción más eficiente en costo-rendimiento**, ofreciendo más rendimiento por dólar.  

En términos de **máximo rendimiento absoluto**, el **Ryzen 9 7950X** es la mejor opción.  
En términos de **mejor eficiencia por costo**, el **i5-13600K** es la mejor opción.

# Prueba de performance de un microprocesador
En este apartado, utilizamos un controlador Arduino Uno con un procesador ATmega328, que por defecto opera con un clock de 16 MHz. Se desarrolló un código que ejecuta una serie de sumas en un bucle, permitiendo modificar además la frecuencia del clock por software mediante el prescaler.

El código empleado es el siguiente:

```
  
#include <avr/power.h>

void setup() {
    Serial.begin(9600);
    while (!Serial);

    Serial.println("Ejecutando pruebas con diferentes frecuencias...");

    // Vamos comentando y descomentando de a una las funciones para hacer distintas ejecuciones

    pruebaFrecuencia(clock_div_1, "16 MHz");

    //pruebaFrecuencia(clock_div_2, "8 MHz");
    
    //pruebaFrecuencia(clock_div_4, "4 MHz");

}

void pruebaFrecuencia(uint8_t prescaler, const char* frecuenciaTexto) {
  
    clock_prescale_set(prescaler);
    delay(100);
    Serial.begin(9600);

    Serial.print("Frecuencia actual: ");
    Serial.println(frecuenciaTexto);

    Serial.println(Inicio de la prueba);  // Timestamp antes de ejecutar la prueba

    volatile unsigned long sumInt = 0;
    for (unsigned long i = 0; i < 5000000; i++) {
        sumInt += i;
    }

    Serial.println(Fin de la prueba);  // Timestamp después de ejecutar la prueba

    Serial.println("---------------------------");
}

void loop() {}
```
Este código se ejecutó variando la frecuencia del clock de 16 MHz a 8 MHz y 4 MHz, obteniendo los siguientes resultados:

![Evolución del tiempo de ejecución del programa](images/Gráfico.png)
 
Esta gráfica se generó a partir de los logs obtenidos con el programa RealTerm, el cual nos permitió añadir una marca de tiempo (YMDHS) a cada línea de salida de la terminal. Dado que esta funcionalidad no está disponible de forma nativa en el IDE de Arduino, utilizamos RealTerm como alternativa. Los registros recopilados se encuentran en el archivo **capturesarduino.txt**, disponible en este repositorio.


# Time Profiling

En esta sección se busca documentar el proceso de análisis de rendimiento de funciones en un programa mediante técnicas de time profiling. Para ello, se siguió el tutorial "Time Profiling" y se aplicó la herramienta gprof para medir el tiempo de ejecución de las funciones de un programa. Lo que se debe realizar es:
- Habilitar “profiling” durante la compilación
- Ejecutar el código del programa para producir los datos de perfil
- Ejecute la herramienta gprof en el archivo de datos de generación de perfiles (generado en el paso anterior).

Del último paso obtendremos un archivo de análisis legible.

Este apartado contiene una descripción de los pasos realizados, junto con los resultados obtenidos en el análisis del tiempo de ejecución de cada función. 

## Seguimiento del tutorial

Para el desarrollo se usarán los códigos cargados en este repositorio [test_gprof.c](test_gprof.c) y [test_gprof_new.c](test_gprof_new.c)

### Paso 1: creación de perfiles habilitada durante la compilación
Para poder analizar el rendimiento del código, primero se compiló el programa con la opción -pg de gcc, que permite generar información de perfilado.

```
$ gcc -Wall -pg test_gprof.c test_gprof_new.c -o test_gprof
$ ls
```
![Compilación del código con perfiles](images/timeprof1.png)

### Paso 2: ejecución del programa
Una vez compilado, se ejecutó el archivo binario para generar datos de perfilado.
```
$ ./test_gprof 
$ ls
```
![Ejecucion y reporte de la ejecucion](images/timeprof2.png)

### Paso 3: uso de la herramienta gprof

Para analizar el rendimiento del programa, se utilizó gprof para generar un informe detallado del tiempo de ejecución de cada función.
```
$  gprof test_gprof gmon.out > analysis_nombreIntegranteGrupo.txt
$ ls
```
Este comando genera un archivo analysis.txt con información sobre el tiempo de ejecución de cada función y la estructura de llamadas.

![Generacion del informe](images/timeprof3.png)

### Personalizar la salida con flags

Para obtener un análisis mas detallado hay distintas flags que podemos utilizar. Algunas de ellas:

1. Suprima la impresión de funciones declaradas estáticamente (privadas) 
  ```
  $ gprof -a test_gprof gmon.out > analysis_nombreIntegranteGrupo.txt
  ```
![flag 1](images/timeprof4.png)

2. Elimine los textos detallados 
  ```
  $ gprof -b test_gprof gmon.out > analysis_nombreIntegranteGrupo.txt
  ```
![flag 1](images/timeprof5.png)

3. Imprimir solo perfil plano 
   ```
   $ gprof -b test_gprof gmon.out > analysis_nombreIntegranteGrupo.txt
   ```
![flag 1](images/timeprof6.png)

4. Imprimir información relacionada con funciones específicas en perfil plano
   ```
   $ gprof -pfunc1 -b test_gprof gmon.out > analysis_nombreIntegranteGrupo.txt
   ```
![flag 1](images/timeprof7.png)


### Generar un gráfico
Para visualizar mejor el flujo del programa, se utilizó gprof2dot junto con dot para generar gráficos:
```
$ gprof test_gprof gmon.out | python3 -m gprof2dot -o output.dot
$ dot -Tpng output.dot -o profile_nombreIntegranteGrupo.png
$ xdg-open profile_nombreIntegranteGrupo.png
```
![Generar grafico](images/timeprof8.png)

![grafico](images/profile_sol.png)

El archivo muestra un diagrama de llamadas entre funciones, facilitando la identificación de los cuellos de botella.

## Profiling con linux perf

Otra herramienta utilizada fue perf, que permite analizar el rendimiento del programa de manera más detallada y con menos overhead que gprof

### Instalación
```
$ sudo apt install linux-tools-common
$ sudo apt install linux-tools-5.19.0-35-generic
```

## Ejecución
```
 $ sudo perf record ./test_gprof
```
![Generar grafico](images/timeprof9.png)

Esto genera un archivo perf.data, que luego se analiza con:
```
 $ sudo perf report
```
![Generar grafico](images/timeprof10.png)

El reporte obtenido proporciona estadísticas detalladas sobre el tiempo de ejecución de cada función y permite detectar posibles optimizaciones ad



