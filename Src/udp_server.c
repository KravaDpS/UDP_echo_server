#include "udp_server.h"

static uint8_t rx_buffer[BUFF_SIZE];
static uint8_t tx_buffer[BUFF_SIZE];

void udp_server_init(void)
{
	///< пример инициализации порта udp для работы сервера
	struct udp_pcb *upcb;
	err_t err;
	upcb = udp_new();
    if (upcb == NULL) {
    	return;
	}
	err = udp_bind(upcb, IP_ADDR_ANY, VK_UDP_PORT);
	if (err != ERR_OK) {
		udp_remove(upcb);
	    return;
	}
	udp_recv(upcb, udp_server_recv_callback, NULL);
}


void udp_server_recv_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
	///< пример обработки принятого сообщения и отправки ответа
	if(p != NULL){
		uint16_t rx_len = p->tot_len;
        uint16_t copy_rx_len = (rx_len > BUFF_SIZE) ? BUFF_SIZE : rx_len;
        ///< копирование принятых данных в локальный буфер
        pbuf_take(p, rx_buffer, copy_rx_len);

		///< место для вставки обработчика принимаемых сообщений

		struct pbuf *p_out = pbuf_alloc(PBUF_TRANSPORT, BUFF_SIZE, PBUF_RAM);
		if (p_out != NULL) {
			///< копирование ответного сообщения в структуру LwIP
			pbuf_take(p_out, tx_buffer, BUFF_SIZE);
			///< отправка ответного сообщение клиенту
			udp_sendto(upcb, p_out, addr, port);
			pbuf_free(p_out);
		}
	}
	pbuf_free(p);
}
