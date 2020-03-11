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

1. Vertex 벡터로 찍을 점의 데이터를 만든다. vector<float> vertices{}
2. VAO와 loadShader함수 사이에 VBO를 만든다gen, bind, data 수행
  GLuint Buffer;
  glGenBuffers(1, &Buffer);
  glBindBuffer(GL_ARRAY_BUFFER, Buffer); 
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW); 

3. MainLoop가 돌아가며 실행되는 renderScene에서 매 회마다 점의 위치가 갱신되도록 만들자.
4. VertexShader에서 받을 점의 변수pos를 생성. (코드간 공유가 가능하도록 in을 이용한다.) gl_Position에 연결.
5. pos를 불러오는 함수인 glVertexAttribLocation를 통해 변수이름으로 레퍼런스를 가져온다.
6. glVertexAttribPointer로 포인터 생성(attrib, 개수, 인자형, 자료사이의 거리, offset)
7. glEnableVertexAttribArray로 해당attrib 활성화.
8. glClear후 glDrawArrays(type, start, pointNum)해주자 


[실습3_ 점마다 다른색 입히기]

1. color vector생성
2. Buffer[2]로 VBO 하나 추가후 vertex와 같이 bind, data채우기
3. renderScene에서 pos불러왔던것과 같이 a_Color불러와서 pointer생성, 활성화
4. 이러면 색의 위치로 pos가 가죠? ㅋㄷ
5. 이 결과의 이유는 buffer[1]이 활성화(bind)되어있어서 이렇게된다.
6. pos와 color각각 맞춰주려면 해당코드(bind, data)를 renderScene로 데려와야한다.
7. VertexShader에 out v_Color를 추가하고 a_Color와 연결해주고, FragmentShader에 반드시 같은 이름의 out변수를 만들어준다.