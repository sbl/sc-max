#include "c74_min.h"
#include "warps/dsp/modulator.h"

using namespace c74::min;

inline short to_shortframe(double v) {
	return (short(v * 16384));
}
inline double from_shortframe(short v) {
	return (double(v) / 16384);
}

class falter_tilde : public object<falter_tilde>, public sample_operator<2, 2> {

private:
	int                  frame      = 0;
	constexpr static int frame_size = 60;

	warps::Modulator modulator;

	warps::ShortFrame input_frames[frame_size];
	warps::ShortFrame output_frames[frame_size];

public:
	MIN_DESCRIPTION {"Mutable Instruments warps wrapped as max/msp. <br />"
					 "see https://mutable-instruments.net/modules/warps/"};
	MIN_TAGS {"audio"};
	MIN_AUTHOR {"Stephen Lumenta"};
	MIN_RELATED {""};

	inlet<> in1 {this, "(signal) external carrier signal"};
	inlet<> in2 {this, "(signal) external modulator signal"};

	outlet<> out1 {this, "(signal) main out", "signal"};
	outlet<> out2 {this, "(signal) aux out", "signal"};

	falter_tilde() {
		modulator.Init(samplerate());
		modulator.set_bypass(false);
		modulator.set_easter_egg(false);
	}

#pragma mark -
#pragma mark attributes

	attribute<number, threadsafe::no, limit::clamp> m_algorithm {this, "algorithm", 0,
		description {"Selects which signal processing operation is performed on the carrier and modulator."}, range {0, 8}};

	attribute<int, threadsafe::no, limit::clamp> m_oscillator_state {
		this, "oscillator_state", 0, description {"Enables the internal oscillator and selects its waveform"}, range {0, 3}};

	attribute<number, threadsafe::no, limit::clamp> m_timbre {this, "timbre", 0.5,
		description {"controls the intensity of the high harmonics created by cross-modulation - or provides another dimension of tone "
					 "control for some algorithms."},
		range {0, 1}};

	attribute<number, threadsafe::no, limit::clamp> m_carrier_drive {
		this,
		"carrier_drive",
		0.5,
		description {"Control the amplitude of the carrier."},
		range {0, 1},
	};

	attribute<number, threadsafe::no, limit::clamp> m_modulator_drive {
		this,
		"modulator_drive",
		0.5,
		description {"Control the amplitude of the modulator."},
		range {0, 1},
	};

	attribute<number, threadsafe::no, limit::clamp> m_pitch {
		this,
		"pitch",
		60,
		description {"Pitch of the internal oscillator."},
		range {0, 127},
	};

#pragma mark -
#pragma mark dsp

	samples<2> operator()(sample in_carrier, sample in_modulator) {
		auto p = modulator.mutable_parameters();

		if (++frame >= frame_size) {
			frame = 0;

			p->carrier_shape    = m_oscillator_state;    // 0 - 3
			p->channel_drive[0] = clamp<number>(m_carrier_drive, 0, 1);
			p->channel_drive[1] = clamp<number>(m_modulator_drive, 0, 1);

			p->modulation_algorithm = clamp<number>(m_algorithm / 8.0, 0, 1);
			p->modulation_parameter = clamp<number>(m_timbre, 0, 1);

			p->note = clamp<number>(m_pitch, 0, 128);

			modulator.Process(input_frames, output_frames, frame_size);
		}

		input_frames[frame].l = to_shortframe(in_carrier);
		input_frames[frame].r = to_shortframe(in_modulator);

		return {from_shortframe(output_frames[frame].l) * 0.5, from_shortframe(output_frames[frame].r) * 0.5};
	}
};

MIN_EXTERNAL(falter_tilde);
