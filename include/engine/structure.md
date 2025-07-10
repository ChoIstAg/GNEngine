

# Core
## main function
어플의 시작점

## Application
어플의 모든것을 총괄하는 객체.
메인 콜백, 종료, Delta time 체크

# Manager

## RenderManager
### 렌더링 순서
1. effect: UI 바로 아랫단이며 가장 앞에서 렌더링됨.
2. character: 주인공 캐릭터, 아군 터렛, 적 등 주요 오브젝트. 상호작용 가능
3. object: 상호작용 가능한 오브젝트
4. background 1: 가장 가까운 배경 오브젝트. 
5. background 2: 배경 오브젝트 중 거리가 가장 멂. (건물)
6. background 3: 가장 뒤의 배경. 카메라의 움직임에 따라 중심을 기준으로 미세하게 움직임. (ex 하늘)

배경 오브젝트마다 각각 얼마나 멀리 있는지를 나타내는 수치를 바탕으로 크기와 이동속도에 차등을 두어 렌더하여 패럴랙스 효과를 줌.

1. UI effect (클릭(터치) 효과 등등)
2. UI object (버튼, 헤더, 텍스트 박스 등 UI 요소)
3. UI background (블러 효과, 암전, 페이드 인/아웃)


- [ ] 화면 초기화
- [ ] 텍스처 렌더링 (Texture* 를 인자로 받아옴)
- [ ] 애니메이션 렌더링(Animation* 을 인자로 받아옴)
	- [ ] SDL_GetTicks()로 시간을 인자로 받아와 Animation의 각 타이밍에 맞춰 텍스처 교체하기.
	- [ ] 애니메이션 초기화
	- [ ] AnimationManager::changeAnimation() 처리
	- [ ] 키 입력을 받고 애니메이션 조작하기
	- [ ] 애니메이션 리버스 렌더링( 반대 순서로 렌더링 )
- [ ] 

## InputManager
- [ ] 키보드, 마우스 장치 이벤트 처리
- [ ] 키보드 장치 id 불러오고 적용하기
- [ ] 마우스 장치 id 불러오고 적용하기
- [ ] 키보드의 모든 키의 상태를 불러와 동시 입력 처리하기.
- [ ] 마우스의 모든 키의 상태를 불러와 동시 입력 처리하기.
- [ ] 마우스 움직일 때 이벤트 처리
- [ ] 마우스 호버링 이벤트 처리
## TextureManager
- [ ] 스크립트 파일을 불러온 후 스크립트대로 텍스처 불러오기.
- [x] 오브젝트 하나당 여러 텍스처를 unordered_map으로 저장하고 파일 이름을 통해 불러옴.
- [x] 불러온 파일마다 크기(scaleX * width) 설정
- [x] 파일 불러오기 (bmp)
- [x] 파일 불러오기 (png)
- [x] 텍스처 렌더링
- [x] getter, setter

## TextManager

- [ ] TTF_Init(), TTF_Quit() 처리하기
- [ ] 폰트 불러오기
- [x] 파일 불러오기 (.txt)
- [ ] 텍스트 파일을 텍스트 오브젝트로 만들기
- [ ] 텍스트 입력 받기

## AnimationManager

- [ ] 애니메이션 로딩
- [ ] 애니메이션 전환
- [ ] 

## Animation

- [ ] 애니메이션 로딩 (gif)
- [ ] 애니메이션 로딩(스프라이트 시트 png)
- [ ] 애니메이션 렌더링

## FileManager

### function

- [ ] 세이브 저장

## SceneManager

장면들을 관리

- 메인
- 설정
- 게임오버
- 인트로 스토리
- 엔딩

## SoundManager

배경음악, 효과음 관리

- [ ] 오디오 장치 불러오기
- [ ] 오디오 장치 교체
- [ ] 파일 불러오기 (.wav)
- [ ] 파일 불러오기 (.mp3)
- [ ] 음악 스크립트 불러오기 (.midi)


## Component
하나의 오브젝트가 가지는 지역적 컴포넌트.
(Manager는 전역적으로 MainCallback에서 중심으로 존재, 컴포넌트가 각 Manager에 접촉, **명령을 내림**. **각 Manager가 처리**.)


### InputComponent

### SoundComponent
