#ifndef SCREEN_CAPTURE_AND_DECRYPT_DECRYPTOR_H
#define SCREEN_CAPTURE_AND_DECRYPT_DECRYPTOR_H
#include <string>
#include <vector>
#include <opencv2/core.hpp>


class Decryptor{
private:
	// The parameters below should be the same as the sender's 
	const static unsigned wid_ori = 1280;
	const static unsigned hei_ori = 720;
	const static unsigned w_padding = 16;	// Horizontal padding size (in pixel)
	const static unsigned h_padding = 32;	// Vertical padding size (in pixel)
	const static unsigned blocksize = 4;  // Block size of QR code

	unsigned widClient;
	unsigned heiClient;

	double w_pad;
	double h_pad;
	double step;

	unsigned w_num;
	unsigned h_num;

public:
	std::vector<int> bbox;

	Decryptor(std::vector<int> bounding, int wid, int hei) :bbox(bounding), widClient(wid), heiClient(hei) {
		double width = bbox[3] - bbox[2];
		double factor = width / double(wid_ori);
		w_pad = w_padding * factor;
		h_pad = h_padding * factor;
		step = blocksize * factor;
		w_num = (wid_ori - w_padding * 2) / blocksize;
		h_num = (hei_ori - h_padding * 2) / blocksize;
	};

	bool decode(cv::Mat &crop);
};

void start(std::string title, int widthClient, int heightClient);
#endif //SCREEN_CAPTURE_AND_DECRYPT_DECRYPTOR_H


