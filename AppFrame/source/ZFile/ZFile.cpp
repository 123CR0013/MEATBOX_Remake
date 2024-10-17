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

	//開いたファイルのサイズを取得
	ifs.seekg(0, std::ios_base::end);

	_size = (size_t)ifs.tellg();

	ifs.seekg(0, std::ios_base::beg);

	std::vector<Bytef>inBuffer(_size);
	// すべてのメモリ管理をライブラリに任せる
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
	z.next_out = outBuffer.data();            // 出力ポインタ
	z.avail_out = OUT_BUFFER_SIZE;        // 出力バッファ残量
	status = Z_OK;

	z.next_in = inBuffer.data();      // 入力ポインタを元に戻す
	ifs.read((char*)inBuffer.data(), _size);
	z.avail_in = _size;				// データを読む

	while (status != Z_STREAM_END) {

		status = inflate(&z, Z_NO_FLUSH);

		if (status == Z_STREAM_END) break; // 完了
		if (status != Z_OK) {   // エラー
			MessageBoxA(NULL, "inflate", NULL, MB_OK);
			return;
		}
		if (z.avail_out == 0) { // 出力バッファが尽きれば
			size_t size = outBuffer.size();
			outBuffer.resize(size + OUT_BUFFER_SIZE);
			z.next_out = outBuffer.data() + size;		// 出力ポインタを増やしたメモリまで移動
			z.avail_out = OUT_BUFFER_SIZE;				// 出力バッファ残量を元に戻す
		}
	}

	_size = outBuffer.size() - z.avail_out;

	_data = new char[_size + 1];

	for (int i = 0; i < _size; ++i)
	{
		_data[i] = (char)outBuffer[i];
	}

	_data[_size] = '\0';

	// 後始末
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
	//データ複製
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

	// すべてのメモリ管理をライブラリに任せる
	z.zalloc = Z_NULL;
	z.zfree = Z_NULL;
	z.opaque = Z_NULL;

	// 初期化  第2引数は圧縮の度合。0～9 の範囲の整数で，0 は無圧縮, Z_DEFAULT_COMPRESSION (= 6) が標準
	if (deflateInit(&z, Z_BEST_COMPRESSION) != Z_OK)
	{
		MessageBox(NULL, TEXT("deflateInit:Error"), NULL, MB_OK);
	}

	int flush = Z_NO_FLUSH, status = 0;

	z.avail_in = 0;                         // 入力バッファ中のデータのバイト数
	z.next_out = outBuffer.data();            // 出力ポインタ
	z.avail_out = OUT_BUFFER_SIZE;        // 出力バッファのサイズ

	z.next_in = (Bytef*)_data;      // 入力ポインタを元に戻す
	z.avail_in = _size;				// データを読む
	while (true)
	{
		if (z.avail_in == 0) {  // 入力が尽きれば
			flush = Z_FINISH;
		}

		status = deflate(&z, flush);

		//終了
		if (status == Z_STREAM_END)break;
		else if (status != Z_OK)
		{
			MessageBox(NULL, TEXT("deflate:Error"), NULL, MB_OK);
			return;
		}
		//出力残量が0なら
		if (z.avail_out == 0) {
			size_t size = outBuffer.size();
			outBuffer.resize(size + OUT_BUFFER_SIZE);
			z.next_out = outBuffer.data() + size;		// 出力ポインタを増やしたメモリまで移動
			z.avail_out = OUT_BUFFER_SIZE;				// 出力バッファ残量を元に戻す
		}
	}

	_size = outBuffer.size() - z.avail_out;
	//出力
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