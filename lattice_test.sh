#Summe : 210
#TCP example
#sudo hping3 -V -S -p 9992 -c 10 172.18.0.3

#UDP example
#sudo hping3 -V --udp -p 9991 -c 10 172.18.0.4

SLAVE_1=172.19.0.3
SLAVE_2=172.19.0.5
SLAVE_3=172.19.0.6
SLAVE_4=172.19.0.4

#85x 60.240.134.94:4313:1434:UDP
# SLAVE-1
sudo hping3 -V --udp -s 4313 -a 60.240.134.94 -k -p 9991 -c 85 $SLAVE_1

#5x 60.240.134.94:4313:1434:TCP
# SLAVE-4
sudo hping3 -V -S -p 9991 -s 4313 -a 60.240.134.94 -k -c 5 $SLAVE_4

#5x	60.240.134.94:4313:3276:UDP
# SLAVE-3
sudo hping3 -V --udp -s 4313 -k -a 60.240.134.94 -p 9992 -c 5 $SLAVE_3

#5x 60.240.134.94:4313:3276:TCP
# SLAVE-2
sudo hping3 -V -S -p 9992 -s 4313 -a 60.240.134.94 -k -c 5 $SLAVE_2

#65x	60.240.134.94:2771:1434:UDP
# SLAVE-1
sudo hping3 -V --udp -s 2771 -k -a 60.240.134.94 -p 9991 -c 65 $SLAVE_1

#5x	60.240.134.94:2771:1434:TCP
# SLAVE-4
sudo hping3 -V -S -p 9991 -s 2771 -a 60.240.134.94 -k -c 5 $SLAVE_4

#35x 60.240.134.94:2771:3276:UDP
# SLAVE-3
sudo hping3 -V --udp -s 2771 -k -a 60.240.134.94 -p 9992 -c 35 $SLAVE_3

#5x	60.240.134.94:2771:3276:TCP
# SLAVE-2
sudo hping3 -V -S -p 9992 -s 2771 -a 60.240.134.94 -k -c 5 $SLAVE_2

echo "result should be: W32/Blast worm\n"
