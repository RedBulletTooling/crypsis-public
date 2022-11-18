EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 4
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 7150 6950 0    129  ~ 0
Powermanagement  & Interface
Text Notes 7150 6950 0    129  ~ 0
Powermanagement  & Interface
$Comp
L SamacSys_Parts:UP2-AH-1-TH USBMALE1
U 1 1 5ED705D9
P 2250 3500
F 0 "USBMALE1" H 2650 3765 50  0000 C CNN
F 1 "UP2-AH-1-TH" H 2650 3674 50  0000 C CNN
F 2 "SamacSys_Parts:UP2-AH-1-TH" H 2900 3600 50  0001 L CNN
F 3 "http://www.cui.com/product/resource/up2-ah-th.pdf" H 2900 3500 50  0001 L CNN
F 4 "USB Connectors USB 2.0 type A plug 4 pin Horizontal TH" H 2900 3400 50  0001 L CNN "Description"
F 5 "" H 2900 3300 50  0001 L CNN "Height"
F 6 "490-UP2-AH-1-TH" H 2900 3200 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/CUI-Devices/UP2-AH-1-TH?qs=5mqXD9RfOg0slNuu3mCk4w%3D%3D" H 2900 3100 50  0001 L CNN "Mouser Price/Stock"
F 8 "CUI Devices" H 2900 3000 50  0001 L CNN "Manufacturer_Name"
F 9 "UP2-AH-1-TH" H 2900 2900 50  0001 L CNN "Manufacturer_Part_Number"
	1    2250 3500
	1    0    0    -1  
$EndComp
$Comp
L SamacSys_Parts:GRM033C80J105ME05D 1uF5
U 1 1 5ED8EA3D
P 8250 5300
F 0 "1uF5" H 8500 5565 50  0000 C CNN
F 1 "GRM033C80J105ME05D" H 8500 5474 50  0000 C CNN
F 2 "SamacSys_Parts:CAPC0603X33N" H 8600 5350 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/1/GRM033C80J105ME05D.pdf" H 8600 5250 50  0001 L CNN
F 4 "Multilayer Ceramic Capacitors MLCC - SMD/SMT" H 8600 5150 50  0001 L CNN "Description"
F 5 "0.33" H 8600 5050 50  0001 L CNN "Height"
F 6 "81-GRM033C80J105ME5D" H 8600 4950 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.com/Search/Refine.aspx?Keyword=81-GRM033C80J105ME5D" H 8600 4850 50  0001 L CNN "Mouser Price/Stock"
F 8 "Murata Electronics" H 8600 4750 50  0001 L CNN "Manufacturer_Name"
F 9 "GRM033C80J105ME05D" H 8600 4650 50  0001 L CNN "Manufacturer_Part_Number"
	1    8250 5300
	0    1    -1   0   
$EndComp
$Comp
L SamacSys_Parts:GRM033C80J105ME05D 1uF4
U 1 1 5ED9155B
P 5800 5300
F 0 "1uF4" H 6050 5565 50  0000 C CNN
F 1 "GRM033C80J105ME05D" H 6050 5474 50  0000 C CNN
F 2 "SamacSys_Parts:CAPC0603X33N" H 6150 5350 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/1/GRM033C80J105ME05D.pdf" H 6150 5250 50  0001 L CNN
F 4 "Multilayer Ceramic Capacitors MLCC - SMD/SMT" H 6150 5150 50  0001 L CNN "Description"
F 5 "0.33" H 6150 5050 50  0001 L CNN "Height"
F 6 "81-GRM033C80J105ME5D" H 6150 4950 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.com/Search/Refine.aspx?Keyword=81-GRM033C80J105ME5D" H 6150 4850 50  0001 L CNN "Mouser Price/Stock"
F 8 "Murata Electronics" H 6150 4750 50  0001 L CNN "Manufacturer_Name"
F 9 "GRM033C80J105ME05D" H 6150 4650 50  0001 L CNN "Manufacturer_Part_Number"
	1    5800 5300
	0    -1   -1   0   
$EndComp
Text GLabel 2100 3600 0    50   Input ~ 0
D-AT
Wire Wire Line
	2250 3500 2100 3500
Wire Wire Line
	2100 3600 2250 3600
Text GLabel 3200 3500 2    50   Input ~ 0
D+AT
$Comp
L power:+5V #PWR039
U 1 1 5EE18ABF
P 2100 3500
F 0 "#PWR039" H 2100 3350 50  0001 C CNN
F 1 "+5V" H 2115 3673 50  0000 C CNN
F 2 "" H 2100 3500 50  0001 C CNN
F 3 "" H 2100 3500 50  0001 C CNN
	1    2100 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 3500 3050 3500
$Comp
L power:GND #PWR041
U 1 1 5EE1A6AF
P 3200 3600
F 0 "#PWR041" H 3200 3350 50  0001 C CNN
F 1 "GND" H 3205 3427 50  0000 C CNN
F 2 "" H 3200 3600 50  0001 C CNN
F 3 "" H 3200 3600 50  0001 C CNN
	1    3200 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 3600 3200 3600
$Comp
L power:GND #PWR057
U 1 1 5EE3439D
P 8250 5400
F 0 "#PWR057" H 8250 5150 50  0001 C CNN
F 1 "GND" H 8255 5227 50  0000 C CNN
F 2 "" H 8250 5400 50  0001 C CNN
F 3 "" H 8250 5400 50  0001 C CNN
	1    8250 5400
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8250 5300 8250 5400
Wire Wire Line
	6450 4800 5800 4800
Wire Wire Line
	5800 4800 5800 4550
Connection ~ 5800 4800
$Comp
L power:GND #PWR049
U 1 1 5EE37F05
P 5800 5850
F 0 "#PWR049" H 5800 5600 50  0001 C CNN
F 1 "GND" H 5805 5677 50  0000 C CNN
F 2 "" H 5800 5850 50  0001 C CNN
F 3 "" H 5800 5850 50  0001 C CNN
	1    5800 5850
	-1   0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR048
U 1 1 5EE38D5F
P 5800 4550
F 0 "#PWR048" H 5800 4400 50  0001 C CNN
F 1 "+3V3" H 5815 4723 50  0000 C CNN
F 2 "" H 5800 4550 50  0001 C CNN
F 3 "" H 5800 4550 50  0001 C CNN
	1    5800 4550
	-1   0    0    -1  
$EndComp
$Comp
L power:+5V #PWR042
U 1 1 5EE3286A
P 3400 5250
F 0 "#PWR042" H 3400 5100 50  0001 C CNN
F 1 "+5V" H 3415 5423 50  0000 C CNN
F 2 "" H 3400 5250 50  0001 C CNN
F 3 "" H 3400 5250 50  0001 C CNN
	1    3400 5250
	-1   0    0    1   
$EndComp
Wire Wire Line
	3400 5100 3400 5250
Wire Wire Line
	3300 5100 3400 5100
Wire Wire Line
	1850 5450 1850 5650
Connection ~ 1850 5450
Wire Wire Line
	1850 5450 2300 5450
$Comp
L power:GND #PWR040
U 1 1 5EE299D3
P 2800 5600
F 0 "#PWR040" H 2800 5350 50  0001 C CNN
F 1 "GND" H 2805 5427 50  0000 C CNN
F 2 "" H 2800 5600 50  0001 C CNN
F 3 "" H 2800 5600 50  0001 C CNN
	1    2800 5600
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR037
U 1 1 5EE27ABC
P 1850 4650
F 0 "#PWR037" H 1850 4500 50  0001 C CNN
F 1 "VCC" H 1865 4823 50  0000 C CNN
F 2 "" H 1850 4650 50  0001 C CNN
F 3 "" H 1850 4650 50  0001 C CNN
	1    1850 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 5450 3850 5650
Connection ~ 3850 5450
Wire Wire Line
	3700 5450 3850 5450
Wire Wire Line
	3700 5000 3700 5450
Wire Wire Line
	3300 5000 3700 5000
$Comp
L power:+5V #PWR043
U 1 1 5EE26462
P 3850 4650
F 0 "#PWR043" H 3850 4500 50  0001 C CNN
F 1 "+5V" H 3865 4823 50  0000 C CNN
F 2 "" H 3850 4650 50  0001 C CNN
F 3 "" H 3850 4650 50  0001 C CNN
	1    3850 4650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR044
U 1 1 5EE262CF
P 3850 5650
F 0 "#PWR044" H 3850 5400 50  0001 C CNN
F 1 "GND" H 3855 5477 50  0000 C CNN
F 2 "" H 3850 5650 50  0001 C CNN
F 3 "" H 3850 5650 50  0001 C CNN
	1    3850 5650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR038
U 1 1 5EE26025
P 1850 5650
F 0 "#PWR038" H 1850 5400 50  0001 C CNN
F 1 "GND" H 1855 5477 50  0000 C CNN
F 2 "" H 1850 5650 50  0001 C CNN
F 3 "" H 1850 5650 50  0001 C CNN
	1    1850 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 5400 3850 5450
Wire Wire Line
	1850 5400 1850 5450
Connection ~ 3850 4900
Wire Wire Line
	3850 4900 3850 4650
Wire Wire Line
	3300 4900 3850 4900
Connection ~ 1850 4900
Wire Wire Line
	1850 4650 1850 4900
Wire Wire Line
	1850 4900 2300 4900
$Comp
L SamacSys_Parts:GRM033C80J105ME05D 1uF2
U 1 1 5ED8E4B0
P 1850 5400
F 0 "1uF2" H 2100 5665 50  0000 C CNN
F 1 "GRM033C80J105ME05D" H 2100 5574 50  0000 C CNN
F 2 "SamacSys_Parts:CAPC0603X33N" H 2200 5450 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/1/GRM033C80J105ME05D.pdf" H 2200 5350 50  0001 L CNN
F 4 "Multilayer Ceramic Capacitors MLCC - SMD/SMT" H 2200 5250 50  0001 L CNN "Description"
F 5 "0.33" H 2200 5150 50  0001 L CNN "Height"
F 6 "81-GRM033C80J105ME5D" H 2200 5050 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.com/Search/Refine.aspx?Keyword=81-GRM033C80J105ME5D" H 2200 4950 50  0001 L CNN "Mouser Price/Stock"
F 8 "Murata Electronics" H 2200 4850 50  0001 L CNN "Manufacturer_Name"
F 9 "GRM033C80J105ME05D" H 2200 4750 50  0001 L CNN "Manufacturer_Part_Number"
	1    1850 5400
	0    -1   -1   0   
$EndComp
$Comp
L SamacSys_Parts:GRM033C80J105ME05D 1uF3
U 1 1 5ED8BFC1
P 3850 5400
F 0 "1uF3" H 4100 5665 50  0000 C CNN
F 1 "GRM033C80J105ME05D" H 4100 5574 50  0000 C CNN
F 2 "SamacSys_Parts:CAPC0603X33N" H 4200 5450 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/1/GRM033C80J105ME05D.pdf" H 4200 5350 50  0001 L CNN
F 4 "Multilayer Ceramic Capacitors MLCC - SMD/SMT" H 4200 5250 50  0001 L CNN "Description"
F 5 "0.33" H 4200 5150 50  0001 L CNN "Height"
F 6 "81-GRM033C80J105ME5D" H 4200 5050 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.com/Search/Refine.aspx?Keyword=81-GRM033C80J105ME5D" H 4200 4950 50  0001 L CNN "Mouser Price/Stock"
F 8 "Murata Electronics" H 4200 4850 50  0001 L CNN "Manufacturer_Name"
F 9 "GRM033C80J105ME05D" H 4200 4750 50  0001 L CNN "Manufacturer_Part_Number"
	1    3850 5400
	0    1    -1   0   
$EndComp
Connection ~ 8250 4800
Connection ~ 7300 3400
Wire Wire Line
	7300 3300 7300 3400
$Comp
L power:GND #PWR054
U 1 1 5EE3D836
P 7300 3400
F 0 "#PWR054" H 7300 3150 50  0001 C CNN
F 1 "GND" H 7305 3227 50  0000 C CNN
F 2 "" H 7300 3400 50  0001 C CNN
F 3 "" H 7300 3400 50  0001 C CNN
	1    7300 3400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR051
U 1 1 5EE3CD96
P 6300 3600
F 0 "#PWR051" H 6300 3350 50  0001 C CNN
F 1 "GND" H 6305 3427 50  0000 C CNN
F 2 "" H 6300 3600 50  0001 C CNN
F 3 "" H 6300 3600 50  0001 C CNN
	1    6300 3600
	1    0    0    -1  
$EndComp
Text GLabel 6300 3500 0    50   Input ~ 0
D+US
Text GLabel 6300 3400 0    50   Input ~ 0
D-US
Wire Wire Line
	6400 3500 6300 3500
Wire Wire Line
	6400 3400 6300 3400
Wire Wire Line
	6400 3300 6300 3300
Wire Wire Line
	6400 3600 6300 3600
$Comp
L power:VCC #PWR056
U 1 1 5EE354E2
P 8250 4550
F 0 "#PWR056" H 8250 4400 50  0001 C CNN
F 1 "VCC" H 8265 4723 50  0000 C CNN
F 2 "" H 8250 4550 50  0001 C CNN
F 3 "" H 8250 4550 50  0001 C CNN
	1    8250 4550
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8250 4800 8250 4550
$Comp
L SamacSys_Parts:87583-2010RLF USBFEMALE1
U 1 1 5ED6F327
P 7300 3300
F 0 "USBFEMALE1" H 7750 3565 50  0000 C CNN
F 1 "87583-2010RLF" H 7750 3474 50  0000 C CNN
F 2 "SamacSys_Parts:875832010RLF" H 8050 3400 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/1/87583-2010RLF.pdf" H 8050 3300 50  0001 L CNN
F 4 "Amphenol FCI, 87583 Series Number SMT, Version 2.0 Type A USB Connector, Receptacle" H 8050 3200 50  0001 L CNN "Description"
F 5 "7" H 8050 3100 50  0001 L CNN "Height"
F 6 "649-87583-2010RLF" H 8050 3000 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/Amphenol-FCI/87583-2010RLF?qs=nBv%252BRDKgYU3votcI4YYBSA%3D%3D" H 8050 2900 50  0001 L CNN "Mouser Price/Stock"
F 8 "Amphenol" H 8050 2800 50  0001 L CNN "Manufacturer_Name"
F 9 "87583-2010RLF" H 8050 2700 50  0001 L CNN "Manufacturer_Part_Number"
	1    7300 3300
	-1   0    0    -1  
$EndComp
$Comp
L SamacSys_Parts:EC14-2AP U1
U 1 1 5F0545BE
P 4750 1350
F 0 "U1" H 5069 736 50  0000 L CNN
F 1 "EC14-2AP" H 5069 645 50  0000 L CNN
F 2 "SamacSys_Parts:EC14-2AP" H 4700 1550 50  0001 C CNN
F 3 "" H 4700 1550 50  0001 C CNN
	1    4750 1350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR046
U 1 1 5F0545C4
P 4350 2650
F 0 "#PWR046" H 4350 2400 50  0001 C CNN
F 1 "GND" H 4355 2477 50  0000 C CNN
F 2 "" H 4350 2650 50  0001 C CNN
F 3 "" H 4350 2650 50  0001 C CNN
	1    4350 2650
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR045
U 1 1 5F0545CA
P 4250 1450
F 0 "#PWR045" H 4250 1300 50  0001 C CNN
F 1 "+3V3" H 4265 1623 50  0000 C CNN
F 2 "" H 4250 1450 50  0001 C CNN
F 3 "" H 4250 1450 50  0001 C CNN
	1    4250 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 1350 4450 1350
Wire Wire Line
	4250 1450 4550 1450
$Comp
L power:VCC #PWR047
U 1 1 5F0545D2
P 4450 1350
F 0 "#PWR047" H 4450 1200 50  0001 C CNN
F 1 "VCC" H 4465 1523 50  0000 C CNN
F 2 "" H 4450 1350 50  0001 C CNN
F 3 "" H 4450 1350 50  0001 C CNN
	1    4450 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 2650 4550 2650
Text GLabel 4450 1550 0    50   Input ~ 0
SCLK
Text GLabel 4450 1650 0    50   Input ~ 0
MOSI
Text GLabel 4450 1750 0    50   Input ~ 0
MISO
Wire Wire Line
	4450 1550 4550 1550
Wire Wire Line
	4550 1650 4450 1650
Wire Wire Line
	4450 1750 4550 1750
Wire Wire Line
	4550 1850 4450 1850
Text GLabel 4450 1850 0    50   Input ~ 0
RST
$Comp
L SamacSys_Parts:4-103327-3 GND1
U 1 1 5F064F72
P 6450 2750
F 0 "GND1" H 6742 3015 50  0000 C CNN
F 1 "4-103327-3" H 6742 2924 50  0000 C CNN
F 2 "HDRV1W66P0X254_1X1_213X234X813P" H 7100 2850 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/1/4-103327-3.pdf" H 7100 2750 50  0001 L CNN
F 4 "Headers & Wire Housings 01 MODII HDR SRST B/A .100CL" H 7100 2650 50  0001 L CNN "Description"
F 5 "8.128" H 7100 2550 50  0001 L CNN "Height"
F 6 "571-4-103327-3" H 7100 2450 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/TE-Connectivity-AMP/4-103327-3?qs=A0DvvJjHse9xDwPYfNPp%2FQ%3D%3D" H 7100 2350 50  0001 L CNN "Mouser Price/Stock"
F 8 "TE Connectivity" H 7100 2250 50  0001 L CNN "Manufacturer_Name"
F 9 "4-103327-3" H 7100 2150 50  0001 L CNN "Manufacturer_Part_Number"
	1    6450 2750
	1    0    0    -1  
$EndComp
$Comp
L SamacSys_Parts:4-103327-3 5V+1
U 1 1 5F06705F
P 6450 2300
F 0 "5V+1" H 6742 2565 50  0000 C CNN
F 1 "4-103327-3" H 6742 2474 50  0000 C CNN
F 2 "HDRV1W66P0X254_1X1_213X234X813P" H 7100 2400 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/1/4-103327-3.pdf" H 7100 2300 50  0001 L CNN
F 4 "Headers & Wire Housings 01 MODII HDR SRST B/A .100CL" H 7100 2200 50  0001 L CNN "Description"
F 5 "8.128" H 7100 2100 50  0001 L CNN "Height"
F 6 "571-4-103327-3" H 7100 2000 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/TE-Connectivity-AMP/4-103327-3?qs=A0DvvJjHse9xDwPYfNPp%2FQ%3D%3D" H 7100 1900 50  0001 L CNN "Mouser Price/Stock"
F 8 "TE Connectivity" H 7100 1800 50  0001 L CNN "Manufacturer_Name"
F 9 "4-103327-3" H 7100 1700 50  0001 L CNN "Manufacturer_Part_Number"
	1    6450 2300
	1    0    0    -1  
$EndComp
$Comp
L SamacSys_Parts:4-103327-3 TX1
U 1 1 5F068E82
P 6450 1400
F 0 "TX1" H 6742 1665 50  0000 C CNN
F 1 "4-103327-3" H 6742 1574 50  0000 C CNN
F 2 "HDRV1W66P0X254_1X1_213X234X813P" H 7100 1500 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/1/4-103327-3.pdf" H 7100 1400 50  0001 L CNN
F 4 "Headers & Wire Housings 01 MODII HDR SRST B/A .100CL" H 7100 1300 50  0001 L CNN "Description"
F 5 "8.128" H 7100 1200 50  0001 L CNN "Height"
F 6 "571-4-103327-3" H 7100 1100 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/TE-Connectivity-AMP/4-103327-3?qs=A0DvvJjHse9xDwPYfNPp%2FQ%3D%3D" H 7100 1000 50  0001 L CNN "Mouser Price/Stock"
F 8 "TE Connectivity" H 7100 900 50  0001 L CNN "Manufacturer_Name"
F 9 "4-103327-3" H 7100 800 50  0001 L CNN "Manufacturer_Part_Number"
	1    6450 1400
	1    0    0    -1  
$EndComp
$Comp
L SamacSys_Parts:4-103327-3 RX1
U 1 1 5F06801F
P 6450 1850
F 0 "RX1" H 6742 2115 50  0000 C CNN
F 1 "4-103327-3" H 6742 2024 50  0000 C CNN
F 2 "HDRV1W66P0X254_1X1_213X234X813P" H 7100 1950 50  0001 L CNN
F 3 "https://componentsearchengine.com/Datasheets/1/4-103327-3.pdf" H 7100 1850 50  0001 L CNN
F 4 "Headers & Wire Housings 01 MODII HDR SRST B/A .100CL" H 7100 1750 50  0001 L CNN "Description"
F 5 "8.128" H 7100 1650 50  0001 L CNN "Height"
F 6 "571-4-103327-3" H 7100 1550 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/TE-Connectivity-AMP/4-103327-3?qs=A0DvvJjHse9xDwPYfNPp%2FQ%3D%3D" H 7100 1450 50  0001 L CNN "Mouser Price/Stock"
F 8 "TE Connectivity" H 7100 1350 50  0001 L CNN "Manufacturer_Name"
F 9 "4-103327-3" H 7100 1250 50  0001 L CNN "Manufacturer_Part_Number"
	1    6450 1850
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR053
U 1 1 5F076502
P 6400 2300
F 0 "#PWR053" H 6400 2150 50  0001 C CNN
F 1 "+5V" V 6415 2428 50  0000 L CNN
F 2 "" H 6400 2300 50  0001 C CNN
F 3 "" H 6400 2300 50  0001 C CNN
	1    6400 2300
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR052
U 1 1 5F076E04
P 6350 2750
F 0 "#PWR052" H 6350 2500 50  0001 C CNN
F 1 "GND" V 6355 2622 50  0000 R CNN
F 2 "" H 6350 2750 50  0001 C CNN
F 3 "" H 6350 2750 50  0001 C CNN
	1    6350 2750
	0    1    1    0   
$EndComp
Wire Wire Line
	6400 2300 6450 2300
Wire Wire Line
	6450 2750 6350 2750
Text GLabel 6250 1850 0    50   Input ~ 0
NRFRX
Text GLabel 6250 1400 0    50   Input ~ 0
NRFTX
Wire Wire Line
	6250 1400 6450 1400
Wire Wire Line
	6450 1850 6250 1850
Text GLabel 4450 1950 0    50   Input ~ 0
RXUSBH-PGRDATA
Text GLabel 4450 2050 0    50   Input ~ 0
TXUSBH-PGRCLK
Text GLabel 4450 2150 0    50   Input ~ 0
MCLR
Wire Wire Line
	4550 1950 4450 1950
Wire Wire Line
	4450 2050 4550 2050
Wire Wire Line
	4550 2150 4450 2150
Connection ~ 2300 5100
Wire Wire Line
	2300 5100 2300 5000
Wire Wire Line
	2300 5450 2300 5100
$Comp
L SamacSys_Parts:LP5912-5.0DRVR PS1
U 1 1 5ED726B0
P 2300 4900
F 0 "PS1" H 2800 5165 50  0000 C CNN
F 1 "LP5912-5.0DRVR" H 2800 5074 50  0000 C CNN
F 2 "SamacSys_Parts:SON65P200X200X80-7N" H 3150 5000 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/lp5912.pdf" H 3150 4900 50  0001 L CNN
F 4 "LDO Voltage Regulators 500mA low-noise low-Iq low-dropout (LDO) linear regulator 6-WSON -40 to 125" H 3150 4800 50  0001 L CNN "Description"
F 5 "0.8" H 3150 4700 50  0001 L CNN "Height"
F 6 "595-LP5912-5.0DRVR" H 3150 4600 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/Texas-Instruments/LP5912-50DRVR?qs=%252BEew9%252B0nqrBQWfuia6oGxQ%3D%3D" H 3150 4500 50  0001 L CNN "Mouser Price/Stock"
F 8 "Texas Instruments" H 3150 4400 50  0001 L CNN "Manufacturer_Name"
F 9 "LP5912-5.0DRVR" H 3150 4300 50  0001 L CNN "Manufacturer_Part_Number"
	1    2300 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 4800 8250 4800
$Comp
L power:GND #PWR0102
U 1 1 5F615A54
P 3950 3850
F 0 "#PWR0102" H 3950 3600 50  0001 C CNN
F 1 "GND" H 3955 3677 50  0000 C CNN
F 2 "" H 3950 3850 50  0001 C CNN
F 3 "" H 3950 3850 50  0001 C CNN
	1    3950 3850
	1    0    0    -1  
$EndComp
NoConn ~ 4100 3950
$Comp
L power:VDC #PWR0104
U 1 1 5F61A3CA
P 6300 3300
F 0 "#PWR0104" H 6300 3200 50  0001 C CNN
F 1 "VDC" H 6315 3473 50  0000 C CNN
F 2 "" H 6300 3300 50  0001 C CNN
F 3 "" H 6300 3300 50  0001 C CNN
	1    6300 3300
	1    0    0    -1  
$EndComp
Text GLabel 5300 3750 2    50   Input ~ 0
D11
Wire Wire Line
	4100 3850 3950 3850
$Comp
L power:VCC #PWR0101
U 1 1 5F6153D9
P 5700 3850
F 0 "#PWR0101" H 5700 3700 50  0001 C CNN
F 1 "VCC" H 5715 4023 50  0000 C CNN
F 2 "" H 5700 3850 50  0001 C CNN
F 3 "" H 5700 3850 50  0001 C CNN
	1    5700 3850
	-1   0    0    1   
$EndComp
$Comp
L SamacSys_Parts:TPS2051CDBVR KEYPWR1
U 1 1 5F60F54B
P 4100 3750
F 0 "KEYPWR1" H 4700 4015 50  0000 C CNN
F 1 "TPS2051CDBVR" H 4700 3924 50  0000 C CNN
F 2 "SOT95P280X145-5N" H 5150 3850 50  0001 L CNN
F 3 "http://www.ti.com/lit/gpn/tps2051c" H 5150 3750 50  0001 L CNN
F 4 "Single Channel, Current-Limited USB Power Distribution Switch" H 5150 3650 50  0001 L CNN "Description"
F 5 "1.45" H 5150 3550 50  0001 L CNN "Height"
F 6 "595-TPS2051CDBVR" H 5150 3450 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/Texas-Instruments/TPS2051CDBVR?qs=PF3AD18CSE5vi2HeWLJCmw%3D%3D" H 5150 3350 50  0001 L CNN "Mouser Price/Stock"
F 8 "Texas Instruments" H 5150 3250 50  0001 L CNN "Manufacturer_Name"
F 9 "TPS2051CDBVR" H 5150 3150 50  0001 L CNN "Manufacturer_Part_Number"
	1    4100 3750
	1    0    0    -1  
$EndComp
$Comp
L power:VDC #PWR0103
U 1 1 5F61816B
P 3850 3750
F 0 "#PWR0103" H 3850 3650 50  0001 C CNN
F 1 "VDC" H 3865 3923 50  0000 C CNN
F 2 "" H 3850 3750 50  0001 C CNN
F 3 "" H 3850 3750 50  0001 C CNN
	1    3850 3750
	1    0    0    -1  
$EndComp
$Comp
L SamacSys_Parts:02016D104KAT2A 0.1uF5
U 1 1 5F64906E
P 5300 3850
F 0 "0.1uF5" H 5550 4115 50  0000 C CNN
F 1 "02016D104KAT2A" H 5550 4024 50  0000 C CNN
F 2 "SamacSys_Parts:CAPC0603X33N" H 5650 3900 50  0001 L CNN
F 3 "" H 5650 3800 50  0001 L CNN
F 4 "Capacitor MLCC 0201 100nF 6.3V AVX 0201 Standard 100nF Ceramic Multilayer Capacitor, 6.3 V dc, +85C, X5R Dielectric, +/-10% SMD" H 5650 3700 50  0001 L CNN "Description"
F 5 "0.33" H 5650 3600 50  0001 L CNN "Height"
F 6 "581-02016D104KAT2A" H 5650 3500 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/AVX/02016D104KAT2A?qs=PN7sAUOUrntlfwxbZzo6OQ%3D%3D" H 5650 3400 50  0001 L CNN "Mouser Price/Stock"
F 8 "AVX" H 5650 3300 50  0001 L CNN "Manufacturer_Name"
F 9 "02016D104KAT2A" H 5650 3200 50  0001 L CNN "Manufacturer_Part_Number"
	1    5300 3850
	0    1    1    0   
$EndComp
Wire Wire Line
	5300 3850 5700 3850
Connection ~ 5300 3850
$Comp
L power:GND #PWR0105
U 1 1 5F64CB6B
P 5300 4350
F 0 "#PWR0105" H 5300 4100 50  0001 C CNN
F 1 "GND" H 5305 4177 50  0000 C CNN
F 2 "" H 5300 4350 50  0001 C CNN
F 3 "" H 5300 4350 50  0001 C CNN
	1    5300 4350
	1    0    0    -1  
$EndComp
$Comp
L SamacSys_Parts:GRM31CR60J157ME11L C1
U 1 1 5F653145
P 4100 3500
F 0 "C1" V 4396 3372 50  0000 R CNN
F 1 "GRM31CR60J157ME11L" V 4305 3372 50  0000 R CNN
F 2 "CAPC3216X180N" H 4450 3550 50  0001 L CNN
F 3 "http://www.murata.com/~/media/webrenewal/support/library/catalog/products/capacitor/mlcc/c02e.pdf" H 4450 3450 50  0001 L CNN
F 4 "MURATA - GRM31CR60J157ME11L - CAPACITOR, MLCC, X5R, 150UF, 6.3V, 1206" H 4450 3350 50  0001 L CNN "Description"
F 5 "1.8" H 4450 3250 50  0001 L CNN "Height"
F 6 "81-GRM31CR60J157ME1L" H 4450 3150 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/Murata-Electronics/GRM31CR60J157ME11L?qs=qSv5qXjCIG3a12X8fU8miQ%3D%3D" H 4450 3050 50  0001 L CNN "Mouser Price/Stock"
F 8 "Murata Electronics" H 4450 2950 50  0001 L CNN "Manufacturer_Name"
F 9 "GRM31CR60J157ME11L" H 4450 2850 50  0001 L CNN "Manufacturer_Part_Number"
	1    4100 3500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3850 3750 4100 3750
Wire Wire Line
	4100 3750 4100 3500
Connection ~ 4100 3750
$Comp
L power:GND #PWR0106
U 1 1 5F656671
P 4100 3000
F 0 "#PWR0106" H 4100 2750 50  0001 C CNN
F 1 "GND" H 4105 2827 50  0000 C CNN
F 2 "" H 4100 3000 50  0001 C CNN
F 3 "" H 4100 3000 50  0001 C CNN
	1    4100 3000
	-1   0    0    1   
$EndComp
Wire Wire Line
	5800 5300 5800 5700
$Comp
L SamacSys_Parts:TCR5RG33A,LF IC4
U 1 1 61514653
P 7000 4750
F 0 "IC4" V 7604 4878 50  0000 L CNN
F 1 "TCR5RG33A,LF" V 7695 4878 50  0000 L CNN
F 2 "BGA4C35P2X2_64X64X33" H 8150 4850 50  0001 L CNN
F 3 "https://toshiba.semicon-storage.com/info/docget.jsp?did=70203&prodName=TCR5RG33A" H 8150 4750 50  0001 L CNN
F 4 "LDO Voltage Regulators LDO Reg, Iout: 500mA Vout: 3.3V Auto-Disch Low noise ultra high PSRR" H 8150 4650 50  0001 L CNN "Description"
F 5 "0.33" H 8150 4550 50  0001 L CNN "Height"
F 6 "" H 8150 4450 50  0001 L CNN "Mouser Part Number"
F 7 "" H 8150 4350 50  0001 L CNN "Mouser Price/Stock"
F 8 "Toshiba" H 8150 4250 50  0001 L CNN "Manufacturer_Name"
F 9 "TCR5RG33A,LF" H 8150 4150 50  0001 L CNN "Manufacturer_Part_Number"
	1    7000 4750
	0    1    1    0   
$EndComp
Wire Wire Line
	6900 4750 6450 4750
Wire Wire Line
	6450 4750 6450 4800
Wire Wire Line
	7000 4750 7450 4750
Wire Wire Line
	7450 4750 7450 4800
Wire Wire Line
	7450 4800 7450 6050
Wire Wire Line
	7450 6050 7000 6050
Connection ~ 7450 4800
Wire Wire Line
	6900 6050 6000 6050
Wire Wire Line
	6000 6050 6000 5700
Wire Wire Line
	6000 5700 5800 5700
Connection ~ 5800 5700
Wire Wire Line
	5800 5700 5800 5850
$EndSCHEMATC
