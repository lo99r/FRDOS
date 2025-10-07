MYOS 에 오신것을 환영합니다!
commands.txt 에 있는 명령어를 WSL 에 실행해주세요.

그러고 나서 Visual studio 를 끄시고..

https://learn.microsoft.com/ko-kr/cpp/build/cmake-projects-in-visual-studio?view=msvc-170

이 링크를 따라 확장을 설치해주세요.

그리고 다시 이 MYOS 폴더를 Visual studio 로 여시면..

위에 노란색바가 뜨고 잠시 후 상단에 빌드(B) 가 뜨게 됩니다.

그리고 빌드 -> 모두빌드 (Ctrl + Shift + b) 를 하면 main 코드대로 간단한 OS가 만들어지고 qemu가 실행됩니다.

iso 파일은 파일탐색기에서 WSL(보통 Ubuntu) 로 들어가시고 home -> (사용자 이름) -> .vs -> MYOS ch.2 -> out -> OS.iso 에 있습니다.

실제 컴퓨터에서 OS 를 부팅하려면 rufus 같은 툴로 OS.iso를 찾아 USB에 구우시면 됩니다.

단, 반드시 DD 모드로 쓰기를 선택해주시고 바이오스 셋팅은 레거시로 해주세요!

현재 UEFI는 아직 지원하지 않습니다.