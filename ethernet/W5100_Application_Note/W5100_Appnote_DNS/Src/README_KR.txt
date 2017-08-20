+ 컴파일 방법

  ~/compile 폴더에서 make 를 실행하면 자동으로 컴파일된다.


    [ Makefile에서 -D옵션(-D__COMPILER_VERSION__=2)을 통해 define되어 있으며,
      소스상에서 define(types.h 참고)으로 구분된다. ]


+ 옵션변경

  * 모드의 변경을 원한다면, types.h파일의 define(__DEF_IINCHIP_BUS__ ) 값을
    수정하면 원하는 컴파일된다. [생성되는 hex파일명 변경은 makefile에서 가능]

    - 현재 가능 모드
      1. __DEF_IINCHIP_DIRECT_MODE__ 
      2. __DEF_IINCHIP_INDIRECT_MODE__ 
      3. __DEF_IINCHIP_SPI_MODE__ 