while true; \
do mosquitto_pub -h 192.168.0.81 -t "hall/pair/0" -p 1660 -m 1;
# sleep .1;
mosquitto_pub -h 192.168.0.81 -t "hall/pair/0" -p 1660 -m 0;
# sleep .1;
done;
