# setup-webmin
## Instalamos las dependencias:
```bash
sudo yum install -y perl
```
Ya podemos crear el nuevo archivo de configuración para el repositorio oficial de Webmin:
```bash
sudo nano /etc/yum.repos.d/webmin.repo
```
El contenido de este nuevo archivo será el siguiente:
```bash
[Webmin]
name=Repositorio de Webmin para CentOS 7
mirrorlist=http://download.webmin.com/download/yum/mirrorlist
enabled=1
```
Guardamos los cambios y cerramos el archivo.
Para poder verificar las firmas de los paquetes de este repositorio, tendremos que importar la clave pública a nuestro llavero de claves:
```bash
sudo rpm --import http://www.webmin.com/jcameron-key.asc
```
Y ya podemos actualizar las listas de paquetes:

```bash
sudo yum update
```
## Instalar webmin
```bash
sudo yum install -y webmin
```
Por defecto el firewall de CentOS 7 suele estar activado, así que tendremos que configurarlo si queremos acceder a Webmin desde la red:
```bash
sudo firewall-cmd --permanent --add-port=10000/tcp
#Si el anterior comando falla usar estos
#NO PROBAR AUN ESTOS creo que desconfiguran algo
sudo semanage port -a -t ssh_port_t -p tcp 10000
iptables -I INPUT 1 -p tcp --dport 10000 -j ACCEPT
```

/etc/init.d/webmin restart