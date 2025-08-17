- System: 직접적인 연산을 수행하는 노동자.
- Manager: System, Entity, 각 요소들을 모두 관리하는 총 책임자.
  - FileManager : ※Singleton pattern※ 
  flatBuffer 라이브러리를 활용하여 config 설정값을 불러오고 기록함. 모든 파일에서 로그를 기록.
- Component: 
  - ComponentArray: SoA 설계를 구현하기 위해 컴포넌트.
  각 컴포넌트 타입마다 템플릿 특수화로 SoA를 구형해야함.
- Prefab: Entity를 만드는 하나의 프리셋