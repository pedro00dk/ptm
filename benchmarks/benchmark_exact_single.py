import subprocess
import timeit

import matplotlib.pyplot as plt
from matplotlib import pyplot


def compile():
    command = "g++ ../src/main.cpp -std=c++11 -O2 -o pmt"
    subprocess.call(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)


def run(pattern, text_file, algorithm, has_pattern_file=False):
    if algorithm == "grep":
        command = 'grep {0} {1} -c'.format(pattern,
                                           text_file)
    else:
        command = './pmt {0} {1} {2} -a {3} -c'.format("-p" if has_pattern_file else "",
                                                       pattern,
                                                       text_file,
                                                       algorithm)
    proc = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    proc.wait()

    return proc


def main():
    compile()

    exact_algorithms = ['bf', 'kmp', 'bm', 'ac', 'so', 'grep']
    patterns = sorted(['aba', 'abacaba', 'beauty', 'mediumsizepatterntotest', 'gentleman', 'gentlewoman'], key=len)
    text_files = ['shakespeare.txt']

    number_executions = 10
    max_pattern_len = 30

    results = {}

    for algorithm in exact_algorithms:
        results[algorithm] = [0] * (max_pattern_len + 1)

    for text_file in text_files:
        for pattern in patterns:
            pattern_len = len(pattern)
            for algorithm in exact_algorithms:
                timer = timeit.Timer(lambda: run(pattern, text_file, algorithm))
                time = timer.timeit(number=number_executions) / number_executions
                print(algorithm, pattern_len, time)
                results[algorithm][pattern_len] += time

    # plot results
    for (algorithm, algorithm_results) in results.items():
        x = []
        y = []
        for (pattern_len, result) in enumerate(algorithm_results):
            if result > 0:
                x.append(pattern_len)
                y.append(result)

        pyplot.plot(x, y, label=algorithm)

    plt.legend()
    pyplot.show()


if __name__ == "__main__":
    main()
