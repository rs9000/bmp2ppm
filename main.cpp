#include "bitop.h"
#include <bitset>

using namespace std;

int main(int argc, char *argv[]) {

	if (argc != 3) {
		cout << "Use: ./bmp2ppm <input_file> <output_file> \n";
		return 1;
	}

	bitop bitoperator;
	bitoperator.read(string(argv[1]));

	vector<uint8_t>r, g, b;
	unsigned int w, h;

	bitoperator.parse_bmp(r, g, b, w, h);
	bitoperator.write_ppm(string(argv[2]),r,g,b,w,h);

	return 0;

}