#include "c74_min.h"
#include "plaits/dsp/voice.h"
#include "dsp/samplerate.hpp"
#include "dsp/ringbuffer.hpp"

using namespace c74::min;

class plaits_tilde : public object<plaits_tilde>, public sample_operator<5, 2> {

private:
	plaits::Voice       voice;
	plaits::Patch       patch;
	plaits::Modulations modulations;
	char                shared_buffer[16384];

	rack::SampleRateConverter<2>                outputSrc;
	rack::DoubleRingBuffer<rack::Frame<2>, 256> outputBuffer;
	bool                                        lpg = false;

public:
	MIN_DESCRIPTION {"Mutable Instruments plaits wrapped as max/msp external."
					 "see https://mutable-instruments.net/modules/plaits/"};
	MIN_TAGS {"audio"};
	MIN_AUTHOR {"Stephen Lumenta"};
	MIN_RELATED {""};

	inlet<> in1 {this, "(signal) engine modulation input", "signal"};
	inlet<> in2 {this, "(signal) note modulation input", "signal"};
	inlet<> in3 {this, "(signal) freq modulation input", "signal"};
	inlet<> in4 {this, "(signal) harmonics modulation input", "signal"};
	inlet<> in5 {this, "(signal) timbre modulation input", "signal"};

	outlet<> out1 {this, "(signal) main out", "signal"};
	outlet<> out2 {this, "(signal) aux out", "signal"};

	plaits_tilde() {
		memset(shared_buffer, 0, sizeof(shared_buffer));
		stmlib::BufferAllocator allocator(shared_buffer, sizeof(shared_buffer));
		voice.Init(&allocator);

		memset(&patch, 0, sizeof(patch));
		memset(&modulations, 0, sizeof(modulations));
		onReset();
	}

#pragma mark -
#pragma mark helpers

	void onReset() {
		patch.engine     = 0;
		patch.lpg_colour = 0.5f;
		patch.decay      = 0.5f;
	}

#pragma mark -
#pragma mark attributes

	attribute<int, threadsafe::no, limit::clamp> m_engine_index {this, "engine_index", 0, description {"Model selection"}, range {0, 15}};

	attribute<number, threadsafe::no, limit::clamp> m_pitch {this, "pitch", 0, description {"Coarse frequency control."}, range {-8, 8}};

	attribute<number, threadsafe::no, limit::clamp> m_harmonics {this, "harmonics", 0.5,
		description {"Harmonics controls the frequency spread or the balance between the various constituents of the tone."}, range {0, 1}};

	attribute<number, threadsafe::no, limit::clamp> m_timbre {
		this, "timbre", 0.5, description {"Timbre sweeps the spectral content from dark/sparse to bright/dense."}, range {0, 1}};

	attribute<number, threadsafe::no, limit::clamp> m_morph {
		this, "morph", 0.5, description {"Morph explores lateral timbral variations."}, range {0, 1}};

#pragma mark -
#pragma mark dsp

	samples<2> operator()(sample engine_input, sample note_input, sample freq_input, sample harmonics_input, sample timbre_input) {
		if (outputBuffer.empty()) {
			constexpr int blockSize = 12;

			// Model buttons
			patch.engine    = m_engine_index;
			patch.note      = 60.f + m_pitch * 12.f;
			patch.harmonics = m_harmonics;

			if (!lpg) {
				patch.timbre = m_timbre;
				patch.morph  = m_morph;
			}
			else {
				patch.lpg_colour = m_timbre;
				patch.decay      = m_morph;
			}

			// TODO: modulation amount
			patch.frequency_modulation_amount = 0;
			patch.timbre_modulation_amount    = 0;
			patch.morph_modulation_amount     = 0;

			// TODO Update modulations

			if (in1.has_signal_connection()) {
			}


			modulations.engine    = engine_input;
			modulations.note      = note_input * 12.f;
			modulations.frequency = freq_input * 6.f;
			modulations.harmonics = harmonics_input / 5.f;
			modulations.timbre    = timbre_input / 8.f;
			//            modulations.morph = inputs[MORPH_INPUT].value / 8.f;
			// Triggers at around 0.7 V
			//            modulations.trigger = inputs[TRIGGER_INPUT].value / 3.f;
			//            modulations.level = inputs[LEVEL_INPUT].value / 8.f;

			// TODO can check cord status in max
			modulations.frequency_patched = false;
			modulations.timbre_patched    = false;
			modulations.morph_patched     = false;
			modulations.trigger_patched   = false;
			modulations.level_patched     = false;

			// Render frames
			plaits::Voice::Frame output[blockSize];
			voice.Render(patch, modulations, output, blockSize);

			// Convert output to frames
			rack::Frame<2> outputFrames[blockSize];
			for (int i = 0; i < blockSize; i++) {
				outputFrames[i].samples[0] = output[i].out / 32768.f;
				outputFrames[i].samples[1] = output[i].aux / 32768.f;
			}

			// Convert output
			outputSrc.setRates(48000, samplerate());
			int inLen  = blockSize;
			int outLen = outputBuffer.capacity();
			outputSrc.process(outputFrames, &inLen, outputBuffer.endData(), &outLen);
			outputBuffer.endIncr(outLen);
		}

		// Set output
		if (!outputBuffer.empty()) {
			rack::Frame<2> outputFrame = outputBuffer.shift();
			// Inverting op-amp on outputs
			return {
				-outputFrame.samples[0],
				-outputFrame.samples[1],
			};
		}
		return {0, 0};
	}
};

MIN_EXTERNAL(plaits_tilde);
;
