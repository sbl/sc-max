#include "c74_min.h"
#include "random.hpp"

using namespace c74::min;

class gendyn_tilde : public object<gendyn_tilde>, public vector_operator<> {

private:
	bruits::rng rng;

	static constexpr int knum = 12;

	double         m_phase = 1;
	unsigned short m_index = 0;

	double m_amp     = 0;
	double m_nextamp = 0;

	double m_dur   = 1.0;
	double m_speed = 1.0;

	double m_ampstep1[knum];
	double m_ampstep2[knum];

	double m_durstep1[knum];
	double m_durstep2[knum];

	double m_isamplerate;

public:
	MIN_DESCRIPTION {"Gendy after Xenakis. See Xenakis, Hoffmann, Lincoln."};
	MIN_TAGS {"audio, stochastic, noise"};
	MIN_AUTHOR {"Stephen Lumenta"};
	MIN_RELATED {"noise~, pink~"};

	inlet<> inlet_minfreq {this, "(signal) minfreq", "signal"};
	inlet<> inlet_maxfreq {this, "(signal) maxfreq", "signal"};
	inlet<> inlet_ampscale {this, "(signal) ampscale", "signal"};
	inlet<> inlet_durscale {this, "(signal) durscale", "signal"};

	outlet<> outlet_main {this, "(signal) gendyn", "signal"};

	gendyn_tilde() {
		m_isamplerate = 1 / samplerate();

		for (int i = 0; i < knum; i++) {
			m_ampstep1[i] = 2 * rng.uniform() - 1;
			m_ampstep2[i] = 2 * rng.uniform() - 1;

			m_durstep1[i] = 2 * rng.uniform() - 1;
			m_durstep2[i] = rng.uniform();
		}
	}

#pragma mark -
#pragma mark attributes

	attribute<number, threadsafe::no, limit::clamp> m_minfreq {
		this,
		"minfreq",
		440,
		description {"Minimum frequency of random walk"},
		range {0.01, 14000},
	};

	attribute<number, threadsafe::no, limit::clamp> m_maxfreq {
		this,
		"maxfreq",
		660,
		description {"Maximum frequency of random walk"},
		range {0.01, 14000},
	};

	attribute<int, threadsafe::no, limit::clamp> m_ampdist {
		this,
		"ampdist",
		1,
		description {"The amplitude distribution used."},
		range {0, 5},
	};

	attribute<number, threadsafe::no, limit::clamp> m_ampparam {
		this,
		"ampparam",
		0.5,
		description {"Param for the amplitude distribution function."},
		range {0.00001, 1},
	};

	attribute<number, threadsafe::no, limit::clamp> m_ampscale {
		this,
		"ampscale",
		0.5,
		description {"Amplitude scaling factor."},
		range {0, 1},
	};

	attribute<int, threadsafe::no, limit::clamp> m_durdist {
		this,
		"durdist",
		1,
		description {"The duration distribution used."},
		range {0, 5},
	};

	attribute<number, threadsafe::no, limit::clamp> m_durparam {
		this,
		"durparam",
		0.5,
		description {"Param for the duration distribution function."},
		range {0.00001, 1},
	};

	attribute<number, threadsafe::no, limit::clamp> m_durscale {
		this,
		"durscale",
		0.5,
		description {"Duration scaling factor."},
		range {0, 1},
	};

#pragma mark -
#pragma mark dsp
	message<> dspsetup {
		this,
		"dspsetup",
		[this](const c74::min::atoms& args, int inlet) -> c74::min::atoms {
			number samplerate = args[0];
			m_isamplerate     = 1.0 / samplerate;
			return {};
		},
	};

	void operator()(audio_bundle input, audio_bundle output) {
		auto in_ampscale = input.samples(2);
		auto in_durscale = input.samples(3);

		auto out_main = output.samples(0);

		auto rate    = m_dur;
		auto phase   = m_phase;
		auto amp     = m_amp;
		auto nextamp = m_nextamp;
		auto speed   = m_speed;

		for (auto i = 0; i < input.frame_count(); ++i) {
			auto ampscale = m_ampscale.get();
			if (inlet_ampscale.has_signal_connection()) {
				ampscale = wrap<double>(fabs(in_ampscale[i]), 0, 1);
			}
			auto durscale = m_durscale.get();
			if (inlet_durscale.has_signal_connection()) {
				durscale = wrap<double>(fabs(in_durscale[i]), 0, 1);
			}

			auto z = 0.0;

			if (phase >= 1) {
				phase -= 1;

				auto index = m_index;
				index      = (index + 1) % knum;
				m_index    = index;

				// amp
				amp = nextamp;

				auto ampdist      = static_cast<distro>(m_ampdist.get());
				auto ampstep      = m_ampstep1[index] + distribution(ampdist, m_ampparam);
				ampstep           = mirror(ampstep, -1.0, 1.0);
				m_ampstep1[index] = ampstep;

				nextamp           = m_ampstep2[index] + (ampscale * ampstep);
				nextamp           = mirror(nextamp, -1.0, 1.0);
				m_ampstep2[index] = nextamp;

				// dur
				auto durdist      = static_cast<distro>(m_durdist.get());
				auto durstep      = m_durstep1[index] + distribution(durdist, m_durparam);
				durstep           = mirror(durstep, -1.0, 1.0);
				m_durstep1[index] = durstep;

				rate              = m_durstep2[index] + (durscale * durstep);
				rate              = mirror(rate, 0.0, 1.0);
				m_durstep2[index] = rate;

				speed = (m_minfreq + ((m_maxfreq - m_minfreq) * rate)) * m_isamplerate;
				speed *= knum;
			}

			z = ((1.0 - phase) * amp) + (phase * nextamp);
			phase += speed;

			out_main[i] = z;
		}

		m_phase   = phase;
		m_amp     = amp;
		m_nextamp = nextamp;
		m_speed   = speed;
		m_dur     = rate;
	};

#pragma mark -
#pragma mark helpers

	// Distribution functions are not according to the original gendy
	// implementation. Those are picked by ear.
	enum class distro {
		uniform = 0,
		cauchy,
		lognormal,
		chisquared,
		exponential,
		extreme,
	};

	sample distribution(distro which, double param) {
		double z = 0;

		switch (which) {
			case distro::uniform:
				z = rng.uniform();
				break;
			case distro::cauchy:
				z = rng.cauchy(1, param);
				break;
			case distro::lognormal:
				z = rng.lognormal(0.5, param);
				break;
			case distro::chisquared:
				z = rng.chisquared(param);
				break;
			case distro::exponential:
				z = rng.exponential(param);
				break;
			case distro::extreme:
				z = rng.extreme(2 * param - 1, 1);
				break;
			default:
				break;
		}
		return 2 * z - 1.0;
	}

	sample mirror(sample in, double lower, double upper) {
		return fold(in, lower, upper);
	}
};

MIN_EXTERNAL(gendyn_tilde);
