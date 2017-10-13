import re
import subprocess
import timeit

import matplotlib.pyplot as plt
from matplotlib import pyplot


def compile():
    command = "g++ ../src/ptm.cpp -std=c++11 -O3 -o pmt"
    subprocess.call(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)


def run_aprox(pattern, text_file, algorithm, has_pattern_file=False, edit_distance=0):
    command = './pmt {0} {1} {2} -a {3} -e {4} -c'.format("-p" if has_pattern_file else "",
                                                          pattern,
                                                          text_file,
                                                          algorithm,
                                                          edit_distance)

    proc = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    proc.wait()

    return proc.stdout.read()


def get_execution_time(pattern, text_file, algorithm, number_executions, edit_distance=0):
    timer = timeit.Timer(lambda: run_aprox(pattern, text_file, algorithm, edit_distance=edit_distance))
    time = timer.timeit(number=number_executions) / number_executions
    return time


def get_occurrences_count(pattern, text_file, algorithm, edit_distance=0):
    output_str = run_aprox(pattern, text_file, algorithm, edit_distance=edit_distance).decode('ascii')
    print(output_str)
    occurrences_count = re.search(r'\d+', output_str).group()

    return occurrences_count


def assert_algorithms_match(pattern, text_file, algorithms):
    for edit_distance in range(3):
        occurrences_counts = {}
        for algorithm in algorithms:
            occurrences_count = get_occurrences_count(pattern, text_file, algorithm, edit_distance=edit_distance)
            if occurrences_count not in occurrences_counts:
                occurrences_counts[occurrences_count] = [algorithm]
            else:
                occurrences_counts[occurrences_count].append([algorithm])

        if len(occurrences_counts) != 1:
            print(occurrences_counts)

        assert len(occurrences_counts) == 1


def main():
    compile()

    aprox_algorithms = ['se', 'uk']
    patterns = sorted(['aba', 'abacaba', 'beauty', 'mediumsizepatterntotest', 'gentleman', 'gentlewoman'], key=len)
    text_files = ['shakespeare.txt']

    number_executions = 10
    max_pattern_len = 30

    # assert correctness
    for text_file in text_files:
        for pattern in patterns:
            assert_algorithms_match(pattern, text_file, aprox_algorithms)


    # calculate execution times
    execution_times = {}

    for algorithm in aprox_algorithms:
        execution_times[algorithm] = [0] * (max_pattern_len + 1)

    for text_file in text_files:
        for pattern in patterns:
            pattern_len = len(pattern)
            for algorithm in aprox_algorithms:
                for edit_distance in range(2):
                    time = get_execution_time(pattern, text_file, algorithm, number_executions, edit_distance=edit_distance)
                    print(algorithm, pattern_len, time)
                    execution_times[algorithm][pattern_len] += time

    # plot results
    for (algorithm, algorithm_execution_times) in execution_times.items():
        x = []
        y = []
        for (pattern_len, execution_time) in enumerate(algorithm_execution_times):
            if execution_time > 0:
                x.append(pattern_len)
                y.append(execution_time)

        pyplot.plot(x, y, label=algorithm)

    plt.legend()
    pyplot.show()


if __name__ == "__main__":
    main()
