Ardunio-Clock (아두이노 시계) 
=============
* 마이크로프로세서 수업에서 1인으로 수행한 프로젝트입니다.(2023.8 - 2023.12)

<br>목차
-------------
- [Ardunio-Clock (아두이노 시계)](#ardunio-clock-아두이노-시계)
  - [목차](#목차)
  - [프로젝트 개요](#프로젝트-개요)
  - [기능 소개](#기능-소개)
  - [임베디드 요소](#임베디드-요소)
  - [구현 영상](#구현-영상)
    - [센서 무드등](#센서-무드등)


## <br>프로젝트 개요
* **ardiono를 이용한 스마트 탁자 시계** 기능 구현을 목표로 하였습니다.
* 2023 마이크로프로세서 2학년 수업에서 만든 간단한 토이 프로젝트입니다.
* 블루투스 통신을 통해 기능들을 제어할 수 있게 만들었습니다.
* 센서를 통해 탁자 시계 뿐만 아니라 조명의 기능도 할 수 있게 만들었습니다.

## 기능 소개
1. 센서 무드등 - 어두워지면 자동으로 인식하여 조명을 점등합니다.
2. 시계 모드 - 7세그먼트에는 년도를 LCD에는 년도+시간을 표시합니다.
3. 타이머 모드 - 이는 휴대폰과 블루투스 통신을 이용합니다. 휴대폰으로 타이머 시간을 입력하면 LCD에는 잔여 시간이 시:분:초 단위로 나오고 7세그먼트에는 초단위로 출력됩니다. 시간이 끝나면 차임벨 소리와 함께 알람이 울립니다.
4. 메모 모드 - 이는 휴대폰 블루투스 통신을 이용합니다. 휴대폰으로 메모 내용을 입력하면 LCD에 이를 출력합니다.

## 임베디드 요소
* Arduino Mega 2560
* 브레드보드
* 점퍼선
* 4Digit 7 Segment
* l2C LCD
* Photoresistor
* 저항
* RGB LED
* 피에조 스피커
* BlueTooth 4.0 BLE Module
* 푸쉬버튼<br>

## 구현 영상
[![유튜브이동](http://img.youtube.com/vi/watch?v=xoXcwPfs7Pc/0.jpg)](https://youtu.be/watch?v=xoXcwPfs7Pc)




