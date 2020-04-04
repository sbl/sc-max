#include "c74_min.h"
#include "Waveshaping.hpp"

using namespace c74::min;

class warps_tilde : public object<warps_tilde>, public sample_operator<1, 1> {

private:
	HardClipper clipper;
	Wavefolder  folder[4];

public:
	MIN_DESCRIPTION {"A waveshaping folder."
					 "Original implementation by F. Esqueda https://github.com/fabianesqueda"};
	MIN_TAGS {"audio"};
	MIN_AUTHOR {"Stephen Lumenta"};
	MIN_RELATED {""};

	inlet<>  input {this, "(signal) Folding input", "signal"};
	outlet<> output {this, "(signal) Folded output", "signal"};

#pragma mark -
#pragma mark attributes
	attribute<number, threadsafe::no, limit::fold> m_fold {
		this,
		"fold",
		1,
		description {"The amount of folding to apply."},
		range {1, 100},
	};

	attribute<number, threadsafe::no, limit::clamp> m_symmetry {
		this,
		"symmetry",
		0,
		description {"A symmetry offset."},
		range {0, 1},
	};

#pragma mark -
#pragma mark dsp

	sample operator()(sample input) {
		auto foldedOutput = input * m_fold + m_symmetry;
		for (auto i = 0; i < 4; ++i) {
			folder[i].process(foldedOutput);
			foldedOutput = folder[i].getFoldedOutput();
		}
		clipper.process(foldedOutput);
		return clipper.getClippedOutput();
	}
};

MIN_EXTERNAL(warps_tilde);
