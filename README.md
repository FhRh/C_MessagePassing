C언어 CRC_CCITT16 MessagePassing 프로그램
==
이 Repository에는 C언어로 CRC-16-CCITT로 처리된 데이터를 MessagePassing프로그램을 통해 송, 수신하는 두 개의 프로그램의 코드가 작성되어 있습니다.


기능
--
- CRC 인코딩 및 디코딩 구현 (16진수로 반환)
- 데이터 파일을 16진수 데이터로 변환하는 함수 구현
- CRC 계산 결과를 통해 나온 codeword를 데이터파일 뒤에 이어 붙여 최종 전송할 데이터를 생성 
- 서로 다른 프로그램이 MessagePassing기법을 이용하여 데이터를 주고 받기
- Stop&Wait 프로토콜을 직접 만들어 실습해보기 위해 Sender와 Receiver를 리팩토링 (Stop&Wait Protocol)
- 중간과정마다 데이터의 변환과정을 출력하여 확인할 수 있는 코드를 주석처리

사용 방법
--
1. 저장소를 클론 합니다.
2. Receiver.c 코드를 빌드 및 실행합니다.
3. Sender.c 코드를 빌드 및 실행합니다.
4. 터미널을 통해 출력을 확인합니다.

요구 사항
--
- 해당 프로그램은 WinAPI를 사용하므로 윈도우 환경에서만 유효합니다.
- 

