
echo 'NEW_CLIENT 622 18' | mosquitto_pub -h $1 -t banker -s
sleep 0.2
echo 'NEW_CLIENT 12 3' | mosquitto_pub -h $1 -t banker -s
sleep 0.2
echo 'LEND 18 5' | mosquitto_pub -h $1 -t banker -s
sleep 0.2
echo 'LEND 18 5' | mosquitto_pub -h $1 -t banker -s
sleep 0.2
echo 'LEND 3 3' | mosquitto_pub -h $1 -t banker -s
sleep 0.2
echo 'LEND 18 3' | mosquitto_pub -h $1 -t banker -s
sleep 0.2
echo 'LEND 3 5' | mosquitto_pub -h $1 -t banker -s

