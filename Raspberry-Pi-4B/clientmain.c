#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cJSON.h"
#include "MQTTClient.h"
#include "All.c"

//gcc cJSON.c clientmain.c -o publish -lpaho-mqtt3c -lpaho-mqtt3a -lwiringPi
#define QOS         1
#define TIMEOUT     0

char *inf_payload;

int main(int argc, char *argv[]) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;
    if (argc < 5)
    //帮助
        printf("example: ./client tcp://ip:port clientid topic timeout(s)\n");
    else {
        for (int i = 0; i < argc; ++i) {
            printf("%s ", argv[i]);
        }
        printf("\n");
        while (1) {
            cJSON *inf = NULL;
            cJSON *tsuiot = NULL;
            cJSON *data = NULL;
            cJSON *datainfos = NULL;
            cJSON *datainfo = NULL;
            cJSON *tmp = NULL;
            cJSON *hum = NULL;
            cJSON *ill = NULL;

            char *str = NULL;
            char hUm[] = {0};
            char tMp[] = {0};
            char iLl[] = {0};
            illuminance(iLl);
            tmpAndhum(hUm, tMp);

            /* 创建一个JSON数据对象 */
            //inf = cJSON_CreateObject();
            //tsuiot = cJSON_CreateObject();
            //cJSON_AddStringToObject(tsuiot, "RESULT", "S（成功）或者E（失败）");
            //cJSON_AddStringToObject(tsuiot, "DESC", "数据处理情况的描述");
            //data = cJSON_CreateObject();
            datainfos = cJSON_CreateObject();
            cJSON_AddStringToObject(datainfos, "DeviceCode", "0100000121");
            datainfo = cJSON_CreateArray();
            //温度
            tmp = cJSON_CreateObject();
            cJSON_AddStringToObject(tmp, "SensorCode", "1100000284");
            cJSON_AddStringToObject(tmp, "EnvirCode", "101");
            cJSON_AddStringToObject(tmp, "SensorType", "raspi01");
            cJSON_AddStringToObject(tmp, "NodeCode", "1000000084");
            cJSON_AddStringToObject(tmp, "DataValue", hUm);

            cJSON_AddItemToObject(datainfo, "", tmp);
            //湿度
            hum = cJSON_CreateObject();
            cJSON_AddStringToObject(hum, "SensorCode", "1100000284");
            cJSON_AddStringToObject(hum, "EnvirCode", "102");
            cJSON_AddStringToObject(hum, "SensorType", "raspi01");
            cJSON_AddStringToObject(hum, "NodeCode", "1000000084");
            cJSON_AddStringToObject(hum, "DataValue", tMp);

            cJSON_AddItemToObject(datainfo, "", hum);
            //光照强度
            ill = cJSON_CreateObject();
            cJSON_AddStringToObject(ill, "SensorCode", "1100000790");
            cJSON_AddStringToObject(ill, "EnvirCode", "103");
            cJSON_AddStringToObject(ill, "SensorType", "raspi01");
            cJSON_AddStringToObject(ill, "NodeCode", "1000000084");
            cJSON_AddStringToObject(ill, "DataValue", iLl);

            cJSON_AddItemToObject(datainfo, "", ill);

            cJSON_AddItemToObject(datainfos, "DATAINFO", datainfo);
            //cJSON_AddItemToObject(data, "DATAINFOS", datainfos);
            //cJSON_AddItemToObject(tsuiot, "DATA", data);
            //cJSON_AddItemToObject(inf, "TSUIOT", tsuiot);
            //str = cJSON_Print(inf);

            inf_payload = cJSON_Print(datainfos);

            if ((rc = MQTTClient_create(&client, argv[1], argv[2],
                                        MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS) {
                printf("Failed to create client, return code %d\n", rc);
                exit(EXIT_FAILURE);
            }

            conn_opts.keepAliveInterval = 20;
            conn_opts.cleansession = 1;
            //conn_opts.username = argv[4];
            //conn_opts.password = argv[5];
            if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
                printf("Failed to connect, return code %d\n", rc);
                exit(EXIT_FAILURE);
            }

            pubmsg.payload = inf_payload;
            pubmsg.payloadlen = (int) strlen(inf_payload);
            pubmsg.qos = QOS;
            pubmsg.retained = 0;

            //判断是否上传成功
            if ((rc = MQTTClient_publishMessage(client, argv[3], &pubmsg, &token)) != MQTTCLIENT_SUCCESS) {
                printf("Failed to publish message, return code %d\n", rc);
                exit(EXIT_FAILURE);
            }
            rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
            printf("Message with delivery token %d delivered\n", token);
            sleep(atoi(argv[4]));

        }
    }
}



