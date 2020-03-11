# ComputerGraphics_2020

[실습1_ 예제코드 실행하기]

0. 콘솔 프로그램으로 프로젝트 생성
1. freeglut다운로드(반드시 msvs용 다운해야 lib파일이 있음)
2. glew다운로드(이것도 lib파일 있는걸로 다운)
3. 압축을 푼 두 파일에서 각각 lib와 include폴더를 꺼내 프로젝트 내부에 넣는다.
4. dll은 각각 찾아서 dll파일만 프로젝트 내부로 옮긴다.
5. 프로젝트 - 속성 에서 디렉토리 설정
6. 링커-입력 에서 추가 종속성에 freeglut.lib와 glew32.lib를 추가
7. 맥용인 것들을 지우고 윈도우용의 주석을 푼다.
(glew 부분 풀고 gl3.h 삭제, initDisplayMode삭제, glut, glew, wglew include)


[실습2_ 점 여러개 찍기]

1. 