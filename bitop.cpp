#include "bitop.h"

#include <bitset>
#include <algorithm>
#include <iostream>
#include <math.h>    

using std::vector;
using std::string;


bitop::bitop()
{
}


bitop::~bitop()
{
}


//Leggo n bytes 
string bitop::read_data(unsigned int index, unsigned int n_bytes, bool littlend) {

	string read_s;

	for (size_t i = 0; i < n_bytes; i++) {
		if (littlend) {
			read_s = std::bitset<8>(data[index + i]).to_string() + read_s;
		}
		else {
			read_s += std::bitset<8>(data[index + i]).to_string();
		}
		
	}

	return read_s;

}


//Leggo un byte (8 bit) alla volta e lo salvo come elemento di un vettore
void bitop::read(string path) {


	std::ifstream is(path, std::ios::binary);
	if (!is) {
		std::cout << "Errore lettura file \n";
	}

	is.unsetf(std::ifstream::skipws);

	while (is.good()) {
		data.push_back(is.get());
	}
	

	is.close();

}

void bitop::write(string path) {

	std::ofstream fp;
	fp.open(path, std::ios::out | std::ios::binary);
	std::copy(data.begin(), data.end(), std::ostream_iterator<uint8_t>(fp));

}

void bitop::write(vector<uint8_t> &mydata, string path) {

	std::ofstream fp;
	fp.open(path, std::ios::out | std::ios::binary);
	std::copy(mydata.begin(), mydata.end(), std::ostream_iterator<uint8_t>(fp));

}



void writedata(vector<uint8_t> &ppmdata, string str){

	for (size_t i = 0; i < str.size(); i++) {
		ppmdata.push_back(str[i]);
	}

}

void writedata(vector<uint8_t> &ppmdata, char str) {
	
	ppmdata.push_back(str);	

}


void bitop::write_ppm(string path, vector<uint8_t>&r, vector<uint8_t>&g, vector<uint8_t>&b, unsigned int width, unsigned int height) {

	vector<uint8_t> ppmdata;
	writedata(ppmdata, "P6\n" );
	writedata(ppmdata, "# ppm\n");
	writedata(ppmdata, std::to_string(width) + " ");
	writedata(ppmdata, std::to_string(height) + '\n');
	writedata(ppmdata, "255\n");
	
	for (size_t i = 0; i < r.size(); i++) {
		ppmdata.push_back(r[i]);
		ppmdata.push_back(g[i]);
		ppmdata.push_back(b[i]);
	}

	write(ppmdata, path);
}

void bitop::parse_bmp(vector<uint8_t>&r, vector<uint8_t>&g, vector<uint8_t>&b,unsigned int &w, unsigned int &h) {

	unsigned int index = 0;
	float test;

	string bpp_s = read_data(28, 2, true);
	unsigned int bpp = std::bitset<16>(bpp_s).to_ulong();
  std::cout << "bpp: " << bpp << std::endl;

	string width_s = read_data(18, 4, true);
	w = std::bitset<32>(width_s).to_ulong();
  std::cout << "width: " << w << std::endl;

	string hei_s = read_data(22, 4, true);
	h = std::bitset<32>(hei_s).to_ulong();
  std::cout << "heigth: " << h << std::endl;
  
	string offset_s = read_data(10, 4, true);
  unsigned int offset = std::bitset<32>(offset_s).to_ulong();
  
  index = offset;

  if (bpp == 32) {

    for (size_t j = 0; j < h; j++) {
        for (size_t i = 0; i < w; i++) {
          
          string b_pix = read_data(index, 1, false);
          unsigned int b_pixel = std::bitset<8>(b_pix).to_ulong();
          b.push_back(b_pixel);
          index += 1;
          
          string g_pix = read_data(index, 1, false);
          unsigned int g_pixel = std::bitset<8>(g_pix).to_ulong();
          g.push_back(g_pixel);
          index += 1;
          
          string r_pix = read_data(index, 1, false);
          unsigned int r_pixel = std::bitset<8>(r_pix).to_ulong();
          r.push_back(r_pixel);
          index += 1;
          
          // skip alpha
          index += 1;
          
        }
        index += (4 - (((w * bpp + 7) / 8) % 4)) % 4;
    }
  }
	else if (bpp == 24) {

		for(size_t j = 0; j < h; j++) {
			for (size_t i = 0; i < w; i++) {
				string b_pix = read_data(index, 1, false);
				unsigned int b_pixel = std::bitset<8>(b_pix).to_ulong();
				b.push_back(b_pixel);
				index += 1;

				string g_pix = read_data(index, 1, false);
				unsigned int g_pixel = std::bitset<8>(g_pix).to_ulong();
				g.push_back(g_pixel);
				index += 1;

				string r_pix = read_data(index, 1, false);
				unsigned int r_pixel = std::bitset<8>(r_pix).to_ulong();
				r.push_back(r_pixel);
				index += 1;
			}
		  index += (4 - (((w * bpp + 7) / 8) % 4)) % 4;
		}
	}
	else if (bpp == 8) {
 
    index -= 1024;
		//color table
		vector<uint8_t> color_r, color_b, color_g;
		string ncolors_s = read_data(46, 4, true);
		unsigned int ncolors = std::bitset<32>(ncolors_s).to_ulong();
		create_colorTable(color_r, color_b, color_g, ncolors, index, bpp);
	
		for (size_t j = 0; j < h; j++) {
			for (size_t i = 0; i < w; i++) {
				string pix = read_data(index, 1, false);
				unsigned int pixel = std::bitset<8>(pix).to_ulong();
				r.push_back(color_r[pixel]);
				g.push_back(color_g[pixel]);
				b.push_back(color_b[pixel]);
				index += 1;
			}
		  index += (4 - (((w * bpp + 7) / 8) % 4)) % 4;
		}
	}
	else if (bpp == 4) {
 
    index -= 1024;
		//color table
		vector<uint8_t> color_r, color_b, color_g;
		string ncolors_s = read_data(46, 4, true);
		unsigned int ncolors = std::bitset<32>(ncolors_s).to_ulong();
		create_colorTable(color_r, color_b, color_g, ncolors, index, bpp);

		bool myflag = false;
		string test = read_data(index - 1, 1, false);

		for (size_t j = 0; j < h; j++) {
			for (size_t i = 0; i < w; i++) {
				string pix = read_data(index, 1, false);
				unsigned int pixel;
				if (myflag) {
					 pixel = std::bitset<4>(pix.substr(4, 8)).to_ulong();
				}
				else {
					pixel = std::bitset<4>(pix.substr(0, 4)).to_ulong();
				}

				r.push_back(color_r[pixel]);
				g.push_back(color_g[pixel]);
				b.push_back(color_b[pixel]);
				
				if (myflag) {
					index += 1;
					myflag = false;
				}
				else {myflag = true;}
				
			}
		  index += (4 - (((w * bpp + 7) / 8) % 4)) % 4;
			myflag = false;
		}
	}
	else if (bpp == 1) {
 
    index -= 1024;
		//color table
		vector<uint8_t> color_r, color_b, color_g;
		string ncolors_s = read_data(46, 4, true);
		unsigned int ncolors = std::bitset<32>(ncolors_s).to_ulong();
		create_colorTable(color_r, color_b, color_g, ncolors, index, bpp);

		unsigned int padding = ((32 - ((w * bpp) % 32))*h);
		stream = read_data(index, round(((w*h)+padding)/8), false);

		for (size_t j = 0; j < h; j++) {
			for (size_t i = 0; i < w; i++) {
				unsigned int pixel = stoi(stream.substr(0, 1));
				stream = stream.substr(1, stream.size() - 1);

				r.push_back(color_r[pixel]);
				g.push_back(color_g[pixel]);
				b.push_back(color_b[pixel]);
			}
		  index += (4 - (((w * bpp + 7) / 8) % 4)) % 4;
		}
		
	}

	//Reverse row
	vector<uint8_t>r2, g2, b2;
	
	for (size_t j = h; j > 0; j--) {
		for (size_t i = 0; i < w; i++) {
			r2.push_back(r[((j - 1)*w) + i]);
			g2.push_back(g[((j - 1)*w) + i]);
			b2.push_back(b[((j - 1)*w) + i]);
		}
	}

	r = r2;
	g = g2;
	b = b2;
	
}


void bitop::create_colorTable(vector<uint8_t>&color_r, vector<uint8_t>&color_g, vector<uint8_t>&color_b,unsigned int ncolors, unsigned int &index, unsigned int bpp) {
	
	//color table
	if (ncolors == 0) {
		ncolors = pow(2, bpp);
	}

	for (size_t i = 0; i < ncolors; i++) {
		string b_pix = read_data(index, 1, false);
		unsigned int b_pixel = std::bitset<8>(b_pix).to_ulong();
		color_b.push_back(b_pixel);
		index += 1;

		string g_pix = read_data(index, 1, false);
		unsigned int g_pixel = std::bitset<8>(g_pix).to_ulong();
		color_g.push_back(g_pixel);
		index += 1;

		string r_pix = read_data(index, 1, false);
		unsigned int r_pixel = std::bitset<8>(r_pix).to_ulong();
		color_r.push_back(r_pixel);
		index += 2;
	}
}