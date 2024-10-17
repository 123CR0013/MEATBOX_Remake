#include"ZFile.h"
#include"zlib/zlib.h"
#include <Windows.h>
#include<fstream>
#include<vector>
#include"../Application/UtilMacro.h"

constexpr size_t OUT_BUFFER_SIZE = 400;

ZFile::ZFile(const std::string& fileName)
	:_filename(fileName)
	, _data(nullptr)
	, _size(0)
	, _success(false)
{
	std::ifstream ifs(_filename, std::ios::binary);

	if (!ifs) return;

	z_stream z;

	//�J�����t�@�C���̃T�C�Y���擾
	ifs.seekg(0, std::ios_base::end);

	_size = (size_t)ifs.tellg();

	ifs.seekg(0, std::ios_base::beg);

	std::vector<Bytef>inBuffer(_size);
	// ���ׂẴ������Ǘ������C�u�����ɔC����
	z.zalloc = Z_NULL;
	z.zfree = Z_NULL;
	z.opaque = Z_NULL;

	z.next_in = Z_NULL;
	z.avail_in = 0;

	if (inflateInit(&z) != Z_OK)
	{
		MessageBoxA(NULL, "inflateInit", NULL, MB_OK);
		return;
	}

	int status = Z_OK;

	std::vector<Bytef>outBuffer;
	outBuffer.resize(OUT_BUFFER_SIZE);
	z.next_out = outBuffer.data();            // �o�̓|�C���^
	z.avail_out = OUT_BUFFER_SIZE;        // �o�̓o�b�t�@�c��
	status = Z_OK;

	z.next_in = inBuffer.data();      // ���̓|�C���^�����ɖ߂�
	ifs.read((char*)inBuffer.data(), _size);
	z.avail_in = _size;				// �f�[�^��ǂ�

	while (status != Z_STREAM_END) {

		status = inflate(&z, Z_NO_FLUSH);

		if (status == Z_STREAM_END) break; // ����
		if (status != Z_OK) {   // �G���[
			MessageBoxA(NULL, "inflate", NULL, MB_OK);
			return;
		}
		if (z.avail_out == 0) { // �o�̓o�b�t�@���s�����
			size_t size = outBuffer.size();
			outBuffer.resize(size + OUT_BUFFER_SIZE);
			z.next_out = outBuffer.data() + size;		// �o�̓|�C���^�𑝂₵���������܂ňړ�
			z.avail_out = OUT_BUFFER_SIZE;				// �o�̓o�b�t�@�c�ʂ����ɖ߂�
		}
	}

	_size = outBuffer.size() - z.avail_out;

	_data = new char[_size + 1];

	for (int i = 0; i < _size; ++i)
	{
		_data[i] = (char)outBuffer[i];
	}

	_data[_size] = '\0';

	// ��n��
	if (inflateEnd(&z) != Z_OK)
	{
		MessageBoxA(NULL, "inflateEnd", NULL, MB_OK);
		return;
	}

	_success = true;
}

ZFile::ZFile(const std::string& fileName, char* data, size_t size)
	:_filename(fileName)
	, _data(nullptr)
	, _size(size)
	, _success(false)
{
	//�f�[�^����
	_data = new char[size];
	memcpy(_data, data, size);

	std::ofstream ofs(fileName, std::ios::out | std::ios::binary);

	if (!ofs)
	{
		MessageBox(NULL, TEXT("file open error"), NULL, MB_OK);
		return;
	}

	z_stream z;

	std::vector<Bytef>outBuffer(OUT_BUFFER_SIZE);

	// ���ׂẴ������Ǘ������C�u�����ɔC����
	z.zalloc = Z_NULL;
	z.zfree = Z_NULL;
	z.opaque = Z_NULL;

	// ������  ��2�����͈��k�̓x���B0�`9 �͈̔͂̐����ŁC0 �͖����k, Z_DEFAULT_COMPRESSION (= 6) ���W��
	if (deflateInit(&z, Z_BEST_COMPRESSION) != Z_OK)
	{
		MessageBox(NULL, TEXT("deflateInit:Error"), NULL, MB_OK);
	}

	int flush = Z_NO_FLUSH, status = 0;

	z.avail_in = 0;                         // ���̓o�b�t�@���̃f�[�^�̃o�C�g��
	z.next_out = outBuffer.data();            // �o�̓|�C���^
	z.avail_out = OUT_BUFFER_SIZE;        // �o�̓o�b�t�@�̃T�C�Y

	z.next_in = (Bytef*)_data;      // ���̓|�C���^�����ɖ߂�
	z.avail_in = _size;				// �f�[�^��ǂ�
	while (true)
	{
		if (z.avail_in == 0) {  // ���͂��s�����
			flush = Z_FINISH;
		}

		status = deflate(&z, flush);

		//�I��
		if (status == Z_STREAM_END)break;
		else if (status != Z_OK)
		{
			MessageBox(NULL, TEXT("deflate:Error"), NULL, MB_OK);
			return;
		}
		//�o�͎c�ʂ�0�Ȃ�
		if (z.avail_out == 0) {
			size_t size = outBuffer.size();
			outBuffer.resize(size + OUT_BUFFER_SIZE);
			z.next_out = outBuffer.data() + size;		// �o�̓|�C���^�𑝂₵���������܂ňړ�
			z.avail_out = OUT_BUFFER_SIZE;				// �o�̓o�b�t�@�c�ʂ����ɖ߂�
		}
	}

	_size = outBuffer.size() - z.avail_out;
	//�o��
	ofs.write((char*)outBuffer.data(), _size);

	if (deflateEnd(&z) == Z_DATA_ERROR) {
		MessageBox(NULL, TEXT("deflateEnd:Error"), NULL, MB_OK);
		return;
	}

	_success = true;
}

ZFile::~ZFile()
{
	SAFE_DELETE(_data);
}