//#pragma waring(disable:4099)

#include "pch.h"
#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include <setjmp.h>
#include <algorithm>

#include "IppDib.h"
#include "jpeglib.h"

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

	FILE* infile;		// �Է� ����
	JSAMPARRAY buffer;	// ���� ���� ��� ����
	int row_stride;		// ���� ���� ũ��

	BOOL ret = FALSE;

	fopen_s(&infile, filename, "rb"); // �Է� ���� ����
	if (!infile)
		return FALSE;

	// Step 1: JPEG ���� ������ ���� ��ü ���� �� �ʱ�ȭ
	cinfo.err = jpeg_std_error(&jerr.pub); // jpeg ���̺귯�� ���� ó�� ����
	jerr.pub.error_exit = my_error_exit;

	if (setjmp(jerr.setjmp_buffer))
	{
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return FALSE;
	}

	jpeg_create_decompress(&cinfo);

	// Step 2: �Է� ���� ����
	jpeg_stdio_src(&cinfo, infile);

	// Step 3: ���� ��� �б�
	(void)jpeg_read_header(&cinfo, TRUE);

	// Step 4: decompression �Ķ���� ����
	// �⺻ ���� �״�� ����� ���̹Ƿ� �߰� �ڵ� ����.

	// Step 5: JPEG ���� ���� ����
	(void)jpeg_start_decompress(&cinfo);

	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

	// Step 6: ���� ������ JPEG ������ �ȼ� �����͸� �о�ͼ� IppDib �ȼ� �� ����
	// �׷��̽� ���� ����� Ʈ���÷� ������ �����Ͽ� ó����
	if (cinfo.out_color_space == JCS_GRAYSCALE)
	{
		CreateGrayBitmap(cinfo.image_width, cinfo.image_height);
		BYTE* pixels = GetDIBitsAddr();
		int ws = (cinfo.image_width + 3) & ~3;

		while (cinfo.output_scanline < cinfo.output_height)
		{
			(void)jpeg_read_scanlines(&cinfo, buffer, 1);
			memcpy(&pixels[(cinfo.image_height - cinfo.output_scanline) * ws], buffer[0], row_stride);
		}
	}
	else if (cinfo.out_color_space == JCS_RGB)
	{
		CreateRgbBitmap(cinfo.image_width, cinfo.image_height);
		BYTE* pixels = GetDIBitsAddr();
		int ws = (cinfo.image_width * 3 + 3) & ~3;

		while (cinfo.output_scanline < cinfo.output_height)
		{
			(void)jpeg_read_scanlines(&cinfo, buffer, 1);

			// RGB -> BGR
			for (int i = 0; i < row_stride; i += 3)
			{
				std::swap(buffer[0][i], buffer[0][i + 2]);
			}

			memcpy(&pixels[(cinfo.image_height - cinfo.output_scanline) * ws], buffer[0], row_stride);
		}
	}

	// Step 7: JPEG ���� ���� �Ϸ�
	(void)jpeg_finish_decompress(&cinfo);

	// Step 8: JPEG ���� ������ ���� ��ü ����
	jpeg_destroy_decompress(&cinfo);

	fclose(infile);

	return TRUE;
}
