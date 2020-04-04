#pragma once

#include <cstdint>
#include <random>


namespace bruits {
	/**
	 Fast number generator engine. See http://xoshiro.di.unimi.it/
	 */
	struct xoroshiro_engine {
		using result_type = uint64_t;

		xoroshiro_engine() {
			std::random_device rd;
			state[0] = rd();
			state[1] = rd();
		}

		constexpr static result_type min() {
			return 0;
		}

		constexpr static result_type max() {
			return -1;
		}

		result_type operator()() {
			const uint64_t s0     = state[0];
			uint64_t       s1     = state[1];
			const uint64_t result = s0 + s1;

			s1 ^= s0;
			state[0] = rotl(s0, 55) ^ s1 ^ (s1 << 14);    // a, b
			state[1] = rotl(s1, 36);                      // c

			return result;
		}

	private:
		uint64_t state[2];

		static inline uint64_t rotl(const uint64_t x, int k) {
			return (x << k) | (x >> (64 - k));
		}
	};

	// random number generator
	struct rng {
	private:
		xoroshiro_engine engine {};

	public:
		double rand() {
			return engine();
		}

		double uniform() {
			std::uniform_real_distribution<> dist {0, 1};
			return dist(engine);
		}

		double cauchy(double a, double b) {
			std::cauchy_distribution<> dist {a, b};
			return dist(engine);
		}

		double normal(double m, double s) {
			std::normal_distribution<> dist {m, s};
			return dist(engine);
		}

		double lognormal(double m, double s) {
			std::lognormal_distribution<> dist {m, s};
			return dist(engine);
		}

		double chisquared(double n) {
			std::chi_squared_distribution<> dist {n};
			return dist(engine);
		}

		double exponential(double lambda) {
			std::exponential_distribution<> dist {lambda};
			return dist(engine);
		}

		double extreme(double a, double b) {
			std::extreme_value_distribution<> dist {a, b};
			return dist(engine);
		}
	};

}    // namespace bruits
