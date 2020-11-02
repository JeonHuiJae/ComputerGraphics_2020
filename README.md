# ComputerGraphics_2020

** 모든 코드는 Commits을 통해 열람 가능합니다.

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


[실습4_ interleaved buffer 활용]

1. 두개였던 vector를 하나로 합친다. 3개씩인걸 6개씩으로
2. 버퍼는 하나로 다시 줄인다. 이렇게 만들면, renderScene에 있던 bind, data를 다시 main으로 데려올수 있다.
3. glVertexAttribPointer를 조정해서 버퍼를 나눠서 쓸수있다.
4. 각 변수에 값을 저장하게하면 끝. 물론 glDraw의 size도 조절해줘야한다.(몇개 그릴지)


[실습5_ 마우스함수, 키보드함수]

1. 마우스와 키보드 함수를 만든다 mouse(status, x, y), key(key, x, y)
2. 함수를 등록한다. glulMouseFunc, key도
3. vertices벡터를 잘 조정한다. (인덱스: vertices[], push: vertices.push_back(), pop: vertices.pop_back)
4. line을 만들기 위해서는 draw함수의 type을 GL_LINES로 바꾸면 된다.
5. line의 두께를 조절하려면, glLineWidth()함수 사용하면 된다.

+ Depth Buffer Function 활성화(Sort by Depth)
glEnable(GL_DEPTH_TEST);
glEnable(GL_DEPTH_FUNC);
glDepthFunc(GL_LESS);
glDepthRange(0.5, 1.0);


[실습5_ 삼각형 그리기]

1. 강의자료대로 좌표 데이터를 작성
2. draw에서 GL_TRIANGLES로 수정
3. 끝..


[실습6_ 큐프 두개 그리고 움직이기]

1. 데이터를 넣는다.(배열 vector로 고쳐서 사용)
2. 큐브가 출력되는지 확인
3. vertices데이터가 수정되지않으니 VBO관련 코드는 main으로 옮기고 draw를 두번한다.
4. trans 행렬배열을 2개 생성(4X4단위행렬)
5. VertexShader에서 uniform mat4 T 행렬 생성, T * pos로 곱해지도록 고치기
6. q, w, a, z 누르면 transL를 조정, 반대쪽도. homogeneous coordinate 참고
7. renderScene에서 glUniformMatrix4fv를 이용해 사용할 행렬 지정 + 그리기 두번 반복
8. 움직이는거에 빨간색 칠하기는 해당 키를 누를때 왼, 오른 구분할 true false를 조정한다.
9. 해당 큐브가 움직이는 중이라면, glDisableVertexAttribArray()를 이용해 원래 지정된 색을 죽이고 glVertexAttrib3f()를 이용해 빨강 지정
10. 움직이는중이 아니라면, glEnableVertexAttribArray()로 다시 살려준다.


[실습6_ 큐브 look-at 행렬기반 회전]

1. glm 다운, C- ProgramFiles(x86) - MicrosoftSDKs - Windows - v7.1A - include 에 glm저장.
2. #include<GLM/glm.h>인클루드
3. MyMouse 활성화(glutMouseFunc)
4. 마우스로 받은nx, ny를 이용해 lookat matrix 생성
  - uz = vec3(nx, ny, 0)
  - ux = cross(ux, vec3(0, 0, 1))
  - uy = cross(ux, uz)
  - M = vec4 M[4]
  - M[0] = vec4(x, 0), M[1] = vec4(y, 0), M[2] = vec4(z, 0), M[3] = vec4(0, 0, 0, 1)
5. glUniformMatrix4fv이용해서 VertexShader와 연결(mat4 R은 알아서 만들길)


[실습7_ index Buffer사용 출력]
1. Buffer 크기 3으로 늘리기
2. i, c, col vector추가, 데이터 생성
3. 각각 Buffer의 방에 할당. 단, index Buffer는 GL_ELEMENT_ARRAY_BUFFER 지정
4. 세부사항 조정(arrayPointer등)
5. 주의할점 = buffer Bind 후 ArrayPointer지정 을 c, col에 대해 순서대로 해줘야함.  
6. RenderScene에서는 큐브 두개 만드는부분, 빨간색 지정 부분을 제거, 깔끔하게 한다.
