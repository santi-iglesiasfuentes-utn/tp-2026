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
anteriormente. 



2) Ventas

En caso de que el usuario ingrese un 'ID de mozo' invalido o un 'ID de mozo' valido
pero una contraseña que no coincida con la guardada dentro del archivo "mozos.dat",
el programa no lo deja continuar con la carga de información de la comanda.

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




Sacar despues -----------------------------------------------------------------------------

## Funcionamiento de cada programa:

1) Normalizacion

Este programa recorre el archivo "comandas_historicas.dat" mientras haya registros en
él, actualiza la cantidad de stock disponible de los productos en "inventario.dat"
y agrega la comanda a la planilla del dia correspondiente.

Una vez leida una comanda de "comandas_historicas.dat", se busca la información del
mozo que la cargó dentro del archivo "mozos.dat" de manera secuencial. En caso de
que sea la primer comanda que lee el programa o de que el mozo no esté registrado aún en
el archivo, se extrae de la comanda la información del mozo, se la guarda dentro de una 
variable de tipo "Mozo", se genera su respectiva contraseña y se almacena la información 
del mozo dentro del archivo "mozos.dat".

La contraseña del mozo se genera a partir de su nombre. A cada caracter de su nombre
se le suman 5 unidades, generando asi una contraseña automaticamente para cada
mozo.

En caso de que la comanda "antigua" la haya cargado un mozo que sí está registrado
en el sistema, se busca la posición del mozo dentro de "mozos.dat" de manera secuencial y
luego se modifica la comisión de ese registro en específico.

Luego de registrada la información del mozo, se genera el nombre del archivo de comandas
correspondiente a partir de la fecha cargada dentro de la comanda "antigua" y se
almacena en una nueva estructura (estructura "Comanda") el ID del mozo, el codigo
de producto, la cantidad de dicho producto y la comisión que le corresponda al mozo. Una
vez cargada esta información, se almacena la comanda dentro de la planilla del dia
correspondiente y se ordena el archivo de comandas por 'ID de mozo' de menor a mayor.

Por cada comanda "antigua" leída se actualiza la cantidad de stock disponible
en "inventario.dat" del producto solicitado en la ella. Para esto realizo una busqueda
binaria dentro del archivo de inventario para obtener la posición del producto y, una
vez obtenida la posición, modifico el stock disponible de ese registro en específico.




2) Ventas

Este programa permite generar un archivo de comandas para una fecha ingresada por
el usuario, siempre y cuando sea una fecha valida.

Al iniciar el programa se le pide al usuario que ingrese una fecha a fin de generar
el nombre del archivo de comandas. Hasta que el usuario no ingrese una fecha valida el
programa no continua.

Una vez ingresada la fecha, se genera el nombre del archivo de comandas del dia y
se abre el archivo en modo "wb" (escritura) para ya poder cargar las comandas. Para 
cargar una comanda el mozo debe ingresar su 'ID de mozo' y su 'contraseña'. Hasta que 
los datos ingresados no sean válidos no se puede proceder con la carga de datos de la 
comanda.

Para realizar la validación de la información del mozo realizo una busqueda directa 
dentro del archivo de mozos usando el 'ID de mozo' ingresado para obtener la información 
del mozo, luego comparo en primer lugar la longitud de ambas contraseñas (la guardada 
dentro del archivo de mozos y la nueva contraseña ingresada) y en segundo lugar, en caso 
de que coincidan en longitud, comparo caracter por caracter. En caso de no coincidir
alguna de estas cosas se muestra un mensaje de error y se le solicita al usuario que
ingrese nuevamente la información.

Luego de validada la información del mozo, se le solicita al usuario que ingrese el codigo de
producto y la cantidad del mismo. Si el usuario ingresa un codigo de producto inexistente el
programa no lo deja continuar hasta que ingrese un codigo valido. En caso de que la cantidad
del producto solicitada exceda la cantidad de stock disponible, se muestra un mensaje de error
notificando esto y se muestra la información del producto (codigo de producto, descripción
y stock disponible) y se le vuelve a solicitar al usuario que ingrese el codigo de producto
y la cantidad requerida, para completar la comanda. En el momento en que los datos ingresados
son válidos, se calcula la comisión que le corresponde al mozo, se carga la comanda en el
archivo y se muestra un mensaje preguntando si se quiere cargar una nueva comanda. Al terminar
con la carga de comandas se cierra el archivo.

Antes de finalizar el programa se ordena el archivo de comandas por 'ID de mozo' de menor
a mayor. Para esto, abro nuevamente el archivo pero ahora en modo "rb+" para asi poder editar
el archivo. Cargo las comandas almacenadas en un array de comandas, ordeno el array, posiciono
el puntero del archivo al inicio y cargo nuevamente las comandas, ahora ordenadas.




3) Cierre

