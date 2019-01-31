
// Simple branch predictor using 2-bit saturating counters

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include <array>

// Maximum state allowed is 128Kbits

typedef std::pair<unsigned, unsigned> TracePoint;

class PredictorBase {
public:
	virtual bool predict(unsigned int address) = 0;
	virtual void post_predict(unsigned int address, unsigned int taken) = 0;
	float test(std::vector<TracePoint> data) {
		unsigned int correct = 0;
		for (TracePoint& trace : data) {
			unsigned int address = trace.first;
			unsigned int taken = trace.second;
			if (this->predict(address) == taken) {
				++correct;
			}
			this->post_predict(address, taken);
		}
		return correct;
	}
};

class TwoBitSaturating : public PredictorBase {
private:
	// 64K entries * 2-bits = 128Kbits
	static const unsigned int TABLE_SIZE = 1024 * 64;
	std::array<char, TABLE_SIZE> bht = {0};
public:
	virtual bool predict(unsigned int address) {
		unsigned int entry = (address / 2) % TABLE_SIZE;
		if ((entry < 0) || (entry > TABLE_SIZE)) {
			printf("Error!\n");
		}
		return bht[entry] >= 2;
	}
	virtual void post_predict(unsigned int address, unsigned int taken) {
		unsigned int entry = (address / 2) % TABLE_SIZE;
		unsigned int mod = 0;
		if (taken && bht[entry] < 3) mod = 1;
		if (!taken && bht[entry] > 0) mod = -1;
		bht[entry] += mod;
	}
};

int main() {
	std::vector<TracePoint> data;
	TracePoint trace_point;
 	while (scanf("%x %d", &trace_point.first, &trace_point.second) > 0) {
		data.push_back(trace_point);
	}
	printf("Branch count = %i\n", data.size());
	TwoBitSaturating two_bit_saturating;
	printf (
		"2-bit saturating counter = %.2f%%\n",
		two_bit_saturating.test(data) / data.size() * 100.0f
	);
}
