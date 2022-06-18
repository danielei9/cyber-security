# Setup-VPS
Contamos con un vps y Centos 7.
## Actualizar Sistema Operativo
Los desarrolladores de distribuciones y sistemas operativos ofrecen actualizaciones frecuentes de los paquetes, en muchas ocasiones por motivos de seguridad.
Garantizar que su distribución o sistema operativo estén actualizados es un aspecto fundamental para proteger su VPS.

Esta actualización consta de dos etapas.

Actualización de la lista de paquetes:
```bash
sudo yum update
```
Actualización de los paquetes propiamente dichos :
```bash
sudo yum upgrade
```
## Cambiar puerto de escucha por defecto SSH

Una de las primeras acciones que deberá realizar en su servidor es configurar el puerto de escucha del servicio SSH. Por defecto, este se define en el puerto 22, por lo que los intentos de hackeo del servidor por parte de robots se dirigirán prioritariamente a este puerto. La modificación de este parámetro, en beneficio de un puerto diferente, es una medida sencilla para reforzar la protección de su servidor contra los ataques automatizados.

Para ello, edite el archivo de configuración del servicio con el editor de texto que desee (nano se utiliza en este ejemplo):
```bash
sudo yum install nano # Instalación de Nano centos7
sudo nano /etc/ssh/sshd_config
```
Encontrará las siguientes líneas o equivalentes:
```bash
#Port 22
```
La cambiamos por 
```bash
Port 7777
```
Seguidamente, si tenemos activado SELinux, debemos escribir el siguiente comando para permitir acceso por SSH en el puerto nuevo.
```bash
sudo semanage port -a -t ssh_port_t -p tcp 7777
```
De momento lo dejamos habilitado pero...
**Si quisieramos deshabilitar SELinux, devemos hacer lo siguiente. Editar el archivo /etc/sysconfig/selinux, y poner:** 
```bash
SELINUX=disabled
```
Realizamos un exit, y probamos a volver a entrar por ssh
```bash
ssh username@IPv4_of_your_VPS -p NewPortNumber
# En nuestro caso:
ssh centos@51.219.119.23 -p 7777
```
## Crear un usuario con permisos restringidos
Por lo general, las tareas que no requieran privilegios root deben realizarse a través de un usuario estándar. Para crear un nuevo usuario introduzca el siguiente comando:
```bash
sudo adduser NombreUsuarioPersonalizado
# En nuestro caso:
sudo adduser buchu
# Para cambiar password:
sudo passwd buchu
# Add password
# Confirm Password
```
El nuevo usuario podrá conectarse por SSH. Al establecer una conexión, utilice los datos de identificación especificados.

Una vez que se haya conectado, introduzca el siguiente comando para realizar operaciones que requieran permisos sudo:
```bash
su centos
```
## Desactivar el acceso al servidor a través del usuario centos

El usuario centos se crea por defecto en los sistemas GNU/Linux. Es el nivel de acceso más alto a un sistema operativo.
No es recomendable (ni seguro) que solo se pueda acceder al VPS como centos, ya que esta cuenta puede realizar operaciones irreversiblemente dañinas.

Le recomendamos que desactive el acceso directo de los usuarios centos mediante el protocolo SSH. No olvide crear otro usuario antes de seguir los pasos que se indican a continuación.

Es necesario modificar el archivo de configuración SSH tal y como se explica más arriba:
```bash
sudo nano /etc/ssh/sshd_config
```
Identifique la siguiente sección:
```bash
# Authentication: 
LoginGraceTime 120
PermitRootLogin yes 
StrictModes yes
```
Sustituya yes por no en la línea PermitRootLogin.
Para denegar el uso del usuario centos mediante ssh, en el archivo anterior añadiremos:
```bash
DenyUsers usuario1 usuario2 usuario3 usuario4
```
Reinicie el servicio SSH para que se apliquen los cambios:
```bash
sudo systemctl restart sshd
sudo systemctl reload sshd
```

## Instalación de Fail2Ban
```bash
 sudo yum install -y epel-release
 sudo yum update -y
 sudo yum install -y fail2ban
```
Como ocurre habitualmente en CentOS 7, el nuevo servicio fail2ban no queda en ejecución tras la instalación, ni tampoco está activado para iniciar automáticamente en cada arranque de CentOS 7, así que solucionaremos ambas situaciones con un solo comando:
```bash
sudo systemctl enable --now fail2ban
```
En estos momentos ya está en ejecución el servicio fail2ban y listo para iniciar automáticamente con CentOS 7. Puedes comprobar el estado del servicio siempre que lo necesites con el comando 
```bash
systemctl status fail2ban
```
### Configuración Fail2Ban

Al empezar a configurar Fail2Ban en CentOS 7 hay que tener en cuenta la gran cantidad de archivos de configuración de que consta, organizados bajo la ruta /etc/fail2ban/.

El archivo principal es jail.conf pero en vez de editarlo lo habitual es trabajar sobre jail.local, que inicialmente no existe, para evitar que futuras actualizaciones sobrescriban las configuraciones actuales.

En el directorio filter.d/ encontraremos una serie de filtros ya predefinidos para multitud de servicios.

Y en jail.d/ podemos situar configuraciones individuales para cada «jaula» que establezcamos, si no lo hacemos a través del archivo jail.local.

Empezaremos con la configuración general, creando el nuevo archivo jail.local:
```bash
sudo nano /etc/fail2ban/jail.local
```
El contenido inicial podría ser el siguiente:

```bash
[DEFAULT]
ignoreip = 127.0.0.1/8 x.x.x.x
bantime  = 43200
findtime  = 300
maxretry = 3
```
La explicación de las directivas y sus valores es la siguiente:

- Con ignoreip excluimos direcciones o redes de la supervisión de Fail2Ban (nuestra propia dirección IP, red local, etc.). Podría ser una lista separada por espacios en blanco.
- El tiempo de bloqueo se establece con bantime, en este ejemplo serían 12 horas (43.200 segundos).
- La directiva findtime establece el tiempo, a partir del momento actual, durante el que se toman en cuenta los posibles ataques, en el ejemplo serían los últimos 300 segundos.
- El tope de fallos que dispara el bloqueo se configura en maxretry.

**En este ejemplo, por tanto, al cometer 3 conexiones maliciosas (según el criterio de los filtros que vayamos a usar) durante los últimos 5 minutos, el firewall bloqueará la dirección IP infractora durante 12 horas. Puedes, por supuesto establecer tus propios valores de configuración.**

### Configuración de las jaulas de Fail2Ban

Empezaremos añadiendo una jaula sencilla para el servicio SSH. Usaremos el enfoque de crear archivos individuales en el directorio jail.d/, así que crearemos la nueva jaula:
```bash
sudo nano /etc/fail2ban/jail.d/10-sshd.conf
```
El contenido es el siguiente:

```bash
[sshd]

enabled = true
port = ssh
filter = sshd
maxretry = 3
findtime = 5m
bantime  = 30m
```
Básicamente la etiqueta entre corchetes es el nombre de la jaula y, si no incluyes una directiva filter, hace referencia al filtro que se va a usar, que debe coincidir con el nombre de alguno de los filtros presentes en el directorio filter.d/:

Y la directiva enabled simplemente indica si se activa la jaula o no. Por tanto no hace falta borrar el archivo de configuración de una jaula para desactivarla, bastará con indicar el valor false en la directiva enabled.
ecargamos la configuración del servicio fail2ban como cualquier otro servicio:
```bash
sudo systemctl reload fail2ban
```
Para comprobar el log de fail2ban:
```bash
# Log fail2ban
sudo tail /var/log/fail2ban.log
# Estado client fail2ban
sudo fail2ban-client status
# Comprobar estado de la jaula sshd
sudo fail2ban-client status sshd
# Finalmente reinicie el servicio para asegurarse de que se ejecuta con las personalizaciones aplicadas:
sudo service fail2ban restart

```
