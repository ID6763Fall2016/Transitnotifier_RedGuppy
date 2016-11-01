EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L CONN_01X10 P2
U 1 1 580666ED
P 3200 2950
F 0 "P2" H 3200 3500 50  0000 C CNN
F 1 "CONN_01X10" V 3300 2950 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x10" H 3200 2950 50  0001 C CNN
F 3 "" H 3200 2950 50  0000 C CNN
	1    3200 2950
	-1   0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 58066786
P 4000 4100
F 0 "R2" V 4080 4100 50  0000 C CNN
F 1 "R" V 4000 4100 50  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Horizontal_RM10mm" V 3930 4100 50  0001 C CNN
F 3 "" H 4000 4100 50  0000 C CNN
	1    4000 4100
	1    0    0    -1  
$EndComp
Text GLabel 3400 2500 2    60   Input ~ 0
GND
$Comp
L CP1 C1
U 1 1 58066A58
P 6000 2250
F 0 "C1" H 6025 2350 50  0000 L CNN
F 1 "CP1" H 6025 2150 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Radial_D5_L11_P2.5" H 6000 2250 50  0001 C CNN
F 3 "" H 6000 2250 50  0000 C CNN
	1    6000 2250
	1    0    0    -1  
$EndComp
$Comp
L Switch_SPDT_x2 SW2
U 1 1 58066CE8
P 5100 2250
F 0 "SW2" H 4900 2400 50  0000 C CNN
F 1 "Switch_SPDT_x2" H 4850 2100 50  0000 C CNN
F 2 "Buttons_Switches_ThroughHole:SW_NKK_G1xJP" H 5100 2250 50  0001 C CNN
F 3 "" H 5100 2250 50  0000 C CNN
	1    5100 2250
	1    0    0    -1  
$EndComp
$Comp
L Battery BT1
U 1 1 58066F7A
P 4400 2250
F 0 "BT1" H 4500 2300 50  0000 L CNN
F 1 "Battery" H 4500 2200 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02" V 4400 2290 50  0001 C CNN
F 3 "" V 4400 2290 50  0000 C CNN
	1    4400 2250
	0    1    1    0   
$EndComp
Wire Wire Line
	4550 2250 4800 2250
Text GLabel 4250 2250 0    60   Input ~ 0
GND
Text GLabel 6000 2400 3    60   Input ~ 0
GND
Text GLabel 3400 2700 2    60   Input ~ 0
V+
$Comp
L CONN_01X10 P1
U 1 1 58067235
P 2550 2950
F 0 "P1" H 2550 3500 50  0000 C CNN
F 1 "CONN_01X10" V 2650 2950 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x10" H 2550 2950 50  0001 C CNN
F 3 "" H 2550 2950 50  0000 C CNN
	1    2550 2950
	1    0    0    1   
$EndComp
$Comp
L SW_PUSH SW1
U 1 1 580672E6
P 2200 3900
F 0 "SW1" H 2350 4010 50  0000 C CNN
F 1 "SW_PUSH" H 2200 3820 50  0000 C CNN
F 2 "Buttons_Switches_ThroughHole:SW_PUSH_6mm" H 2200 3900 50  0001 C CNN
F 3 "" H 2200 3900 50  0000 C CNN
	1    2200 3900
	1    0    0    -1  
$EndComp
Text GLabel 2350 3400 0    60   Input ~ 0
TX
Text GLabel 2350 3300 0    60   Input ~ 0
RX
Text GLabel 2350 3200 0    60   Input ~ 0
5
Text GLabel 2350 3100 0    60   Input ~ 0
4
Text GLabel 2350 3000 0    60   Input ~ 0
0
Text GLabel 2350 2900 0    60   Input ~ 0
2
Text GLabel 2350 2800 0    60   Input ~ 0
15
Text GLabel 2350 2700 0    60   Input ~ 0
V33
Text GLabel 2350 2600 0    60   Input ~ 0
LDD
Text GLabel 2350 2500 0    60   Input ~ 0
GND
Text GLabel 3400 2600 2    60   Input ~ 0
VBat
Text GLabel 3400 2800 2    60   Input ~ 0
13
Text GLabel 3400 2900 2    60   Input ~ 0
12
Text GLabel 3400 3000 2    60   Input ~ 0
14
Text GLabel 3400 3100 2    60   Input ~ 0
16
Text GLabel 3400 3200 2    60   Input ~ 0
EN
Text GLabel 3400 3300 2    60   Input ~ 0
A
Text GLabel 3400 3400 2    60   Input ~ 0
RST
Text GLabel 4000 3950 1    60   Input ~ 0
14
Text GLabel 2600 3900 2    60   Input ~ 0
15
Text GLabel 4100 4250 1    60   Input ~ 0
GND
Text GLabel 2550 4250 3    60   Input ~ 0
GND
$Comp
L R R1
U 1 1 580685CF
P 2550 4100
F 0 "R1" V 2630 4100 50  0000 C CNN
F 1 "R" V 2550 4100 50  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Horizontal_RM10mm" V 2480 4100 50  0001 C CNN
F 3 "" H 2550 4100 50  0000 C CNN
	1    2550 4100
	1    0    0    -1  
$EndComp
Text GLabel 1900 3900 0    60   Input ~ 0
V33
Wire Wire Line
	2500 3900 2600 3900
Wire Wire Line
	2550 3950 2550 3900
Connection ~ 2550 3900
$Comp
L CONN_01X03 P3
U 1 1 5806943E
P 4000 4450
F 0 "P3" H 4000 4650 50  0000 C CNN
F 1 "CONN_01X03" V 4100 4450 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03" H 4000 4450 50  0001 C CNN
F 3 "" H 4000 4450 50  0000 C CNN
	1    4000 4450
	0    1    1    0   
$EndComp
Text GLabel 5750 2100 1    60   Input ~ 0
VBat
Wire Wire Line
	5400 2150 5900 2150
Wire Wire Line
	5750 2150 5750 2100
Wire Wire Line
	5900 2150 5900 2100
Wire Wire Line
	5900 2100 6000 2100
Connection ~ 5750 2150
Text GLabel 3900 4250 1    60   Input ~ 0
VBat
$EndSCHEMATC
