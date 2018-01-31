#pragma once

#include <string>
#include <vector>
#include <iterator> 
#include <iostream>
#include <fstream>


class bitop
{
	std::vector<uint8_t>data;
	std::string stream;

public:
	bitop();
	~bitop();
	void read(std::string path);
	void write(std::string path);
	void write(std::vector<uint8_t> &mydata, std::string path);
	std::string read_data(unsigned int index, unsigned int n_bytes, bool littlend);
	void parse_bmp(std::vector<uint8_t>&r, std::vector<uint8_t>&g, std::vector<uint8_t>&b, unsigned int &w, unsigned int &h);
	void write_ppm(std::string path, std::vector<uint8_t>&r, std::vector<uint8_t>&g, std::vector<uint8_t>&b, unsigned int width, unsigned int height);
	void create_colorTable(std::vector<uint8_t>&color_r, std::vector<uint8_t>&color_g, std::vector<uint8_t>&color_b, unsigned int ncolors, unsigned int &index,unsigned int bpp);

};