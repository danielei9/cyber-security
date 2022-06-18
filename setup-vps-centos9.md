# Setup-VPS
Contamos con un vps y Centos 9.
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

## Configurar ssh 
En la terminal comprobamos el estado de SSH con el comando:
```bash
rpm -qa | grep ssh
#configurar archivo
nano /etc/ssh/sshd_config
```
- Desmarcamos la línea "port" y establecemos un nuevo puerto por seguridad ya que por defecto es el puerto 22 y un atacante puede aprovechar este valor predeterminado.
- Ahora desmarcamos la línea "ListenAddresses" y asignamos la IP de CentOS 9 Stream 0.0.0.0
- Ahora ubicamos la línea PermitRootLogin

```bash
#Nº puerto
Port 7777
#donde escucha
ListenAddress 0.0.0.0
#No permitre root en ssh
PermitRootLogin no
#Maximo de pruebas de contraseña user 5
MaxAuthTries 5
###########################
#Nuevo ejemplo actualizado 
###########################
    #Nº puerto
    Port 432    (o el que se quiera menor a 1024)
    #Nº Version de protocolo ssh
    Protocol 2
    #El número indica la cantidad de segundos en que la pantalla de login estará disponible para que el usuario capture su nombre de usuario y contraseña
    #donde escucha
    ListenAddress 0.0.0.0
    LoginGraceTime 30
    #No permitre root en ssh
    PermitRootLogin no
    #antidad de veces que podemos equivocarnos en ingresar el usuario
    MaxAuthTries 2
    #cantidad de conexiones simultaneas de login 
    MaxStartups 3
    #Allow users  se usa asi: 
    #AllowUsers sergio  o también:      
    #AllowUsers sergio analuisa@10.0.1.100
```

Iniciamos el servicio de SSH:
```bash
systemctl start sshd
systemctl restart sshd
```
## Configurando la hora del vps
```bash
nano /etc/ntp.conf
```
Añadimos los servidores ntp específicos para una región, en nuestro caso España:
```bash
server 0.es.pool.ntp.org
server 0.europe.pool.ntp.org
server 2.europe.pool.ntp.org
```
Una vez modificada la configuración, par aplicar los cambios realizados, reiniciamos el servicio ntp:

```bash
# saber el timedata del server
timedatectl

/etc/init.d/ntp restart
```
### Cambiando nuestra zona horaria
```bash
 timedatectl set-timezone Europe/Madrid
```

## Rkhunter
Nos permite llevar un control de los programas instalados, detectando modificaciones o nuevos programas no autorizados instalados. Mediante un control de firmas md5, comprueba si algún rootkit o backdoor se ha instalado en nuestro vps.

```bash
cd /usr/local/src
wget https://sourceforge.net/projects/rkhunter/files/rkhunter/1.4.6/rkhunter-1.4.6.tar.gz/
tar -zxvf rkhunter-1.4.6.tar.gz
cd rkhunter-1.4.6
./installer.sh –-layout default –-install
rkhunter –-update
rkhunter –-propupd
```
Luego de Instalar RKHunter en Centos, lo agregamos al CRON para que se ejecute a diario.
```bash
nano -w /etc/cron.daily/rkhunter.sh
```

Agregamos el siguiente texto al archivo chkrootkit.sh:
```bash
#!/bin/sh
/usr/local/bin/rkhunter –versioncheck
/usr/local/bin/rkhunter –update
/usr/local/bin/rkhunter –cronjob –report-warnings-only
/bin/mail -s 'rkhunter Daily Run (Nombre de tu Servidor)' tu@email.com
```

Añadir permisos necesarios.
```bash
chmod 700 /etc/cron.daily/rkhunter.sh
```

Ejecutamos la aplicación.
```bash
rkhunter -c
```
## Fail2Ban
### Instalación de Fail2Ban
```bash
 sudo yum install -y epel-release
 sudo yum update -y
 sudo yum install -y fail2ban
```
Como ocurre habitualmente en CentOS 7, el nuevo servicio fail2ban no queda en ejecución tras la instalación, ni tampoco está activado para iniciar automáticamente en cada arranque de CentOS 7, así que solucionaremos ambas situaciones con un solo comando:
```bashv
sudo systemctl enable --now fail2ban
```
En estos momentos ya está en ejecución el servicio fail2ban y listo para iniciar automáticamente con CentOS 7. Puedes comprobar el estado del servicio siempre que lo necesites con el comando 
```bash
systemctl status fail2ban
```
### Configuración de alertas vía email en Fail2ban
Si queremos podemos hacer que fail2ban nos avise cada vez que se bloquee una IP o haya alguna incidencia. Para ello, como hemos visto en apartados anteriores tendremos que tener instalado sendmail.

Una vez instalado sendmail ejecutaremos el siguiente comando para averiguar la dirección de email usada por sendmail para enviar los emails:
```bash
echo "Email test enviado desde sendmail" | /usr/sbin/sendmail dirección_de_email_personal
```
TODO: PROBAR 