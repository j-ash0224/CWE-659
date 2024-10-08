# CWE-659
C++/C 을 사용한 CWE-659 규칙 위반 코드

CWE-659는 "Insufficiently Protected Credentials"의 약자로 나열된 단어 자체만 본다면 ‘일반적인 결함의 나열’ 이다.
‘CWE’를 이용하면 정적 분석의 한 부분인 실행시간 오류 ( Runtime Error )를 검출할 수 있다.
즉, 프로그램을 실행시키지 않고 분석하면서 오동작을 일으킬 수 있는 코드를 검출 할 수 있게 된다.
실제로 방위사업청에서는 무기체계 SW 지침을 통해서 CWE-658과 CWE-659를 바탕으로 실행시간 결함에 대한 검출 기준을 요구하고있다.
이러한 CWE는 SQL 인젝션, 크로스 사이트 스크립팅, 버퍼 오버 플로우 등 다양한 소프트웨어의 결함을 식별하기 위해 결함의 종류(결함의 유형) 목록을 체계화하여 제공하고 있으며, CWE를 사용한다면 소프트웨어 개발자와 보안 전문가 등이 다음과 같은 장점을 얻을 수 있다.

1. 소프트웨어 아키텍처, 디자인, 코드에 내재 된 결함에 대해 일반적인 검토를 할 수 있게 된다.
2. 결함 검사 도구 등 소프트웨어의 보안을 향상시키기 위한 도구의 표준 척도로 사용할 수 있다.
3. 결함의 원인을 인식하고 결함을 감소시키며, 재발의 방지를 위한 공통의 기준으로서 활용할 수  있다.
   
대표적인 항목은 CWE- 658, CWE-659, CWE-660 이 있으며
그 중 CWE – 659 은 C++에서 정적 분석 도구에 의해 검출되는 오류를 규정해 놓은 것으로써 전체 85개의 취약점을 가지고 있으며, 
각각 유형에 따라 카테고리(Category) 3개, 취약점(Weakness) 81개, 복합 요인(Compound Element) 1개를 지니고 있다.

규칙을 일부로 위반하는 프로젝트를 생성하여 인턴십을 진행한 회사에서 개발한 staic 테스트 자동화 도구를 사용해 분석을 수행하였다.

프로그램의 내용은 사용자 정보와 이를 활용하여 은행 계좌와 관련된 서비스를 제공하는 프로젝트이다.
데이터는 파일 시스템으로 다룬다.

main.cpp : 프로그램 메인 실행 파일
account.h : 계좌 정보 관련 기능 파일
account.txt : 계좌 정보 텍스트 파일
user.h : 사용자 정보 관련 기능 파일
user.txt : 사용자 정보 텍스트 파일
extra.c : CWE-659 규칙 위반을 위한 더미 C 파일
usingC.h : 위의 C 파일을 C++ 파일과 함께 컴파일 시키기 위한 헤더 파일
