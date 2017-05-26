(ns choreo-tester.core
  (:use [serial.core]
        [serial.util])
  (:import [java.nio ByteBuffer]))

(def s (open "/dev/ttyACM0" :baud-rate 9600))


(def ex [
         0x02 ;number of motors
         0x00 ;motor id
         0x04 ;num-commands
         0x00 ;command id
         0x00 0x14 ;amount0
         0x03 0xE8 ;duration0
         0x01 ;command id
         0xFF 0xEC ;amount1
         0x03 0xE8 ;duration1
         0x02 ;command id
         0x00 0x14 ;amount0
         0x03 0xE8 ;duration0
         0x03 ;command id
         0xFF 0xEC ;amount1
         0x03 0xE8 ;duration1
         0x01 ;motor id
         0x04 ;num-commands
         0x04 ;command id
         0x00 0x14 ;amount0
         0x03 0xE8 ;duration0
         0x05 ;command id
         0xFF 0xEC ;amount1
         0x03 0xE8 ;duration1
         0x06 ;command id
         0x00 0x14 ;amount0
         0x03 0xE8 ;duration0
         0x07 ;command id
         0xFF 0xEC ;amount1
         0x03 0xE8 ;duration1
         ])


(defn -main
  []
  (write s ex))
