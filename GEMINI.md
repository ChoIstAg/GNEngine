# GNEngine Project Configuration for Gemini

## 1. 프로젝트 개요 (Project Overview)

- **프로젝트명:** GNEngine
- **목표:** Modern C++과 SDL3를 이용한 2D 게임 엔진 개발.
- **주요 기술:** Modern C++, CMake, SDL3, SDL3_image, SDL3_ttf

## 2. 빌드 및 실행 (Build & Run)

이 프로젝트는 CMake와 `CMakePresets.json`을 사용하여 빌드합니다. g++ 컴파일러와 Ninja 빌드 시스템을 사용합니다.
- 현재 환경: vscode

1.  **CMake 설정 (Configure):**
    `CMakePresets.json`에 정의된 "Custom configure preset"을 사용합니다.
    ```bash
    cmake --preset custom-preset
    ```

2.  **빌드 (Build):**
    ```bash
    cmake --build out/build/custom-preset
    ```

3.  **실행 (Run):**
    빌드가 완료되면 실행 파일은 `out/build/Custom configure preset/` 디렉터리에 생성됩니다.
    ```bash
    ./out/build/"Custom configure preset"/GNEngine.exe
    ```

## 3. 의존성 (Dependencies)

- **SDL3:** v3.2.18
- **SDL3_image:** v3.2.4
- **SDL3_ttf:** v3.2.2      

의존성 라이브러리들은 `include/lib/` 디렉터리에서 관리됩니다.

## 4. 디렉터리 구조 (Directory Structure)

- `src/`: 메인 애플리케이션 소스 코드 (`main.cpp`, `Application.cpp` 등)
- `include/engine/`: 엔진 핵심 로직
    - `event/`: 이벤트 및 입력 관리
    - `object/`: 게임 오브젝트 관리
    - `resource/`: 텍스처, 애니메이션, 폰트, 사운드 등 리소스 관리
    - `scene/`: 씬(Scene) 관리
- `asset/`: 게임에 사용될 이미지, 폰트, 텍스트 파일
- `out/build/`: 빌드 결과물이 저장되는 디렉터리
- `CMakeLists.txt`: 최상위 CMake 빌드 스크립트

<!-- ## 5. 테스트 (Testing)

 - 현재 설정된 테스트 프레임워크나 명령어가 없습니다.
- 테스트를 추가할 경우, 실행 방법과 절차를 여기에 명시해 주세요.
  (예: `ctest --preset <preset-name>`) 
- 없음. -->

## 5. 소스 파일들의 역할과 구조
- src/main.cpp: 어플리케이션의 진입점. 단지 메인 콜백을 실행하기 위한 시발점으로 사용됨.
- src/Application.cpp: 메인 콜백 함수와 어플리케이션에서 필요한 각 매니저와 부가 요소들을 멤버 변수로 정의함.


- 



## 6. 코딩 스타일 및 규칙 (Coding Style & Conventions)

- 프로젝트 전반에 걸쳐 일관된 코딩 스타일을 유지합니다.
- 새로운 코드를 작성할 때는 주변 코드의 명명 규칙(naming convention), 포맷팅 등을 따라주세요.

### 명명 규칙:
- 변수 명: Camel Case 
- 함수 명: Camel Case
- 클래스: Pascal Case
- 타입 명: Pascal Case
- 상수: Screaming Snake Case

### 그 외 코드 스타일 규칙
- 주석 표기는 대부분 /* */를 사용해줘.
- 주석은 되도록이면 .h 헤더 파일 말고도 .cpp 구현 파일에도 작성해줘.
- 한글 주석을 사용할 때에는 "입니다." 대신 "~임." "~함."를 사용해줘
- lambda 객체는 간단하게 한 줄로 작성할 수 있다고 판단될 때(대부분의 경우)에는 아래의 형식대로 작성합니다. (람다 전후로 띄어쓰기에 집중해줘!!)
    function( []() { return 1; } )

- 한 줄로 작성하기 힘들다고 판단될 때에는에는 아래의 형식대로 작성합니다.
    function( []() {
        /* 
         * 무수한 코드들...
        */
    }; )

## 7. 그 밖의 규칙
- 나는 Modern C++(23까지)와 게임 엔진을 배우고자 하는 열렬한 학습자야.
- 나는 코드의 작동 원리를 알고싶어. 반드시 코드를 작성해준 후 코드의 작동 원리를 세세하게 명시해줘.
- 조금 시간이 더 걸리더라도 정확한 정보를 가져오고, 더 효율적인 코드를 작성해줘.
- 나는 항상 컴파일 성능 보다는 '런타임 성능'을 중시해. 되도록이면 컴파일 타임 연산과 상수를 사용했으면 좋겠어. (하지만 런타임으로 돌아가는 어플리케이션이므로 런타임 연산이 필요할 때에는 런타임 연산을 해야해.)
- 실시간 어플리케이션인 만큼 불필요한 연산과 오버헤드를 최대한 줄이고 싶어.

- SDL3 공식 문서 : https://wiki.libsdl.org/wiki/index
    SDL3는 릴리즈한지 얼마 되지 않아서 정보가 빈약해. 반드시 공식 문서를 잘 살펴보고 사용할 함수가 SDL3에 존재하는지 찾고 함수의 인자를 잘 살펴보고 사용해줘.