import numpy as np
import matplotlib.pyplot as plt
from cycler import cycler
import scipy.fftpack


def norm_unit_bi(x):
    # normalize a numpy vector to bipolar unit range [-1, 1]
    m = max(abs(np.min(x)), np.max(x))
    m = max(m, 0.001)
    return x / m


def test_shaper(func, a, gain=1, norm=True):
    name = func.__name__
    # test a shaper function by plotting its transfer function,
    # and its spectrum when applied to a sine wave
    x = np.arange(-1 * gain, gain, 1 / 4096)
    n = len(x)
    m = len(a)
    print(n)
    y = np.zeros((n, m))
    z = np.zeros((n, m))
    mag = np.zeros((int((n * 2) + 1), m))

    for i in range(n):
        for j in range(m):
            y[i, j] = func(x[i], a[j])

    plt.rc('axes', prop_cycle=(cycler(color=reversed(plt.cm.copper(np.linspace(0.1, 0.9, m))))))

    fig, ax = plt.subplots(3, 1)

    phase = 0
    inc = 1 / (2 ** 2 * np.pi)
    for i in range(n):
        for j in range(m):
            z[i, j] = func(np.sin(phase) * gain, a[j])
        phase = phase + inc
    print(func.__name__)
    for j in reversed(range(m)):
        ax[0].plot(norm_unit_bi(y[:, j]), linewidth=0.5)
        ax[1].plot(norm_unit_bi(z[:300, j]), linewidth=0.5)
        mag[:, j] = np.abs(scipy.fft.rfft(z[:, j], n * 4, 0, "ortho"))
        db = np.log10(mag[:, j]) * 20
        print([np.amin(mag), np.amax(mag)])
        print([np.amin(db), np.amax(db)])
        ax[2].plot(db, linewidth=0.5)
        #ax[2].plot(mag[:,j])
    print("")
    np.save('{}_mag.npy'.format(name), mag)
    plt.savefig(name)
    plt.title(name)

    plt.show()


def shaper_tsq(x, t):
    # two-stage quadratic
    # t is softclip threshold in (0, 1) exclusive
    g = 2.0
    ax = abs(x)
    sx = np.sign(x)
    t_1 = t - 1.
    a = g / (2. * t_1)
    b = g * t - (a * t_1 * t_1)
    if ax > t:
        q = ax - 1.
        y = a * q * q + b
        return y * sx / 2
    else:
        return x * g / 2


def shaper_bram(x, a):
    ax = np.abs(x)
    return x * (ax + a) / (x * x + (a - 1) * ax + 1)


def shaper_bram2(x, a):
    ax = np.abs(x)
    sx = np.sign(x)

    if ax < a:
        return x
    if ax > 1:
        return sx * (a + 1) / 2

    return sx * (a + (ax - a) / (1 + ((ax - a) / (1 - a)) ** 2))


def shaper_cubic(x, a):
    g = 2 ** a
    x = x * g
    y = ((3 * x / 2) - ((x * x * x) / 2))
    return y / g


def shaper_expo(x, a):
    sx = np.sign(x)
    return sx * (1 - (np.abs(x - sx) ** a))


def shaper_sine(x, a):
    # param = logarithmic pre-gain
    x = x * (2 ** a)
    return np.sin(np.pi * x / 2)


def shaper_reciprocal(x, a):
    # param = pregain
    x = x * (2 ** a)
    return np.sign(x) * (1 - (1 / (np.abs(x) + 1)))


def shaper_tanh(x, a):
    # param = pregain
    x = x * (2 ** a)
    return np.tanh(x)


test_shaper(shaper_bram, [1, 2, 3, 5, 7, 8, 9, 10])
test_shaper(shaper_bram2, [0.999, 0.8, 0.7, 0.5, 0.3, 0.15, 0.05, 0.001])
test_shaper(shaper_tsq, [0.9, 0.8, 0.7, 0.5, 0.3, 0.2, 0.1, 0.001], 1)
test_shaper(shaper_cubic, [-1, -0.5, 0, 0.25, 0.5], 1)
test_shaper(shaper_expo, [1, 2, 3, 4, 5, 6], 1)
test_shaper(shaper_sine, [-1, -0.5, -0.25, 0, 0.25, 0.5], 1)
test_shaper(shaper_reciprocal, [1, 2, 3, 4, 6, 8, 9, 10, 11, 12], 1)
test_shaper(shaper_tanh, [0, 0.5, 1, 2, 3, 4], 1)
