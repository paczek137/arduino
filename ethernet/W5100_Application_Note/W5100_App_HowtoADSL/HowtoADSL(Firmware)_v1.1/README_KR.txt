+ ������ ���

  ~/compile �������� make �� �����ϸ� �ڵ����� �����ϵȴ�.


    [ Makefile���� -D�ɼ�(-D__COMPILER_VERSION__=2)�� ���� define�Ǿ� ������,
      �ҽ��󿡼� define(types.h ����)���� ���еȴ�. ]


+ �ɼǺ���

  * ����� ������ ���Ѵٸ�, types.h������ define(__DEF_IINCHIP_BUS__ ) ����
    �����ϸ� ���ϴ� �����ϵȴ�. [�����Ǵ� hex���ϸ� ������ makefile���� ����]

    - ���� ���� ���
      1. __DEF_IINCHIP_DIRECT_MODE__ 
      2. __DEF_IINCHIP_INDIRECT_MODE__ 
      3. __DEF_IINCHIP_SPI_MODE__ 