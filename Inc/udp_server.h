#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <string.h>
#include <stdint.h>

#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/timeouts.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "ethernetif.h"
#include "app_ethernet.h"

#define VK_UDP_PORT 4001 	///< Номер udp-порта
#define BUFF_SIZE 1500		///< Максимальный размер буфера данных

/**
 * @brief Инициализация UDP сервера
 * @details Создаёт и настраивает UDP PCB
 * Привязывает PCB к любому IP-адресу и регистрирует функцию обратного вызова для обработки входящих данных.
 * @param None
 * @retval None
 */
void udp_server_init(void);

/**
 * @brief Обратный вызов для обработки входящих UDP-пакетов
 * @details Шаблон для обработки входящих пакетов и отправки ответа клиенту.
 * @param arg Указатель на пользовательские данные
 * @param upcb Указатель на структуру UDP PCB
 * @param p Указатель на буфер с входящими данными
 * @param addr IP-адрес отправителя
 * @param port Порт отправителя
 * @retval None
 */
void udp_server_recv_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, uint16_t port);

#endif
