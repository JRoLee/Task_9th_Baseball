# Number Baseball - Multiplayer
<img width="2555" height="1365" alt="image" src="https://github.com/user-attachments/assets/f9f69dfa-57f4-4b9f-96a0-a6c37c3eddb3" />

> **Unreal Engine 5.5 기반 Dedicated Server 멀티플레이어 숫자 야구 게임**  
> 채팅을 통해 1~9 사이의 중복 없는 3자리 숫자를 맞추는 턴제 멀티플레이어 게임

### ✔️ Engine & Language
* **Engine:** Unreal Engine 5.5.4
* **Language:** C++

<br>

## 📖 목차
1. [프로젝트 소개](#프로젝트-소개)
2. [설치 및 실행 방법](#설치-및-실행-방법)
3. [주요 기능](#주요-기능)
4. [시스템 설계](#시스템-설계)
5. [트러블 슈팅](#트러블-슈팅)
6. [프로젝트 구조](#프로젝트-구조)

<br>

## 👨‍🏫 프로젝트 소개

* **프로젝트 의도:** Unreal Engine 5의 Dedicated Server 기반 멀티플레이어 네트워크 구조(RPC, Property Replication, GameplayTag 등)를 학습하고 실제 게임에 적용하기 위해 제작
* **개발 기간:** 2026.03.13 ~ 2026.03.19
* **플랫폼:** PC
* **플레이 영상:** [멀티플레이 숫자 야구](https://youtu.be/U_4eurygQuU)

### 게임 규칙

* 서버가 1~9 사이의 중복 없는 3자리 숫자를 무작위로 생성
* 플레이어는 턴 순서에 따라 채팅으로 숫자를 입력
* **Strike** : 숫자와 자리가 모두 일치
* **Ball** : 숫자는 맞지만 자리가 다름
* **OUT** : 숫자가 하나도 없음
* **3 Strike = HOME RUN** (승리)
* 모든 플레이어의 시도 횟수 소진 시 **Draw**

<br>

## 🚀 설치 및 실행 방법

### 브랜치 선택

이 프로젝트는 두 가지 브랜치로 제공됩니다.

| 브랜치 | 설명 | 권장 대상 |
|--------|------|----------|
| `feature/required` | 필수 기능만 포함된 기본 버전 | 핵심 네트워크 구조 학습 |
| `feature/challenge` | 턴 시스템, UI 연출 등 추가 기능 포함 | 전체 기능 확인 |

---

### 설치 방법

**1. 원하는 브랜치 클론**

```bash
# 기본 버전
git clone -b feature/required https://github.com/JRoLee/Task_9th_Baseball.git

# 추가 기능 버전
git clone -b feature/challenge https://github.com/JRoLee/Task_9th_Baseball.git
```

**2. 프로젝트 빌드**
```
1. Task_9th_Baseball.uproject 우클릭
2. Generate Visual Studio project files 클릭
3. Task_9th_Baseball.sln 열기
4. Development Editor / Win64 로 빌드
```

**3. 에디터 실행**
```
Task_9th_Baseball.uproject 실행
```

---

### PIE 실행 설정

```
Editor Preferences → Play → Multiplayer Options
  - Launch Separate Server : true
  - Play Net Mode          : Play As Client
  - Number of Clients      : 2 이상

Project Settings → Maps & Modes
  - Game Default Map : L_Main
  - Server Default Map : L_Main
```

---

### feature/required vs feature/challenge 기능 비교

| 기능 | feature/required | feature/challenge |
|------|:---:|:---:|
| 멀티플레이어 채팅 | ✅ | ✅ |
| 숫자야구 판정 | ✅ | ✅ |
| 로그인 시스템 | ✅ | ✅ |
| 플레이어 리스트 | ✅ | ✅ |
| 턴 시스템 | ❌ | ✅ |
| 턴 타이머 | ❌ | ✅ |
| UI 연출 (이미지 애니메이션) | ❌ | ✅ |
| 플레이 로그 카드 | ❌ | ✅ |
| HOME RUN 연출 | ❌ | ✅ |


<br>

## 💜 주요 기능

### 1. 닉네임 로그인 시스템
* 게임 입장 시 LoginUI에서 닉네임 입력 후 서버에 전달
* 닉네임 미입력 또는 공백 포함 시 Player 1, Player 2 형식으로 자동 부여
* 최대 6자 제한 (NickNameLengthLimit, WBP_LoginUI 에디터에서 수정 가능)
* 로그인 완료 시 LoginUI → MainUI 자동 전환

### 2. 턴제 시스템
* 접속 순서대로 턴 진행
* 턴당 제한 시간 10초 (BP_NBGmaeMode 에디터에서 수정 가능)
* 제한 시간 초과 시 자동으로 다음 턴 전환 및 시도 횟수 차감
* 시도 횟수 소진 플레이어 턴 자동 스킵
* GameState의 RemainingTurnTime을 Property Replication으로 모든 클라이언트에 동기화

### 3. 채팅 기반 게임 판정
* 채팅창 입력으로 정답 시도
* 본인 턴이 아닌 경우 일반 채팅으로만 처리
* 유효하지 않은 숫자 입력 시 안내 메시지 출력
* Strike / Ball / Out 결과를 0.2초 간격으로 순차 표시 (이미지 애니메이션)
* HOME RUN 특별 연출

### 4. 플레이어 리스트 실시간 동기화
* 로그인 순서대로 PlayerCard 표시
* 시도 횟수 변경 시 GameMode에서 ClientRPCRefreshPlayerList로 모든 클라이언트에 브로드캐스트
* 플레이 로그 ScrollBox에 입력 기록 자동 누적

### 5. 게임 상태 관리 (GameplayTag)
* Game.State.InGame / Game.State.Wait 로 게임 진행 상태 관리
* Player.State.MyTurn / Player.State.NotMyTurn 으로 플레이어 턴 상태 관리
* PropertyReplication으로 PlayerState에서 모든 클라이언트에 동기화

<br>

## 🏗️ 시스템 설계 (Architecture)

### 네트워크 객체 존재 위치

| 클래스 | Dedicated 서버 | 클라이언트 |
|--------|---------------|-----------|
| GameMode | ✅ | ❌ |
| GameState | ✅ | ✅ (Replicated) |
| PlayerController | ✅ (모든 PC) | ✅ (본인 PC만) |
| PlayerState | ✅ | ✅ (Replicated) |

### RPC 흐름도

```
[로그인]
LoginUI → ServerRPCLogInGame → GameMode.SetPlayerNickName
       → ClientRPCLogInGame (MainUI 전환)
       → ClientRPCRefreshPlayerList (PlayerCard 갱신)

[채팅 / 정답 입력]
MainUI → ServerRPCPrintChatMessageString
       → GameMode.PrintChatMessageString (판정)
       → ClientRPCShowResult / ClientRPCPrintChatMessageString

[턴 타이머]
GameMode → GameState.RemainingTurnTime (Replicated)
         → OnRep_RemainingTurnTime → PlayerController.UpdateUITimer → UI 갱신

[게임 시작]
MainUI → ServerRPCRequestStartGame → GameMode.StartGamePlay
```

### Design Pattern

* **Dedicated Server Pattern**
    * GameMode는 서버에만 존재하며 모든 게임 규칙과 판정을 담당
    * 클라이언트는 RPC를 통해 서버에 요청하고 결과를 수신하는 구조

* **Observer Pattern (OnRep)**
    * GameState의 RemainingTurnTime 변경 시 OnRep_RemainingTurnTime 자동 호출로 UI 업데이트

* **Command Pattern (RPC)**
    * 클라이언트의 모든 액션(채팅, 로그인, 게임시작)은 ServerRPC를 통해 서버에 전달
    * 서버의 결과는 ClientRPC를 통해 클라이언트에 전달

<br>

## 🚀 트러블 슈팅 (Trouble Shooting)

### 문제 1: TActorIterator vs AllPlayerControllers 혼용으로 인한 순서 불일치
* **원인:** 채팅 브로드캐스트는 TActorIterator를 사용하고, 턴 관리는 AllPlayerControllers를 사용하면서 인덱스 기준이 달라짐
* **해결:** 브로드캐스트(순서 불필요)는 TActorIterator, 인덱스 기반 턴 관리는 AllPlayerControllers로 역할을 명확히 분리
* **결과:** 턴 순서 보장 및 브로드캐스트 안정성 확보

### 문제 2: PlayerCard 생성 순서 불일치
* **원인:** OnRep 호출 순서가 네트워크 상황에 따라 달라져 PlayerCard 생성 순서가 보장되지 않음
* **해결:** OnRep 대신 GameMode의 AllPlayerControllers 순서 기반으로 BroadCastPlayerList를 구현하여 ClientRPC로 순서가 보장된 배열을 직접 전달
* **결과:** 항상 접속 순서대로 PlayerCard 표시

### 문제 3: JudgeGame에서 EndGamePlay 중복 호출
* **원인:** for문 안에서 EndGamePlay를 호출하여 플레이어 수만큼 반복 호출됨
* **해결:** EndGamePlay 호출을 for문 밖으로 이동
* **결과:** 게임 종료 처리가 정확히 1회만 실행

### 문제 4: PIE에서 레벨 트래블 미동작
* **원인:** PIE 환경에서는 ClientTravel이 정상적으로 동작하지 않는 UE의 알려진 한계
* **해결:** 로그인 레벨을 별도로 분리하지 않고 L_Main 하나에서 LoginUI → MainUI 전환 방식으로 구조 변경
* **결과:** PIE 환경에서도 안정적으로 로그인 흐름 테스트 가능

<br>

## 📂 프로젝트 구조

```
Task_9th_Baseball
├── Config
├── Content
│   └── NumberBaseball
│       ├── Maps
│       │   └── L_Main
│       ├── UI
│       │   ├── WBP_LoginUI
│       │   ├── WBP_MainUI
│       │   ├── WBP_PlayerCardWidget
│       │   └── WBP_PlayLogCard
│       └── Blueprints
│           ├── BP_NBGameModeBase
│           ├── BP_NBGameStateBase
│           ├── BP_NBPlayerController
│           └── BP_NBPlayerState
└── Source
    └── Task_9th_Baseball
        ├── Game
        │   ├── NBGameModeBase.h / .cpp
        │   └── NBGameStateBase.h / .cpp
        ├── Player
        │   ├── NBPlayerController.h / .cpp
        │   └── NBPlayerState.h / .cpp
        └── UI
            ├── NBLoginUI.h / .cpp
            ├── NBMainUI.h / .cpp
            ├── NBPlayerCardWidget.h / .cpp
            └── NBPlayLogCard.h / .cpp
```
