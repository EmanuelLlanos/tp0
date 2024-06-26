#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */
	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Hola soy un log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	valor = config_get_string_value(config, "CLAVE");
	puerto = config_get_string_value(config, "PUERTO");
	ip = config_get_string_value(config, "IP");

	// Loggeamos el valor de config
	log_info(logger,"%s\n", valor);
	log_info(logger,"%s\n", puerto);
	log_info(logger,"%s\n", ip);


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	/* se crea el logger que va a escribir en el archivo "tp0.log", 
	el nombre del program main "TP0", consola activa (1) y desde el nivel de seguridad "LOG_LEVEL_INFO"*/
	t_log* nuevo_logger;
	nuevo_logger = log_create("tp0.log", "Instancia logger", true, LOG_LEVEL_INFO);

	if(nuevo_logger==NULL){
		printf("NO se puede crear el logger \n");
		exit(-1);
	}
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	nuevo_config = config_create("cliente.config");

	if(nuevo_config == NULL){
		printf("NO se pudo leer el config \n");
		exit(-1);
	}

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while (1){
		if (strcmp(leido, "")== 0){
			break;
		}
		log_info(logger, leido);

		free(leido);
		leido = readline("> ");
	}
	// ¡No te olvides de liberar las lineas antes de regresar!

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido = readline("> ");
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	while (1){
		if (strcmp(leido, "")== 0){
			break;
		}
		agregar_a_paquete(paquete, leido, strlen(leido)+1);
		free(leido);
		leido = readline("> ");
	}	

	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */

	// si se tienen, se destruye
	if(logger!=NULL){
		log_destroy(logger);
	}
	if(config!=NULL){
		config_destroy(config);
	}

	liberar_conexion(conexion);
}
