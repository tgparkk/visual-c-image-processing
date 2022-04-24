#pragma waring(disable:4099)

#include "pch.h"
#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include <setjmp.h>
#include <algorithm>

#include "IppDib.h"
#include "../lib/jpeglib.h"

struct my_error_mgr {
	struct jpeg_error_mgr pub;

	jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr* my_error_ptr;

METHODDEF(void)
my_error_exit(j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr)cinfo->err;
	(*cinfo->err->output_message)(cinfo);
	longjmp(myerr->setjmp_buffer, 1);
}

BOOL IppDib::LoadJPG(const char* filename)
{
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;

	FILE* infile;		// 입력 파일
	JSAMPARRAY buffer;	// 라인 단위 출력 버퍼
	int row_stride;		// 츨력 버퍼 크기

	BOOL ret = FALSE;

	fopen_s(&infile, filename, "rb"); // 입력 파일 열기
	if (!infile)
		return FALSE;

	// Step 1: JPEG 압축 해제를 위한 객체 생성 및 초기화
	cinfo.err = jpeg_std_error(&jerr.pub); // jpeg 라이브러리 에러 처리 설정
	jerr.pub.error_exit = my_error_exit;

	if (setjmp(jerr.setjmp_buffer))
	{
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return FALSE;
	}

	jpeg_create_decompress(&cinfo);

	// Step 2: 입력 파일 지정
	
}
