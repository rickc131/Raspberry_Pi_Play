# Raspberry_Pi_4b_mqtt_client_master

## Instruction

```bash
# compile
gcc cJSON.c clientmian.c -o client -lpaho-mqtt3c -lpaho-mqtt3a -lwiringPi
# run
./client tcp://ip:port clientid topic timeout

# example
./client tcp://127.0.0.1:1883 id_1 topic_1 300
```

## Environment

### WiringPi

```bash
git clone https://github.com/WiringPi/WiringPi.git
cd WiringPi
# 安装
./build
# 卸载
./build uninstall
```

## mosquitto

```bash
sudo apt-add-repository ppa:mosquitto-dev/mosquitto-ppa
sudo apt-get install mosquitto -y
sudo apt-get install mosquitto-clients -y
```

## paho.mqtt.c

```bash
sudo apt install cmake
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
cmake .
make
make install
```

