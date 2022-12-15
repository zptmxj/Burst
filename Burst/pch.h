// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

#define ACK_SUCCESS 		0x00
#define ACK_FAIL		  	0x01
#define ACK_FULL		  	0x04
#define ACK_NOUSER			0x05
#define ACK_USER_EXIST		0x06
#define ACK_FIN_EXIST		0x07
#define ACK_TIMEOUT			0x08

#define CMD_SLEEP		  	0x2c
#define CMD_MODE		  	0x2d
#define CMD_ADD1		  	0x01
#define CMD_ADD2		  	0x02
#define CMD_ADD3		  	0x03
#define CMD_DEL_USER		0x04
#define CMD_D_ALL_USER		0x05
#define CMD_CNT_USER		0x09
#define CMD_COMP_1_1  		0x0b
#define CMD_COMP_I_N		0x0c
#define CMD_USER_PRIV		0x0A
#define CMD_GET_VER			0x26//soon..
#define CMD_COMP_LV			0x28//soon..
#define CMD_UPLOAD			0x24//soon..
#define CMD_UPLOAD_EX	  0x03//soon..
#define UART_BAUD       19200

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"
#include <afxcontrolbars.h>

#endif //PCH_H
