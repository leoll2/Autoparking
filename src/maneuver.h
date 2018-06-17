#ifndef MANEUVER_H
#define MANEUVER_H

#define NUM_RADI		2
#define NUM_DISP		3
#define MIN_RADI		500
#define MIN_DISP		50

class Maneuver {
	private:
		bool forward;			//true->forward, false->backwards
		int8_t spin;			//1->clockwise, 0->straight, (-1)->counterclowckise
		uint16_t turning_radius;//MIN_RADI, 2*MIN_RADI
		uint16_t displacement;	//50, 100, 150
		void decode_maneuver(unsigned int code);
	public:
		unsigned int encode_maneuver();
		friend std::ostream& operator<<(std::ostream& os, const Maneuver& m);
		Maneuver(unsigned int code);
};

#endif
