# TP Integrador — Buffet Albert Einstein

Trabajo Práctico grupal de **Algoritmos y Estructuras de Datos** (UTN FRBA) —
unidad de **archivos binarios en C/C++**.

> **Grupo:** 10
> **Integrantes:** Santiago Iglesias Fuentes  -  santi-iglesiasfuentes-utn



## Orden de ejecución de los programas:

Los programas dentro del repositorio deben ejecutarse en el siguiente orden
para que funcionen de manera correcta:

1) normalizacion.cpp

2) ventas.cpp

3) cierre.cpp

4) resumen.cpp

Para ejecutar el programa de "normalizacion" se debe tener en la misma carpeta
los archivos "comandas_historicas.dat" e "inventario.dat" dado que leerá las
comandas guardadas en "comandas_historicas.dat" y luego se actualizará el 
archivo de inventario dejando el stock de productos disponibles. Además, luego de
la ejecución de este programa, se generará el archivo "mozos.dat", necesario
posteriormente para ejecutar los programas de "ventas" y "cierre".

Para ejecutar el programa de "resumen" se debe haber ejecutado al menos una vez
el programa de "cierre" para tener un archivo de comandas semanal.




## Explicación de partes clave:

1) Normalización

La contraseña del mozo se genera a partir de su nombre. A cada caracter de su nombre
se le suman 5 unidades, generando asi una contraseña automaticamente para cada
mozo.

Por cada comanda leída de "comandas_historicas.dat" se genera un nombre de archivo de
comandas del dia y se abre el archivo en modo "ab" (agregado binario). De esta forma
se carga la comanda al final del archivo, preservando las comandas ya cargadas
anteriormente, en caso de que el archivo ya estuviera creado, o directamente se
crea el nuevo archivo y se carga la comanda. 



2) Ventas

En caso de que el usuario ingrese un 'ID de mozo' invalido o un 'ID de mozo' valido
pero una contraseña que no coincida con la guardada dentro del archivo "mozos.dat",
el programa le vuelve a pedir al usuario que ingrese la información y no lo deja 
continuar con la carga de información de la comanda hasta que ingrese los datos
válidos.

Si al momento de cargar la información de la comanda se ingresa una cantidad de
cierto producto que exceda la cantidad de stock disponible, se muestra un mensaje
de error notificando esta situación, se muestra la información del producto
(codigo de producto, descripción y stock disponible) y se le vuelve a pedir al
usuario que ingrese un codigo de producto y una cantidad.




3) Cierre

En caso de que alguna fecha no tenga planilla (podría ser que esa fecha fue feriado)
se muestra un mensaje diciendo que "no hay planilla para el dia 'X'" y se continua con
la carga de comandas de las planillas que sí existen en el archivo semanal.



4) Resumen

El programa lee las comandas del archivo semanal utlizando "corte de control" mientras
haya registros en el archivo. Por lo tanto, al agotar las comandas del último mozo
del archivo, se muestra la información del mozo y finaliza el programa.