/**
 * @file cyber-security.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-08
 * 
 * @copyright Copyright (c) 2022
 * 
 * ****************************************************
 * ****************************************************
 * ****************     SMB     ***********************
 * ****************************************************
 * ****************************************************
 * 
 * SMB (Server Message Block Protocol) es un protocolo de comunicación cliente-servidor que se utiliza para compartir el acceso a archivos, impresoras, puertos serie y otros recursos en una red. 
 * El protocolo SMB se conoce como protocolo de solicitud de respuesta, lo que significa que transmite múltiples mensajes entre el cliente y el servidor para establecer una conexión. Los clientes
 * se conectan a los servidores mediante TCP/IP (en realidad NetBIOS sobre TCP/IP como se especifica en RFC1001 y RFC1002), NetBEUI o IPX/SPX.
 * 
 * Una vez que han establecido una conexión, los clientes pueden enviar comandos (SMB) al servidor que les permiten acceder a recursos compartidos, abrir archivos, leer y escribir archivos y, en general, 
 * hacer todo el tipo de cosas que desea hacer con un sistema de archivos. . Sin embargo, en el caso de SMB, estas cosas se hacen a través de la red.
 * 
 * ¿Qué ejecuta SMB?
 * Los sistemas operativos Microsoft Windows desde Windows 95 han incluido compatibilidad con el protocolo SMB de cliente y servidor. Samba, un servidor de código abierto que admite el protocolo SMB, se lanzó para los sistemas Unix.
 * Enumeración
 * La enumeración es el proceso de recopilación de información sobre un objetivo para encontrar posibles vectores de ataque y ayudar en la explotación.
 * Este proceso es esencial para que un ataque tenga éxito, ya que perder el tiempo con exploits que no funcionan o pueden colapsar el sistema puede ser una pérdida de energía. La enumeración se puede utilizar para recopilar nombres 
 * de usuario, contraseñas, información de red, nombres de host, datos de aplicaciones, servicios o cualquier otra información que pueda ser valiosa para un atacante.
 * 
 * PYME
 * Por lo general, hay unidades compartidas SMB en un servidor que se pueden conectar y usar para ver o transferir archivos. SMB a menudo puede ser un excelente punto de partida para un atacante que busca descubrir información 
 * confidencial: se sorprendería de lo que a veces se incluye en estos recursos compartidos.
 * Escaneo de puertos
 * El primer paso de la enumeración es realizar un escaneo de puertos para obtener la mayor cantidad de información posible sobre los servicios, las aplicaciones, la estructura y el sistema operativo de la máquina de destino.
 * Si aún no ha examinado el escaneo de puertos, le recomiendo que consulte la sala Nmap aquí .
 * 
 * Enum4Linux
 * Enum4linux es una herramienta que se utiliza para enumerar recursos compartidos SMB en sistemas Windows y Linux . Es básicamente un contenedor de las herramientas del paquete Samba y facilita la extracción rápida de información del objetivo perteneciente a SMB. Está instalado de forma predeterminada en Parrot y Kali, sin embargo, si necesita instalarlo, puede hacerlo desde el github oficial .
 * La sintaxis de Enum4Linux es agradable y simple: "enum4linux [opciones] ip"
 *      FUNCIÓN DE ETIQUETA
 * -U obtener lista de usuarios
 * -M obtener lista de máquinas
 * -N obtener volcado de lista de nombres (diferente de -U y -M)
 * -S obtener lista compartida
 * -P obtener información de política de contraseñas
 * -G obtener lista de miembros y grupos
 * -a todo lo anterior (enumeración básica completa)
 * 
 * 
 * ****************************************************
 * ****************************************************
 * ****************     NMAP     ***********************
 * ****************************************************
 * ****************************************************
 * 
 */