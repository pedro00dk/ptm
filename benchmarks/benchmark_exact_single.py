import random
import subprocess
import timeit

import matplotlib.pyplot as plt


def get_text_patterns(text_file, pattern_sizes):
    current_pattern_size_index = 0
    patterns = []
    with open(text_file, 'r', encoding='latin1') as text:
        for line in text:
            pattern_size = pattern_sizes[current_pattern_size_index]
            size_difference = len(line) - pattern_size
            if size_difference > 0:
                start_index = random.randrange(size_difference)
                pattern = line[start_index: start_index + pattern_size]
                if '\n' in pattern or '%' in pattern or '`' in pattern or '"' in pattern or "'" in pattern or '[' in pattern or ']' in pattern:
                    continue
                patterns.append(pattern)
                current_pattern_size_index += 1
            if current_pattern_size_index >= len(pattern_sizes):
                break
    return patterns


def run(algorithm, pattern, text_file):
    if algorithm == "grep":
        command = 'grep "%s" "%s" -o | wc -l' % (pattern, text_file)
    else:
        command = './ptm -a %s -c "%s" "%s"' % (algorithm, pattern, text_file)
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    process.wait()
    return process.stdout.read()


def get_run_time(algorithm, pattern, text_file, runs):
    timer = timeit.Timer(lambda: run(algorithm, pattern, text_file))
    time = timer.timeit(number=runs) / runs
    return time


def main():
    gen_text_source = 'gentext.c'
    gen_text = 'gentext.o'
    text_file = 'text.txt'
    text_size = 100
    text_sigma = 255
    compile_command = 'gcc -O2 %s -o %s' % (gen_text_source, gen_text)
    subprocess.call(compile_command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    run_command = './%s %s %d %d' % (gen_text, text_file, text_size, text_sigma)
    subprocess.call(run_command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    print('text file -> name: %s, size: %d mb, ab size: %d' % (text_file, text_size, text_sigma))

    patterns = get_text_patterns(text_file, [*range(5, 101, 5)])
    print('patterns -> count: %d:' % len(patterns))
    for pattern in patterns:
        print('(%d): %s' % (len(pattern), pattern))

    print('compiling program')
    ptm_source = '../src/ptm.cpp'
    ptm = 'ptm'
    compile_command = '%s %s %s -o %s' % ('g++', '-std=c++11 -O2', ptm_source, ptm)
    subprocess.call(compile_command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

    exact_algorithms = ['grep', 'bf', 'kmp', 'bm', 'ac', 'so']
    print('algorithms:')
    print(exact_algorithms)

    run('grep', patterns[0], text_file)  # Just to load file

    runs = 5
    run_times = {}

    print('testing algorithms in %d runs' % runs)
    for algorithm in exact_algorithms:
        run_times[algorithm] = []
        for pattern in patterns:
            time = get_run_time(algorithm, pattern, text_file, runs)
            print('algorithm %s, pattern length %d, mean run time %f' % (algorithm, len(pattern), time))
            run_times[algorithm].append(time)
        print()

    x = [len(pattern) for pattern in patterns]
    for algorithm, results in run_times.items():
        plt.plot(x, results, label=algorithm)
    plt.xlabel('pattern size')
    plt.ylabel('time (sec)')
    plt.legend()
    plt.show()


if __name__ == "__main__":
    main()
